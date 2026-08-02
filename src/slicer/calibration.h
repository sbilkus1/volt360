#pragma once
// calibration.h - OrcaSlicer-style calibration test G-code generators.
#include <stdbool.h>
#include "../model/project.h"
// Each function writes a standalone, ready-to-print G-code file.

// Temperature tower: print a tower with temp changes every `step_mm` Z.
bool calib_temp_tower(const char *path, float bed_temp, int temp_min, int temp_max, int temp_step,
                      float layer_h, float line_w, float print_speed, float travel_speed,
                      float retract_mm, float filament_dia);

// Flow-rate calibration: single-layer squares at varied extrusion multipliers.
bool calib_flow_rate(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                     float line_w, float print_speed, float travel_speed, float retract_mm,
                     float filament_dia, float mult_min, float mult_max, float mult_step);

// Pressure-advance (linear-advance) pattern: lines at increasing PA values.
// Uses M900 K<value> Marlin / SET_PRESSURE_ADVANCE for Klipper.
bool calib_pressure_advance(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                            float line_w, float print_speed, float travel_speed, float retract_mm,
                            float filament_dia, float pa_min, float pa_max, float pa_step,
                            bool klipper);

// Max volumetric flow test: ramps speed from slow to extreme.
bool calib_max_flow(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                    float line_w, float speed_min, float speed_max, float speed_step,
                    float travel_speed, float retract_mm, float filament_dia);

// VFA (Vertical Fine Artifact) tower: prints at fixed intervals to identify
// resonance frequencies. Varies print speed in bands.
bool calib_vfa_tower(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                     float line_w, float speed_min, float speed_max, float speed_step,
                     float travel_speed, float retract_mm, float filament_dia,
                     float tower_w, float tower_d, float z_height);

// Input shaper ringing tower: prints sections at different shaper frequencies.
// Uses Klipper SET_INPUT_SHAPER commands at each level.
// Calibration: all OrcaSlicer-style test generators
bool calib_temp_tower(const char *path, float bed_temp, int temp_min, int temp_max, int temp_step,
                      float layer_h, float line_w, float print_speed, float travel_speed,
                      float retract_mm, float filament_dia);
bool calib_flow_rate(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                     float line_w, float print_speed, float travel_speed, float retract_mm,
                     float filament_dia, float mult_min, float mult_max, float mult_step);
bool calib_pressure_advance(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                            float line_w, float print_speed, float travel_speed, float retract_mm,
                            float filament_dia, float pa_min, float pa_max, float pa_step, bool klipper);
bool calib_max_flow(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                    float line_w, float speed_min, float speed_max, float speed_step,
                    float travel_speed, float retract_mm, float filament_dia);
bool calib_vfa_tower(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                     float line_w, float speed_min, float speed_max, float speed_step,
                     float travel_speed, float retract_mm, float filament_dia,
                     float tower_w, float tower_d, float z_height);
bool calib_input_shaper(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                        float line_w, float print_speed, float travel_speed, float retract_mm,
                        float filament_dia, float freq_min, float freq_max, float freq_step,
                        float tower_w, float tower_d, float z_height);

// Additional calibration tests
bool calib_retraction(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                      float line_w, float print_speed, float travel_speed, float filament_dia,
                      float ret_min, float ret_max, float ret_step);
bool calib_bridge(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                  float line_w, float print_speed, float travel_speed,
                  float filament_dia, float bridge_len_mm);
bool calib_tolerance(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                     float line_w, float print_speed, float travel_speed,
                     float filament_dia, float clearance_min, float clearance_max);
bool calib_overhang(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                    float line_w, float print_speed, float travel_speed, float filament_dia);
bool calib_speed(const char *path, float bed_temp, int nozzle_temp, float layer_h,
                 float line_w, float print_speed, float travel_speed, float filament_dia,
                 float speed_min, float speed_max, float speed_step);

// Auto-arrange: given a list of model bounding boxes (x,y sizes), compute
// optimal grid positions on a build plate of size plate_x by plate_y.
// Fills `out_x` and `out_y` arrays. Returns number placed (may be < count).
int calib_auto_arrange(int count, float *sizes_x, float *sizes_y,
                       float plate_x, float plate_y, float spacing,
                       float *out_x, float *out_y);

// Auto-orient: find the mesh face with largest area to use as the base,
// and return the rotation (rx, ry, rz in degrees) to align it with XY plane.
// Analyzes the mesh directly.
bool calib_auto_orient(CadMesh *mesh, float *out_rx, float *out_ry, float *out_rz);
