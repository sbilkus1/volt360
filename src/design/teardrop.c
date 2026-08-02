#include "teardrop.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int teardrop_add(Pcb *pcb, float teardrop_length, float teardrop_width) {
    if (!pcb || teardrop_length <= 0) return 0;
    int added = 0;
    for (int i = 0; i < pcb->nfps; i++) {
        V2 pad = pcb->fps[i].pos;
        for (int t = 0; t < pcb->ntracks; t++) {
            Track *tr = &pcb->tracks[t];
            float d1 = sqrtf((tr->a.x - pad.x) * (tr->a.x - pad.x) + (tr->a.y - pad.y) * (tr->a.y - pad.y));
            float d2 = sqrtf((tr->b.x - pad.x) * (tr->b.x - pad.x) + (tr->b.y - pad.y) * (tr->b.y - pad.y));
            if (d1 < 0.5f || d2 < 0.5f) {
                // widen the track endpoint near the pad
                if (d1 < d2) tr->width += teardrop_width * 0.5f;
                else tr->width += teardrop_width * 0.5f;
                added++;
            }
        }
    }
    return added;
}

int teardrop_remove(Pcb *pcb, float max_len) {
    if (!pcb || max_len <= 0) return 0;
    int removed = 0;
    for (int t = pcb->ntracks - 1; t >= 0; t--) {
        Track *tr = &pcb->tracks[t];
        float len = sqrtf((tr->b.x - tr->a.x) * (tr->b.x - tr->a.x) + (tr->b.y - tr->a.y) * (tr->b.y - tr->a.y));
        if (len < max_len) {
            // check if near a pad
            bool near_pad = false;
            for (int i = 0; i < pcb->nfps && !near_pad; i++) {
                V2 pad = pcb->fps[i].pos;
                float d1 = sqrtf((tr->a.x - pad.x) * (tr->a.x - pad.x) + (tr->a.y - pad.y) * (tr->a.y - pad.y));
                float d2 = sqrtf((tr->b.x - pad.x) * (tr->b.x - pad.x) + (tr->b.y - pad.y) * (tr->b.y - pad.y));
                if (d1 < 2.0f || d2 < 2.0f) near_pad = true;
            }
            if (near_pad) {
                for (int j = t + 1; j < pcb->ntracks; j++) pcb->tracks[j - 1] = pcb->tracks[j];
                pcb->ntracks--;
                removed++;
            }
        }
    }
    if (removed > 0)
        pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track) * (size_t)pcb->ntracks);
    return removed;
}
