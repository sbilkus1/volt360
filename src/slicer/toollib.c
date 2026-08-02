#include "toollib.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

static CamToolLib g_tools[] = {
    {"1/8\" flat", 1, 3.175f, 25.4f, 2, 12000, 800, 200, "endmill"},
    {"1/16\" flat", 2, 1.587f, 15.0f, 2, 15000, 500, 150, "endmill"},
    {"1/4\" flat", 3, 6.350f, 38.1f, 3, 10000, 1200, 300, "endmill"},
    {"1/8\" ball", 4, 3.175f, 25.4f, 2, 12000, 600, 200, "ball"},
    {"60 v-bit", 5, 0.2f, 20.0f, 1, 15000, 400, 150, "vbit"},
    {"#65 drill", 6, 0.889f, 20.0f, 2, 10000, 200, 100, "drill"},
    {"1\" facemill", 7, 25.4f, 50.0f, 4, 5000, 1000, 200, "facemill"},
    {"M3 tap", 8, 3.0f, 30.0f, 3, 500, 100, 50, "tap"},
};

int toollib_count(void) { return sizeof(g_tools) / sizeof(g_tools[0]); }
CamToolLib *toollib_get(int i) { if (i<0||i>=toollib_count()) return NULL; return &g_tools[i]; }
CamToolLib *toollib_find(const char *name) {
    for (int i=0;i<toollib_count();i++) if(strcmp(g_tools[i].name,name)==0) return &g_tools[i];
    return NULL;
}

char *cam_postprocess(const char *gcode_path, const char *controller) {
    char buf[1024];
    snprintf(buf, sizeof(buf),
        "Post-processor for %s\n"
        "Input: %s\n"
        "Output: build/post_%s.ngc\n"
        "Status: stub — full post-processors need G-code parsing",
        controller ? controller : "grbl", gcode_path ? gcode_path : "?",
        controller ? controller : "default");
    return str_dup(buf);
}

char *toollib_report(void) {
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== CAM TOOL LIBRARY ===\n");
    off += snprintf(buf+off, sizeof(buf)-off, "%-4s %-16s %-8s %-8s %-10s %-8s\n", "#", "Name", "Dia(mm)", "Len(mm)", "RPM", "Type");
    for (int i=0;i<toollib_count();i++) {
        CamToolLib *t = &g_tools[i];
        off += snprintf(buf+off, sizeof(buf)-off, "%-4d %-16s %-8.3f %-8.1f %-10d %-8s\n",
            t->tool_number, t->name, t->diameter_mm, t->length_mm, t->spindle_rpm, t->type);
    }
    return str_dup(buf);
}
