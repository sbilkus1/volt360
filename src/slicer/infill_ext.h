#pragma once
// infill_ext.h - Extended infill patterns: gyroid, cubic, lightning.
#include <stdbool.h>
#include "../model/project.h"

// Generate gyroid-style infill for a polygon (approximated as sine-wave lines).
// Returns number of segments written to `out`.
int infill_gyroid(V2 *poly, int n, float spacing, float angle, V2 *out, int maxseg);

// Generate lightning infill (sparse internal support structure).
// Returns number of segments.
int infill_lightning(V2 *poly, int n, float line_w, V2 *out, int maxseg);

// Generate cubic infill pattern (3D lattice projected).
int infill_cubic(V2 *poly, int n, float spacing, int layer_idx, V2 *out, int maxseg);
