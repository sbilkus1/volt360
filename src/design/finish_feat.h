#pragma once
// finish_feat.h - Mesh repair, hollow, orient, signal trace, AI learn, remote, batch, section, net classes.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"
#include "../slicer/farm.h"

// ===== MESH REPAIR UI =====
typedef struct { int degenerates; int normals_fixed; int holes_found; char status[128]; } MeshRepairResult;
MeshRepairResult mesh_repair_full(CadMesh *m);
char *mesh_repair_report(MeshRepairResult r);

// ===== HOLLOWING PREVIEW =====
char *hollow_preview(CadMesh *m, float wall_thickness);

// ===== AUTO-ORIENT VISUAL =====
char *auto_orient_report(CadMesh *m);

// ===== SIGNAL TRACING =====
// Trace a signal through the schematic — follow net connections.
char *signal_trace(Schematic *s, const char *start_pin_name);

// ===== AI LEARNING ASSISTANT =====
char *ai_learn_topic(const char *topic);
char *ai_explain_pcb_concept(const char *concept);

// ===== SECURE REMOTE ACCESS =====
char *remote_access_tunnel_status(const char *farm_id);

// ===== MULTI-PRINTER BATCH =====
char *batch_control_start_all(Farm *f);
char *batch_control_stop_all(Farm *f);
char *batch_status_report(Farm *f);

// ===== SECTION VIEW =====
char *section_view_report(CadMesh *m, float cut_z);
char *section_view_slice(CadMesh *m, float cut_z);

// ===== NET CLASSES EDITOR =====
typedef struct { char name[32]; float track_width; float clearance; float via_drill; float via_outer; } NetClassDef;
int netclass_define(Pcb *pcb, NetClassDef nc);
char *netclass_list_report(Pcb *pcb);
