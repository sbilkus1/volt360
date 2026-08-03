#include "live_drc.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

void drc_init(DrcEngine *drc) {
    memset(drc, 0, sizeof(*drc));
    drc_set_rules_default(drc);
}

void drc_set_rules_default(DrcEngine *drc) {
    drc->rules.min_track_width = 0.15f;
    drc->rules.min_clearance = 0.20f;
    drc->rules.min_track_pad = 0.20f;
    drc->rules.min_via_drill = 0.30f;
    drc->rules.min_via_annular = 0.15f;
    drc->rules.min_hole_hole = 0.50f;
    drc->rules.min_silkscreen = 0.20f;
    drc->rules.enabled = 1;
}

static void drc_add_violation(DrcEngine *drc, V2 pos, DrcSeverity sev, const char *msg, int track_idx, int via_idx) {
    int n = drc->nviolations;
    DrcViolation *v = (DrcViolation *)realloc(drc->violations, sizeof(DrcViolation) * (size_t)(n + 1));
    if (!v) return;
    drc->violations = v;
    memset(&drc->violations[n], 0, sizeof(DrcViolation));
    drc->violations[n].pos = pos;
    drc->violations[n].severity = sev;
    snprintf(drc->violations[n].message, sizeof(drc->violations[n].message), "%s", msg);
    drc->violations[n].track_idx = track_idx;
    drc->violations[n].via_idx = via_idx;
    drc->nviolations = n + 1;
}

static float dist_seg_point(V2 a, V2 b, V2 p) {
    V2 ab = v2(b.x - a.x, b.y - a.y);
    V2 ap = v2(p.x - a.x, p.y - a.y);
    float ab2 = ab.x * ab.x + ab.y * ab.y;
    if (ab2 < 1e-6f) return sqrtf(ap.x * ap.x + ap.y * ap.y);
    float t = (ap.x * ab.x + ap.y * ab.y) / ab2;
    if (t < 0.0f) return sqrtf(ap.x * ap.x + ap.y * ap.y);
    if (t > 1.0f) {
        V2 bp = v2(p.x - b.x, p.y - b.y);
        return sqrtf(bp.x * bp.x + bp.y * bp.y);
    }
    V2 proj = v2(a.x + t * ab.x, a.y + t * ab.y);
    V2 dp = v2(p.x - proj.x, p.y - proj.y);
    return sqrtf(dp.x * dp.x + dp.y * dp.y);
}

static float dist_rect_point(V2 rmin, V2 rmax, V2 p) {
    float dx = p.x < rmin.x ? rmin.x - p.x : p.x > rmax.x ? p.x - rmax.x : 0.0f;
    float dy = p.y < rmin.y ? rmin.y - p.y : p.y > rmax.y ? p.y - rmax.y : 0.0f;
    return sqrtf(dx * dx + dy * dy);
}

static float dist_seg_rect(V2 a, V2 b, V2 rmin, V2 rmax) {
    float d = dist_seg_point(a, b, rmin);
    float t = dist_seg_point(a, b, rmax); if (t < d) d = t;
    t = dist_seg_point(a, b, v2(rmin.x, rmax.y)); if (t < d) d = t;
    t = dist_seg_point(a, b, v2(rmax.x, rmin.y)); if (t < d) d = t;
    V2 mid = v2((rmin.x + rmax.x) * 0.5f, (rmin.y + rmax.y) * 0.5f);
    t = dist_rect_point(rmin, rmax, a); if (t < d) d = t;
    t = dist_rect_point(rmin, rmax, b); if (t < d) d = t;
    return d;
}

void live_drc_check(DrcEngine *drc, const Pcb *pcb) {
    if (!drc || !pcb) return;
    if (!drc->rules.enabled) return;

    free(drc->violations);
    drc->violations = NULL;
    drc->nviolations = 0;

    float min_cw = drc->rules.min_track_width;
    float min_cl = drc->rules.min_clearance;
    float min_tp = drc->rules.min_track_pad;
    float min_drill = drc->rules.min_via_drill;
    float min_ann = drc->rules.min_via_annular;
    float min_hh = drc->rules.min_hole_hole;
    float min_ss = drc->rules.min_silkscreen;

    /* 1. Track width check */
    for (int i = 0; i < pcb->ntracks; i++) {
        if (pcb->tracks[i].width < min_cw) {
            char ms[128];
            snprintf(ms, sizeof(ms), "Track %d width %.2f < min %.2f mm", i, pcb->tracks[i].width, min_cw);
            V2 mid = v2((pcb->tracks[i].a.x + pcb->tracks[i].b.x) * 0.5f,
                        (pcb->tracks[i].a.y + pcb->tracks[i].b.y) * 0.5f);
            drc_add_violation(drc, mid, DRC_ERROR, ms, i, -1);
        }
    }

    /* 2. Track-to-track clearance (same layer assumption) */
    for (int i = 0; i < pcb->ntracks; i++) {
        for (int j = i + 1; j < pcb->ntracks; j++) {
            float d = dist_seg_point(pcb->tracks[i].a, pcb->tracks[i].b, pcb->tracks[j].a);
            float t = dist_seg_point(pcb->tracks[i].a, pcb->tracks[i].b, pcb->tracks[j].b); if (t < d) d = t;
            t = dist_seg_point(pcb->tracks[j].a, pcb->tracks[j].b, pcb->tracks[i].a); if (t < d) d = t;
            t = dist_seg_point(pcb->tracks[j].a, pcb->tracks[j].b, pcb->tracks[i].b); if (t < d) d = t;

            float ef = d - pcb->tracks[i].width * 0.5f - pcb->tracks[j].width * 0.5f;
            if (ef < min_cl) {
                char ms[128];
                snprintf(ms, sizeof(ms), "Track %d-%d clearance %.2f < %.2f mm", i, j, ef, min_cl);
                V2 mid = v2((pcb->tracks[i].a.x + pcb->tracks[j].a.x) * 0.5f,
                            (pcb->tracks[i].a.y + pcb->tracks[j].a.y) * 0.5f);
                drc_add_violation(drc, mid, DRC_ERROR, ms, i, -1);
            }
        }
    }

    /* 3. Track-to-pad clearance */
    for (int i = 0; i < pcb->ntracks; i++) {
        for (int j = 0; j < pcb->nfps; j++) {
            float half_w = pcb->tracks[i].a.x == pcb->tracks[i].b.x ? 2.0f : 2.0f;
            V2 rmin = v2(pcb->fps[j].pos.x - half_w, pcb->fps[j].pos.y - half_w);
            V2 rmax = v2(pcb->fps[j].pos.x + half_w, pcb->fps[j].pos.y + half_w);
            float d = dist_seg_rect(pcb->tracks[i].a, pcb->tracks[i].b, rmin, rmax);
            if (d < min_tp) {
                char ms[128];
                snprintf(ms, sizeof(ms), "Track %d near pad %s dist %.2f mm", i,
                         pcb->fps[j].ref ? pcb->fps[j].ref : "?", d);
                drc_add_violation(drc, pcb->fps[j].pos, DRC_ERROR, ms, i, -1);
            }
        }
    }

    /* 4. Via annular ring check */
    for (int i = 0; i < pcb->nvias; i++) {
        float ann = (pcb->vias[i].outer - pcb->vias[i].drill) * 0.5f;
        if (pcb->vias[i].drill < min_drill) {
            char ms[128];
            snprintf(ms, sizeof(ms), "Via %d drill %.2f < min %.2f mm", i, pcb->vias[i].drill, min_drill);
            drc_add_violation(drc, pcb->vias[i].pos, DRC_ERROR, ms, -1, i);
        }
        if (ann < min_ann) {
            char ms[128];
            snprintf(ms, sizeof(ms), "Via %d annular ring %.2f < min %.2f mm", i, ann, min_ann);
            drc_add_violation(drc, pcb->vias[i].pos, DRC_ERROR, ms, -1, i);
        }
    }

    /* 5. Hole-to-hole clearance */
    for (int i = 0; i < pcb->nvias; i++) {
        for (int j = i + 1; j < pcb->nvias; j++) {
            float dx = pcb->vias[i].pos.x - pcb->vias[j].pos.x;
            float dy = pcb->vias[i].pos.y - pcb->vias[j].pos.y;
            float d = sqrtf(dx * dx + dy * dy);
            float min_edge = d - pcb->vias[i].drill * 0.5f - pcb->vias[j].drill * 0.5f;
            if (min_edge < min_hh) {
                char ms[128];
                snprintf(ms, sizeof(ms), "Via %d-%d hole clearance %.2f < %.2f mm", i, j, min_edge, min_hh);
                V2 mid = v2((pcb->vias[i].pos.x + pcb->vias[j].pos.x) * 0.5f,
                            (pcb->vias[i].pos.y + pcb->vias[j].pos.y) * 0.5f);
                drc_add_violation(drc, mid, DRC_ERROR, ms, -1, i);
            }
        }
    }

    /* 6. Silkscreen overlap check: check each prim against pads */
    for (int i = 0; i < pcb->nprims; i++) {
        if (!pcb->prims[i].layer) continue;
        if (!strstr(pcb->prims[i].layer, "Silk") && !strstr(pcb->prims[i].layer, "silk")) continue;
        for (int j = 0; j < pcb->nfps; j++) {
            V2 rmin = v2(pcb->fps[j].pos.x - 2.0f, pcb->fps[j].pos.y - 2.0f);
            V2 rmax = v2(pcb->fps[j].pos.x + 2.0f, pcb->fps[j].pos.y + 2.0f);
            float d;
            if (pcb->prims[i].type == 3) {
                d = dist_seg_rect(pcb->prims[i].p1, pcb->prims[i].p2, rmin, rmax);
            } else {
                V2 cen = pcb->prims[i].type == 1 ? pcb->prims[i].p1 : v2(0, 0);
                float dx = cen.x - pcb->fps[j].pos.x;
                float dy = cen.y - pcb->fps[j].pos.y;
                d = sqrtf(dx * dx + dy * dy) - 2.0f;
            }
            if (d < min_ss) {
                char ms[128];
                snprintf(ms, sizeof(ms), "Silkscreen near pad %s dist %.2f mm",
                         pcb->fps[j].ref ? pcb->fps[j].ref : "?", d);
                drc_add_violation(drc, pcb->fps[j].pos, DRC_WARNING, ms, -1, -1);
            }
        }
    }

    drc->last_check_frame = 0;
}

void drc_render_overlay(const DrcEngine *drc, const Pcb *pcb, Rectangle viewport, Vector2 pan, float zoom) {
    if (!drc || drc->nviolations == 0) return;

    for (int i = 0; i < drc->nviolations; i++) {
        const DrcViolation *v = &drc->violations[i];

        float sx = viewport.x + (v->pos.x - pan.x) * zoom;
        float sy = viewport.y + (v->pos.y - pan.y) * zoom;

        if (sx < viewport.x - 20 || sx > viewport.x + viewport.width + 20 ||
            sy < viewport.y - 20 || sy > viewport.y + viewport.height + 20)
            continue;

        Color base = v->severity == DRC_ERROR ? RED : YELLOW;
        Color glow = base;
        glow.a = (unsigned char)(60 + (int)(195.0f * (0.5f + 0.5f * sinf(drc->glow_timer * 3.0f + (float)i))));

        float rad = 2.0f + 2.0f * (0.5f + 0.5f * sinf(drc->glow_timer * 3.0f + (float)i * 0.7f));

        DrawCircleLines((int)sx, (int)sy, rad * zoom, glow);
        DrawCircle((int)sx, (int)sy, rad * 0.3f * zoom, base);

        if (i == drc->highlighted_violation) {
            DrawCircleLines((int)sx, (int)sy, (rad + 1) * zoom, WHITE);
            DrawCircleLines((int)sx, (int)sy, (rad + 2) * zoom, WHITE);
        }

        if (v->track_idx >= 0 && v->track_idx < pcb->ntracks) {
            V2 ta = pcb->tracks[v->track_idx].a;
            V2 tb = pcb->tracks[v->track_idx].b;
            float tx1 = viewport.x + (ta.x - pan.x) * zoom;
            float ty1 = viewport.y + (ta.y - pan.y) * zoom;
            float tx2 = viewport.x + (tb.x - pan.x) * zoom;
            float ty2 = viewport.y + (tb.y - pan.y) * zoom;
            DrawLine((int)tx1, (int)ty1, (int)tx2, (int)ty2, (Color){ 255, 50, 50, 80 });
        }
    }

    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < drc->nviolations; i++) {
        const DrcViolation *v = &drc->violations[i];
        float sx = viewport.x + (v->pos.x - pan.x) * zoom;
        float sy = viewport.y + (v->pos.y - pan.y) * zoom;
        float dx = mouse.x - sx;
        float dy = mouse.y - sy;
        if (dx * dx + dy * dy < 64.0f) {
            int tw = MeasureText(v->message, 12);
            DrawRectangle((int)(sx + 8), (int)(sy - 20), tw + 8, 20, (Color){ 0, 0, 0, 200 });
            DrawText(v->message, (int)(sx + 12), (int)(sy - 16), 12, WHITE);
        }
    }
}

int drc_violation_count(const DrcEngine *drc, DrcSeverity min_sev) {
    if (!drc) return 0;
    int count = 0;
    for (int i = 0; i < drc->nviolations; i++) {
        if (drc->violations[i].severity >= min_sev) count++;
    }
    return count;
}

char *drc_report(const DrcEngine *drc) {
    if (!drc) return str_dup("DRC: no engine");
    Buf b;
    buf_init(&b);
    if (drc->nviolations == 0) {
        buf_append_str(&b, "DRC passed: no violations found.");
    } else {
        buf_appendf(&b, "DRC: %d violation(s) found.\n", drc->nviolations);
        for (int i = 0; i < drc->nviolations; i++) {
            const char *lvl = drc->violations[i].severity == DRC_ERROR ? "ERROR" : "WARNING";
            buf_appendf(&b, "  [%s] %s\n", lvl, drc->violations[i].message);
        }
    }
    char *r = buf_cstr(&b);
    char *dup = str_dup(r);
    buf_free(&b);
    return dup;
}
