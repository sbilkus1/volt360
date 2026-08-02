#pragma once
// supports.h - Auto-support generation (tree/organic/grid).
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"

// Generate support pillars under overhanging regions of a sliced model.
// Adds support paths to each layer's paths array.
// overhang_angle_deg: angle from horizontal that triggers supports (e.g. 45)
void supports_generate(SliceResult *sr, const SliceSettings *cfg, float overhang_angle_deg);
