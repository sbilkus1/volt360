#pragma once
// dxf_import.h - Simple 2D DXF file importer for outlines and tracks.
#include <stdbool.h>
#include "../model/project.h"

// Import a 2D DXF file. Reads LINE, LWPOLYLINE, CIRCLE entities into a Pcb.
bool dxf_import(const char *path, Pcb *out);