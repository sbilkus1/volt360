#pragma once
// powerint.h - Power integrity analysis for PCB.
#include <stdbool.h>
#include "../model/project.h"

// Estimate DC voltage drop across power distribution (IR drop).
// current_A: total current, trace_count: number of parallel power traces
float powerint_ir_drop(float current_A, float width_mm, float length_mm, float copper_oz, int trace_count);

// Estimate decoupling capacitance needed for a given switching current.
float powerint_decoupling_cap(float switching_current_A, float dV_max, float frequency_Hz);

// Estimate PDN target impedance.
float powerint_pdn_z_target(float supply_voltage, float ripple_pct, float max_current);

// Power plane impedance estimate.
float powerint_plane_impedance(float width_mm, float length_mm, float dielectric_thickness_um, float er);

// Full PDN report.
char *powerint_report(Pcb *pcb);
