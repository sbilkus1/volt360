#pragma once
// svg_import.h - SVG path import for PCB footprints.
#include <stdbool.h>
#include "../model/project.h"

// Import SVG paths as PCB tracks/footprints.
bool svg_import_pcb(const char *path, Pcb *out);

// SVG basic path parser: extracts line segments from path data.
int svg_parse_path(const char *d, V2 **points); // malloc'd, caller frees
