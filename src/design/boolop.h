#pragma once
// boolop.h - Boolean operations on meshes (union, intersect, difference).
#include <stdbool.h>
#include "../model/project.h"

// Compute a boolean intersection of two boxes (A && B).
// Works on the bounding boxes for simplicity.
V3 bool_intersect_bbox(V3 a_min, V3 a_max, V3 b_min, V3 b_max, bool *valid);

// Compute bounding box of union (A || B).
void bool_union_bbox(V3 a_min, V3 a_max, V3 b_min, V3 b_max, V3 *out_min, V3 *out_max);

// Compute difference box (A - B): clips A by removing overlap with B.
// Returns number of resulting boxes (0-3).
int bool_diff_box(V3 a_min, V3 a_max, V3 b_min, V3 b_max,
                  V3 *out_mins, V3 *out_maxs, int max_out);

// Generate a mesh for the intersection of two bbox-defined regions.
bool bool_intersect_mesh(CadMesh *a, CadMesh *b, CadMesh *out);

// Generate a mesh for the union of two meshes (simple: combines verts).
// Mesh must be properly merged/tesselated for clean results.
bool bool_union_mesh(CadMesh *a, CadMesh *b, CadMesh *out);
