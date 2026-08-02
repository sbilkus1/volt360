#pragma once
// thermal.h - Steady-state thermal analysis for PCBs.
#include <stdbool.h>
#include "../model/project.h"

// Estimate PCB temperature rise given copper area and current.
// Returns temperature rise in C above ambient.
float thermal_trace_temp_rise(float current_a, float width_mm, float copper_oz);

// Estimate board-level temperature for given total power dissipation.
// board_area_cm2: board area in cm²
// power_w: total power dissipated on the board
float thermal_board_temp_rise(float board_area_cm2, float power_w);

// Compute junction temperature for a component.
// power_w: component power, rth_jc: junction-to-case thermal resistance
// rth_ca: case-to-ambient, ambient_c
float thermal_junction_temp(float power_w, float rth_jc, float rth_ca, float ambient_c);

// Full thermal report text.
char *thermal_report(Pcb *pcb, float ambient_c);
