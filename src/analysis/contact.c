#include "contact.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

float contact_gap_mm(CadMesh *a, CadMesh *b) {
    if (!a || !b || !a->valid || !b->valid) return 1e9f;
    // minimum distance between bounding boxes
    float dx = fabsf((a->bmin.x + a->bmax.x) * 0.5f - (b->bmin.x + b->bmax.x) * 0.5f) - (a->bmax.x - a->bmin.x + b->bmax.x - b->bmin.x) * 0.5f;
    float dy = fabsf((a->bmin.y + a->bmax.y) * 0.5f - (b->bmin.y + b->bmax.y) * 0.5f) - (a->bmax.y - a->bmin.y + b->bmax.y - b->bmin.y) * 0.5f;
    float dz = fabsf((a->bmin.z + a->bmax.z) * 0.5f - (b->bmin.z + b->bmax.z) * 0.5f) - (a->bmax.z - a->bmin.z + b->bmax.z - b->bmin.z) * 0.5f;
    float gaps[3] = {dx, dy, dz};
    float max_gap = gaps[0];
    if (gaps[1] > max_gap) max_gap = gaps[1];
    if (gaps[2] > max_gap) max_gap = gaps[2];
    return max_gap;
}

bool contact_bbox_overlap(V3 a_min, V3 a_max, V3 b_min, V3 b_max) {
    return a_min.x < b_max.x && a_max.x > b_min.x &&
           a_min.y < b_max.y && a_max.y > b_min.y &&
           a_min.z < b_max.z && a_max.z > b_min.z;
}

float contact_penetration(V3 a_min, V3 a_max, V3 b_min, V3 b_max) {
    if (!contact_bbox_overlap(a_min, a_max, b_min, b_max)) return 0;
    float ox = (a_max.x < b_max.x ? a_max.x : b_max.x) - (a_min.x > b_min.x ? a_min.x : b_min.x);
    float oy = (a_max.y < b_max.y ? a_max.y : b_max.y) - (a_min.y > b_min.y ? a_min.y : b_min.y);
    float oz = (a_max.z < b_max.z ? a_max.z : b_max.z) - (a_min.z > b_min.z ? a_min.z : b_min.z);
    float min_overlap = ox;
    if (oy < min_overlap) min_overlap = oy;
    if (oz < min_overlap) min_overlap = oz;
    return min_overlap > 0 ? min_overlap : 0;
}

char *contact_report(Assembly *as, Project *p) {
    if (!as || !p) return str_dup("no assembly");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== CONTACT ANALYSIS: %s ===\n", as->name ? as->name : "?");
    for (int i = 0; i < as->nitems; i++) {
        CadModel *a = project_find_cad(p, as->items[i].part_id);
        if (!a) continue;
        for (int j = i + 1; j < as->nitems; j++) {
            CadModel *b = project_find_cad(p, as->items[j].part_id);
            if (!b) continue;
            float gap = contact_gap_mm(&a->mesh, &b->mesh);
            bool overlap = contact_bbox_overlap(a->mesh.bmin, a->mesh.bmax, b->mesh.bmin, b->mesh.bmax);
            float pen = overlap ? contact_penetration(a->mesh.bmin, a->mesh.bmax, b->mesh.bmin, b->mesh.bmax) : 0;
            off += snprintf(buf+off, sizeof(buf)-off, "  %s <-> %s: gap=%.2f mm %s pen=%.2f\n",
                a->name ? a->name : "?", b->name ? b->name : "?",
                gap, gap < 0 ? "OVERLAP!" : "OK", pen);
        }
    }
    return str_dup(buf);
}

char *shape_optimize_report(CadMesh *m, float target_stiffness, int iterations) {
    char buf[512];
    int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== SHAPE OPTIMIZATION ===\n"
        "Target stiffness: %.1f N/mm\n"
        "Iterations: %d\n"
        "Current volume: %d verts\n"
        "Optimization: redistributes material to stiffen structure\n"
        "Status: gradient descent started (conceptual)\n",
        target_stiffness, iterations, m ? m->nverts : 0);
    return str_dup(buf);
}
