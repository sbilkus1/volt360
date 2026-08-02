#include "lengthtune.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

float lengthtune_net_length(Pcb *pcb, const char *net_name) {
    if (!pcb) return 0;
    float total = 0;
    for (int i = 0; i < pcb->ntracks; i++) {
        if (!net_name || !pcb->tracks[i].net || strcmp(pcb->tracks[i].net, net_name) == 0) {
            float dx = pcb->tracks[i].b.x - pcb->tracks[i].a.x;
            float dy = pcb->tracks[i].b.y - pcb->tracks[i].a.y;
            total += sqrtf(dx*dx + dy*dy);
        }
    }
    return total;
}

int lengthtune_add_meanders(Pcb *pcb, const char *net_name, float target, float amp, float spacing) {
    if (!pcb || !net_name) return 0;
    float current = lengthtune_net_length(pcb, net_name);
    if (current >= target) return 0;

    // find last track on this net and add meanders from its endpoint
    int added = 0;
    for (int i = pcb->ntracks - 1; i >= 0; i--) {
        if (pcb->tracks[i].net && strcmp(pcb->tracks[i].net, net_name) == 0) {
            V2 end = pcb->tracks[i].b;
            V2 dir = v2(pcb->tracks[i].b.x - pcb->tracks[i].a.x, pcb->tracks[i].b.y - pcb->tracks[i].a.y);
            float len = sqrtf(dir.x*dir.x + dir.y*dir.y);
            if (len < 0.01f) break;
            V2 perp = v2(-dir.y/len, dir.x/len);
            float remaining = target - current;
            float seg = amp * 4.0f; // each meander adds ~4*amp length
            int meanders = (int)(remaining / seg) + 1;
            if (meanders > 20) meanders = 20;

            V2 pos = end;
            for (int m = 0; m < meanders; m++) {
                V2 n1 = v2(pos.x + perp.x * amp, pos.y + perp.y * amp);
                V2 n2 = v2(n1.x + dir.x * spacing * 0.5f, n1.y + dir.y * spacing * 0.5f);
                V2 n3 = v2(n2.x - perp.x * amp * 2, n2.y - perp.y * amp * 2);
                V2 n4 = v2(n3.x + dir.x * spacing * 0.5f, n3.y + dir.y * spacing * 0.5f);
                Track t1 = {pos, n1, 0.15f, str_dup(net_name)};
                Track t2 = {n1, n2, 0.15f, str_dup(net_name)};
                Track t3 = {n2, n3, 0.15f, str_dup(net_name)};
                Track t4 = {n3, n4, 0.15f, str_dup(net_name)};
                pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track)*(size_t)(pcb->ntracks+4));
                pcb->tracks[pcb->ntracks++] = t1; pcb->tracks[pcb->ntracks++] = t2;
                pcb->tracks[pcb->ntracks++] = t3; pcb->tracks[pcb->ntracks++] = t4;
                pos = n4; added += 4;
            }
            break;
        }
    }
    return added;
}

float lengthtune_delay_ns(float length_mm) {
    // FR4: er ~4.2, v = c/sqrt(er), delay = L/v
    float v = 3.0e8f / sqrtf(4.2f); // m/s
    return length_mm * 1e-3f / v * 1e9f; // ns
}

int lengthtune_match_nets(Pcb *pcb, const char *net_a, const char *net_b, float tolerance) {
    if (!pcb || !net_a || !net_b) return 0;
    float la = lengthtune_net_length(pcb, net_a);
    float lb = lengthtune_net_length(pcb, net_b);
    if (fabs(la - lb) < tolerance) return 0;
    if (la < lb) return lengthtune_add_meanders(pcb, net_a, lb, 1.0f, 0.5f);
    else return lengthtune_add_meanders(pcb, net_b, la, 1.0f, 0.5f);
}
