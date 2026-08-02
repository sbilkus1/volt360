#pragma once
// teardrop.h - KiCad-style teardrop (fillet) generator for PCB tracks at pads.
#include <stdbool.h>
#include "../model/project.h"

// Add teardrops to all tracks connecting to pads in a PCB.
// Modifies the Pcb in-place, adding short segments at pad connections.
// teardrop_length: how far the fillet extends from the pad (mm)
// teardrop_width: max additional width at the pad (mm)
int teardrop_add(Pcb *pcb, float teardrop_length, float teardrop_width);

// Remove teardrops (identifies and removes short segments near pads).
// This is a heuristic - removes tracks shorter than max_len that have
// one endpoint near a footprint pad.
int teardrop_remove(Pcb *pcb, float max_len);
