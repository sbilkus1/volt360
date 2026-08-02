#pragma once
// annotations.h - Schematic annotation support.
#include <stdbool.h>
#include "../model/project.h"

// Auto-annotate schematic: assign sequential reference designators.
int annotations_auto_assign(Schematic *s);

// Cross-probe: find PCB footprints matching schematic component positions.
// Returns report text.
char *crossprobe_report(Schematic *s, Pcb *pcb);
