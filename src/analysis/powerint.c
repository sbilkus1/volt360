#include "powerint.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

float powerint_ir_drop(float current_A, float width_mm, float length_mm, float copper_oz, int trace_count) {
    if (current_A <= 0 || width_mm <= 0 || trace_count <= 0) return 0;
    float area = width_mm * copper_oz * 0.035f;
    float R = 0.0172f * length_mm / (area * 1000.0f * (float)trace_count);
    return current_A * R;
}

float powerint_decoupling_cap(float I_sw, float dV_max, float freq) {
    if (dV_max <= 0 || freq <= 0) return 0;
    return I_sw / (dV_max * freq);
}

float powerint_pdn_z_target(float V_supply, float ripple_pct, float I_max) {
    if (I_max <= 0) return 0;
    return V_supply * ripple_pct * 0.01f / I_max;
}

float powerint_plane_impedance(float w, float l, float t_um, float er) {
    if (w <= 0 || l <= 0 || t_um <= 0 || er <= 0) return 0;
    float L = 0.2f * t_um * 1e-6f * logf(2.0f * (w + l) / (w + l)) / (w + l);
    return L * 1e9f; // nH-ish value
}

char *powerint_report(Pcb *pcb) {
    if (!pcb) return str_dup("no PCB");
    char buf[1024]; int off = 0;
    float drop = powerint_ir_drop(2.0f, 1.0f, 50.0f, 1.0f, 3);
    float z_target = powerint_pdn_z_target(3.3f, 5.0f, 2.0f);
    float decap = powerint_decoupling_cap(0.5f, 0.1f, 100e6f);
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== POWER INTEGRITY ===\n"
        "Board: %s\n"
        "IR drop (2A, 1mm*3 traces, 50mm): %.3f V\n"
        "PDN Z_target (3.3V, 5%%, 2A): %.4f ohm\n"
        "Decoupling (0.5A, 0.1V, 100MHz): %.2f uF\n",
        pcb->name ? pcb->name : "?",
        drop, z_target, decap * 1e6f);
    return str_dup(buf);
}
