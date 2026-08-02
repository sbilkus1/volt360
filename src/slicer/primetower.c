#include "primetower.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

float primetower_volume_estimate(float nozzle_dia, float purge_vol, int changes) {
    if (nozzle_dia <= 0 || changes < 1) return 0;
    float tower_base = nozzle_dia * nozzle_dia * 10.0f; // base area ~10x nozzle dia
    float tower_height = (float)changes * 2.0f; // 2mm per color change
    return tower_base * tower_height + purge_vol * (float)changes;
}

float primetower_purge_waste_grams(float purge_vol, float density, int changes) {
    return purge_vol * 0.001f * density * (float)changes; // mm³ → cm³ → g
}

float primetower_optimize_purge(const int *colors, int n, float purge_per_swap) {
    if (n < 2) return 0;
    // greedy: minimize adjacent color repeats
    float total = 0; int prev = colors[0];
    for (int i = 1; i < n; i++) {
        if (colors[i] != prev) total += purge_per_swap;
        else total += purge_per_swap * 0.3f; // same color = less purge
        prev = colors[i];
    }
    return total;
}

char *primetower_report(float nozzle_dia, int changes, float purge_per_change) {
    char buf[512];
    float vol = primetower_volume_estimate(nozzle_dia, purge_per_change, changes);
    float waste = primetower_purge_waste_grams(purge_per_change, 1.24f, changes);
    snprintf(buf, sizeof(buf),
        "=== PRIME TOWER REPORT ===\n"
        "Nozzle: %.2f mm  Tool changes: %d\n"
        "Purge/change: %.0f mm³\n"
        "Tower volume: %.0f mm³\n"
        "Filament waste: %.1f g (PLA)\n",
        nozzle_dia, changes, purge_per_change, vol, waste);
    return str_dup(buf);
}
