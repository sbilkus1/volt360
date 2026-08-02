#include "balloons.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

char *balloons_text(Assembly *as) {
    if (!as) return str_dup("no assembly");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== ASSEMBLY BALLOONS: %s ===\n", as->name ? as->name : "?");
    for (int i = 0; i < as->nitems; i++) {
        PlaceItem *it = &as->items[i];
        off += snprintf(buf+off, sizeof(buf)-off,
            "  [%d] %s  at (%.0f, %.0f, %.0f)  kind: %s\n",
            i+1, it->label ? it->label : "?",
            it->pos.x, it->pos.y, it->pos.z,
            it->kind == PART_PCB ? "PCB" : it->kind == PART_CAD ? "CAD" : "?");
    }
    return str_dup(buf);
}

char *balloons_assembly_instructions(Assembly *as) {
    if (!as) return str_dup("no assembly");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== ASSEMBLY INSTRUCTIONS ===\n"
        "Assembly: %s (%d items)\n"
        "------------------------------\n", as->name ? as->name : "?", as->nitems);
    for (int i = 0; i < as->nitems; i++) {
        off += snprintf(buf+off, sizeof(buf)-off,
            "  Step %d: Place %s at position (%.0f, %.0f, %.0f)\n",
            i+1, as->items[i].label ? as->items[i].label : "?",
            as->items[i].pos.x, as->items[i].pos.y, as->items[i].pos.z);
    }
    return str_dup(buf);
}
