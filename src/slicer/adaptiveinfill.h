#pragma once
// adaptiveinfill.h - Adaptive/variable density infill + gap fill.
#include <stdbool.h>
#include "../model/project.h"

// Compute adaptive infill spacing based on distance from walls.
// Returns spacing value (larger spacing = lower density away from walls).
// base_spacing: default spacing, wall_spacing: spacing near walls
// dist_from_wall_mm: how far from the perimeter wall
float adaptive_spacing(float base_spacing, float wall_spacing, float dist_from_wall_mm);

// Gap fill: generate small infill segments to fill gaps between perimeters.
// poly: outer perimeter, inner: inner perimeter, gap_max: max gap to fill
// Fills out array (caller provides maxseg).
int gap_fill_segments(V2 *outer, int n_outer, V2 *inner, int n_inner,
                      float line_w, float gap_max, V2 *out, int maxseg);

// Variable wall thickness: compute thickness at a point based on model features.
// base_thickness: default wall thickness
// local_feature_factor: 0.5-2.0, how much thicker/thinner at this point
float variable_wall_thickness(float base_thickness, float local_feature_factor);

// Adaptive ironing flow rate.
float ironing_flow_rate(float base_flow, float surface_slope_deg);

// Fan speed by region: compute fan speed for a given print region.
// base_fan: 0-255, region_type: 0=perimeter, 1=infill, 2=bridge, 3=overhang
int fan_speed_region(int base_fan, int region_type);
