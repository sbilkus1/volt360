#include "annotations.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int annotations_auto_assign(Schematic *s) {
    if (!s) return 0;
    int count = 0;
    for (int i = 0; i < s->ninsts; i++) {
        if (!s->insts[i].ref || !s->insts[i].ref[0] || strcmp(s->insts[i].ref, "?") == 0) {
            char buf[32]; snprintf(buf, sizeof(buf), "U%d", i + 1);
            free(s->insts[i].ref);
            s->insts[i].ref = str_dup(buf);
            count++;
        }
    }
    return count;
}

char *crossprobe_report(Schematic *s, Pcb *pcb) {
    if (!s) return str_dup("no schematic");
    char buf[1024]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== CROSS-PROBE: %s ===\n", s->name);
    int matched = 0;
    if (pcb) {
        for (int i = 0; i < s->ninsts; i++) {
            for (int j = 0; j < pcb->nfps; j++) {
                if (s->insts[i].ref && pcb->fps[j].ref && strcmp(s->insts[i].ref, pcb->fps[j].ref) == 0) {
                    off += snprintf(buf+off, sizeof(buf)-off, "  %s: SCH(%.0f,%.0f) <-> PCB(%.1f,%.1f)\n",
                        s->insts[i].ref, s->insts[i].pos.x, s->insts[i].pos.y, pcb->fps[j].pos.x, pcb->fps[j].pos.y);
                    matched++;
                }
            }
        }
    }
    off += snprintf(buf+off, sizeof(buf)-off, "  Matched: %d / %d\n", matched, s->ninsts);
    return str_dup(buf);
}
