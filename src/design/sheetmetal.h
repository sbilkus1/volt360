#pragma once
// sheetmetal.h - Sheet metal design: bend allowance, flat pattern, flange calculator.
#include <stdbool.h>

// Bend allowance using standard formula: BA = (pi/180) * (R + K*T) * angle
// R: inside bend radius (mm), T: material thickness (mm), K: K-factor (0.0-1.0), angle: bend angle (deg)
float sheet_bend_allowance(float R, float T, float K, float angle);

// Bend deduction: BD = 2 * OSB - BA, where OSB = (R + T) * tan(angle/2)
float sheet_bend_deduction(float R, float T, float K, float angle);

// Flat pattern length for a series of bends along a flange.
// lengths[n]: straight segment lengths, angles[n-1]: bend angles, R, T, K
float sheet_flat_length(const float *lengths, const float *angles, int n_segments, float R, float T, float K);

// Minimum flange length for a given bend radius and thickness.
float sheet_min_flange(float R, float T);

// K-factor estimate from bend radius and thickness (DIN 6935).
float sheet_k_factor_estimate(float R, float T);

// Generate a flat-pattern rectangle with bend lines as a simple G-code or DXF outline.
// Writes text representation of the flat pattern.
bool sheet_flat_pattern_text(float length, float width, float flange_l, float flange_r, float R, float T, float K, float angle,
                              char *buffer, int buf_size);
