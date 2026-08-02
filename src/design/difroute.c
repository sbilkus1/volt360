#include "difroute.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int difroute_add_pair(Pcb *pcb, V2 a1, V2 b1, V2 a2, V2 b2, float width, float spacing) {
    if (!pcb) return -1;
    // add two parallel tracks
    Track t1, t2;
    t1.a = a1; t1.b = b1; t1.width = width; t1.net = str_dup("DP+");
    t2.a = a2; t2.b = b2; t2.width = width; t2.net = str_dup("DP-");
    pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track) * (size_t)(pcb->ntracks + 2));
    pcb->tracks[pcb->ntracks++] = t1;
    pcb->tracks[pcb->ntracks++] = t2;
    return 0;
}

int netclass_apply(Pcb *pcb, const char *net_pattern, NetClass *nc) {
    if (!pcb || !net_pattern || !nc) return 0;
    int applied = 0;
    for (int i = 0; i < pcb->ntracks; i++)
        if (pcb->tracks[i].net && strstr(pcb->tracks[i].net, net_pattern)) {
            pcb->tracks[i].width = nc->track_width;
            applied++;
        }
    return applied;
}
