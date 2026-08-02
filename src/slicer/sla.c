#include "sla.h"
#include "../core/util.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

float sla_cure_time_s(float layer_um, float power_mW) {
    if (layer_um <= 0 || power_mW <= 0) return 2.0f;
    return 500.0f / (layer_um * power_mW * 0.01f);
}

float sls_energy_density(float power_W, float speed_mm_s, float hatch_mm) {
    if (speed_mm_s <= 0 || hatch_mm <= 0) return 0;
    return power_W / (speed_mm_s * hatch_mm);
}

float metal_pbf_energy(float power_W, float speed_mm_s, float layer_um, float hatch_mm) {
    if (speed_mm_s <= 0 || hatch_mm <= 0 || layer_um <= 0) return 0;
    return power_W / (speed_mm_s * (layer_um * 0.001f) * hatch_mm);
}

float sla_support_volume_pct(float overhang_angle_deg) {
    if (overhang_angle_deg < 30.0f) return 15.0f;
    if (overhang_angle_deg < 45.0f) return 8.0f;
    if (overhang_angle_deg < 60.0f) return 3.0f;
    return 0;
}

char *additive_report(const char *tech, float layer_um, float power) {
    char buf[512];
    snprintf(buf, sizeof(buf),
        "=== ADDITIVE SETUP: %s ===\n"
        "Layer: %.0f um  Power: %.0f W/mW\n"
        "SLA cure: %.1f s  SLS energy: %.2f J/mm²\n"
        "Support vol: %.0f%% (45° overhang)\n",
        tech ? tech : "FDM", layer_um, power,
        sla_cure_time_s(layer_um, power),
        sls_energy_density(power, 1000.0f, 0.1f),
        sla_support_volume_pct(45.0f));
    return str_dup(buf);
}
