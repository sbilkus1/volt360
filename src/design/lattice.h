#pragma once
// lattice.h - Lattice/grid structure generation for additive manufacturing.
#include <stdbool.h>
#include "../model/project.h"

// Generate a 3D grid lattice inside a bounding box.
// cell_size: spacing between lattice nodes (mm)
// strut_radius: thickness of each connecting beam (mm)
bool mesh_lattice_grid(V3 bmin, V3 bmax, float cell_size, float strut_radius, int strut_segs, CadMesh *out);

// Generate a gyroid lattice (triply periodic minimal surface approximation).
bool mesh_lattice_gyroid(V3 bmin, V3 bmax, float cell_size, float thickness, CadMesh *out);

// Generate a honeycomb lattice (2D extrusion of hex pattern).
bool mesh_lattice_honeycomb(V2 bmin, V2 bmax, float cell_size, float thickness, float z_height, CadMesh *out);
