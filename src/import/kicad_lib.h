#pragma once
// kicad_lib.h - KiCad library parser: symbols (.kicad_sym), footprints (.kicad_mod), 3D models.
// Unified component = symbol + footprint + 3D model + datasheet.
#include <stdbool.h>
#include "../model/project.h"

// ===== LIBRARY ITEM =====
// Unified component that links symbol, footprint, 3D model, and datasheet.
typedef struct {
    char *name;           // e.g. "R_0805_2012Metric"
    char *description;
    char *keywords;       // search tags
    char *symbol_name;    // references a Symbol in the project
    char *footprint_name; // references a Footprint
    char *model3d_path;   // relative path to .step/.wrl 3D model
    char *datasheet_url;  // URL to manufacturer datasheet
    // merged fields
    int merged_from;      // how many duplicates were merged
    char **aliases; int n_aliases; // alternate names merged in
} LibComponent;

// ===== SYMBOL LIBRARY PARSER =====
// Parse a .kicad_sym library file and import all symbols into the project.
// Returns number of symbols imported.
int kicad_import_sym_lib(Project *p, const char *lib_path);

// Parse a single symbol definition from S-expression text.
Symbol *kicad_parse_symbol_def(const char *s_expr, const char *def_name);

// ===== FOOTPRINT LIBRARY PARSER =====
// Parse a .kicad_mod file or .pretty directory and import footprints.
int kicad_import_fp_lib(Project *p, const char *lib_path);

// Parse a single footprint from .kicad_mod text.
Footprint *kicad_parse_footprint(const char *s_expr);

// ===== 3D MODEL ASSOCIATION =====
// Scan a directory for 3D models (.step, .wrl) and link to footprints by name.
int kicad_link_3d_models(Project *p, const char *models_dir);

// ===== UNIFIED COMPONENT SYSTEM =====
// Build a unified component from existing symbol + footprint + 3D model.
LibComponent *libcomponent_create(Project *p, const char *sym_name, const char *fp_name,
                                   const char *model3d, const char *datasheet);
void libcomponent_free(LibComponent *lc);

// Merge two library components (deduplication).
// Keeps the first, adds aliases from the second.
void libcomponent_merge(LibComponent *keep, LibComponent *other);

// Find merged component by any of its alias names.
LibComponent *libcomponent_find(LibComponent *lib, int n_items, const char *name);

// Export library component to project (creates Symbol + Footprint + Component entries).
bool libcomponent_export_to_project(Project *p, LibComponent *lc);

// ===== TEMPLATE SYSTEM =====
// Built-in templates.
typedef enum { TMPL_ARDUINO_UNO, TMPL_PSU_LINEAR, TMPL_H_BRIDGE, TMPL_OPAMP, TMPL_555_TIMER } TemplateID;

// Create a project from a template (adds schematic, PCB, components).
bool template_apply(Project *p, TemplateID tid);

// Get template info text.
char *template_info(TemplateID tid);

// ===== LIBRARY BROWSER =====
// Search library components by name/keywords.
// Returns comma-separated matching names.
char *lib_browser_search(LibComponent *lib, int n_items, const char *query);

// Generate library report text.
char *lib_browser_report(LibComponent *lib, int n_items);
