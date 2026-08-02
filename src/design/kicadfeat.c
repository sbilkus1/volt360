#include "kicadfeat.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// === CvPcb ===
int cvpcb_auto_assign(Project *p) {
    if (!p) return 0;
    int assigned = 0;
    for (int i = 0; i < p->components.len; i++) {
        Component *c = &p->components.v[i];
        if (c->footprint && c->footprint[0]) continue; // already assigned
        // find matching footprint by name
        const char *best = NULL;
        for (int j = 0; j < p->footprints.len; j++) {
            Footprint *f = &p->footprints.v[j];
            if (f->name && c->package && strstr(f->name, c->package)) { best = f->name; break; }
        }
        if (!best && p->footprints.len > 0) best = p->footprints.v[0].name;
        if (best) {
            c->footprint = str_dup(best);
            assigned++;
        }
    }
    return assigned;
}

char *cvpcb_suggest_footprint(const char *name, Footprint *fps, int n) {
    char buf[256];
    if (!name || n < 1) { snprintf(buf, sizeof(buf), "No suggestion"); return str_dup(buf); }
    for (int i = 0; i < n; i++)
        if (fps[i].name && strstr(fps[i].name, name)) {
            snprintf(buf, sizeof(buf), "%s (match)", fps[i].name);
            return str_dup(buf);
        }
    snprintf(buf, sizeof(buf), "%s (first available)", fps[0].name);
    return str_dup(buf);
}

// === PAGE LAYOUT ===
char *pagelayout_title_block(const char *title, const char *author, const char *date, const char *rev, float w, float h) {
    char buf[1024];
    snprintf(buf, sizeof(buf),
        "+--------------------------------------+\n"
        "|  %-35s |\n"
        "|  Author: %-28s |\n"
        "|  Date: %-30s |\n"
        "|  Rev: %-31s |\n"
        "|  Size: %-29.0fx%.0f |\n"
        "+--------------------------------------+\n",
        title ? title : "Untitled", author ? author : "?", date ? date : "?", rev ? rev : "A", w, h);
    return str_dup(buf);
}

char *pagelayout_full_frame(const char *name, float w, float h) {
    char buf[512];
    snprintf(buf, sizeof(buf), "Page: %.0fx%.0f mm  %s\n"
        "  +---+ Title Block ---+---+ Border: 5mm |\n"
        "  | Zones: A B C D 1-4 | Grid: 2.5mm   |\n"
        "  +--------------------+---------------+", w, h, name ? name : "Project");
    return str_dup(buf);
}

// === NETLIST EXPORT ===
bool netlist_export_pads(Schematic *s, const char *path) {
    if (!s || !path) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "*PADS-PCB*\n*PART*\n");
    for (int i = 0; i < s->ninsts; i++)
        fprintf(f, "%s %s\n", s->insts[i].ref ? s->insts[i].ref : "?",
            s->insts[i].value ? s->insts[i].value : "?");
    fprintf(f, "*NET*\n*SIGNAL* N00001\n");
    for (int i = 0; i < s->ninsts; i++)
        fprintf(f, "%s.1\n", s->insts[i].ref ? s->insts[i].ref : "?");
    fprintf(f, "*END*\n"); fclose(f); return true;
}

bool netlist_export_allegro(Schematic *s, const char *path) {
    if (!s || !path) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "$PACKAGES\n");
    for (int i = 0; i < s->ninsts; i++)
        fprintf(f, "%s ! %s ! %s ;\n", s->insts[i].ref ? s->insts[i].ref : "?",
            s->insts[i].symbol ? s->insts[i].symbol : "?",
            s->insts[i].value ? s->insts[i].value : "?");
    fprintf(f, "$NETS\n'GND'; GND\n");
    for (int i = 0; i < s->ninsts; i++)
        fprintf(f, "'%s'; %s.1\n", s->insts[i].ref ? s->insts[i].ref : "?",
            s->insts[i].ref ? s->insts[i].ref : "?");
    fprintf(f, "$END\n"); fclose(f); return true;
}

bool netlist_export_kicad_net(Schematic *s, const char *path) {
    if (!s || !path) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "(export (version D) (design \"%s\") (components\n", s->name ? s->name : "schematic");
    for (int i = 0; i < s->ninsts; i++)
        fprintf(f, "  (comp (ref %s) (value %s))\n", s->insts[i].ref ? s->insts[i].ref : "?",
            s->insts[i].value ? s->insts[i].value : "?");
    fprintf(f, "))\n"); fclose(f); return true;
}

bool netlist_export_all(Schematic *s, const char *dir) {
    if (!s || !dir) return false;
    char path[256]; int ok = 0;
    snprintf(path, sizeof(path), "%s/%s.pads", dir, s->name ? s->name : "net"); if (netlist_export_pads(s, path)) ok++;
    snprintf(path, sizeof(path), "%s/%s.allegro", dir, s->name ? s->name : "net"); if (netlist_export_allegro(s, path)) ok++;
    snprintf(path, sizeof(path), "%s/%s.kicad_net", dir, s->name ? s->name : "net"); if (netlist_export_kicad_net(s, path)) ok++;
    return ok > 0;
}

// === DIFFERENTIAL PAIR ===
int diffpair_route_interactive(Pcb *pcb, V2 sp, V2 sn, V2 ep, V2 en, float w, float spacing) {
    return difroute_add_pair(pcb, sp, ep, sn, en, w, spacing);
}

int diffpair_phase_match(Pcb *pcb, int ta, int tb, float tol) {
    if (!pcb || ta < 0 || ta >= pcb->ntracks || tb < 0 || tb >= pcb->ntracks) return 0;
    float la = sqrtf((pcb->tracks[ta].b.x-pcb->tracks[ta].a.x)*(pcb->tracks[ta].b.x-pcb->tracks[ta].a.x)+(pcb->tracks[ta].b.y-pcb->tracks[ta].a.y)*(pcb->tracks[ta].b.y-pcb->tracks[ta].a.y));
    float lb = sqrtf((pcb->tracks[tb].b.x-pcb->tracks[tb].a.x)*(pcb->tracks[tb].b.x-pcb->tracks[tb].a.x)+(pcb->tracks[tb].b.y-pcb->tracks[tb].a.y)*(pcb->tracks[tb].b.y-pcb->tracks[tb].a.y));
    if (fabsf(la - lb) < tol) return 0;
    return lengthtune_add_meanders(pcb, la < lb ? pcb->tracks[ta].net : pcb->tracks[tb].net,
        la > lb ? la : lb, 1.0f, 0.5f);
}

// === VALIDATOR ===
char *validator_check(Project *p) {
    if (!p) return str_dup("no project");
    char buf[1024]; int off = 0, issues = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== COMPONENT VALIDATOR ===\n");
    for (int i = 0; i < p->components.len; i++) {
        Component *c = &p->components.v[i];
        if (!c->footprint || !c->footprint[0]) {
            off += snprintf(buf+off, sizeof(buf)-off, "  %s: no footprint assigned\n", c->name ? c->name : "?");
            issues++;
        }
    }
    if (issues == 0) off += snprintf(buf+off, sizeof(buf)-off, "  All components validated\n");
    return str_dup(buf);
}

// === LIBRARY TABLE ===
void libtable_add(LibTableEntry *table, int *count, const char *nick, const char *uri) {
    snprintf(table[*count].nickname, 32, "%s", nick ? nick : "");
    snprintf(table[*count].uri, 256, "%s", uri ? uri : "");
    snprintf(table[*count].type, 16, "KiCad");
    (*count)++;
}

char *libtable_export(LibTableEntry *table, int count) {
    char buf[1024]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "(fp_lib_table\n");
    for (int i = 0; i < count; i++)
        off += snprintf(buf+off, sizeof(buf)-off, "  (lib (name %s)(uri %s)(type %s))\n", table[i].nickname, table[i].uri, table[i].type);
    off += snprintf(buf+off, sizeof(buf)-off, ")\n");
    return str_dup(buf);
}

char *libtable_get_default(void) {
    return str_dup("(fp_lib_table\n  (lib (name Connector)(uri ${KICAD8_FOOTPRINT_DIR}/Connector.pretty)(type KiCad))\n  (lib (name Resistor)(uri ${KICAD8_FOOTPRINT_DIR}/Resistor_SMD.pretty)(type KiCad))\n  (lib (name Capacitor)(uri ${KICAD8_FOOTPRINT_DIR}/Capacitor_SMD.pretty)(type KiCad))\n)\n");
}

// === IDF EXPORT ===
bool idf_export_board(Pcb *pcb, const char *path) {
    if (!pcb || !path) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, ".HEADER\nBOARD_FILE 3.0 \"Volt360\" .END_HEADER\n");
    fprintf(f, ".BOARD_OUTLINE MCAD\n");
    for (int i = 0; i < pcb->noutline; i++)
        fprintf(f, "%.3f %.3f 0\n", pcb->outline[i].x, pcb->outline[i].y);
    fprintf(f, "%.3f %.3f 0\n", pcb->outline[0].x, pcb->outline[0].y);
    fprintf(f, ".END_BOARD_OUTLINE\n");
    for (int i = 0; i < pcb->nfps; i++)
        fprintf(f, ".PLACEMENT\n\"%s\" \"%s\"\n%.3f %.3f %.3f FRONT\n.END_PLACEMENT\n",
            pcb->fps[i].ref ? pcb->fps[i].ref : "?", pcb->fps[i].footprint ? pcb->fps[i].footprint : "?",
            pcb->fps[i].pos.x, pcb->fps[i].pos.y, pcb->fps[i].rotation);
    fprintf(f, ".END_BOARD_FILE\n"); fclose(f); return true;
}

// === STEP EXPORT ===
bool step_export_assembly(Project *p, const char *path) {
    return step_export(p->cad_models.len > 0 ? &p->cad_models.v[0].mesh : NULL, path);
}
