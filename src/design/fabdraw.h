#pragma once
// fabdraw.h - Fabrication drawing export with GD&T callouts.
#include <stdbool.h>
#include "../model/project.h"

// Generate a fabrication drawing text report for a PCB.
char *fabdraw_report(Pcb *pcb);

// Generate GD&T callout text for a feature dimension.
char *fabdraw_gdt_feature(const char *name, float nominal, float tolerance_plus, float tolerance_minus);

// Generate a full manufacturing package (Gerber + drill + fab drawing).
bool fabdraw_export_manufacturing(Pcb *pcb, const char *output_dir);
