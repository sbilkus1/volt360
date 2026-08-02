#include "orcafeat.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

void brim_add_mouse_ears(SliceLayer *first_layer, V2 *poly, int n, float ear_radius, float ear_count, float line_w) {
    if (!first_layer || !poly || n < 3 || ear_radius <= 0) return;
    // place ears at polygon vertices
    int ears = (int)ear_count; if (ears < 3) ears = 3; if (ears > n) ears = n;
    int step = n / ears;
    for (int i = 0; i < n; i += step) {
        float cx = poly[i].x, cy = poly[i].y;
        // generate circular brim pad (approximate with concentric rings)
        for (float r = line_w; r <= ear_radius; r += line_w) {
            int segs = (int)(r * 2.0f * (float)M_PI / line_w);
            if (segs < 8) segs = 8;
            int ni = first_layer->npaths++;
            first_layer->paths = (SlicePath *)realloc(first_layer->paths, sizeof(SlicePath)*(size_t)first_layer->npaths);
            SlicePath *p = &first_layer->paths[ni];
            p->kind = 3; // brim type
            p->npts = segs + 1;
            p->width = line_w; p->solid = 1;
            p->pts = (V2 *)malloc(sizeof(V2)*(size_t)(segs + 1));
            for (int j = 0; j <= segs; j++) {
                float a = (float)j * 2.0f * (float)M_PI / (float)segs;
                p->pts[j] = v2(cx + r * cosf(a), cy + r * sinf(a));
            }
        }
    }
}

char *bed_mesh_gcode(const char *printer_type, float x0, float x1, float y0, float y1, int pts) {
    char buf[512];
    if (strstr(printer_type, "klipper"))
        snprintf(buf, sizeof(buf), "BED_MESH_CALIBRATE MESH_MIN=%.0f,%.0f MESH_MAX=%.0f,%.0f PROBE_COUNT=%d,%d",
            x0, y0, x1, y1, pts, pts);
    else
        snprintf(buf, sizeof(buf), "G29 P1 ; auto bed level\nG29 P3 ; fill unprobed\nG29 S0 ; save");
    return str_dup(buf);
}

bool arcs_enabled(const SliceSettings *cfg) {
    return cfg && cfg->farm_mode; // use farm_mode as arcs flag for now
}

char *arc_gcode(float cx, float cy, float ex, float ey, float r, bool cw, float feed) {
    char buf[128];
    snprintf(buf, sizeof(buf), "G%d X%.3f Y%.3f R%.3f F%.0f", cw ? 2 : 3, ex, ey, r, feed);
    return str_dup(buf);
}

void supports_generate_tree(SliceResult *sr, const SliceSettings *cfg, float overhang_deg) {
    if (!sr || sr->nlayers < 2 || !cfg) return;
    // organic tree supports: start from bed, branch upward toward overhangs
    // Simplified: place support pillars at regular intervals on first layer
    SliceLayer *L0 = &sr->layers[0];
    if (!L0 || L0->npaths < 1) return;
    // find outer perimeter
    V2 *outer = NULL; int on = 0;
    for (int j = 0; j < L0->npaths; j++)
        if (L0->paths[j].kind == 0 && L0->paths[j].npts > on) { on = L0->paths[j].npts; outer = L0->paths[j].pts; }
    if (!outer || on < 3) return;

    float spacing = cfg->line_width * 6.0f;
    for (int i = 0; i < on; i += (int)(spacing / cfg->line_width)) {
        V2 pt = outer[i];
        // add support pillar through all layers
        for (int li = 0; li < sr->nlayers; li++) {
            SliceLayer *L = &sr->layers[li];
            V2 pts[2] = { pt, v2(pt.x + 0.1f, pt.y) };
            int ni = L->npaths++;
            L->paths = (SlicePath *)realloc(L->paths, sizeof(SlicePath)*(size_t)L->npaths);
            SlicePath *p = &L->paths[ni];
            p->kind = 2; p->npts = 2; p->width = cfg->line_width * 0.8f; p->solid = 0;
            p->pts = (V2 *)malloc(sizeof(V2)*2);
            p->pts[0] = pts[0]; p->pts[1] = pts[1];
        }
    }
    (void)overhang_deg;
}

static PrinterProfile g_printers[] = {
    {"Voron 2.4", 350, 350, 350, 0.4f, 300, 300, "klipper"},
    {"RatRig V-Core 3.1", 500, 500, 500, 0.4f, 300, 400, "klipper"},
    {"VzBot 330", 330, 330, 330, 0.4f, 350, 500, "klipper"},
    {"Creality Ender-3 V2", 220, 220, 250, 0.4f, 260, 150, "marlin"},
    {"Bambu Lab X1C", 256, 256, 256, 0.4f, 300, 500, "rff"},
    {"Prusa MK4", 250, 210, 220, 0.4f, 290, 200, "marlin"},
    {"Creality K1", 220, 220, 250, 0.4f, 300, 600, "klipper"},
    {"Prusa XL", 360, 360, 360, 0.4f, 290, 200, "marlin"},
};
static int g_nprinters = 8;

PrinterProfile *printer_profile(const char *name) {
    for (int i = 0; i < g_nprinters; i++)
        if (strcmp(g_printers[i].name, name) == 0) return &g_printers[i];
    return NULL;
}

char *printer_profile_list(void) {
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== PRINTER PROFILES ===\n");
    for (int i = 0; i < g_nprinters; i++)
        off += snprintf(buf+off, sizeof(buf)-off, "  %-22s %dx%d Z=%d FW:%s\n",
            g_printers[i].name, (int)g_printers[i].bed_x, (int)g_printers[i].bed_y,
            (int)g_printers[i].bed_z, g_printers[i].firmware);
    return str_dup(buf);
}

void belt_rotate_mesh(CadMesh *m) {
    if (!m || !m->valid) return;
    // rotate 45° around X axis for belt printing
    float cos45 = cosf(45.0f * (float)M_PI / 180.0f);
    float sin45 = sinf(45.0f * (float)M_PI / 180.0f);
    for (int i = 0; i < m->nverts; i++) {
        float y = m->pos[i*3+1], z = m->pos[i*3+2];
        m->pos[i*3+1] = y * cos45 - z * sin45;
        m->pos[i*3+2] = y * sin45 + z * cos45;
    }
    mesh_bbox(m);
}

char *bambu_plugin_status(void) {
    return str_dup(
        "=== BAMBU NETWORKING ===\n"
        "Status: Plugin stub (requires Bambu Studio SDK)\n"
        "Features: FTP upload, video feed, AMS mapping\n"
        "Bambu printers supported via Moonraker-compatible API\n");
}
