#pragma once
// painting.h - Surface painting tools for supports, seams, and color marking.
#include <stdbool.h>
#include "../model/project.h"

// Paint type: what the painted region represents
typedef enum { PAINT_SUPPORT, PAINT_SUPPORT_BLOCKER, PAINT_SEAM, PAINT_COLOR } PaintType;

// A painted region on the mesh surface (approximated as triangle indices).
typedef struct {
    int *tri_indices;   // triangles in this painted region
    int n_tris;
    PaintType type;
    int color_index;    // for PAINT_COLOR: AMS/MMU color index
} PaintedRegion;

// Container for all painted regions on a model.
typedef struct {
    PaintedRegion *regions;
    int n_regions;
} PaintData;

void paint_data_free(PaintData *pd);

// Paint a set of triangles with a given type.
void paint_region_add(PaintData *pd, int *tris, int n_tris, PaintType type, int color);

// Remove all paint of a given type.
void paint_region_clear(PaintData *pd, PaintType type);

// Check if a triangle is painted with a given type.
bool paint_is_painted(PaintData *pd, int tri_idx, PaintType type);
