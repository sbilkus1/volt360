#include "length_tune.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

void meander_config_default(MeanderConfig *cfg) { cfg->amplitude = 1.0f; cfg->gap = 0.5f; cfg->min_segment = 0.5f; cfg->max_meanders = 100; }

static float path_length(const V2 *pts, int n) {
    float L = 0;
    for (int i = 1; i < n; i++) { float dx = pts[i].x - pts[i-1].x; float dy = pts[i].y - pts[i-1].y; L += sqrtf(dx*dx+dy*dy); }
    return L;
}

MeanderResult meander_generate(const V2 *original_path, int npoints, float target_length, const MeanderConfig *cfg) {
    MeanderResult r; memset(&r, 0, sizeof(r));
    if (!original_path || npoints < 2 || target_length <= 0) return r;

    r.original_length = path_length(original_path, npoints);
    r.target_length = target_length;
    if (target_length <= r.original_length) { r.success = 1; r.tuned_length = r.original_length; return r; }

    float extra = target_length - r.original_length;
    float meander_add = 2.0f * cfg->amplitude + 2.0f * cfg->gap; /* extra length per meander */
    if (meander_add < 0.1f) meander_add = 0.1f;
    int nmeanders = (int)(extra / meander_add) + 1;
    if (nmeanders > cfg->max_meanders) nmeanders = cfg->max_meanders;

    int out_cap = npoints + nmeanders * 4;
    r.meander_points = (V2 *)malloc(sizeof(V2) * (size_t)out_cap);
    r.npoints = 0;
    int spacing = npoints / (nmeanders + 1);
    if (spacing < 2) spacing = 2;

    for (int i = 0, mi = 0; i < npoints; i++) {
        r.meander_points[r.npoints++] = original_path[i];
        if (i == spacing * (mi + 1) && mi < nmeanders && i < npoints - 1) {
            float dx = original_path[i+1].x - original_path[i].x;
            float dy = original_path[i+1].y - original_path[i].y;
            float len = sqrtf(dx*dx+dy*dy);
            if (len < 0.01f) len = 0.01f;
            V2 dir = v2(dx / len, dy / len);
            V2 perp = v2(-dir.y, dir.x);
            float sign = (mi % 2 == 0) ? 1.0f : -1.0f;
            if (r.npoints + 3 < out_cap) {
                V2 mid = v2(original_path[i].x + dir.x * len * 0.25f, original_path[i].y + dir.y * len * 0.25f);
                V2 p1 = v2(mid.x + perp.x * cfg->amplitude * sign, mid.y + perp.y * cfg->amplitude * sign);
                V2 p2 = v2(mid.x + dir.x * cfg->gap + perp.x * cfg->amplitude * sign, mid.y + dir.y * cfg->gap + perp.y * cfg->amplitude * sign);
                r.meander_points[r.npoints++] = p1;
                r.meander_points[r.npoints++] = p2;
            }
            mi++;
        }
    }

    r.tuned_length = path_length(r.meander_points, r.npoints);
    r.success = (r.tuned_length >= target_length * 0.95f);
    return r;
}

void meander_result_free(MeanderResult *r) { if (r) free(r->meander_points); }

MeanderResult meander_diff_pair(const V2 *trace_a, int na, const V2 *trace_b, int nb, const MeanderConfig *cfg) {
    MeanderResult r; memset(&r, 0, sizeof(r));
    if (!trace_a || !trace_b || na < 2 || nb < 2) return r;
    float la = path_length(trace_a, na), lb = path_length(trace_b, nb);
    float target = la > lb ? la : lb;
    MeanderConfig cfg2 = *cfg;
    if (la < target) return meander_generate(trace_a, na, target, &cfg2);
    if (lb < target) return meander_generate(trace_b, nb, target, &cfg2);
    r.success = 1; r.tuned_length = target; return r;
}

void meander_apply_to_pcb(Pcb *pcb, int track_idx, const MeanderResult *mr) {
    if (!pcb || track_idx < 0 || track_idx >= pcb->ntracks || !mr || !mr->success) return;
    Track *t = &pcb->tracks[track_idx];
    t->a = mr->meander_points[0];
    t->b = mr->meander_points[mr->npoints - 1];
}
