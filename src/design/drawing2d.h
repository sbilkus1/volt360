#pragma once
// drawing2d.h - 2D orthographic drawing generator from CAD meshes.
#include <stdbool.h>
#include "../model/project.h"

// Generate a dimensioned 2D drawing (text format) from a mesh's bounding box.
// Views: front (XY), top (XZ), right (YZ).
// Returns malloc'd text.
char *drawing2d_from_mesh(CadMesh *mesh, const char *title);

// Generate a DXF-like outline of the mesh projected to XY plane.
char *drawing2d_outline(CadMesh *mesh);

// Generate a bill of materials drawing page (text format).
char *drawing2d_bom_sheet(Project *p, const char *title);
