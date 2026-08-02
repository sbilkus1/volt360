#pragma once
// finalmods.h - Conveyor/robot control, sheet nav, backup, BOM, live refresh, render materials.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/farm.h"
#include "../slicer/slicer.h"

// Conveyor belt control: generate auto-eject with conveyor sequence.
char *conveyor_eject_gcode(const char *surface_type);

// Robot arm part removal G-code sequence.
char *robot_eject_gcode(V3 part_center, float approach_height);

// Multi-sheet navigation: switch to a specific schematic sheet.
Schematic *sheet_navigate(Project *p, int sheet_index);

// Project backup: copy project.json + all assets to a timestamped backup dir.
char *project_backup(Project *p);

// Project restore from backup.
bool project_restore(Project *p, const char *backup_path);

// Full BOM with cost estimation.
char *bom_with_cost(Project *p, float cost_per_component);

// Live farm status: refresh printer statuses (mark offline if unresponsive).
void farm_live_refresh(Farm *f);

// Render materials: apply render properties to a model for display.
typedef struct { float r,g,b,a; float rough, metal; } RenderProps;
RenderProps render_material_for_model(CadModel *cm, const char *material_name);

// Status history tracking.
typedef struct { char timestamp[32]; char event[128]; } StatusEvent;
void status_log_event(const char *event);
char *status_history_report(void);

// Trend analytics: report trends over time.
char *trend_analytics_report(void);
