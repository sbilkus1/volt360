#pragma once
// ieda_eda.h - iEDA-style EDA algorithms: placement, routing, CTS, floorplan, timing, DRC.
#include <stdbool.h>
#include "../model/project.h"

// === SIMULATED ANNEALING PLACEMENT ===
// Optimize footprint positions to minimize total wire length (half-perimeter).
// temp_start: initial temperature, temp_end: final, cooling_rate: 0.95 typical
// moves_per_temp: iterations per temperature step
int place_simulated_annealing(Pcb *pcb, float temp_start, float temp_end, float cooling_rate, int moves_per_temp);

// Compute total half-perimeter wirelength for current placement.
float place_total_wirelength(Pcb *pcb);

// === A* MAZE ROUTER ===
// Find a path from start to end avoiding obstacles (other tracks/pads).
// grid_resolution: grid cell size in mm (0.1mm typical).
// Returns number of waypoints, fills path array (caller provides max_points).
int route_astar(Pcb *pcb, V2 start, V2 end, float grid_res, float clearance, V2 *path, int max_points);

// Route all nets in the PCB.
int route_all_nets(Pcb *pcb, float grid_res, float clearance);

// === CLOCK TREE SYNTHESIS (CTS) ===
// Build a balanced H-tree clock distribution network.
// clock_source: clock input point, sinks: clock destinations, n_sinks: number
// Returns number of tree segments added.
int cts_build_htree(Pcb *pcb, V2 clock_source, V2 *sinks, int n_sinks, float track_width);

// Measure clock skew (max delay difference between sinks).
float cts_measure_skew(Pcb *pcb);

// === FLOORPLANNING ===
// B*-tree based floorplanner: partition board into rectangular blocks.
// blocks: array of (width, height) pairs, n_blocks: count
// Fills positions array with optimal placement.
int floorplan_bstar(float *block_w, float *block_h, int n_blocks, V2 *positions, float board_w, float board_h);

// === TIMING OPTIMIZATION ===
// Insert buffer (repeater) at optimal location along a long track.
// Returns optimal buffer position, or (-1,-1) if no buffer needed.
V2 timing_optimal_buffer_pos(V2 a, V2 b, float max_len_before_buf);

// === ENHANCED DRC ===
// Check minimum spacing rules for all track pairs.
typedef struct { V2 pos; int type; char msg[128]; } DRCViolation;
int drc_enhanced_check(Pcb *pcb, float min_spacing, float min_width, float min_annular, DRCViolation *violations, int max_violations);

// === NETLIST TO GDS ===
// Generate a simple GDSII layout from PCB data (stub).
char *gds_export_stub(Pcb *pcb, const char *output_path);

// === POWER GRID ANALYSIS ===
// Analyze IR drop and electromigration on power distribution.
char *power_grid_analysis(Pcb *pcb, float supply_voltage, float max_current);
