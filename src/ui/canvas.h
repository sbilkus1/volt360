#pragma once
// canvas.h - Per-mode canvas engines: EDA (KiCad+iEDA), CAD (FreeCAD), Slicer (OrcaSlicer).
#include <stdbool.h>
#include "../model/project.h"
#include "../ui/ui.h"

// ===== COMMON CANVAS =====
typedef struct {
    int x, y, w, h;       // canvas rect
    float zoom; V2 pan;   // view transform
    float grid_size;
    int show_grid;
    int show_origin;       // crosshair at origin
} CanvasView;

CanvasView canvas_view_create(int x, int y, int w, int h);

// ===== EDA CANVAS (KiCad+iEDA) =====
// Draw the EDA canvas background: KiCad-style grid + origin crosshair.
void canvas_eda_background(CanvasView *cv);

// Draw KiCad-style origin crosshair (blue axes).
void canvas_eda_origin(CanvasView *cv);

// Draw iEDA-style placement heat map (color by wire length density).
void canvas_eda_heatmap(Pcb *pcb, CanvasView *cv);

// KiCad-style right-click context menu for EDA.
// Returns action string (caller frees) or NULL.
char *canvas_eda_context_menu(int mx, int my, Pcb *pcb, int sel_fp);

// ===== CAD CANVAS (FreeCAD) =====
// Draw FreeCAD-style workbench tabs.
void canvas_cad_workbench_tabs(int x, int y, int w);

// Draw FreeCAD-style feature tree in sidebar area.
void canvas_cad_feature_tree(DesignFeature *feats, int n, int sel, int x, int y, int w, int h);

// Draw transform gizmo at position (translate arrows).
void canvas_cad_gizmo(V3 pos, int *drag_axis, int x, int y);

// ===== SLICER CANVAS (OrcaSlicer) =====
// Draw OrcaSlicer-style build plate background.
void canvas_slicer_build_plate(CanvasView *cv, float bed_x, float bed_y);

// Draw OrcaSlicer-style model arrangement on plate.
void canvas_slicer_model_arrange(SliceResult *sr, CanvasView *cv);

// Draw slice preview layer.
void canvas_slicer_layer_preview(SliceResult *sr, int layer_idx, CanvasView *cv);

// Draw OrcaSlicer-style stats panel (filament, time, cost).
void canvas_slicer_stats_panel(SliceResult *sr, const SliceSettings *cfg, int x, int y);

// Draw layer navigation slider.
int canvas_slicer_layer_slider(int x, int y, int w, int n_layers);

// ===== MIXED CANVAS RENDERER =====
// Main canvas renderer: dispatches to the appropriate engine based on mode.
void canvas_render(App *app, CanvasView *cv);
