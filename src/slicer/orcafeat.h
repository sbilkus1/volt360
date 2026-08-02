#pragma once
// orcafeat.h - OrcaSlicer-specific features: brim ears, arcs, tree supports, profiles.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"

// Mouse ear brims: generate small circular brim pads at model corners.
// Adds brim paths to the first layer of the slice result.
void brim_add_mouse_ears(SliceLayer *first_layer, V2 *poly, int n, float ear_radius, float ear_count, float line_w);

// Adaptive bed mesh: generate G29 / BED_MESH_CALIBRATE commands.
char *bed_mesh_gcode(const char *printer_type, float x_min, float x_max, float y_min, float y_max, int points);

// Arcs support: check if arcs are enabled in settings.
bool arcs_enabled(const SliceSettings *cfg);

// Generate G2/G3 arc move G-code.
char *arc_gcode(float cx, float cy, float ex, float ey, float r, bool clockwise, float feed);

// Tree/organic supports: generate dendritic support structure.
// Adds support paths to slice result layers.
void supports_generate_tree(SliceResult *sr, const SliceSettings *cfg, float overhang_deg);

// Printer profile definitions for popular printers.
typedef struct {
    char name[48];
    float bed_x, bed_y, bed_z;
    float nozzle_dia;
    int max_temp;
    float max_speed;
    char firmware[16]; // "marlin", "klipper", "rff"
} PrinterProfile;

PrinterProfile *printer_profile(const char *name);
char *printer_profile_list(void);

// Belt/conveyor printer slicing: rotate model 45° for belt.
void belt_rotate_mesh(CadMesh *m);

// Bambu networking plugin stub.
char *bambu_plugin_status(void);
