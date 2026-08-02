#pragma once
// more_414.h - Object manipulation, overhang, purchasing, tags, blocks, footprint, difpair, 3MF.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"
#include "../slicer/farm.h"

// ===== OBJECT MANIPULATION =====
// Scale a CAD model by factors.
void object_scale(CadModel *cm, float sx, float sy, float sz);
// Rotate a CAD model around its center.
void object_rotate(CadModel *cm, float rx, float ry, float rz);
// Split and add components to project. Returns count added.
int object_split_add(Project *p, CadModel *cm);
// Manipulation report.
char *object_manip_report(CadModel *cm);

// ===== OVERHANG VISUALIZATION =====
// Detect overhang regions in a sliced result.
// Returns percentage of perimeter that is overhanging.
float overhang_detect_pct(SliceResult *sr);
// Visualize overhang regions as text.
char *overhang_report(SliceResult *sr);

// ===== PURCHASE FORECASTING =====
// Forecast filament purchases based on usage.
typedef struct { char filament[32]; float used_per_week; float spool_cost; int weeks_remaining; } PurchaseForecast;
int purchase_forecast_run(Farm *f, PurchaseForecast *forecasts, int max);
char *purchase_forecast_report(PurchaseForecast *fcs, int n);

// ===== SMARTTAGS SEARCH =====
// Search farm files by tag.
char *smarttags_search(Farm *f, const char *query);
// List all unique tags.
char *smarttags_list(Farm *f);

// ===== DESIGN BLOCKS BROWSER =====
// Save current view as design block.
char *designblock_save_region(Pcb *pcb, V2 origin, float w, float h, const char *name);
// List saved blocks.
char *designblock_list(void);
// Apply a saved block at position.
int designblock_apply(Pcb *pcb, const char *name, V2 pos);

// ===== CUSTOM FOOTPRINT WIZARD =====
// Create a custom SMD pad footprint with given parameters.
Footprint *footprint_wizard_smd(Project *p, const char *name, int n_pads, float pitch, float pad_w, float pad_h);
// Create a DIP/THT footprint.
Footprint *footprint_wizard_dip(Project *p, const char *name, int n_pins, float pitch, float row_spacing);
// Footprint wizard report.
char *footprint_wizard_report(Footprint *fp);

// ===== DIFFERENTIAL PAIR RULES EDITOR =====
// Set differential pair routing rules.
typedef struct { float track_width; float track_spacing; float max_unpaired_length; float target_impedance; } DiffPairRules;
void difpair_rules_set(Pcb *pcb, DiffPairRules rules);
DiffPairRules difpair_rules_get(void);
char *difpair_rules_report(DiffPairRules rules);

// ===== 3MF IMPORT + ADAPTIVE IRONING UI =====
// 3MF file import stub.
bool mesh_import_3mf_stub(const char *path, CadMesh *out);

// Adaptive ironing: compute ironing settings for top layers.
char *ironing_adaptive_settings(SliceResult *sr, const SliceSettings *cfg);
