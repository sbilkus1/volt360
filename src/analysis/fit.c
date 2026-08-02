#include "fit.h"
#include "../core/util.h"
#include <stdarg.h>
#include <math.h>
#include <stdio.h>

static void add_issue(FitReport *rep, FitLevel lv, double metric, const char *fmt, ...) {
    rep->nissues++;
    rep->issues = (FitIssue *)realloc(rep->issues, sizeof(FitIssue) * (size_t)rep->nissues);
    FitIssue *it = &rep->issues[rep->nissues - 1];
    it->level = lv;
    it->metric = metric;
    char buf[512];
    va_list ap; va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    it->message = str_dup(buf);
}

void fit_report_free(FitReport *rep) {
    for (int i = 0; i < rep->nissues; i++) free(rep->issues[i].message);
    free(rep->issues);
    memset(rep, 0, sizeof(*rep));
}

bool point_in_poly(V2 pt, V2 *poly, int n) {
    if (n < 3) return false;
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        V2 a = poly[i], b = poly[j];
        if (((a.y > pt.y) != (b.y > pt.y)) &&
            (pt.x < (b.x - a.x) * (pt.y - a.y) / (b.y - a.y) + a.x)) inside = !inside;
    }
    return inside;
}

float dist_to_poly(V2 pt, V2 *poly, int n) {
    float best = INFINITY;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        V2 a = poly[i], b = poly[j];
        V2 ab = v2_sub(b, a);
        float l2 = v2_dot(ab, ab);
        float t = l2 > 0 ? (v2_dot(v2_sub(pt, a), ab) / l2) : 0;
        if (t < 0) t = 0; if (t > 1) t = 1;
        V2 closest = v2_add(a, v2_scale(ab, t));
        float d = v2_dist(pt, closest);
        if (d < best) best = d;
    }
    return best;
}

// ---- assembly analysis ----
void fit_analyze_assembly(Project *p, Assembly *a, FitReport *rep) {
    if (!rep) return;
    memset(rep, 0, sizeof(*rep));
    if (!a || !p) return;

    // collect bounding boxes per item
    for (int i = 0; i < a->nitems; i++) {
        PlaceItem *it = &a->items[i];
        for (int j = i + 1; j < a->nitems; j++) {
            PlaceItem *jt = &a->items[j];
            CadMesh *mi = NULL, *mj = NULL;
            if (it->kind == PART_CAD) { CadModel *cm = project_find_cad(p, it->part_id); if (cm) mi = &cm->mesh; }
            else { Pcb *pc = project_find_pcb(p, it->part_id); if (pc && pc->has_bbox) { /* approximate pcb as flat box */ } }
            if (jt->kind == PART_CAD) { CadModel *cm = project_find_cad(p, jt->part_id); if (cm) mj = &cm->mesh; }
            else { Pcb *pc = project_find_pcb(p, jt->part_id); if (pc && pc->has_bbox) { /* flat box */ } }
            if (!mi || !mj || !mi->valid || !mj->valid) continue;

            // translate CadMesh bboxes to world position
            V3 ai = v3_add(mi->bmin, it->pos), bi = v3_add(mi->bmax, it->pos);
            V3 aj = v3_add(mj->bmin, jt->pos), bj = v3_add(mj->bmax, jt->pos);
            float dx = fminf(bi.x, bj.x) - fmaxf(ai.x, aj.x);
            float dy = fminf(bi.y, bj.y) - fmaxf(ai.y, aj.y);
            float dz = fminf(bi.z, bj.z) - fmaxf(ai.z, aj.z);
            if (dx > 0 && dy > 0 && dz > 0) {
                float overlap = fminf(fminf(dx, dy), dz);
                char *nm = it->label ? it->label : (jt->label ? jt->label : "?");
                add_issue(rep, FIT_ERROR, overlap, "collision: '%s' overlaps by %.2f mm", nm, overlap);
            }
        }
    }

    // height clearance: max top surface Z across items
    for (int i = 0; i < a->nitems; i++) {
        PlaceItem *it = &a->items[i];
        if (it->kind != PART_CAD) continue;
        CadModel *cm = project_find_cad(p, it->part_id);
        if (!cm || !cm->mesh.valid) continue;
        float h = (cm->mesh.bmax.z - cm->mesh.bmin.z) + it->pos.z;
        add_issue(rep, FIT_OK, h, "height '%s': %.2f mm", it->label ? it->label : "?", h);
    }
}

// ---- pcb analysis ----
void fit_analyze_pcb(Project *p, Pcb *pcb, FitReport *rep) {
    if (!rep) return;
    memset(rep, 0, sizeof(*rep));
    if (!pcb || !p) return;

    for (int i = 0; i < pcb->nfps; i++) {
        FpInst *f = &pcb->fps[i];
        Footprint *fp = project_find_footprint(p, f->footprint);
        if (!fp) continue;
        if (pcb->noutline >= 3) {
            // sample the footprint bbox corners; warn if any outside board
            V2 lo = v2_add(fp->bmin, f->pos), hi = v2_add(fp->bmax, f->pos);
            V2 corners[4] = { lo, v2(hi.x, lo.y), hi, v2(lo.x, hi.y) };
            int outside = 0;
            for (int c = 0; c < 4; c++) if (!point_in_poly(corners[c], pcb->outline, pcb->noutline)) outside++;
            if (outside > 0) {
                float d = dist_to_poly(v2_add(v2_scale(v2_add(fp->bmin, fp->bmax), 0.5f), f->pos), pcb->outline, pcb->noutline);
                add_issue(rep, FIT_WARNING, d, "footprint '%s' %d/4 corners outside board (%.2f mm)", f->ref, outside, d);
            }
        }
    }

    // footprint overlap
    for (int i = 0; i < pcb->nfps; i++) {
        FpInst *a = &pcb->fps[i];
        Footprint *fa = project_find_footprint(p, a->footprint);
        if (!fa) continue;
        for (int j = i + 1; j < pcb->nfps; j++) {
            FpInst *b = &pcb->fps[j];
            Footprint *fb = project_find_footprint(p, b->footprint);
            if (!fb) continue;
            float dx = fminf(fa->bmax.x, fb->bmax.x) - fmaxf(fa->bmin.x, fb->bmin.x);
            float dy = fminf(fa->bmax.y, fb->bmax.y) - fmaxf(fa->bmin.y, fb->bmin.y);
            if (dx > 0 && dy > 0) {
                float overlap = fminf(dx, dy);
                add_issue(rep, FIT_WARNING, overlap, "overlap: '%s' vs '%s' (%.2f mm)", a->ref, b->ref, overlap);
            }
        }
    }
}
