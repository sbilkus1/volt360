#pragma once
// finish_414.h - Remaining 414 features: face/color paint, cost, bus, curvature, BOM, ironing.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"
#include "../slicer/farm.h"

// ===== ORCASLICER: FACE/COLOR PAINTING =====
// Paint a face (set of triangles) with a specified AMS color index.
int paint_face_color(CadMesh *mesh, int *tri_indices, int n_tris, int color_index);

// Get all triangles visible from a given camera direction.
int paint_visible_tris(CadMesh *mesh, V3 camera_dir, int *tris, int max_tris);

// ===== ORCASLICER: PRIME TOWER + PURGE =====
// Compute optimal prime tower position on build plate.
V2 primetower_optimal_position(V2 *model_centers, int n_models, float bed_x, float bed_y, float tower_w, float tower_h);

// Estimate filament waste across all color changes in a print.
float purge_total_waste(const SliceSettings *cfg, int color_changes);

// ===== AUTOFARM3D: COST/REVENUE/ENERGY/GROUPING =====
// Track print costs and revenue.
typedef struct { float filament_cost; float electricity_cost; float labor_cost; float price_charged; char job_name[64]; } CostEntry;
float cost_profit_estimate(float filament_m, float hours, float filament_cost_per_kg, float electricity_cost_per_kwh, float labor_rate_per_hour, float markup_pct);
char *cost_report(Farm *f);

// Printer grouping: assign printers to named groups.
typedef struct { char name[32]; int printer_indices[8]; int n_printers; } PrinterGroup;
int farm_group_create(Farm *f, const char *group_name, int *printer_indices, int n);
char *farm_group_report(Farm *f, PrinterGroup *groups, int n_groups);
void farm_group_action(PrinterGroup *group, const char *action);
extern PrinterGroup g_groups[8];
extern int g_ng; // "start", "stop", "pause"

// ===== KICAD: BUS + NET CLASSES + DESIGN BLOCKS =====
// Create a bus from specified signals.
void bus_create_with_signals(Schematic *s, const char *bus_name, const char **signals, int n);

// Assign net class to matched tracks.
int netclass_assign_by_pattern(Pcb *pcb, const char *pattern, float track_width, float clearance);

// Design block: save/restore with preview.
char *designblock_preview(Pcb *pcb, V2 origin, float w, float h);

// ===== FREECAD: CURVATURE + GDT =====
// Curvature heat map data (0-1 values, one per vertex).
float *curvature_heatmap(CadMesh *m);

// GD&T feature control frame text.
char *gdt_feature_frame(const char *feature, float nominal, float tol_plus, float tol_minus, const char *datum);

// ===== FLUX AI: BOM + MANUFACTURING OPTIMIZATION =====
// Optimize BOM: consolidate duplicates, suggest alternatives.
char *bom_optimize(Project *p);

// Manufacturing optimization: suggest cheapest fab house, panelization.
char *manufacturing_optimize(Pcb *pcb, int quantity);

// ===== ORCASLICER: ADAPTIVE IRONING + AUTO-ORIENT UI =====
// Adaptive ironing intensity by surface curvature.
float ironing_intensity(float curvature, float base_flow);

// Auto-orient: rotate mesh for best print orientation.
V3 auto_orient_angles(CadMesh *m);
