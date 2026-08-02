#pragma once
// hollow.h - Mesh shell/hollow operation.
#include <stdbool.h>
#include "../model/project.h"

// Create a hollow shell of the input mesh with given wall thickness.
// wall_thickness_mm: inward offset distance.
bool mesh_hollow(CadMesh *in, CadMesh *out, float wall_thickness_mm);

// Reduce mesh triangle count by removing short edges.
bool mesh_decimate(CadMesh *in, CadMesh *out, float target_ratio);
