#include "fatigue.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

float fatigue_cycles_basquin(float stress_amp, float C, float m) {
    if (stress_amp <= 0) return 1e9f;
    float N = C * powf(stress_amp, -m);
    return N > 0 ? N : 1e9f;
}

float fatigue_goodman_corrected(float stress_amp, float stress_mean, float UTS_MPa) {
    if (UTS_MPa <= 0) return stress_amp;
    float ratio = stress_mean / UTS_MPa;
    if (ratio >= 1.0f) return 1e9f;
    return stress_amp / (1.0f - ratio);
}

float fatigue_miner_damage(const float *stress_amps, const float *cycles, int n_levels, float C, float m) {
    float total = 0;
    for (int i = 0; i < n_levels; i++) {
        float Ni = fatigue_cycles_basquin(stress_amps[i], C, m);
        if (Ni > 0) total += cycles[i] / Ni;
    }
    return total;
}

float fatigue_endurance_limit(float UTS_MPa) {
    if (UTS_MPa <= 0) return 0;
    if (UTS_MPa < 1400) return UTS_MPa * 0.5f;
    else return 700.0f;
}

char *fatigue_report(const char *name, float stress_amp, float stress_mean, float UTS_MPa, int cycles_applied) {
    char buf[1024];
    float corrected = fatigue_goodman_corrected(stress_amp, stress_mean, UTS_MPa);
    float N_pred = fatigue_cycles_basquin(corrected, 1e12f, 3.0f);
    float end_limit = fatigue_endurance_limit(UTS_MPa);
    float damage = (float)cycles_applied / (N_pred > 0 ? N_pred : 1e9f);
    int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== FATIGUE ANALYSIS: %s ===\n"
        "Stress amp: %.1f MPa (mean: %.1f)\n"
        "Goodman corrected: %.1f MPa\n"
        "UTS: %.0f MPa  Endurance limit: %.0f MPa\n"
        "Predicted life: %.0f cycles\n"
        "Applied: %d cycles  Damage: %.4f (%s)\n",
        name ? name : "?", stress_amp, stress_mean, corrected, UTS_MPa, end_limit,
        N_pred, cycles_applied, damage, damage >= 1.0f ? "FAILED" : "OK");
    return str_dup(buf);
}
