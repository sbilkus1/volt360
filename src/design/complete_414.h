#pragma once
// complete_414.h - Final 28 features: webcam frame, tags UI, profile edit, maintenance, spool alerts,
// energy dash, QR scan, batch actions, paint brush, face color, manip sliders, 3MF import,
// ironing panel, AI schematic, design explain, SI viz, BOM prices, panelization,
// stackup calc, meander viz, gerber viewer, 3D preview, pin swap,
// curvature map, T-Spline, cloud render, explode animation, section slider.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"
#include "../slicer/farm.h"

// ===== AUTOFARM3D (8 features) =====
char *webcam_capture_frame_data(void);          // actual frame data status
char *smarttags_search_ui(Farm *f, const char *q);// search with formatted results
char *printer_profile_editor(Farm *f, int idx);    // edit printer name/filament/slots
char *maintenance_schedule_ui(Farm *f);            // add/edit/delete maint intervals
char *spool_low_alert(Farm *f, float threshold);   // alert when spools below N meters
char *energy_cost_dashboard(Farm *f);              // per-printer energy cost
char *qr_scanner_from_webcam(void);                // QR decode from webcam
char *printer_group_batch_actions(Farm *f);         // start/stop/pause groups

// ===== ORCASLICER (5 features) =====
char *support_paint_brush_ui(void);              // brush radius/type selector
char *face_color_painting_ui(void);              // color assignment per face
char *object_manip_sliders(CadModel *cm);         // scale X/Y/Z sliders
bool import_3mf_actual(const char *path, CadMesh *out); // actual 3MF import
char *ironing_settings_panel(SliceSettings *cfg);  // adaptive ironing controls

// ===== FLUX AI (5 features) =====
char *ai_schematic_modify(Schematic *s, const char *cmd); // add wire/component
char *ai_design_explain_full(Pcb *pcb);           // full circuit analysis
char *signal_integrity_viz_report(Pcb *pcb);      // SI visualization data
char *bom_price_comparison(Project *p);            // vendor price shopping
char *manufacturing_panelization(Pcb *pcb, int qty);// panelization layout

// ===== KICAD (5 features) =====
char *stackup_impedance_calc(void);               // impedance calculator
char *length_tune_meander_viz(Pcb *pcb);           // meander pattern preview
char *gerber_layer_viewer(void);                   // layer toggle viewer
char *footprint_3d_preview(Footprint *fp);         // 3D model preview status
char *pin_swap_optimizer(Pcb *pcb);                // optimize pin assignments

// ===== FREECAD (5 features) =====
char *curvature_color_map(CadMesh *m);            // vertex color data for heatmap
char *tspline_control_mesh(void);                  // T-Spline control point UI
char *cloud_render_queue(void);                    // render queue status
char *exploded_view_animation(Assembly *as);       // explode/implode animation data
char *section_clip_plane_slider(CadMesh *m);       // clip plane Z slider status
