#pragma once
// copperpour.h - PCB copper pour / zone fill generator.
#include <stdbool.h>
#include "../model/project.h"

// Fill a zone with a hatch pattern of copper traces.
// zone_poly: polygon defining the pour area (clockwise)
// clearance: distance from other objects (tracks, pads)
// hatch_spacing: distance between fill lines
// Adds tracks to the PCB for the fill pattern.
int copper_pour_fill(Pcb *pcb, V2 *zone_poly, int n_pts, float clearance, float hatch_spacing, float track_width);

// Auto-fill the board outline area with a copper pour (GND plane).
// clearance from board edge, pads, and tracks.
int copper_pour_ground_plane(Pcb *pcb, float edge_clearance, float object_clearance, float hatch_spacing);
