#pragma once
// turning.h - 2-axis CNC lathe toolpath generator.
#include <stdbool.h>

typedef struct {
    float spindle_rpm;
    float feed_mm_rev;    // feed per revolution (mm/rev)
    float doc_mm;         // depth of cut per pass (mm)
    float safe_x;         // safe retract diameter
    float safe_z;         // safe retract Z
} TurnTool;

void turn_tool_defaults(TurnTool *t);

// Facing: face from OD to center at Z position.
bool turn_face(const char *path, TurnTool *t, float start_diam, float target_z, int passes);

// Turning (external): reduce diameter from start to target along length.
bool turn_cyl(const char *path, TurnTool *t, float start_diam, float target_diam, float length_z, int passes);

// Grooving: cut a groove of given width and depth at Z position.
bool turn_groove(const char *path, TurnTool *t, float diam, float groove_diam, float groove_width, float z_start);

// Threading: single-point threading cycle.
bool turn_thread(const char *path, TurnTool *t, float major_diam, float minor_diam, float pitch, float length_z, int passes);

// Parting off: cut through from OD to center at Z.
bool turn_partoff(const char *path, TurnTool *t, float diam, float z_pos);
