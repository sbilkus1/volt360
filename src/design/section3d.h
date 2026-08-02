#pragma once
// section3d.h - 3D section/clipping plane view.
#include <stdbool.h>
#include "../model/project.h"

// Clipping plane: defined by point + normal.
typedef struct { V3 point; V3 normal; float offset; } ClipPlane;

// Apply clipping plane to mesh: keep vertices on positive side of plane.
// Fills output mesh with clipped geometry.
bool mesh_clip_plane(CadMesh *in, CadMesh *out, ClipPlane plane);

// Create section view text report.
char *section3d_report(CadMesh *m, ClipPlane plane);
