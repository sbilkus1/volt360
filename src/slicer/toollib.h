#pragma once
// toollib.h - CAM tool library and post-processors.
#include <stdbool.h>

typedef struct {
    char name[32];
    int tool_number;
    float diameter_mm;
    float length_mm;
    float flutes;
    int spindle_rpm;
    float feed_xy_mm_min;
    float feed_z_mm_min;
    char type[16]; // "endmill", "ball", "drill", "vbit", "facemill", "tap"
} CamToolLib;

// Built-in tool library.
int toollib_count(void);
CamToolLib *toollib_get(int index);
CamToolLib *toollib_find(const char *name);

// Post-processor: convert generic G-code to machine-specific format.
// controller: "grbl", "mach3", "linuxcnc", "haas", "fanuc"
char *cam_postprocess(const char *gcode_path, const char *controller);

// Export tool list as text.
char *toollib_report(void);
