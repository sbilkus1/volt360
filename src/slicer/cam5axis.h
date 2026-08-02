#pragma once
// cam5axis.h - 4-axis and 5-axis CAM toolpath stubs.
#include <stdbool.h>

// 4-axis rotary (A-axis) toolpath: profile with rotation.
bool cam4_rotary(const char *path, float radius, float length, float feed, int steps);

// 5-axis simultaneous contour.
bool cam5_contour(const char *path, float feed);

// Mill-turn: combined milling + turning.
bool cam_millturn(const char *path, float turn_diam, float mill_depth, float feed);

// Probing cycle: touch probe at grid points.
bool cam_probe_grid(const char *path, float x, float y, float w, float h, float safe_z, float probe_depth, int nx, int ny);

// Toolpath simulation text playback.
char *cam_simulate(const char *gcode_path, float time_scale);

// Collision detection stub (checks for tool-object intersection).
int cam_collision_check(const char *gcode_path);
