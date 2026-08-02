#include "pushroute.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static float seg_dist(V2 a1, V2 a2, V2 b1, V2 b2) {
    // minimum distance between two line segments
    V2 da = v2(a2.x - a1.x, a2.y - a1.y);
    V2 db = v2(b2.x - b1.x, b2.y - b1.y);
    float best = 1e30f;
    // check endpoint distances
    float d1 = sqrtf((a1.x-b1.x)*(a1.x-b1.x) + (a1.y-b1.y)*(a1.y-b1.y));
    float d2 = sqrtf((a1.x-b2.x)*(a1.x-b2.x) + (a1.y-b2.y)*(a1.y-b2.y));
    float d3 = sqrtf((a2.x-b1.x)*(a2.x-b1.x) + (a2.y-b1.y)*(a2.y-b1.y));
    float d4 = sqrtf((a2.x-b2.x)*(a2.x-b2.x) + (a2.y-b2.y)*(a2.y-b2.y));
    if (d1 < best) best = d1; if (d2 < best) best = d2;
    if (d3 < best) best = d3; if (d4 < best) best = d4;
    return best;
}

int pushroute_add(Pcb *pcb, V2 a, V2 b, float width, float min_clearance) {
    if (!pcb) return -1;
    float total_clr = width * 0.5f + min_clearance;

    // check clearance with existing tracks
    for (int i = 0; i < pcb->ntracks; i++) {
        float clr = total_clr + pcb->tracks[i].width * 0.5f;
        float d = seg_dist(a, b, pcb->tracks[i].a, pcb->tracks[i].b);
        if (d < clr) {
            // push the existing track perpendicularly
            V2 dir = v2(b.x - a.x, b.y - a.y);
            float len = sqrtf(dir.x*dir.x + dir.y*dir.y);
            if (len < 0.01f) continue;
            V2 perp = v2(-dir.y / len, dir.x / len);
            float push_dist = clr - d + 0.05f;
            // determine which side to push
            V2 mid = v2((pcb->tracks[i].a.x + pcb->tracks[i].b.x) * 0.5f,
                       (pcb->tracks[i].a.y + pcb->tracks[i].b.y) * 0.5f);
            V2 line_mid = v2((a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f);
            V2 to_mid = v2(mid.x - line_mid.x, mid.y - line_mid.y);
            float sign = (perp.x * to_mid.x + perp.y * to_mid.y) > 0 ? 1.0f : -1.0f;
            pcb->tracks[i].a.x += perp.x * push_dist * sign;
            pcb->tracks[i].a.y += perp.y * push_dist * sign;
            pcb->tracks[i].b.x += perp.x * push_dist * sign;
            pcb->tracks[i].b.y += perp.y * push_dist * sign;
        }
    }

    // add the new track
    Track t; t.a = a; t.b = b; t.width = width; t.net = NULL;
    pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track) * (size_t)(pcb->ntracks + 1));
    pcb->tracks[pcb->ntracks++] = t;
    return 0;
}
