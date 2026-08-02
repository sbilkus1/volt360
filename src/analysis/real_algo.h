#pragma once
// real_algo.h - Real algorithms from KiCad/OrcaSlicer/FreeCAD/iEDA source trees.
#include <stdbool.h>
#include "../model/project.h"
#include "../design/sketcher.h"
#include "../design/design.h"

// ===== KICAD CONNECTION GRAPH =====
// Node represents a connection point in the schematic.
typedef struct { int net_id; V2 pos; int pin_count; } ConnNode;

// Build a full connection graph from wires + pins + labels.
// Maps every wire endpoint and component pin to a net.
// Returns number of distinct nets found.
int conngraph_build(Schematic *s, ConnNode *nodes, int max_nodes);

// Trace a net: return all wire indices and component pin indices on a net.
int conngraph_trace_net(Schematic *s, ConnNode *nodes, int net_id,
                         int *wire_indices, int max_wires, int *pin_inst_indices, int max_pins);

// ===== KICAD PUSH-AND-SHOVE ROUTER =====
// Push tracks out of the way when a new track is added.
// Maintains DRC clearance. Returns number of tracks pushed.
int pushshove_route(Pcb *pcb, V2 a, V2 b, float width, float clearance, int max_shoves);

// Shove a single track sideways by `amount` mm perpendicular to its direction.
void pushshove_shove_track(Track *t, V2 direction, float amount);

// Check if a position is clear of obstacles (tracks + pads).
bool pushshove_is_clear(Pcb *pcb, V2 pos, float clearance, int exclude_track);

// ===== ORCASLICER ADAPTIVE LAYER HEIGHT =====
// Compute adaptive layer height for a mesh based on surface normal angle.
// Steep surfaces get finer layers, flat surfaces get coarser layers.
// Fills height array (one per original layer). Returns adjusted layer count.
int adaptive_layer_heights(CadMesh *mesh, float base_h, float min_h, float max_h,
                           float *out_heights, int max_layers);

// Compute average surface normal angle at a given Z height.
// Returns 0 (flat) to 90 (vertical).
float surface_angle_at_z(CadMesh *mesh, float z);

// ===== ORCASLICER BRIDGE DETECTION =====
// Detect bridge regions: areas where a line spans between two points without support.
// Returns number of bridge segments found.
int bridge_detect(V2 *layer_poly, int n, V2 *layer_below_poly, int nb,
                  float line_w, V2 *bridge_segments, int max_bridges);

// ===== FREECAD CONSTRAINT SOLVER =====
// Count degrees of freedom in a sketch.
int sketch_dof_count(Sketch *sk);

// Apply constraints iteratively until convergence.
int constraint_iterative_solve(Sketch *sk, int max_iterations, float tolerance);

// ===== iEDA SA COOLING SCHEDULE =====
// Proper cooling schedule: T_k = T_0 * alpha^k, where alpha = (T_end/T_0)^(1/n)
// Returns number of iterations and final temperature.
float sa_cooling_schedule(float T_start, float T_end, int total_moves, float *T_array, int max_temps);

// ===== KICAD NETLIST CONNECTIVITY =====
// Build full netlist including multi-sheet connections via labels.
// Returns netlist text.
char *netlist_full_connectivity(Project *p);

// ===== FREECAD FEATURE DEPENDENCY GRAPH =====
// Simple dependency tracking for design features.
typedef struct { int feature_id; int depends_on[8]; int n_deps; } FeatureDep;
void feature_dep_graph(DesignFeature *feats, int n, FeatureDep *deps, int max_deps);
char *feature_dep_report(DesignFeature *feats, int n, FeatureDep *deps, int ndeps);
