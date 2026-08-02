#pragma once
// gerber_out.h - RS-274X Gerber file writer from PCB data.
#include <stdbool.h>
#include "../model/project.h"

// Write copper layer Gerber file.
// layer_name: e.g. "F_Cu", "B_Cu"
bool gerber_write_copper(Pcb *pcb, const char *path, const char *layer_name);

// Write board outline (Edge.Cuts equivalent).
bool gerber_write_outline(Pcb *pcb, const char *path);

// Write drill file (Excellon format).
bool gerber_write_drill(Pcb *pcb, const char *path);

// Write all manufacturing files to a directory.
bool gerber_write_all(Pcb *pcb, const char *dir);
