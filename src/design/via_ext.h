#pragma once
// via_ext.h - Blind/buried vias support.
#include <stdbool.h>
#include "../model/project.h"

// Add a via with specified start/end layers (0=top layer, N=bottom layer).
// type: 0=through, 1=blind, 2=buried
int via_add_typed(Pcb *pcb, V2 pos, float drill, float outer, int start_layer, int end_layer, const char *net);

// Count vias by type.
void via_count_types(Pcb *pcb, int *through, int *blind, int *buried);

// Convert all vias to through-hole (for manufacturing simplicity).
int via_convert_all_through(Pcb *pcb);
