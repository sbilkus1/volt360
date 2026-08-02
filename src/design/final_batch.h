#pragma once
// final_batch.h - Last batch of 414 features.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"
#include "../slicer/farm.h"

// ===== OBJECT MANIPULATION PANEL =====
char *object_manip_panel(CadModel *cm); // scale/rotate/position summary

// ===== OVERHANG + BRIDGE VISUALIZATION =====
char *bridge_detect_report(SliceResult *sr);
char *overhang_viz_report(CadMesh *m, float angle);

// ===== NET CLASSES DIALOG =====
char *netclass_dialog_text(void); // editable net class table

// ===== FOOTPRINT WIZARD DIALOG =====
char *footprint_wizard_dialog(void); // step-by-step wizard text

// ===== COPPER POUR VISUALIZATION =====
char *copper_pour_viz_report(Pcb *pcb);

// ===== PASSIVE CONSOLIDATION + PIN LISTING =====
char *passive_consolidation(Project *p);
char *pin_listing_full(Project *p);

// ===== LIVE WEBCAM + SMARTTAGS + DOWNTIME =====
char *webcam_live_preview(void);
char *smarttags_filtered_list(Farm *f, const char *filter);
char *downtime_report_detailed(Farm *f);

// ===== SUPPORT/COLOR PAINTING =====
char *support_paint_ui_status(void);
char *face_paint_ui_status(void);
char *color_paint_format_presets(void);

// ===== 3MF + AI AUTO-LAYOUT + CURVATURE HEATMAP =====
char *import_3mf_status(const char *path);
char *ai_autolayout_estimate(Pcb *pcb);
char *curvature_heatmap_data(CadMesh *m);
