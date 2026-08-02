#pragma once
// meshops.h - Mesh operations: split, merge, decimate, analyze.
#include <stdbool.h>
#include "../model/project.h"

// Split a mesh into connected components. Returns number of components.
// Each component is pushed as a separate CadMesh into `out` array (caller allocates).
// Use mesh_free on each.
int mesh_split_components(CadMesh *in, CadMesh **out_components);

// Find the largest connected component and return it in `out`.
// Returns true on success; out is heap-allocated (caller calls mesh_free).
bool mesh_largest_component(CadMesh *in, CadMesh *out);

// Compute number of connected components without splitting.
int mesh_component_count(CadMesh *in);

// Compute bounding box diagonal (mm).
float mesh_size(CadMesh *in);

// Deep-copy a mesh (caller frees dst with mesh_free).
void mesh_copy(CadMesh *src, CadMesh *dst);
