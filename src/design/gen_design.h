#pragma once
// gen_design.h - generative design via SIMP topology optimization
#include "../model/project.h"

typedef struct {
    int nx, ny;             // element grid resolution (e.g. 48 x 30)
    float volfrac;          // volume fraction constraint (0..1, e.g. 0.4)
    float load_xf;          // load position x fraction (0..1, 0=left 1=right)
    float load_yf;          // load position y fraction (0..1, 0=bottom)
    int fix_left;           // 1 = fix entire left edge
    int fix_bottom;         // 1 = also fix bottom edge
    int iters;              // optimization iterations
    double *dens;           // nx*ny densities in [0,1] (allocated by run)
} GenDesign;

// Run topology optimization (fills gd->dens). Returns true on success.
bool gen_design_run(GenDesign *gd);

// Convert density field into a triangle mesh of voxel boxes extruded by depth_mm.
// cell_mm is the size of one element in mm. Returns true on success.
bool gen_design_mesh(GenDesign *gd, float cell_mm, float depth_mm, CadMesh *out);

void gen_design_free(GenDesign *gd);
