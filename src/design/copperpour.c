#include "copperpour.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static V2 *gp_poly; static int gp_n; static Pcb *gp_pcb; static float gp_clear;

static int pt_in_poly(V2 pt) {
    int in = 0;
    for (int i = 0, j = gp_n - 1; i < gp_n; j = i++)
        if (((gp_poly[i].y > pt.y) != (gp_poly[j].y > pt.y)) &&
            (pt.x < (gp_poly[j].x - gp_poly[i].x) * (pt.y - gp_poly[i].y) /
             (gp_poly[j].y - gp_poly[i].y) + gp_poly[i].x)) in = !in;
    return in;
}

static int near_obj(V2 pt) {
    for (int i = 0; i < gp_pcb->ntracks; i++) {
        float dx = pt.x - gp_pcb->tracks[i].a.x, dy = pt.y - gp_pcb->tracks[i].a.y;
        if (sqrtf(dx*dx+dy*dy) < gp_clear) return 1;
        dx = pt.x - gp_pcb->tracks[i].b.x; dy = pt.y - gp_pcb->tracks[i].b.y;
        if (sqrtf(dx*dx+dy*dy) < gp_clear) return 1;
    }
    for (int i = 0; i < gp_pcb->nfps; i++) {
        float dx = pt.x - gp_pcb->fps[i].pos.x, dy = pt.y - gp_pcb->fps[i].pos.y;
        if (sqrtf(dx*dx+dy*dy) < gp_clear) return 1;
    }
    return 0;
}

int copper_pour_fill(Pcb *pcb, V2 *zone_poly, int n_pts, float clearance, float hatch_spacing, float track_width) {
    if (!pcb || n_pts < 3 || hatch_spacing <= 0) return 0;
    gp_poly = zone_poly; gp_n = n_pts; gp_pcb = pcb; gp_clear = clearance;

    float mnx = zone_poly[0].x, mny = zone_poly[0].y, mxx = mnx, mxy = mny;
    for (int i = 1; i < n_pts; i++) {
        if (zone_poly[i].x < mnx) mnx = zone_poly[i].x;
        if (zone_poly[i].y < mny) mny = zone_poly[i].y;
        if (zone_poly[i].x > mxx) mxx = zone_poly[i].x;
        if (zone_poly[i].y > mxy) mxy = zone_poly[i].y;
    }

    int added = 0;
    float step = 0.5f;
    for (float y = mny + hatch_spacing; y < mxy; y += hatch_spacing) {
        float x_start = -1;
        for (float x = mnx; x <= mxx; x += step) {
            V2 pt = v2(x, y);
            if (pt_in_poly(pt) && !near_obj(pt)) {
                if (x_start < 0) x_start = x;
            } else {
                if (x_start >= 0 && (x - x_start) > hatch_spacing * 0.5f) {
                    Track t; t.a = v2(x_start, y); t.b = v2(x, y);
                    t.width = track_width; t.net = NULL;
                    pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track) * (size_t)(pcb->ntracks + 1));
                    pcb->tracks[pcb->ntracks++] = t;
                    added++;
                }
                x_start = -1;
            }
        }
    }
    return added;
}

int copper_pour_ground_plane(Pcb *pcb, float edge_clearance, float object_clearance, float hatch_spacing) {
    if (!pcb || pcb->noutline < 3) return 0;
    int n = pcb->noutline;
    V2 *zone = (V2 *)malloc(sizeof(V2) * (size_t)n);
    float cx = 0, cy = 0;
    for (int i = 0; i < n; i++) { cx += pcb->outline[i].x; cy += pcb->outline[i].y; }
    cx /= (float)n; cy /= (float)n;
    for (int i = 0; i < n; i++) {
        V2 d = v2(pcb->outline[i].x - cx, pcb->outline[i].y - cy);
        float len = sqrtf(d.x * d.x + d.y * d.y);
        if (len < 0.001f) { zone[i] = pcb->outline[i]; continue; }
        float s = (len - edge_clearance) / len;
        if (s < 0.1f) s = 0.1f;
        zone[i] = v2(cx + d.x * s, cy + d.y * s);
    }
    int result = copper_pour_fill(pcb, zone, n, object_clearance, hatch_spacing, 0.3f);
    free(zone);
    return result;
}
