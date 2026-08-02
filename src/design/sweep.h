#pragma once
// sweep.h - Extrude a 2D profile along a 3D path (sweep surface).
#include <stdbool.h>
#include "../model/project.h"

// Sweep a 2D profile along a 3D polyline path.
// profile: 2D polygon (CCW), n_profile: number of profile vertices
// path: 3D polyline points, n_path: number of path points
// closed: whether to close the end caps
bool mesh_sweep(V2 *profile, int n_profile, V3 *path, int n_path, bool closed, CadMesh *out);
