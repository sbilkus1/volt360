#pragma once
// printquality.h - Quality scoring, failure detection, ironing, presets.
#include <stdbool.h>
#include "../slicer/slicer.h"

// Quality scoring: compute a 0-100 score for a print based on path stats.
int quality_score(const SliceResult *sr, const SliceSettings *cfg);

// Nozzle clog detection: check for sudden extrusion drop.
// Returns: 0=OK, 1=possible clog, 2=definite clog.
int detect_nozzle_clog(float expected_flow, float actual_flow);

// Layer shift detection: check perimeter centroid movement.
int detect_layer_shift(V2 prev_centroid, V2 curr_centroid, float threshold);

// Warping detection: check first-layer area coverage.
int detect_warping(int expected_area_pct, int actual_area_pct);

// Automatic action from failure detection.
char *failure_action(int clog, int shift, int warp);

// Color change optimization: compute optimal swap order.
// colors: array of color indices, n: count
// Returns estimated total waste in mm.
float optimize_color_swaps(const int *colors, int n, float waste_per_swap);

// Part cooling: recommended fan speed for overhang angle.
int cooling_for_overhang(float overhang_angle_deg);

// Print time estimation: refine estimate from path stats.
float refine_time_estimate(const SliceResult *sr, const SliceSettings *cfg);

// Slicer profile presets.
typedef struct {
    char name[32];
    SliceSettings settings;
} SlicerProfile;

SlicerProfile *profile_presets(void); // returns array of 5 profiles
int profile_count(void);
SlicerProfile *profile_get(int idx);
char *profile_list_text(void);
