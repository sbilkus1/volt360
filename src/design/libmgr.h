#pragma once
// libmgr.h - Footprint/symbol library manager.
#include <stdbool.h>
#include "../model/project.h"

// Export a footprint to a .kicad_mod file.
bool lib_export_footprint(Footprint *fp, const char *path);

// Import footprints from a directory into the project.
int lib_import_footprints(Project *p, const char *dir);

// Export all footprints in the project.
int lib_export_all_footprints(Project *p, const char *dir);

// List all library items as a text report.
char *lib_list_report(Project *p);
