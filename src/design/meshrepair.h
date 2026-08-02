#pragma once
// meshrepair.h - Mesh repair: hole fill, normal correction, orientation fix.
#include <stdbool.h>
#include "../model/project.h"

// Fix inverted normals (orient all face normals outward from centroid).
int mesh_fix_normals(CadMesh *m);

// Fill small holes by finding boundary edges and capping them.
int mesh_fill_holes(CadMesh *m, float max_hole_diag);

// Remove duplicate/zero-area triangles.
int mesh_remove_degenerate(CadMesh *m, float min_area);

// Auto-repair: run all repair passes.
void mesh_auto_repair(CadMesh *m);
