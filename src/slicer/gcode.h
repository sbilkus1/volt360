#pragma once
// gcode.h - G-code writer for the slicer. Emits standard Marlin/Klipper-flavoured
// gcode with temps, retraction, and optional AMS/CFS multicolor (random tool) +
// M600 color-change injection.
#include "slicer.h"

// Pick a random AMS/CFS tool index (0..tools-1), or -1 when disabled.
int gcode_pick_tool(const SliceSettings *cfg);

// Core writer: writes into an already-open stream. Uses cfg->forced_tool if
// >= 0, otherwise auto selection.
bool gcode_emit_fp(FILE *f, SliceResult *r, const SliceSettings *cfg);

// Core writer with an explicit forced tool (ignored when < 0).
bool gcode_emit_fp_tool(FILE *f, SliceResult *r, const SliceSettings *cfg, int forced_tool);

// Convenience wrapper that opens `path` and emits the full file.
bool gcode_emit(SliceResult *r, const SliceSettings *cfg, const char *path);
