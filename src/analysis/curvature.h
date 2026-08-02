#pragma once
// curvature.h - Mesh curvature and surface quality analysis.
#include <stdbool.h>
#include "../model/project.h"

// Estimate mean curvature at each vertex (simplified).
// Fills curvature array (caller frees).
float *mesh_curvature_mean(CadMesh *m);

// Estimate Gaussian curvature.
float *mesh_curvature_gaussian(CadMesh *m);

// Surface roughness estimate (deviation from best-fit plane per face).
float mesh_roughness_rms(CadMesh *m);

// Full surface quality report.
char *curvature_report(CadMesh *m);
