#include "farm.h"
#include "gcode.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

static int g_seq = 0;
static void mkid(char *out, int n, const char *prefix) {
    snprintf(out, n, "%s%d", prefix, g_seq++);
}

static int f_count_status(Farm *f, const char *st);

void farm_init(Farm *f, const char *base_dir) {
    memset(f, 0, sizeof(*f));
    if (base_dir) snprintf(f->base_dir, sizeof(f->base_dir), "%s", base_dir);
    f->auto_eject = 1;
    f->eject_release_temp = 40;
    f->eject_angle = 90;
}

void farm_free(Farm *f) {
    if (f) memset(f, 0, sizeof(*f));
}

int farm_add_printer(Farm *f, const char *name, int tool_slots, const char *filament_type) {
    if (f->n_printers >= 64) return -1;
    FarmPrinter *p = &f->printers[f->n_printers];
    memset(p, 0, sizeof(*p));
    mkid(p->id, sizeof(p->id), "prn");
    snprintf(p->name, sizeof(p->name), "%s", name ? name : "printer");
    snprintf(p->status, sizeof(p->status), "idle");
    p->tool_slots = tool_slots < 0 ? 0 : tool_slots;
    if (filament_type) snprintf(p->filament_type, sizeof(p->filament_type), "%s", filament_type);
    return f->n_printers++;
}

int farm_add_job(Farm *f, const char *name, int quantity, int priority, const char *filament, int color_index) {
    if (f->n_jobs >= 256) return -1;
    FarmJob *j = &f->jobs[f->n_jobs];
    memset(j, 0, sizeof(*j));
    mkid(j->id, sizeof(j->id), "job");
    snprintf(j->name, sizeof(j->name), "%s", name ? name : "job");
    j->quantity = quantity > 0 ? quantity : 1;
    j->priority = priority;
    if (filament) snprintf(j->filament, sizeof(j->filament), "%s", filament);
    j->color_index = color_index;
    j->maps_to_slot = -1;
    j->eject_angle = 0;
    snprintf(j->status, sizeof(j->status), "queued");
    return f->n_jobs++;
}

int farm_add_order(Farm *f, const char *name, const int *job_indices, int n) {
    if (f->n_orders >= 16) return -1;
    FarmOrder *o = &f->orders[f->n_orders];
    memset(o, 0, sizeof(*o));
    mkid(o->id, sizeof(o->id), "ord");
    snprintf(o->name, sizeof(o->name), "%s", name ? name : "order");
    int cn = n > 256 ? 256 : n;
    for (int i = 0; i < cn; i++) o->job_indices[i] = job_indices[i];
    o->n_jobs = cn;
    for (int i = 0; i < cn; i++) {
        if (job_indices[i] >= 0 && job_indices[i] < f->n_jobs)
            o->total_units += f->jobs[job_indices[i]].quantity;
    }
    snprintf(o->status, sizeof(o->status), "open");
    return f->n_orders++;
}

int farm_add_spool(Farm *f, const char *filament, const char *color, float remaining_m) {
    if (f->n_spools >= 64) return -1;
    FarmSpool *s = &f->spools[f->n_spools];
    memset(s, 0, sizeof(*s));
    mkid(s->id, sizeof(s->id), "spl");
    snprintf(s->filament, sizeof(s->filament), "%s", filament ? filament : "");
    snprintf(s->color, sizeof(s->color), "%s", color ? color : "");
    s->remaining_m = remaining_m;
    return f->n_spools++;
}

int farm_add_file(Farm *f, const char *name, const char *path, const char **tags, int ntags) {
    if (f->n_files >= 256) return -1;
    FarmFile *fl = &f->files[f->n_files];
    memset(fl, 0, sizeof(*fl));
    mkid(fl->id, sizeof(fl->id), "fil");
    snprintf(fl->name, sizeof(fl->name), "%s", name ? name : "");
    snprintf(fl->file, sizeof(fl->file), "%s", path ? path : "");
    int cn = ntags > 8 ? 8 : ntags;
    for (int i = 0; i < cn; i++) if (tags[i]) snprintf(fl->tags[i], sizeof(fl->tags[i]), "%s", tags[i]);
    fl->n_tags = cn;
    fl->version = 1;
    return f->n_files++;
}

int farm_assign_job(Farm *f, int job, int printer_hint) {
    if (!f) return -1;
    if (job < 0 || job >= f->n_jobs) return -1;
    if (printer_hint < -1 || printer_hint >= f->n_printers) return -1;
    if (printer_hint >= 0 && f->printers[printer_hint].busy) return -1;
    FarmJob *j = &f->jobs[job];
    if (strcmp(j->status, "queued") != 0) return -1;
    int best = -1, best_load = 1000000;
    for (int p = 0; p < f->n_printers; p++) {
        FarmPrinter *pr = &f->printers[p];
        if (pr->busy || strcmp(pr->status, "busy") == 0) continue;
        if (j->filament[0] && pr->filament_type[0] && str_icmp(pr->filament_type, j->filament) != 0) continue;
        int load = pr->total_jobs_done; // simplistic load metric
        if (load < best_load) { best_load = load; best = p; }
    }
    if (best < 0 && printer_hint >= 0 && printer_hint < f->n_printers && !f->printers[printer_hint].busy)
        best = printer_hint;
    if (best >= 0) {
        FarmPrinter *pr = &f->printers[best];
        pr->busy = 1;
        snprintf(pr->status, sizeof(pr->status), "busy");
        snprintf(pr->current_job_id, sizeof(pr->current_job_id), "%s", j->id);
        snprintf(j->status, sizeof(j->status), "running");
        snprintf(j->assigned_printer, sizeof(j->assigned_printer), "%s", pr->id);
    }
    return best;
}

int farm_map_ams(Farm *f) {
    int mapped = 0;
    for (int job = 0; job < f->n_jobs; job++) {
        FarmJob *j = &f->jobs[job];
        if (strcmp(j->status, "queued") != 0 && strcmp(j->status, "running") != 0) continue;
        int pr_idx = -1;
        for (int p = 0; p < f->n_printers; p++)
            if (strcmp(f->printers[p].id, j->assigned_printer) == 0) { pr_idx = p; break; }
        if (pr_idx < 0) { j->maps_to_slot = -1; continue; }
        FarmPrinter *pr = &f->printers[pr_idx];
        int slot = j->color_index;
        if (slot < 0) slot = 0;
        if (pr->tool_slots > 0 && slot >= pr->tool_slots) slot = job % pr->tool_slots;
        int sp = -1;
        for (int s = 0; s < f->n_spools; s++) {
            if (f->spools[s].remaining_m > 0 && j->filament[0] && str_icmp(f->spools[s].filament, j->filament) == 0) { sp = s; break; }
        }
        if (sp >= 0 && pr->tool_slots > 0) {
            j->maps_to_slot = slot;
            mapped++;
        } else {
            j->maps_to_slot = -1;
        }
    }
    return mapped;
}

int farm_detect_anomalies(SliceResult *r, const SliceSettings *cfg) {
    int flags = 0;
    if (!r || r->nlayers <= 0) return FARM_ANOM_FIRST_LAYER | FARM_ANOM_SPAGHETTI;
    // how many top layers are solid (no infill) - a normal part ends with a
    // solid cap, so we never treat the top cap as a collapse.
    int top_cap = cfg && cfg->solid_top_layers > 0 ? cfg->solid_top_layers : 3;
    int firstnz = -1;
    float *laydist = (float *)calloc((size_t)r->nlayers, sizeof(float));
    int total_paths = 0, short_paths = 0;
    for (int i = 0; i < r->nlayers; i++) {
        SliceLayer *L = &r->layers[i];
        if (L->npaths > 0 && firstnz < 0) firstnz = i;
        float d = 0; int perims = 0;
        for (int j = 0; j < L->npaths; j++) {
            SlicePath *p = &L->paths[j];
            for (int k = 1; k < p->npts; k++) d += v2_dist(p->pts[k - 1], p->pts[k]);
            if (p->npts == 2 && v2_dist(p->pts[0], p->pts[1]) < 3.0f) short_paths++;
            if (p->kind == SLICE_PATH_PERIMETER) perims++;
            total_paths++;
        }
        laydist[i] = d;
        if (i == firstnz && perims == 0) flags |= FARM_ANOM_FIRST_LAYER;
    }
    if (firstnz < 0) flags |= FARM_ANOM_FIRST_LAYER | FARM_ANOM_SPAGHETTI;
    else {
        // Spaghetti detection: a real failure is a *sudden* drop in toolpath
        // length vs the immediately preceding layers (not vs the whole print -
        // sparse infill vs solid caps is normal). Compare each layer against a
        // rolling window of the 5 previous non-empty layers.
        float win[5] = { 0, 0, 0, 0, 0 };
        int nwin = 0, collapse = 0;
        for (int i = firstnz; i < r->nlayers; i++) {
            if (laydist[i] <= 0.0f) continue;
            if (i < r->nlayers - top_cap && nwin >= 5) {
                float avg = 0;
                for (int k = 0; k < 5; k++) avg += win[k];
                avg /= 5.0f;
                if (avg > 1e-3f && laydist[i] < avg * 0.4f) {
                    if (++collapse >= 4) { flags |= FARM_ANOM_SPAGHETTI; break; }
                } else {
                    collapse = 0;
                }
            }
            win[nwin % 5] = laydist[i];
            nwin++;
        }
    }
    if (short_paths > 40 && total_paths > 8 && (float)short_paths / (float)total_paths > 0.5f) flags |= FARM_ANOM_STRINGING;
    free(laydist);
    return flags;
}

typedef struct { int job; int priority; } OrderEntry;

char *farm_generate_batch(Farm *f, int printer, FarmSliceFn job_slice, const char *out_path) {
    if (!f || printer < 0 || printer >= f->n_printers || !job_slice || !out_path) return NULL;
    FarmPrinter *pr = &f->printers[printer];
    OrderEntry list[256]; int n = 0;
    for (int j = 0; j < f->n_jobs; j++) {
        FarmJob *job = &f->jobs[j];
        if (strcmp(job->status, "queued") != 0 && strcmp(job->status, "running") != 0) continue;
        if (strcmp(job->assigned_printer, pr->id) != 0) continue;
        if (n < 256) { list[n].job = j; list[n].priority = job->priority; n++; }
    }
    if (n <= 0) return NULL;
    for (int i = 0; i < n; i++)
        for (int k = i + 1; k < n; k++)
            if (list[k].priority > list[i].priority) { OrderEntry t = list[i]; list[i] = list[k]; list[k] = t; }

    FILE *fout = fopen(out_path, "w");
    if (!fout) return NULL;
    fprintf(fout, "; Volt360 print farm batch\n; printer: %s\n; jobs: %d\n", pr->name, n);
    int done = 0;
    for (int i = 0; i < n; i++) {
        int j = list[i].job;
        FarmJob *job = &f->jobs[j];
        SliceResult sr;
        memset(&sr, 0, sizeof(sr));
        if (!job_slice(j, &sr)) {
            snprintf(job->status, sizeof(job->status), "failed");
            continue;
        }
        SliceSettings cfg;
        slice_settings_default(&cfg);
        cfg.printer_name = pr->name;
        cfg.farm_mode = 1;
        cfg.auto_eject = f->auto_eject;
        cfg.eject_angle = job->eject_angle > 0 ? (float)job->eject_angle : (float)f->eject_angle;
        cfg.eject_release_temp = (int)f->eject_release_temp;
        cfg.eject_gcode = job->eject_gcode[0] ? job->eject_gcode : NULL;
        cfg.tools = pr->tool_slots;
        int forced = -1;
        if (job->maps_to_slot >= 0 && job->maps_to_slot < cfg.tools) forced = job->maps_to_slot;
        else if (cfg.tools > 0) cfg.random_color = 1;
        fprintf(fout, ";FARM_JOB:%s ;FARM_NAME:%s ;FARM_QTY:%d\n", job->id, job->name, job->quantity);
        gcode_emit_fp_tool(fout, &sr, &cfg, forced);
        fprintf(fout, ";FARM_JOB_END:%s ;FARM_OK\n", job->id);
        // filament tracking
        float used_m = sr.filament_mm / 1000.0f;
        for (int s = 0; s < f->n_spools; s++) {
            FarmSpool *sp = &f->spools[s];
            if (job->filament[0] && str_icmp(sp->filament, job->filament) == 0 && sp->remaining_m > 0) {
                float take = used_m * job->quantity;
                if (take > sp->remaining_m) take = sp->remaining_m;
                sp->remaining_m -= take;
                sp->used_m += take;
                break;
            }
        }
        job->filament_mm = sr.filament_mm;
        job->est_time_s = sr.est_time_s;
        snprintf(job->output_gcode, sizeof(job->output_gcode), "%s", out_path);
        snprintf(job->status, sizeof(job->status), "done");
        job->finished = 1;
        pr->total_jobs_done += job->quantity;
        pr->hours_running += sr.est_time_s / 3600.0f;
        for (int o = 0; o < f->n_orders; o++) {
            FarmOrder *ord = &f->orders[o];
            for (int k = 0; k < ord->n_jobs; k++)
                if (ord->job_indices[k] == j) ord->done_units += job->quantity;
        }
        slice_result_free(&sr);
        done++;
    }
    fprintf(fout, "; llc farm batch end (%d/%d ok)\n", done, n);
    fclose(fout);
    if (done == 0) {
#ifdef _WIN32
        DeleteFileA(out_path);
#endif
        return NULL;
    }
    pr->busy = 0;
    snprintf(pr->status, sizeof(pr->status), "idle");
    snprintf(pr->current_job_id, sizeof(pr->current_job_id), "");
    return str_dup(out_path);
}

char *farm_stats_text(Farm *f) {
    Buf b; buf_init(&b);
    buf_append_str(&b, "== Volt360 print farm report ==\n\n");
    buf_appendf(&b, "printers: %d\n", f->n_printers);
    for (int p = 0; p < f->n_printers; p++) {
        FarmPrinter *pr = &f->printers[p];
        buf_appendf(&b, "  %-24s %-8s jobs=%d hours=%.2f slots=%d%s%s\n", pr->name, pr->status,
                    pr->total_jobs_done, pr->hours_running, pr->tool_slots,
                    pr->filament_type[0] ? " filament=" : "", pr->filament_type);
    }
    buf_appendf(&b, "\nspools: %d\n", f->n_spools);
    for (int s = 0; s < f->n_spools; s++) {
        FarmSpool *sp = &f->spools[s];
        buf_appendf(&b, "  %-16s %-12s remaining=%.1fm used=%.1fm\n", sp->filament, sp->color, sp->remaining_m, sp->used_m);
    }
    buf_appendf(&b, "\njobs: %d queued, %d running, %d done, %d failed\n",
                f_count_status(f, "queued"), f_count_status(f, "running"),
                f_count_status(f, "done"), f_count_status(f, "failed"));
    buf_appendf(&b, "\norders: %d\n", f->n_orders);
    for (int o = 0; o < f->n_orders; o++) {
        FarmOrder *ord = &f->orders[o];
        buf_appendf(&b, "  %-24s %-12s %d/%d units\n", ord->name, ord->status, ord->done_units, ord->total_units);
    }
    buf_appendf(&b, "\ncentral files: %d\n", f->n_files);
    for (int i = 0; i < f->n_files; i++)
        buf_appendf(&b, "  %-24s v%d %s\n", f->files[i].name, f->files[i].version, f->files[i].file);
    char *s = str_dup(buf_cstr(&b));
    buf_free(&b);
    return s;
}

int f_count_status(Farm *f, const char *st) {
    int c = 0;
    for (int j = 0; j < f->n_jobs; j++) if (strcmp(f->jobs[j].status, st) == 0) c++;
    return c;
}

int farm_scan_store(Farm *f) {
    int added = 0;
#ifdef _WIN32
    const char *patterns[] = { "\\*.gcode", "\\*.GCODE", "\\*.stl", "\\*.STL" };
    char pat[1024];
    WIN32_FIND_DATAA fd;
    for (int pi = 0; pi < 4; pi++) {
        snprintf(pat, sizeof(pat), "%s%s", f->base_dir, patterns[pi]);
        HANDLE h = FindFirstFileA(pat, &fd);
        if (h == INVALID_HANDLE_VALUE) continue;
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
            if (f->n_files >= 256) break;
            // versioning: same base name -> bump
            int ver = 1;
            for (int i = 0; i < f->n_files; i++) {
                if (str_icmp(f->files[i].name, fd.cFileName) == 0 && ver <= f->files[i].version) ver = f->files[i].version + 1;
            }
            char path[1024];
            snprintf(path, sizeof(path), "%s\\%s", f->base_dir, fd.cFileName);
            farm_add_file(f, fd.cFileName, path, NULL, 0);
            f->files[f->n_files - 1].version = ver;
            added++;
        } while (FindNextFileA(h, &fd));
        FindClose(h);
    }
#endif
    return added;
}

// ---------------- minimal JSON (persistence for farm.json) ----------------
#include <ctype.h>

typedef struct JVal {
    int type;             // 0 null, 1 bool, 2 num, 3 str, 4 arr, 5 obj
    double num;
    int boolean;
    char *str;
    struct JVal *items;   // arr elements / obj values
    char **keys;          // obj keys (n entries)
    int n, cap;
} JVal;

static void jval_free(JVal *v) {
    if (!v) return;
    free(v->str);
    for (int i = 0; i < v->n; i++) {
        free(v->keys ? v->keys[i] : NULL);
        jval_free(&v->items[i]);
    }
    free(v->items);
    free(v->keys);
}

static JVal *jval_new(int type) {
    JVal *v = (JVal *)calloc(1, sizeof(JVal));
    v->type = type;
    return v;
}

static void jval_push(JVal *v, const char *key, JVal item) {
    if (v->n >= v->cap) {
        v->cap = v->cap ? v->cap * 2 : 8;
        v->items = (JVal *)realloc(v->items, (size_t)v->cap * sizeof(JVal));
        v->keys = (char **)realloc(v->keys, (size_t)v->cap * sizeof(char *));
    }
    v->items[v->n] = item;
    v->keys[v->n] = key ? str_dup(key) : NULL;
    v->n++;
}

static void j_obj_set_str(JVal *o, const char *k, const char *val) {
    JVal x; memset(&x, 0, sizeof(x));
    x.type = 3;
    x.str = str_dup(val ? val : "");
    jval_push(o, k, x);
}
static void j_obj_set_num(JVal *o, const char *k, double val) {
    JVal x; memset(&x, 0, sizeof(x));
    x.type = 2;
    x.num = val;
    jval_push(o, k, x);
}
static void j_obj_set_int(JVal *o, const char *k, int val) {
    JVal x; memset(&x, 0, sizeof(x));
    x.type = 2;
    x.num = (double)val;
    jval_push(o, k, x);
}

// ----- parser -----
static int j_skip(const char *s, int *i) {
    while (s[*i] == ' ' || s[*i] == '\t' || s[*i] == '\r' || s[*i] == '\n') (*i)++;
    return 1;
}

static int j_parse_str(const char *s, int *i, char **out) {
    j_skip(s, i);
    if (s[*i] != '"') return 0;
    (*i)++;
    Buf b; buf_init(&b);
    while (s[*i] && s[*i] != '"') {
        if (s[*i] == '\\' && s[*i + 1]) {
            char c = s[*i + 1];
            if (c == 'n') buf_append(&b, "\n", 1);
            else if (c == 't') buf_append(&b, "\t", 1);
            else if (c == 'r') buf_append(&b, "\r", 1);
            else buf_append(&b, &c, 1);
            (*i) += 2;
        } else {
            buf_append(&b, &s[*i], 1);
            (*i)++;
        }
    }
    if (s[*i] != '"') { buf_free(&b); return 0; }
    (*i)++;
    *out = buf_cstr(&b);
    return 1;
}

static int j_parse_num(const char *s, int *i, double *out) {
    j_skip(s, i);
    const char *start = s + *i;
    if (s[*i] == '-' || s[*i] == '+') (*i)++;
    int dig = 0;
    while (s[*i] && isdigit((unsigned char)s[*i])) { (*i)++; dig = 1; }
    if (s[*i] == '.') {
        (*i)++;
        while (s[*i] && isdigit((unsigned char)s[*i])) (*i)++;
    }
    if (s[*i] == 'e' || s[*i] == 'E') {
        int save = *i; (*i)++;
        if (s[*i] == '+' || s[*i] == '-') (*i)++;
        if (!isdigit((unsigned char)s[*i])) *i = save;
        else while (s[*i] && isdigit((unsigned char)s[*i])) (*i)++;
    }
    if (!dig) return 0;
    *out = atof(start);
    return 1;
}

static int j_parse_value(const char *s, int *i, JVal **out) {
    j_skip(s, i);
    char c = s[*i];
    if (c == '{') {
        JVal *o = jval_new(5);
        (*i)++;
        j_skip(s, i);
        if (s[*i] == '}') { (*i)++; *out = o; return 1; }
        while (1) {
            char *k = NULL;
            if (!j_parse_str(s, i, &k)) { jval_free(o); free(o); return 0; }
            j_skip(s, i);
            if (s[*i] != ':') { free(k); jval_free(o); free(o); return 0; }
            (*i)++;
            JVal *v = NULL;
            if (!j_parse_value(s, i, &v)) { free(k); jval_free(o); free(o); return 0; }
            jval_push(o, k, *v);
            free(k); free(v);
            j_skip(s, i);
            if (s[*i] == ',') { (*i)++; continue; }
            if (s[*i] == '}') { (*i)++; *out = o; return 1; }
            jval_free(o); free(o); return 0;
        }
    }
    if (c == '[') {
        JVal *a = jval_new(4);
        (*i)++;
        j_skip(s, i);
        if (s[*i] == ']') { (*i)++; *out = a; return 1; }
        while (1) {
            JVal *v = NULL;
            if (!j_parse_value(s, i, &v)) { jval_free(a); free(a); return 0; }
            jval_push(a, NULL, *v);
            free(v);
            j_skip(s, i);
            if (s[*i] == ',') { (*i)++; continue; }
            if (s[*i] == ']') { (*i)++; *out = a; return 1; }
            jval_free(a); free(a); return 0;
        }
    }
    if (c == '"') {
        char *str = NULL;
        if (!j_parse_str(s, i, &str)) return 0;
        JVal *v = jval_new(3);
        v->str = str;
        *out = v;
        return 1;
    }
    if (c == 't' && strncmp(s + *i, "true", 4) == 0) { *i += 4; JVal *v = jval_new(1); v->boolean = 1; *out = v; return 1; }
    if (c == 'f' && strncmp(s + *i, "false", 5) == 0) { *i += 5; JVal *v = jval_new(1); *out = v; return 1; }
    if (c == 'n' && strncmp(s + *i, "null", 4) == 0) { *i += 4; JVal *v = jval_new(0); *out = v; return 1; }
    double num = 0;
    if (j_parse_num(s, i, &num)) { JVal *v = jval_new(2); v->num = num; *out = v; return 1; }
    return 0;
}

// ----- writer -----
static void j_str_esc(Buf *b, const char *s) {
    buf_append_str(b, "\"");
    for (const char *p = s ? s : ""; *p; p++) {
        if (*p == '"') buf_append_str(b, "\\\"");
        else if (*p == '\\') buf_append_str(b, "\\\\");
        else if (*p == '\n') buf_append_str(b, "\\n");
        else if (*p == '\t') buf_append_str(b, "\\t");
        else if (*p == '\r') buf_append_str(b, "\\r");
        else buf_append(b, p, 1);
    }
    buf_append_str(b, "\"");
}

static void j_write(Buf *b, const JVal *v) {
    switch (v->type) {
    case 0: buf_append_str(b, "null"); break;
    case 1: buf_append_str(b, v->boolean ? "true" : "false"); break;
    case 2: {
        char tmp[64];
        if (v->num == (double)(int)v->num) snprintf(tmp, sizeof(tmp), "%d", (int)v->num);
        else snprintf(tmp, sizeof(tmp), "%.6g", v->num);
        buf_append_str(b, tmp);
    } break;
    case 3: j_str_esc(b, v->str); break;
    case 4: {
        buf_append_str(b, "[");
        for (int i = 0; i < v->n; i++) {
            if (i) buf_append_str(b, ",");
            j_write(b, &v->items[i]);
        }
        buf_append_str(b, "]");
    } break;
    case 5: {
        buf_append_str(b, "{");
        for (int i = 0; i < v->n; i++) {
            if (i) buf_append_str(b, ",");
            j_str_esc(b, v->keys[i]);
            buf_append_str(b, ":");
            j_write(b, &v->items[i]);
        }
        buf_append_str(b, "}");
    } break;
    }
}

static const JVal *j_get(const JVal *o, const char *k) {
    if (!o || o->type != 5) return NULL;
    for (int i = 0; i < o->n; i++)
        if (o->keys[i] && strcmp(o->keys[i], k) == 0) return &o->items[i];
    return NULL;
}
static double j_num(const JVal *o, const char *k, double dflt) {
    const JVal *v = j_get(o, k);
    return (v && v->type == 2) ? v->num : dflt;
}
static int j_int(const JVal *o, const char *k, int dflt) { return (int)j_num(o, k, dflt); }
static const char *j_strv(const JVal *o, const char *k) {
    const JVal *v = j_get(o, k);
    return (v && v->type == 3 && v->str) ? v->str : "";
}

bool farm_save(Farm *f) {
    if (!f->base_dir[0]) return false;
    char path[1024];
    snprintf(path, sizeof(path), "%s\\farm.json", f->base_dir);
    JVal *root = jval_new(5);
    j_obj_set_str(root, "base_dir", f->base_dir);
    j_obj_set_int(root, "auto_eject", f->auto_eject);
    j_obj_set_num(root, "eject_release_temp", f->eject_release_temp);
    j_obj_set_int(root, "eject_angle", f->eject_angle);

    JVal arr; memset(&arr, 0, sizeof(arr)); arr.type = 4;
    for (int p = 0; p < f->n_printers; p++) {
        FarmPrinter *pr = &f->printers[p];
        JVal o; memset(&o, 0, sizeof(o)); o.type = 5;
        j_obj_set_str(&o, "id", pr->id);
        j_obj_set_str(&o, "name", pr->name);
        j_obj_set_str(&o, "status", pr->status);
        j_obj_set_str(&o, "current_job_id", pr->current_job_id);
        j_obj_set_int(&o, "busy", pr->busy);
        j_obj_set_str(&o, "filament_type", pr->filament_type);
        j_obj_set_int(&o, "tool_slots", pr->tool_slots);
        j_obj_set_int(&o, "total_jobs_done", pr->total_jobs_done);
        j_obj_set_num(&o, "hours_running", pr->hours_running);
        jval_push(&arr, NULL, o);
    }
    jval_push(root, "printers", arr);

    memset(&arr, 0, sizeof(arr)); arr.type = 4;
    for (int j = 0; j < f->n_jobs; j++) {
        FarmJob *jb = &f->jobs[j];
        JVal o; memset(&o, 0, sizeof(o)); o.type = 5;
        j_obj_set_str(&o, "id", jb->id);
        j_obj_set_str(&o, "name", jb->name);
        j_obj_set_int(&o, "quantity", jb->quantity);
        j_obj_set_int(&o, "priority", jb->priority);
        j_obj_set_str(&o, "filament", jb->filament);
        j_obj_set_int(&o, "color_index", jb->color_index);
        j_obj_set_int(&o, "maps_to_slot", jb->maps_to_slot);
        j_obj_set_int(&o, "eject_angle", jb->eject_angle);
        j_obj_set_str(&o, "eject_gcode", jb->eject_gcode);
        j_obj_set_str(&o, "status", jb->status);
        j_obj_set_str(&o, "assigned_printer", jb->assigned_printer);
        j_obj_set_str(&o, "output_gcode", jb->output_gcode);
        j_obj_set_num(&o, "filament_mm", jb->filament_mm);
        j_obj_set_num(&o, "est_time_s", jb->est_time_s);
        j_obj_set_int(&o, "finished", jb->finished);
        jval_push(&arr, NULL, o);
    }
    jval_push(root, "jobs", arr);

    memset(&arr, 0, sizeof(arr)); arr.type = 4;
    for (int o = 0; o < f->n_orders; o++) {
        FarmOrder *od = &f->orders[o];
        JVal obj; memset(&obj, 0, sizeof(obj)); obj.type = 5;
        j_obj_set_str(&obj, "id", od->id);
        j_obj_set_str(&obj, "name", od->name);
        JVal ja; memset(&ja, 0, sizeof(ja)); ja.type = 4;
        for (int k = 0; k < od->n_jobs; k++) {
            JVal x; memset(&x, 0, sizeof(x)); x.type = 2; x.num = (double)od->job_indices[k];
            jval_push(&ja, NULL, x);
        }
        jval_push(&obj, "job_indices", ja);
        j_obj_set_int(&obj, "total_units", od->total_units);
        j_obj_set_int(&obj, "done_units", od->done_units);
        j_obj_set_str(&obj, "status", od->status);
        jval_push(&arr, NULL, obj);
    }
    jval_push(root, "orders", arr);

    memset(&arr, 0, sizeof(arr)); arr.type = 4;
    for (int s = 0; s < f->n_spools; s++) {
        FarmSpool *sp = &f->spools[s];
        JVal o; memset(&o, 0, sizeof(o)); o.type = 5;
        j_obj_set_str(&o, "id", sp->id);
        j_obj_set_str(&o, "filament", sp->filament);
        j_obj_set_str(&o, "color", sp->color);
        j_obj_set_num(&o, "remaining_m", sp->remaining_m);
        j_obj_set_num(&o, "used_m", sp->used_m);
        j_obj_set_str(&o, "printer_id", sp->printer_id);
        jval_push(&arr, NULL, o);
    }
    jval_push(root, "spools", arr);

    memset(&arr, 0, sizeof(arr)); arr.type = 4;
    for (int i = 0; i < f->n_files; i++) {
        FarmFile *fl = &f->files[i];
        JVal o; memset(&o, 0, sizeof(o)); o.type = 5;
        j_obj_set_str(&o, "id", fl->id);
        j_obj_set_str(&o, "name", fl->name);
        j_obj_set_str(&o, "file", fl->file);
        JVal ta; memset(&ta, 0, sizeof(ta)); ta.type = 4;
        for (int k = 0; k < fl->n_tags; k++) j_obj_set_str(&ta, NULL, fl->tags[k]);
        jval_push(&o, "tags", ta);
        j_obj_set_int(&o, "version", fl->version);
        jval_push(&arr, NULL, o);
    }
    jval_push(root, "files", arr);

    Buf b; buf_init(&b);
    j_write(&b, root);
    buf_append_str(&b, "\n");
    bool ok = file_write(path, b.data, b.len);
    buf_free(&b);
    jval_free(root);
    free(root);
    return ok;
}

bool farm_load(Farm *f, const char *base_dir) {
    if (base_dir) snprintf(f->base_dir, sizeof(f->base_dir), "%s", base_dir);
    char path[1024];
    snprintf(path, sizeof(path), "%s\\farm.json", f->base_dir);
    size_t len = 0;
    char *data = file_read(path, &len);
    if (!data) return false;
    int i = 0;
    JVal *root = NULL;
    if (!j_parse_value(data, &i, &root)) { free(data); return false; }
    farm_init(f, base_dir);
    f->auto_eject = j_int(root, "auto_eject", 1);
    f->eject_release_temp = (float)j_num(root, "eject_release_temp", 40);
    f->eject_angle = j_int(root, "eject_angle", 90);

    const JVal *ps = j_get(root, "printers");
    if (ps && ps->type == 4)
        for (int k = 0; k < ps->n && f->n_printers < 64; k++) {
            const JVal *o = &ps->items[k];
            FarmPrinter pr; memset(&pr, 0, sizeof(pr));
            snprintf(pr.id, sizeof(pr.id), "%s", j_strv(o, "id"));
            snprintf(pr.name, sizeof(pr.name), "%s", j_strv(o, "name"));
            snprintf(pr.status, sizeof(pr.status), "%s", j_strv(o, "status"));
            snprintf(pr.current_job_id, sizeof(pr.current_job_id), "%s", j_strv(o, "current_job_id"));
            pr.busy = j_int(o, "busy", 0);
            snprintf(pr.filament_type, sizeof(pr.filament_type), "%s", j_strv(o, "filament_type"));
            pr.tool_slots = j_int(o, "tool_slots", 0);
            pr.total_jobs_done = j_int(o, "total_jobs_done", 0);
            pr.hours_running = (float)j_num(o, "hours_running", 0);
            f->printers[f->n_printers++] = pr;
        }

    const JVal *js = j_get(root, "jobs");
    if (js && js->type == 4)
        for (int k = 0; k < js->n && f->n_jobs < 256; k++) {
            const JVal *o = &js->items[k];
            FarmJob jb; memset(&jb, 0, sizeof(jb));
            snprintf(jb.id, sizeof(jb.id), "%s", j_strv(o, "id"));
            snprintf(jb.name, sizeof(jb.name), "%s", j_strv(o, "name"));
            jb.quantity = j_int(o, "quantity", 1);
            jb.priority = j_int(o, "priority", 1);
            snprintf(jb.filament, sizeof(jb.filament), "%s", j_strv(o, "filament"));
            jb.color_index = j_int(o, "color_index", 0);
            jb.maps_to_slot = j_int(o, "maps_to_slot", -1);
            jb.eject_angle = j_int(o, "eject_angle", 0);
            snprintf(jb.eject_gcode, sizeof(jb.eject_gcode), "%s", j_strv(o, "eject_gcode"));
            snprintf(jb.status, sizeof(jb.status), "%s", j_strv(o, "status"));
            snprintf(jb.assigned_printer, sizeof(jb.assigned_printer), "%s", j_strv(o, "assigned_printer"));
            snprintf(jb.output_gcode, sizeof(jb.output_gcode), "%s", j_strv(o, "output_gcode"));
            jb.filament_mm = (float)j_num(o, "filament_mm", 0);
            jb.est_time_s = (float)j_num(o, "est_time_s", 0);
            jb.finished = j_int(o, "finished", 0);
            f->jobs[f->n_jobs++] = jb;
        }

    const JVal *os = j_get(root, "orders");
    if (os && os->type == 4)
        for (int k = 0; k < os->n && f->n_orders < 16; k++) {
            const JVal *o = &os->items[k];
            FarmOrder od; memset(&od, 0, sizeof(od));
            snprintf(od.id, sizeof(od.id), "%s", j_strv(o, "id"));
            snprintf(od.name, sizeof(od.name), "%s", j_strv(o, "name"));
            const JVal *ji = j_get(o, "job_indices");
            if (ji && ji->type == 4) {
                int cn = ji->n > 256 ? 256 : ji->n;
                for (int m = 0; m < cn; m++) od.job_indices[m] = (int)ji->items[m].num;
                od.n_jobs = cn;
            }
            od.total_units = j_int(o, "total_units", 0);
            od.done_units = j_int(o, "done_units", 0);
            snprintf(od.status, sizeof(od.status), "%s", j_strv(o, "status"));
            f->orders[f->n_orders++] = od;
        }

    const JVal *ss = j_get(root, "spools");
    if (ss && ss->type == 4)
        for (int k = 0; k < ss->n && f->n_spools < 64; k++) {
            const JVal *o = &ss->items[k];
            FarmSpool sp; memset(&sp, 0, sizeof(sp));
            snprintf(sp.id, sizeof(sp.id), "%s", j_strv(o, "id"));
            snprintf(sp.filament, sizeof(sp.filament), "%s", j_strv(o, "filament"));
            snprintf(sp.color, sizeof(sp.color), "%s", j_strv(o, "color"));
            sp.remaining_m = (float)j_num(o, "remaining_m", 0);
            sp.used_m = (float)j_num(o, "used_m", 0);
            snprintf(sp.printer_id, sizeof(sp.printer_id), "%s", j_strv(o, "printer_id"));
            f->spools[f->n_spools++] = sp;
        }

    const JVal *fs = j_get(root, "files");
    if (fs && fs->type == 4)
        for (int k = 0; k < fs->n && f->n_files < 256; k++) {
            const JVal *o = &fs->items[k];
            FarmFile fl; memset(&fl, 0, sizeof(fl));
            snprintf(fl.id, sizeof(fl.id), "%s", j_strv(o, "id"));
            snprintf(fl.name, sizeof(fl.name), "%s", j_strv(o, "name"));
            snprintf(fl.file, sizeof(fl.file), "%s", j_strv(o, "file"));
            const JVal *tg = j_get(o, "tags");
            if (tg && tg->type == 4) {
                int cn = tg->n > 8 ? 8 : tg->n;
                for (int m = 0; m < cn; m++) snprintf(fl.tags[m], sizeof(fl.tags[m]), "%s", tg->items[m].str ? tg->items[m].str : "");
                fl.n_tags = cn;
            }
            fl.version = j_int(o, "version", 1);
            f->files[f->n_files++] = fl;
        }

    jval_free(root);
    free(root);
    free(data);
    return true;
}

// ================= maintenance & alerts =================

int farm_add_maintenance(Farm *f, const char *printer_name, int interval_hours, const char *task) {
    if (!f || f->n_maint >= 64) return -1;
    FarmMaintenance *m = &f->maint[f->n_maint];
    snprintf(m->id, sizeof(m->id), "maint-%d", f->n_maint);
    snprintf(m->printer_name, sizeof(m->printer_name), "%s", printer_name ? printer_name : "?");
    m->interval_hours = interval_hours;
    m->last_service_hours = 0;
    snprintf(m->task, sizeof(m->task), "%s", task ? task : "service");
    return f->n_maint++;
}

int farm_check_maintenance(Farm *f, char *report, int report_size) {
    if (!f || !report) return 0;
    int due = 0, off = 0;
    for (int i = 0; i < f->n_maint; i++) {
        FarmMaintenance *m = &f->maint[i];
        // find printer hours
        float hrs = 0;
        for (int p = 0; p < f->n_printers; p++)
            if (strcmp(f->printers[p].name, m->printer_name) == 0) { hrs = f->printers[p].hours_running; break; }
        if (hrs - m->last_service_hours > m->interval_hours) {
            off += snprintf(report + off, report_size - off,
                "[%s] %s due (%.0fh since last, interval=%dh)\n",
                m->printer_name, m->task, hrs - m->last_service_hours, m->interval_hours);
            due++;
        }
    }
    return due;
}

float farm_filament_remaining(Farm *f, const char *filament_type) {
    if (!f || !filament_type) return 0;
    float total = 0;
    for (int i = 0; i < f->n_spools; i++)
        if (strcmp(f->spools[i].filament, filament_type) == 0) total += f->spools[i].remaining_m;
    return total;
}

int farm_low_filament_alerts(Farm *f, float threshold_m, char *report, int report_size) {
    if (!f || !report) return 0;
    int count = 0, off = 0;
    for (int i = 0; i < f->n_spools; i++)
        if (f->spools[i].remaining_m < threshold_m) {
            off += snprintf(report + off, report_size - off,
                "LOW: %s %s spool %.1fm remaining\n",
                f->spools[i].filament, f->spools[i].color, f->spools[i].remaining_m);
            count++;
        }
    return count;
}

void farm_select_all_printers(Farm *f, bool busy) {
    if (!f) return;
    for (int i = 0; i < f->n_printers; i++) {
        f->printers[i].busy = busy ? 1 : 0;
        snprintf(f->printers[i].status, sizeof(f->printers[i].status), busy ? "busy" : "idle");
    }
}

void farm_batch_pause(Farm *f) {
    if (!f) return;
    for (int i = 0; i < f->n_printers; i++) {
        if (f->printers[i].busy) snprintf(f->printers[i].status, sizeof(f->printers[i].status), "paused");
    }
}

void farm_batch_resume(Farm *f) {
    if (!f) return;
    for (int i = 0; i < f->n_printers; i++) {
        if (f->printers[i].busy) snprintf(f->printers[i].status, sizeof(f->printers[i].status), "busy");
    }
}

// ================= SmartTags =================

int farm_tag_add(Farm *f, const char *target_id, const char *tag) {
    if (!f || !target_id || !tag) return -1;
    // add tag to matching file ID
    for (int i = 0; i < f->n_files; i++)
        if (strcmp(f->files[i].id, target_id) == 0 && f->files[i].n_tags < 8) {
            snprintf(f->files[i].tags[f->files[i].n_tags], sizeof(f->files[i].tags[0]), "%s", tag);
            f->files[i].n_tags++;
            return 0;
        }
    return -1;
}

int farm_tag_find(Farm *f, const char *tag, char *results, int results_size) {
    if (!f || !tag || !results) return 0;
    int count = 0, off = 0;
    for (int i = 0; i < f->n_files; i++)
        for (int j = 0; j < f->files[i].n_tags; j++)
            if (strstr(f->files[i].tags[j], tag)) {
                off += snprintf(results + off, results_size - off, "%s (%s)\n", f->files[i].name, f->files[i].tags[j]);
                count++; break;
            }
    return count;
}
