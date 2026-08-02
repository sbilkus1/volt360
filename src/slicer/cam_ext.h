#pragma once
// cam_ext.h - Extended CAM operations: laser cutting, engraving, waterjet.
#include <stdbool.h>
#include "../model/project.h"

// Laser cutting: trace polygon outlines (single pass, no depth).
bool cam_laser_cut(const char *path, V2 *polys, int *poly_lengths, int n_polys, float feed_xy);

// Laser engraving: raster-fill a polygon area (hatching).
bool cam_laser_engrave(const char *path, V2 *poly, int n, float hatch_spacing, float feed_xy);

// Waterjet: similar to profile but with piercing lead-in.
bool cam_waterjet_cut(const char *path, V2 *poly, int n, float pierce_time_s, float feed_xy);

// Plasma cutting: with lead-in and kerf compensation.
bool cam_plasma_cut(const char *path, V2 *poly, int n, float kerf_mm, float pierce_delay_s, float feed_xy);

// 3-axis surface contour: follow a 3D polyline toolpath.
bool cam_3d_contour(const char *path, V3 *toolpath, int n_pts, float feed_xy, float safe_z);

// Adaptive clearing: spiral-in pocket clearing (more efficient than raster).
bool cam_adaptive_clear(const char *path, V2 *poly, int n, float target_z, float tool_dia, float stepover, float feed_xy, float feed_z, float safe_z);
