#include "exploded.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void assembly_explode(Assembly *as, float explode_factor) {
    if (!as || as->nitems < 1) return;
    // compute centroid of all items
    float cx = 0, cy = 0, cz = 0;
    for (int i = 0; i < as->nitems; i++) {
        cx += as->items[i].pos.x; cy += as->items[i].pos.y; cz += as->items[i].pos.z;
    }
    cx /= (float)as->nitems; cy /= (float)as->nitems; cz /= (float)as->nitems;

    for (int i = 0; i < as->nitems; i++) {
        // vector from centroid to item
        float dx = as->items[i].pos.x - cx;
        float dy = as->items[i].pos.y - cy;
        float dz = as->items[i].pos.z - cz;
        // scale outward by explode factor
        as->items[i].pos.x = cx + dx * explode_factor;
        as->items[i].pos.y = cy + dy * explode_factor;
        as->items[i].pos.z = cz + dz * explode_factor;
    }
}

char *assembly_explode_text(Assembly *as, float explode_factor) {
    if (!as) return str_dup("no assembly");
    char buf[1024]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off, "Exploded view (factor=%.1f):\n", explode_factor);
    // compute centroid
    float cx = 0, cy = 0, cz = 0;
    for (int i = 0; i < as->nitems; i++) {
        cx += as->items[i].pos.x; cy += as->items[i].pos.y; cz += as->items[i].pos.z;
    }
    cx /= (float)as->nitems; cy /= (float)as->nitems; cz /= (float)as->nitems;
    for (int i = 0; i < as->nitems; i++) {
        float dx = as->items[i].pos.x - cx, dy = as->items[i].pos.y - cy, dz = as->items[i].pos.z - cz;
        float ex = cx + dx * explode_factor, ey = cy + dy * explode_factor, ez = cz + dz * explode_factor;
        off += snprintf(buf + off, sizeof(buf) - off, "  Item %d: (%.0f,%.0f,%.0f) -> (%.0f,%.0f,%.0f)\n",
            i+1, as->items[i].pos.x, as->items[i].pos.y, as->items[i].pos.z, ex, ey, ez);
    }
    return str_dup(buf);
}
