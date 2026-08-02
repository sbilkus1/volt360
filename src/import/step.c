#include "importer.h"
#include <ctype.h>
#include <math.h>

// ================= ISO-10303-21 (STEP) planar+cylindrical BREP mesher =================

// ---- token ----
typedef enum { ST_NUM, ST_STR, ST_REF, ST_SYM, ST_LIST } StokType;
typedef struct Stok {
    StokType type;
    double num;
    char *str;              // string content / symbol content
    int ref;                // entity id for ST_REF
    ARRAY(struct Stok) list; // ST_LIST
} Stok;

static void stok_init(Stok *t) { memset(t, 0, sizeof(*t)); arr_init(t->list); }
static void stok_free(Stok *t) {
    free(t->str);
    for (int i = 0; i < t->list.len; i++) stok_free(&t->list.v[i]);
    arr_free(t->list);
}

// ---- entity ----
typedef struct Sent { int id; char *type; Stok args; } Sent;

static ARRAY(Sent) g_ents;     // all entities
static Sent **g_byid = NULL;   // index by id
static int g_maxid = 0;

static Sent *ent(int id) { return (id >= 0 && id <= g_maxid) ? g_byid[id] : NULL; }
static Sent *tok_ent(Stok *t) { return t->type == ST_REF ? ent(t->ref) : NULL; }
static Stok *ent_arg(Sent *e, int i) { return (e && i < e->args.list.len) ? &e->args.list.v[i] : NULL; }

static Stok *find_ent_arg_type(Sent *e, int from, const char *type) {
    for (int i = from; i < e->args.list.len; i++) {
        Stok *t = &e->args.list.v[i];
        if (t->type == ST_REF) {
            Sent *te = ent(t->ref);
            if (te && strcmp(te->type, type) == 0) return t;
        }
    }
    return NULL;
}

static bool get_point(Sent *e, double out[3]) {
    // e is CARTESIAN_POINT or VERTEX_POINT
    if (!e) return false;
    if (strcmp(e->type, "CARTESIAN_POINT") == 0) {
        Stok *coords = &e->args.list.v[1]; // second arg is list
        if (coords->type != ST_LIST) coords = &e->args.list.v[0];
        int n = 0;
        for (int i = 0; i < coords->list.len && n < 3; i++) {
            if (coords->list.v[i].type == ST_NUM) out[n++] = coords->list.v[i].num;
        }
        for (int i = n; i < 3; i++) out[i] = 0.0;
        return n >= 2;
    }
    if (strcmp(e->type, "VERTEX_POINT") == 0) {
        Stok *cp = ent_arg(e, 1);
        if (cp) return get_point(tok_ent(cp), out);
    }
    if (strcmp(e->type, "AXIS2_PLACEMENT_3D") == 0) {
        Stok *loc = ent_arg(e, 1);
        return get_point(tok_ent(loc), out);
    }
    return false;
}

static bool get_dir(Sent *e, double out[3]) {
    if (!e) return false;
    if (strcmp(e->type, "DIRECTION") == 0) {
        Stok *v = ent_arg(e, 1);
        if (!v || v->type != ST_LIST) return false;
        int n = 0;
        for (int i = 0; i < v->list.len && n < 3; i++) {
            if (v->list.v[i].type == ST_NUM) out[n++] = v->list.v[i].num;
        }
        for (int i = n; i < 3; i++) out[i] = 0.0;
        return true;
    }
    if (strcmp(e->type, "AXIS2_PLACEMENT_3D") == 0) {
        Stok *axis = ent_arg(e, 2);
        return get_dir(tok_ent(axis), out);
    }
    return false;
}

static bool placement(Sent *axis2, double center[3], double axis[3], double ref[3]) {
    if (!axis2) return false;
    if (!get_point(axis2, center)) return false;
    if (!get_dir(axis2, axis)) { axis[0] = 0; axis[1] = 0; axis[2] = 1; }
    Stok *rd = ent_arg(axis2, 3);
    if (!get_dir(tok_ent(rd), ref)) {
        // build a perpendicular
        double a = fabs(axis[0]), b = fabs(axis[1]), c = fabs(axis[2]);
        if (a <= b && a <= c) { ref[0] = 1; ref[1] = 0; ref[2] = 0; }
        else if (b <= c) { ref[0] = 0; ref[1] = 1; ref[2] = 0; }
        else { ref[0] = 0; ref[1] = 0; ref[2] = 1; }
        // orthogonalize
        double d = ref[0]*axis[0] + ref[1]*axis[1] + ref[2]*axis[2];
        ref[0] -= d*axis[0]; ref[1] -= d*axis[1]; ref[2] -= d*axis[2];
    }
    return true;
}

// CadMesh builder
typedef struct MBuilder { ARRAY(float) pos; ARRAY(float) nrm; ARRAY(int) idx; } MBuilder;
static void mb_push_tri(MBuilder *b, double v0[3], double v1[3], double v2[3], double n[3]) {
    int base = b->pos.len / 3;
    for (int k = 0; k < 3; k++) {
        double *v = k == 0 ? v0 : (k == 1 ? v1 : v2);
        arr_push(b->pos, (float)v[0]);
        arr_push(b->pos, (float)v[1]);
        arr_push(b->pos, (float)v[2]);
        arr_push(b->nrm, (float)n[0]);
        arr_push(b->nrm, (float)n[1]);
        arr_push(b->nrm, (float)n[2]);
        arr_push(b->idx, base + k);
    }
}

// triangulate planar face
static void mesh_planar_face(MBuilder *b, Sent *face, int skip_holes);

// CadMesh cylindrical face
static void mesh_cyl_face(MBuilder *b, Sent *face);

// ================= tokenizer for an entity =================
static void skip_ws(const char **p) { while (**p && (isspace((unsigned char)**p) || **p == ',')) (*p)++; }

static Stok parse_token(const char **p);

static void parse_list_body(const char **p, Stok *out) {
    // at '('
    (*p)++;
    for (;;) {
        skip_ws(p);
        if (!**p) break;
        if (**p == ')') { (*p)++; break; }
        Stok t; stok_init(&t);
        t = parse_token(p);
        arr_push(out->list, t);
    }
}

static Stok parse_token(const char **p) {
    Stok t; stok_init(&t);
    skip_ws(p);
    if (!**p) return t;
    char c = **p;
    if (c == '(') { t.type = ST_LIST; parse_list_body(p, &t); return t; }
    if (c == '"') {
        (*p)++;
        Buf s; buf_init(&s);
        while (**p && **p != '"') {
            if (**p == '\'' && (*p)[1] == '\'') { buf_append(&s, "'", 1); (*p) += 2; continue; }
            buf_append(&s, (*p), 1);
            (*p)++;
        }
        if (**p) (*p)++;
        t.type = ST_STR;
        t.str = str_dup(buf_cstr(&s));
        buf_free(&s);
        return t;
    }
    if (c == '#') {
        (*p)++;
        int id = 0;
        while (**p && isdigit((unsigned char)**p)) { id = id * 10 + (**p - '0'); (*p)++; }
        t.type = ST_REF;
        t.ref = id;
        return t;
    }
    // symbol or number
    const char *start = *p;
    while (**p && !strchr(",) \t\r\n", **p)) (*p)++;
    char buf[64];
    size_t n = (size_t)(*p - start);
    if (n >= sizeof(buf)) n = sizeof(buf) - 1;
    memcpy(buf, start, n);
    buf[n] = '\0';
    // number?
    char *end = NULL;
    double v = strtod(buf, &end);
    if (end != buf && *end == '\0') {
        t.type = ST_NUM;
        t.num = v;
    } else {
        t.type = ST_SYM;
        t.str = str_dup(buf);
    }
    return t;
}

// parse one entity line: returns id and sets out Sent
static bool parse_entity_line(const char *line, Sent *out) {
    const char *p = line;
    if (*p != '#') return false;
    p++;
    int id = 0;
    while (isdigit((unsigned char)*p)) { id = id * 10 + (*p - '0'); p++; }
    out->id = id;
    while (*p && *p != '=') p++;
    if (*p != '=') return false;
    p++;
    const char *ts = p;
    while (*p && *p != '(') p++;
    size_t tn = (size_t)(p - ts);
    out->type = str_dup_len(ts, tn);
    if (*p != '(') return false;
    // find matching close paren
    int depth = 0;
    const char *q = p;
    bool in_str = false;
    for (; *q; q++) {
        if (in_str) { if (*q == '"') in_str = false; continue; }
        if (*q == '"') { in_str = true; continue; }
        if (*q == '(') depth++;
        if (*q == ')') { depth--; if (depth == 0) break; }
    }
    if (*q != ')') return false;
    // tokenize args substring (between '(' and matching ')')
    const char *ap = p + 1;
    stok_init(&out->args);
    size_t alen = (size_t)(q - ap);
    char *sub = str_dup_len(ap, alen);
    const char *sp = sub;
    for (;;) {
        skip_ws(&sp);
        if (!*sp || *sp == ')') break;
        Stok t = parse_token(&sp);
        arr_push(out->args.list, t);
    }
    free(sub);
    return true;
}

// split data section into entity lines
static char **split_entities(const char *data, size_t len, int *count) {
    ARRAY(char *) lines; arr_init(lines);
    const char *p = data;
    bool in_str = false;
    const char *start = data;
    for (const char *q = data; q < data + len; q++) {
        if (in_str) {
            if (*q == '"') {
                if (q + 1 < data + len && q[1] == '"') { q++; continue; }
                in_str = false;
            }
            continue;
        }
        if (*q == '"') { in_str = true; continue; }
        if (*q == ';') {
            size_t n = (size_t)(q - start);
            char *line = str_dup_len(start, n);
            str_trim(line);
            if (line[0] == '#') arr_push(lines, line);
            else free(line);
            start = q + 1;
        }
    }
    *count = lines.len;
    return lines.v;
}

// ================= main STEP load =================
typedef struct { Sent **v; int len; int cap; } SentList;
static void collect_faces(Sent *shell, SentList *out) {
    if (!shell) return;
    for (int i = 1; i < shell->args.list.len; i++) {
        Stok *a = &shell->args.list.v[i];
        if (a->type == ST_REF) {
            Sent *e = ent(a->ref);
            if (e && strcmp(e->type, "ADVANCED_FACE") == 0) arr_push(*out, e);
        } else if (a->type == ST_LIST) {
            for (int j = 0; j < a->list.len; j++) {
                if (a->list.v[j].type == ST_REF) {
                    Sent *e = ent(a->list.v[j].ref);
                    if (e && strcmp(e->type, "ADVANCED_FACE") == 0) arr_push(*out, e);
                }
            }
        }
    }
}

static void process_solid(Sent *solid, MBuilder *b) {
    // MANIFOLD_SOLID_BREP(name, #shell)
    Stok *shref = ent_arg(solid, 1);
    Sent *shell = tok_ent(shref);
    if (!shell) return;
    ARRAY(Sent *) faces; arr_init(faces);
    collect_faces(shell, (SentList *)&faces);
    for (int i = 0; i < faces.len; i++) {
        Sent *face = faces.v[i];
        Sent *surf = tok_ent(ent_arg(face, 1));
        if (!surf) continue;
        if (strcmp(surf->type, "PLANE") == 0) mesh_planar_face(b, face, 0);
        else if (strcmp(surf->type, "CYLINDRICAL_SURFACE") == 0) mesh_cyl_face(b, face);
    }
    arr_free(faces);
}

bool mesh_load_step(const char *path, CadMesh *out) {
    memset(out, 0, sizeof(*out));
    size_t len = 0;
    char *data = file_read(path, &len);
    if (!data) return false;

    // find DATA section
    const char *data_sec = strstr(data, "DATA;");
    const char *start = data_sec ? data_sec + 5 : data;
    if (data_sec == NULL) { free(data); return false; }

    arr_init(g_ents);
    g_byid = NULL;
    g_maxid = 0;
    // also catch multiple DATA; / ENDSEC
    {
        // just tokenize first DATA section until ENDSEC
        const char *endsec = strstr(start, "ENDSEC");
        size_t slen = endsec ? (size_t)(endsec - start) : (len - (size_t)(start - data));
        int nlines = 0;
        char **lines = split_entities(start, slen, &nlines);
        // first pass: max id
        for (int i = 0; i < nlines; i++) {
            const char *p = lines[i] + 1;
            int id = 0;
            while (*p && isdigit((unsigned char)*p)) { id = id * 10 + (*p - '0'); p++; }
            if (id > g_maxid) g_maxid = id;
        }
        g_byid = (Sent **)calloc((size_t)g_maxid + 1, sizeof(Sent *));
        for (int i = 0; i < nlines; i++) {
            Sent e;
            memset(&e, 0, sizeof(e));
            if (parse_entity_line(lines[i], &e)) {
                arr_push(g_ents, e);
                if (e.id <= g_maxid) g_byid[e.id] = &g_ents.v[g_ents.len - 1];
            }
        }
        for (int i = 0; i < nlines; i++) free(lines[i]);
        free(lines);
    }

    MBuilder b;
    memset(&b, 0, sizeof(b));
    arr_init(b.pos); arr_init(b.nrm); arr_init(b.idx);

    // process all solids
    for (int i = 0; i < g_ents.len; i++) {
        Sent *e = &g_ents.v[i];
        if (strcmp(e->type, "MANIFOLD_SOLID_BREP") == 0 ||
            strcmp(e->type, "BREP_WITH_VOIDS") == 0 ||
            strcmp(e->type, "FACETED_BREP") == 0) {
            process_solid(e, &b);
        }
    }

    bool ok = b.idx.len >= 3;
    if (ok) {
        out->nverts = b.pos.len / 3;
        out->ntris = b.idx.len / 3;
        out->pos = b.pos.v;
        out->nrm = b.nrm.v;
        out->idx = b.idx.v;
        mesh_bbox(out);
    } else {
        arr_free(b.pos); arr_free(b.nrm); arr_free(b.idx);
    }

    for (int i = 0; i < g_ents.len; i++) {
        free(g_ents.v[i].type);
        stok_free(&g_ents.v[i].args);
    }
    arr_free(g_ents);
    free(g_byid);
    free(data);
    return ok && out->valid;
}

// ---------------- planar face ----------------
static void mesh_planar_face(MBuilder *b, Sent *face, int skip_holes) {
    (void)skip_holes;
    Sent *surf = tok_ent(ent_arg(face, 1));
    if (!surf) return;
    // plane normal
    double nrm[3];
    if (!get_dir(tok_ent(ent_arg(surf, 1)), nrm)) { nrm[0] = 0; nrm[1] = 0; nrm[2] = 1; }
    double len_n = sqrt(nrm[0] * nrm[0] + nrm[1] * nrm[1] + nrm[2] * nrm[2]);
    if (len_n > 1e-9) { nrm[0] /= len_n; nrm[1] /= len_n; nrm[2] /= len_n; }
    else { nrm[0] = 0; nrm[1] = 0; nrm[2] = 1; }

    // first bound (outer)
    Stok *bounds = ent_arg(face, 0);
    if (!bounds || bounds->type != ST_LIST) return;
    for (int bi = 0; bi < bounds->list.len; bi++) {
        Stok *bt = &bounds->list.v[bi];
        if (bt->type == ST_LIST) bt = bt->list.len ? &bt->list.v[0] : NULL;
        if (!bt) continue;
        Sent *bound = tok_ent(bt);
        if (!bound) continue;
        // EDGE_LOOP is arg 1
        Sent *loop = tok_ent(ent_arg(bound, 1));
        if (!loop) continue;
        // gather points in order
        ARRAY(double) pts; arr_init(pts);
        for (int i = 1; i < loop->args.list.len; i++) {
            Stok *oe = &loop->args.list.v[i];
            if (oe->type == ST_LIST) oe = oe->list.len ? &oe->list.v[0] : NULL;
            if (!oe) continue;
            // unwrap ORIENTED_EDGE chain: ORIENTED_EDGE(name,start,end,#edge,.sense)
            bool sense = true;
            Sent *ed = tok_ent(oe);
            while (ed && strcmp(ed->type, "ORIENTED_EDGE") == 0) {
                Stok *sn = ent_arg(ed, 4);
                if (sn && sn->type == ST_SYM) {
                    if (strcmp(sn->str, ".T.") == 0) sense = true;
                    else if (strcmp(sn->str, ".F.") == 0) sense = false;
                }
                ed = tok_ent(ent_arg(ed, 3));
            }
            if (!ed) continue;
            // EDGE_CURVE(name,#vstart,#vend,#curve,.same_sense) -> endpoint = sense ? vend : vstart
            Sent *vp = tok_ent(ent_arg(ed, sense ? 2 : 1));
            double pt[3];
            if (get_point(vp, pt)) {
                arr_push(pts, pt[0]);
                arr_push(pts, pt[1]);
                arr_push(pts, pt[2]);
            }
        }
        // fan triangulate
        int n = pts.len / 3;
        if (n >= 3) {
            double *p0 = &pts.v[0];
            for (int i = 1; i + 1 < n; i++) {
                double *pi = &pts.v[i * 3];
                double *pj = &pts.v[(i + 1) * 3];
                // check orientation
                double e1[3] = { pi[0] - p0[0], pi[1] - p0[1], pi[2] - p0[2] };
                double e2[3] = { pj[0] - p0[0], pj[1] - p0[1], pj[2] - p0[2] };
                double cr[3] = { e1[1] * e2[2] - e1[2] * e2[1], e1[2] * e2[0] - e1[0] * e2[2], e1[0] * e2[1] - e1[1] * e2[0] };
                double d = cr[0] * nrm[0] + cr[1] * nrm[1] + cr[2] * nrm[2];
                if (d >= 0) mb_push_tri(b, p0, pi, pj, nrm);
                else mb_push_tri(b, p0, pj, pi, nrm);
            }
        }
        arr_free(pts);
        if (bounds->list.len > 0) break; // only outer bound for MVP
    }
}

// ---------------- cylindrical face ----------------
static void mesh_cyl_face(MBuilder *b, Sent *face) {
    Sent *surf = tok_ent(ent_arg(face, 1));
    if (!surf) return;
    // placement
    double c0[3], ax[3], rx[3];
    if (!placement(tok_ent(ent_arg(surf, 1)), c0, ax, rx)) return;
    double rad = ent_arg(surf, 2) ? ent_arg(surf, 2)->num : 0.0;
    if (rad <= 0) return;

    // collect circles (edges whose curve is CIRCLE)
    Stok *bounds = ent_arg(face, 0);
    if (!bounds || bounds->type != ST_LIST) return;
    ARRAY(double) tc; arr_init(tc);   // t positions along axis
    ARRAY(double) rc; arr_init(rc);   // radii
    for (int bi = 0; bi < bounds->list.len; bi++) {
        Stok *bt = &bounds->list.v[bi];
        if (bt->type == ST_LIST) bt = bt->list.len ? &bt->list.v[0] : NULL;
        if (!bt) continue;
        Sent *bound = tok_ent(bt);
        if (!bound) continue;
        Sent *loop = tok_ent(ent_arg(bound, 1));
        if (!loop) continue;
        for (int i = 1; i < loop->args.list.len; i++) {
            Stok *oe = &loop->args.list.v[i];
            if (oe->type == ST_LIST) oe = oe->list.len ? &oe->list.v[0] : NULL;
            if (!oe) continue;
            Sent *ed = tok_ent(oe);
            while (ed && strcmp(ed->type, "ORIENTED_EDGE") == 0) ed = tok_ent(ent_arg(ed, 3));
            if (!ed) continue;
            Sent *curve = tok_ent(ent_arg(ed, 3));
            if (!curve) continue;
            if (strcmp(curve->type, "CIRCLE") == 0) {
                double cc[3], ca[3], crx[3];
                if (placement(tok_ent(ent_arg(curve, 1)), cc, ca, crx)) {
                    double cr = ent_arg(curve, 2) ? ent_arg(curve, 2)->num : 0.0;
                    double t = (cc[0] - c0[0]) * ax[0] + (cc[1] - c0[1]) * ax[1] + (cc[2] - c0[2]) * ax[2];
                    bool found = false;
                    for (int k = 0; k < tc.len; k++) {
                        if (fabs(tc.v[k] - t) < 1e-6) { found = true; break; }
                    }
                    if (!found) { arr_push(tc, t); arr_push(rc, cr); }
                }
            }
        }
    }
    if (tc.len < 2) { arr_free(tc); arr_free(rc); return; }
    // sort by t
    for (int i = 0; i < tc.len - 1; i++) {
        for (int j = i + 1; j < tc.len; j++) {
            if (tc.v[j] < tc.v[i]) {
                double t = tc.v[i]; tc.v[i] = tc.v[j]; tc.v[j] = t;
                double r = rc.v[i]; rc.v[i] = rc.v[j]; rc.v[j] = r;
            }
        }
    }
    int segs = 48;
    // basis in circle plane
    double u[3], vv[3];
    {
        double tmp[3];
        if (fabs(ax[2]) < 0.9) { tmp[0] = 0; tmp[1] = 0; tmp[2] = 1; }
        else { tmp[0] = 1; tmp[1] = 0; tmp[2] = 0; }
        double cr[3] = { ax[1] * tmp[2] - ax[2] * tmp[1], ax[2] * tmp[0] - ax[0] * tmp[2], ax[0] * tmp[1] - ax[1] * tmp[0] };
        double l = sqrt(cr[0]*cr[0] + cr[1]*cr[1] + cr[2]*cr[2]);
        if (l < 1e-9) { arr_free(tc); arr_free(rc); return; }
        u[0] = cr[0] / l; u[1] = cr[1] / l; u[2] = cr[2] / l;
        vv[0] = ax[1]*u[2] - ax[2]*u[1];
        vv[1] = ax[2]*u[0] - ax[0]*u[2];
        vv[2] = ax[0]*u[1] - ax[1]*u[0];
    }
    // for each pair of consecutive t, generate tube using circle radius of the first circle
    for (int k = 0; k + 1 < tc.len; k++) {
        double t0 = tc.v[k], t1 = tc.v[k + 1];
        double r = (rc.v[k] > 0) ? rc.v[k] : (rc.v[k + 1] > 0 ? rc.v[k + 1] : rad);
        double c1[3] = { c0[0] + t0 * ax[0], c0[1] + t0 * ax[1], c0[2] + t0 * ax[2] };
        double c2[3] = { c0[0] + t1 * ax[0], c0[1] + t1 * ax[1], c0[2] + t1 * ax[2] };
        double nrm[3] = { ax[0], ax[1], ax[2] };
        for (int s = 0; s < segs; s++) {
            double th0 = (double)s / segs * 2 * 3.14159265358979;
            double th1 = (double)(s + 1) / segs * 2 * 3.14159265358979;
            double p0[3] = { c1[0] + r * (u[0] * cos(th0) + vv[0] * sin(th0)),
                             c1[1] + r * (u[1] * cos(th0) + vv[1] * sin(th0)),
                             c1[2] + r * (u[2] * cos(th0) + vv[2] * sin(th0)) };
            double p1[3] = { c1[0] + r * (u[0] * cos(th1) + vv[0] * sin(th1)),
                             c1[1] + r * (u[1] * cos(th1) + vv[1] * sin(th1)),
                             c1[2] + r * (u[2] * cos(th1) + vv[2] * sin(th1)) };
            double p2[3] = { c2[0] + r * (u[0] * cos(th1) + vv[0] * sin(th1)),
                             c2[1] + r * (u[1] * cos(th1) + vv[1] * sin(th1)),
                             c2[2] + r * (u[2] * cos(th1) + vv[2] * sin(th1)) };
            double p3[3] = { c2[0] + r * (u[0] * cos(th0) + vv[0] * sin(th0)),
                             c2[1] + r * (u[1] * cos(th0) + vv[1] * sin(th0)),
                             c2[2] + r * (u[2] * cos(th0) + vv[2] * sin(th0)) };
            mb_push_tri(b, p0, p1, p2, nrm);
            mb_push_tri(b, p0, p2, p3, nrm);
        }
    }
    arr_free(tc);
    arr_free(rc);
}
