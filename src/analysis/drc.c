#include "drc.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static void drc_add(DrcReport *r, int level, const char *msg, V2 pos) {
    r->issues = (DrcIssue *)realloc(r->issues, sizeof(DrcIssue) * (size_t)(r->nissues + 1));
    DrcIssue *d = &r->issues[r->nissues++];
    d->level = level; d->pos = pos;
    snprintf(d->message, sizeof(d->message), "%s", msg);
}

void drc_report_free(DrcReport *r) { free(r->issues); r->issues = NULL; r->nissues = 0; }

static float dist_seg_point(V2 a, V2 b, V2 p) {
    V2 ab = v2(b.x - a.x, b.y - a.y);
    V2 ap = v2(p.x - a.x, p.y - a.y);
    float ab2 = ab.x * ab.x + ab.y * ab.y;
    if (ab2 < 1e-6f) return sqrtf(ap.x * ap.x + ap.y * ap.y);
    float t = (ap.x * ab.x + ap.y * ab.y) / ab2;
    if (t < 0) return sqrtf(ap.x * ap.x + ap.y * ap.y);
    if (t > 1) {
        V2 bp = v2(p.x - b.x, p.y - b.y);
        return sqrtf(bp.x * bp.x + bp.y * bp.y);
    }
    V2 proj = v2(a.x + t * ab.x, a.y + t * ab.y);
    V2 dp = v2(p.x - proj.x, p.y - proj.y);
    return sqrtf(dp.x * dp.x + dp.y * dp.y);
}

static float dist_seg_seg(V2 a1, V2 a2, V2 b1, V2 b2) {
    float d = dist_seg_point(a1, a2, b1);
    float t = dist_seg_point(a1, a2, b2); if (t < d) d = t;
    t = dist_seg_point(b1, b2, a1); if (t < d) d = t;
    t = dist_seg_point(b1, b2, a2); if (t < d) d = t;
    return d;
}

DrcReport drc_check(Pcb *pcb, float min_track_width, float min_clearance, float min_annular_ring) {
    DrcReport r = {0};
    if (!pcb) return r;

    // 1. track width check
    for (int i = 0; i < pcb->ntracks; i++) {
        if (pcb->tracks[i].width < min_track_width) {
            char msg[256]; snprintf(msg, sizeof(msg), "Track %d width %.2f < min %.2f mm", i, pcb->tracks[i].width, min_track_width);
            drc_add(&r, 2, msg, pcb->tracks[i].a);
        }
    }

    // 2. track-to-track clearance
    for (int i = 0; i < pcb->ntracks; i++) {
        for (int j = i + 1; j < pcb->ntracks; j++) {
            V2 a1 = pcb->tracks[i].a, a2 = pcb->tracks[i].b;
            V2 b1 = pcb->tracks[j].a, b2 = pcb->tracks[j].b;
            float clearance = dist_seg_seg(a1, a2, b1, b2);
            if (clearance < min_clearance) {
                char msg[256]; snprintf(msg, sizeof(msg), "Track %d-%d clearance %.2f < %.2f mm", i, j, clearance, min_clearance);
                drc_add(&r, 2, msg, a1);
            }
        }
    }

    // 3. track-to-pad clearance
    for (int i = 0; i < pcb->ntracks; i++) {
        for (int j = 0; j < pcb->nfps; j++) {
            float d = dist_seg_point(pcb->tracks[i].a, pcb->tracks[i].b, pcb->fps[j].pos);
            if (d < min_clearance + 2.0f) { // approximate pad size
                char msg[256]; snprintf(msg, sizeof(msg), "Track %d near pad %s dist %.2f mm", i, pcb->fps[j].ref ? pcb->fps[j].ref : "?", d);
                drc_add(&r, 1, msg, pcb->tracks[i].a);
            }
        }
    }

    // 4. pad-to-pad overlap check (crude)
    for (int i = 0; i < pcb->nfps; i++) {
        for (int j = i + 1; j < pcb->nfps; j++) {
            float dx = pcb->fps[i].pos.x - pcb->fps[j].pos.x;
            float dy = pcb->fps[i].pos.y - pcb->fps[j].pos.y;
            float d = sqrtf(dx*dx + dy*dy);
            if (d < 1.0f) { // very close pads
                char msg[256]; snprintf(msg, sizeof(msg), "Pad %s-%s overlap dist %.2f mm", pcb->fps[i].ref ? pcb->fps[i].ref : "?", pcb->fps[j].ref ? pcb->fps[j].ref : "?", d);
                drc_add(&r, 2, msg, pcb->fps[i].pos);
            }
        }
    }

    // 5. board outline containment
    if (pcb->noutline >= 3) {
        for (int i = 0; i < pcb->nfps; i++) {
            // simple: check if pad is outside the bbox
            if (pcb->fps[i].pos.x < pcb->bmin.x || pcb->fps[i].pos.x > pcb->bmax.x ||
                pcb->fps[i].pos.y < pcb->bmin.y || pcb->fps[i].pos.y > pcb->bmax.y) {
                char msg[256]; snprintf(msg, sizeof(msg), "Pad %s outside board outline", pcb->fps[i].ref ? pcb->fps[i].ref : "?");
                drc_add(&r, 2, msg, pcb->fps[i].pos);
            }
        }
    }

    // summary
    if (r.nissues == 0) drc_add(&r, 0, "DRC passed: no violations found", v2(0,0));
    else { char msg[256]; snprintf(msg, sizeof(msg), "DRC: %d issue(s) found", r.nissues); drc_add(&r, 0, msg, v2(0,0)); }
    return r;
}
