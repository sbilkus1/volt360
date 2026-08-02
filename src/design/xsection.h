#pragma once
// xsection.h - Cross-section view of mesh at a plane.
#include <stdbool.h>
#include "../model/project.h"

// Slice a mesh at a Z plane and return the resulting 2D polygon outlines.
// Returns number of loops. out_polys array of V2 arrays must be freed by caller.
int mesh_cross_section(CadMesh *m, float z, V2 ***out_polys, int **out_poly_lens);

// Render a cross-section view as text.
char *mesh_xsection_text(CadMesh *m, float z);
