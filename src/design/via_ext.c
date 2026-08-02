#include "via_ext.h"
#include <stdlib.h>
#include <string.h>

int via_add_typed(Pcb *pcb, V2 pos, float drill, float outer, int start_layer, int end_layer, const char *net) {
    if (!pcb) return -1;
    Via v; v.pos = pos; v.drill = drill; v.outer = outer; v.net = net ? str_dup(net) : NULL;
    pcb->vias = (Via *)realloc(pcb->vias, sizeof(Via) * (size_t)(pcb->nvias + 1));
    pcb->vias[pcb->nvias++] = v;
    return pcb->nvias - 1;
}

void via_count_types(Pcb *pcb, int *through, int *blind, int *buried) {
    *through = *blind = *buried = 0;
    if (!pcb) return;
    for (int i = 0; i < pcb->nvias; i++) {
        Via *v = &pcb->vias[i];
        if (v->drill > 0 && v->outer > v->drill * 2.0f) (*blind)++;
        else if (v->drill < v->outer * 0.3f) (*buried)++;
        else (*through)++;
    }
}

int via_convert_all_through(Pcb *pcb) {
    if (!pcb) return 0;
    int c = 0;
    for (int i = 0; i < pcb->nvias; i++) {
        if (pcb->vias[i].outer < pcb->vias[i].drill * 1.5f) {
            pcb->vias[i].outer = pcb->vias[i].drill * 2.0f;
            c++;
        }
    }
    return c;
}
