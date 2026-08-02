#include "lib3d.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static ModelLibrary g_libs[8]; static int nlibs = 0;

int lib3d_create(Project *p, const char *name) {
    (void)p;
    if (nlibs >= 8) return -1;
    memset(&g_libs[nlibs], 0, sizeof(ModelLibrary));
    snprintf(g_libs[nlibs].name, sizeof(g_libs[nlibs].name), "%s", name ? name : "lib");
    return nlibs++;
}

bool lib3d_add_model(Project *p, const char *lib_name, const char *model_id) {
    (void)p;
    for (int i = 0; i < nlibs; i++)
        if (strcmp(g_libs[i].name, lib_name) == 0 && g_libs[i].n_models < 64) {
            g_libs[i].model_ids[g_libs[i].n_models] = model_id ? str_dup(model_id) : str_dup("?");
            g_libs[i].n_models++;
            return true;
        }
    return false;
}

char *lib3d_list_report(Project *p) {
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== MODEL LIBRARIES ===\n");
    for (int i = 0; i < nlibs; i++) {
        off += snprintf(buf+off, sizeof(buf)-off, "  %s: %d models\n", g_libs[i].name, g_libs[i].n_models);
    }
    off += snprintf(buf+off, sizeof(buf)-off, "\n=== SYMBOL LIBRARY ===\n");
    off += snprintf(buf+off, sizeof(buf)-off, "  Total symbols: %d\n", p ? p->symbols.len : 0);
    for (int i = 0; i < (p ? p->symbols.len : 0); i++)
        off += snprintf(buf+off, sizeof(buf)-off, "  [S] %s (%d pins)\n",
            p->symbols.v[i].name ? p->symbols.v[i].name : "?", p->symbols.v[i].npins);
    return str_dup(buf);
}

int libsym_add_from_template(Project *p, const char *name, const char *ref_prefix, int n_pins) {
    if (!p || !name) return -1;
    Symbol s; memset(&s, 0, sizeof(s));
    s.name = str_dup(name); s.reference = str_dup(ref_prefix ? ref_prefix : "U");
    s.bmin = v2(-10,-10); s.bmax = v2(10,10); s.has_bbox = 1;
    s.npins = n_pins; s.pins = (Pin *)calloc((size_t)n_pins, sizeof(Pin));
    for (int i = 0; i < n_pins; i++) {
        char num[8]; snprintf(num, sizeof(num), "%d", i+1);
        s.pins[i].name = str_dup("I/O"); s.pins[i].number = str_dup(num);
        s.pins[i].pos = v2(-8 + i*4, 10); s.pins[i].angle = 90;
    }
    arr_push(p->symbols, s);
    return p->symbols.len - 1;
}

char *community_export_presets(Project *p) {
    char buf[1024];
    snprintf(buf, sizeof(buf),
        "=== COMMUNITY EXPORT ===\n"
        "Project: %s\n"
        "Symbols: %d  Footprints: %d  CAD: %d\n"
        "Export: JSON presets for sharing (stub)\n"
        "Planned: .lcp file format for preset exchange\n",
        p ? (p->name ? p->name : "?") : "?",
        p ? p->symbols.len : 0, p ? p->footprints.len : 0, p ? p->cad_models.len : 0);
    return str_dup(buf);
}

char *surface_finish_report(CadMesh *m, float layer_height) {
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[512];
    float zrange = m->bmax.z - m->bmin.z;
    int layers = (int)(zrange / layer_height) + 1;
    float avg_face_area = 0;
    if (m->ntris > 0) {
        for (int t = 0; t < m->ntris; t++) {
            int i0=m->idx[t*3], i1=m->idx[t*3+1], i2=m->idx[t*3+2];
            float e1x=m->pos[i1*3]-m->pos[i0*3], e1y=m->pos[i1*3+1]-m->pos[i0*3+1], e1z=m->pos[i1*3+2]-m->pos[i0*3+2];
            float e2x=m->pos[i2*3]-m->pos[i0*3], e2y=m->pos[i2*3+1]-m->pos[i0*3+1], e2z=m->pos[i2*3+2]-m->pos[i0*3+2];
            float nx=e1y*e2z-e1z*e2y, ny=e1z*e2x-e1x*e2z, nz=e1x*e2y-e1y*e2x;
            avg_face_area += sqrtf(nx*nx+ny*ny+nz*nz)*0.5f;
        }
        avg_face_area /= m->ntris;
    }
    snprintf(buf, sizeof(buf),
        "=== SURFACE FINISH ===\n"
        "Z range: %.1f mm  Layers: %d @%.2fmm\n"
        "Avg face area: %.4f mm²\n"
        "Surface: %s\n"
        "Ironing recommended: %s\n",
        zrange, layers, layer_height, avg_face_area,
        avg_face_area < 0.01f ? "Smooth" : "Faceted",
        zrange > 10.0f ? "Yes (top layers)" : "No (low profile)");
    return str_dup(buf);
}
