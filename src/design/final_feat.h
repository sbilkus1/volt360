#pragma once
// final_feat.h - Interactive measurements, cross-probe, split, maintenance, design explain, stackup, curvature.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/farm.h"

// ===== INTERACTIVE MEASUREMENTS =====
// Measure distance between two points on PCB.
typedef struct { float x, y; } MeasurePoint;
float measure_distance(MeasurePoint a, MeasurePoint b);
float measure_angle(MeasurePoint a, MeasurePoint b, MeasurePoint c);
char *measure_report(MeasurePoint a, MeasurePoint b);

// ===== CROSS-PROBE VISUALIZATION =====
// Find matching schematic component for a PCB footprint.
const char *crossprobe_find_schematic_ref(Project *p, const char *pcb_fp_ref);
// Find matching PCB footprint for a schematic component.
const char *crossprobe_find_pcb_fp(Project *p, const char *sch_ref);
// Highlight both in a report.
char *crossprobe_highlight(Project *p, const char *ref);

// ===== OBJECT SPLIT =====
// Split mesh into connected components.
int object_split_count(CadMesh *m);
char *object_split_report(CadMesh *m);
int object_split_into_project(Project *p, CadMesh *m);

// ===== MAINTENANCE HISTORY + FIRMWARE =====
// Log a maintenance event.
void maint_log_event(Farm *f, const char *printer, const char *task, float hours);
char *maint_history_report(Farm *f);

// Firmware update stub.
char *firmware_update_status(const char *printer, const char *version);

// ===== DESIGN BLOCK PREVIEW + STACKUP =====
// Generate a visual preview of a design block region.
char *designblock_visual_preview(Pcb *pcb, V2 origin, float w, float h);

// PCB layer stackup manager.
typedef struct { char name[32]; float thickness; float er; char type[16]; } LayerStack;
char *stackup_default_4layer(void);
char *stackup_impedance_report(LayerStack *layers, int n, float target_z0);

// ===== AI DESIGN EXPLANATIONS =====
char *ai_explain_design(Pcb *pcb);
char *ai_troubleshoot(Pcb *pcb, const char *issue);
char *ai_suggest_improvements(Pcb *pcb);

// ===== CURVATURE + T-SPLINE =====
// Curvature visualization data for rendering heatmap.
char *curvature_visual_data(CadMesh *m);
// T-Spline stub.
char *tspline_stub(void);
