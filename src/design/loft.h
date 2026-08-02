#pragma once
// loft.h - Surface loft between two 2D profiles.
#include <stdbool.h>
#include "../model/project.h"

// Loft between two 2D profiles at different Z heights.
// profile_a and profile_b must have the same number of vertices.
// z_a, z_b: Z heights of the profiles.
bool mesh_loft(V2 *profile_a, V2 *profile_b, int n_verts, float z_a, float z_b, CadMesh *out);
