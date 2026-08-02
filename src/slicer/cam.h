#pragma once
// cam.h - 2.5D CNC milling toolpath generator.
// Produces G-code for facing, profiling, pocket, drilling, and PCB isolation routing.
#include <stdbool.h>
#include "../model/project.h"

// Tool definition
typedef struct {
    float diameter;      // mm
    int spindle_rpm;
    float feed_xy;       // mm/min
    float feed_z;        // mm/min (plunge)
    float depth_per_pass; // mm per Z-level
} CamTool;

// CamSettings for a job
typedef struct {
    CamTool tool;
    float safe_z;        // rapid height (mm)
    float retract_z;     // between-passes height
    float clearance_xy;  // extra stock to leave (mm, negative = cut past)
    float stepover;      // fraction of tool diameter for pocket infill (0.3-0.9)
} CamSettings;

// Initialize default cam settings (1.6mm end mill, 12000 RPM, 800 mm/min)
void cam_defaults(CamSettings *s);

// --- 2.5D operations ---

// Face a rectangular region: mills down to target_z in passes.
bool cam_face(const char *path, const CamSettings *s, float x, float y, float w, float h, float target_z);

// Profile cut: follow a closed polygon in 2D, stepping down in Z.
bool cam_profile(const char *path, const CamSettings *s, V2 *poly, int n, float target_z);

// Pocket: clear interior of a closed polygon to target_z.
bool cam_pocket(const char *path, const CamSettings *s, V2 *poly, int n, float target_z);

// Drill: peck-drill holes at the given positions.
bool cam_drill(const char *path, const CamSettings *s, V2 *holes, int n, float target_z, float peck_delta);

// --- PCB-specific ---

// PCB isolation routing: trace around all pcb tracks to isolate them.
// `isolation_width` is the target clearance from copper (0 for exact edge).
bool cam_pcb_isolate(const char *path, const CamSettings *s, Pcb *pcb, float isolation_width, float target_z);

// PCB outline cutting: cut along the board outline (with tabs option).
bool cam_pcb_outline(const char *path, const CamSettings *s, Pcb *pcb, float target_z, int n_tabs, float tab_width);
