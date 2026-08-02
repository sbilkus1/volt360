#pragma once
// primitives.h - Additional CAD primitives: cone, torus, wedge, pyramid.
#include <stdbool.h>
#include "../model/project.h"

// Cone: radius at base, height, segments. Apex at (cx, cy, cz + h/2).
void mesh_cone(CadMesh *m, V3 center, float radius, float height, int segs);

// Torus: ring of radius R with tube radius r.
void mesh_torus(CadMesh *m, V3 center, float R, float r, int segs, int ring_segs);

// Wedge: right-angled triangular prism (like half a box).
void mesh_wedge(CadMesh *m, V3 c, V3 s);

// Pyramid: square base, point at top.
void mesh_pyramid(CadMesh *m, V3 center, float base_w, float base_d, float height);
