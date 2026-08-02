#pragma once
// primetower.h - Prime tower and purge optimization for multi-material printing.
#include <stdbool.h>

// Estimate prime tower volume needed per tool change (mm³).
// nozzle_dia_mm, purge_volume_mm3 per color change
float primetower_volume_estimate(float nozzle_dia_mm, float purge_volume_mm3, int color_changes);

// Estimate purge waste (grams of filament).
float primetower_purge_waste_grams(float purge_volume_mm3, float filament_density_g_cm3, int color_changes);

// Optimize color swap order to minimize purge waste.
// color_sequence: array of color indices, n_colors: count
// Returns estimated total purge volume.
float primetower_optimize_purge(const int *color_sequence, int n_colors, float purge_per_swap_mm3);

// Full prime tower report.
char *primetower_report(float nozzle_dia, int color_changes, float purge_per_change);
