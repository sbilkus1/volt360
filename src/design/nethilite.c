#include "nethilite.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int nethilite_find_tracks(Pcb *pcb, const char *net_name, int *track_indices, int max_tracks) {
    if (!pcb || !net_name || !track_indices) return 0;
    int count = 0;
    for (int i = 0; i < pcb->ntracks && count < max_tracks; i++)
        if (pcb->tracks[i].net && strcmp(pcb->tracks[i].net, net_name) == 0)
            track_indices[count++] = i;
    return count;
}

int nethilite_connected(Pcb *pcb, int fp_index, int *track_indices, int max_tracks) {
    if (!pcb || fp_index < 0 || fp_index >= pcb->nfps || !track_indices) return 0;
    V2 pad = pcb->fps[fp_index].pos;
    int count = 0;
    for (int i = 0; i < pcb->ntracks && count < max_tracks; i++) {
        float d1 = sqrtf((pcb->tracks[i].a.x-pad.x)*(pcb->tracks[i].a.x-pad.x)+(pcb->tracks[i].a.y-pad.y)*(pcb->tracks[i].a.y-pad.y));
        float d2 = sqrtf((pcb->tracks[i].b.x-pad.x)*(pcb->tracks[i].b.x-pad.x)+(pcb->tracks[i].b.y-pad.y)*(pcb->tracks[i].b.y-pad.y));
        if (d1 < 2.0f || d2 < 2.0f) track_indices[count++] = i;
    }
    return count;
}

const char *nethilite_query(Pcb *pcb, V2 pos, float tolerance) {
    if (!pcb) return NULL;
    for (int i = 0; i < pcb->ntracks; i++) {
        float d1 = sqrtf((pcb->tracks[i].a.x-pos.x)*(pcb->tracks[i].a.x-pos.x)+(pcb->tracks[i].a.y-pos.y)*(pcb->tracks[i].a.y-pos.y));
        float d2 = sqrtf((pcb->tracks[i].b.x-pos.x)*(pcb->tracks[i].b.x-pos.x)+(pcb->tracks[i].b.y-pos.y)*(pcb->tracks[i].b.y-pos.y));
        if (d1 < tolerance || d2 < tolerance) return pcb->tracks[i].net;
    }
    return NULL;
}

char *nethilite_nets_report(Pcb *pcb) {
    if (!pcb) return str_dup("no PCB");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== NET LIST ===\n");
    int unique = 0;
    for (int i = 0; i < pcb->ntracks; i++) {
        const char *net = pcb->tracks[i].net;
        if (!net) continue;
        int seen = 0;
        for (int j = 0; j < i; j++)
            if (pcb->tracks[j].net && strcmp(pcb->tracks[j].net, net) == 0) { seen = 1; break; }
        if (seen) continue;
        int cnt = 0; float len = 0;
        for (int j = 0; j < pcb->ntracks; j++)
            if (pcb->tracks[j].net && strcmp(pcb->tracks[j].net, net) == 0) {
                cnt++; len += sqrtf((pcb->tracks[j].b.x-pcb->tracks[j].a.x)*(pcb->tracks[j].b.x-pcb->tracks[j].a.x)+(pcb->tracks[j].b.y-pcb->tracks[j].a.y)*(pcb->tracks[j].b.y-pcb->tracks[j].a.y));
            }
        off += snprintf(buf+off, sizeof(buf)-off, "  %s: %d segs, %.1f mm\n", net, cnt, len);
        unique++;
    }
    off += snprintf(buf+off, sizeof(buf)-off, "  Total unique nets: %d\n", unique);
    return str_dup(buf);
}
