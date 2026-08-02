#include "thermal.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

float thermal_trace_temp_rise(float current_a, float width_mm, float copper_oz) {
    if (current_a <= 0 || width_mm <= 0 || copper_oz <= 0) return 0;
    float area = width_mm * copper_oz * 0.035f; // mm²
    float R = 0.0172f * 1.0f / (area * 1000.0f); // trace resistance ohms/mm
    float P = current_a * current_a * R * 50.0f; // power for 50mm trace
    return P * 150.0f; // rough thermal resistance to ambient
}

float thermal_board_temp_rise(float board_area_cm2, float power_w) {
    if (board_area_cm2 <= 0 || power_w <= 0) return 0;
    return power_w * 100.0f / board_area_cm2; // rough: 100 C*cm²/W
}

float thermal_junction_temp(float power_w, float rth_jc, float rth_ca, float ambient_c) {
    return ambient_c + power_w * (rth_jc + rth_ca);
}

char *thermal_report(Pcb *pcb, float ambient_c) {
    if (!pcb) return str_dup("no PCB");
    char buf[2048]; int off = 0;
    float area = (pcb->bmax.x - pcb->bmin.x) * (pcb->bmax.y - pcb->bmin.y) * 0.01f; // cm²
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== THERMAL ANALYSIS ===\n"
        "Board: %s (%.1f cm²)\n"
        "Ambient: %.0f C\n\n",
        pcb->name ? pcb->name : "?", area, ambient_c);

    float total_power = 0;
    for (int i = 0; i < pcb->nfps; i++) {
        // estimate: each footprint ~0.1W
        total_power += 0.1f;
    }
    float rise = thermal_board_temp_rise(area, total_power);
    off += snprintf(buf+off, sizeof(buf)-off,
        "Est. power: %.1f W\n"
        "Board temp rise: %.0f C\n"
        "Board temp: %.0f C\n\n",
        total_power, rise, ambient_c + rise);

    off += snprintf(buf+off, sizeof(buf)-off,
        "TRACE THERMAL:\n"
        "  Tracks: %d\n"
        "  Thermal relief recommended for high-current traces (>1A)\n",
        pcb->ntracks);
    return str_dup(buf);
}
