#include "cam5axis.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

bool cam4_rotary(const char *path, float radius, float length, float feed, int steps) {
    if (!path || radius <= 0 || steps < 4) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; 4-axis rotary\nG21 G90 G17\nG0 A0\n");
    for (int i = 0; i < steps; i++) {
        float a = (float)i * 360.0f / (float)steps;
        fprintf(f, "G0 A%.1f\n", a);
        fprintf(f, "G1 Y%.3f F%.0f\n", length, feed);
        fprintf(f, "G0 Y0\n");
    }
    fprintf(f, "M30\n"); fclose(f); return true;
}

bool cam5_contour(const char *path, float feed) {
    if (!path) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; 5-axis contour\nG21 G90 G17\n");
    fprintf(f, "G1 X10 Y10 Z-1 A0 B0 F%.0f\n", feed);
    fprintf(f, "G1 X50 Y10 Z-2 A15 B5 F%.0f\n", feed);
    fprintf(f, "M30\n"); fclose(f); return true;
}

bool cam_millturn(const char *path, float turn_diam, float mill_depth, float feed) {
    if (!path) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; Mill-Turn\nG21 G90 G18\n");
    fprintf(f, "G0 X%.3f Z5\n", turn_diam + 2);
    fprintf(f, "G1 Z-50 F%.0f ; turning\n", feed);
    fprintf(f, "G0 Z5\nG17\n");
    fprintf(f, "G1 X0 Y0 Z%.3f ; milling\n", mill_depth);
    fprintf(f, "M30\n"); fclose(f); return true;
}

bool cam_probe_grid(const char *path, float x, float y, float w, float h, float safe_z, float probe_z, int nx, int ny) {
    if (!path || nx < 1 || ny < 1) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; Probe grid\nG21 G90\nG0 Z%.3f\n", safe_z);
    for (int i = 0; i < nx; i++)
        for (int j = 0; j < ny; j++) {
            float px = x + w * i / (float)(nx > 1 ? nx-1 : 1);
            float py = y + h * j / (float)(ny > 1 ? ny-1 : 1);
            fprintf(f, "G0 X%.3f Y%.3f\nG38.2 Z%.3f F50\nG0 Z%.3f\n", px, py, probe_z, safe_z);
        }
    fprintf(f, "M30\n"); fclose(f); return true;
}

char *cam_simulate(const char *gcode_path, float time_scale) {
    char buf[256];
    snprintf(buf, sizeof(buf),
        "Toolpath Simulation\n"
        "File: %s\n"
        "Time scale: %.1fx\n"
        "Status: text-based simulation not yet implemented (needs 3D rendering)",
        gcode_path ? gcode_path : "?", time_scale);
    return str_dup(buf);
}

int cam_collision_check(const char *gcode_path) {
    (void)gcode_path;
    // stub: would check tool positions against stock model
    return 0; // no collisions
}
