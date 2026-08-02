#pragma once
// pcbcalc.h - PCB track width, current capacity, thermal calculator.
#include <stdbool.h>

// IPC-2221 based calculations

// Calculate approximate current capacity (A) for a given track width and copper weight.
// width_mm: track width in mm, copper_oz: copper weight in oz (1 = standard, 2 = heavy).
// temperature_rise: allowed temp rise in C (default 10).
float pcbcalc_current_capacity(float width_mm, float copper_oz, float temp_rise);

// Calculate minimum track width for a given current.
float pcbcalc_track_width(float current_a, float copper_oz, float temp_rise);

// Calculate resistance of a track segment.
// width_mm, length_mm, copper_oz
float pcbcalc_track_resistance(float width_mm, float length_mm, float copper_oz);

// Voltage drop across a track segment at given current.
float pcbcalc_voltage_drop(float current_a, float width_mm, float length_mm, float copper_oz);

// Thermal: estimate trace temperature for given current and geometry.
float pcbcalc_trace_temp(float current_a, float width_mm, float copper_oz, float ambient_c);
