#include "cam_ext.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

bool cam_laser_cut(const char *path, V2 *polys, int *poly_lengths, int n_polys, float feed_xy) {
    if (!path || n_polys < 1) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; Laser cutting G-code\nG21 G90 G17\nM4 S1000 ; laser on\n");
    int off = 0;
    for (int i = 0; i < n_polys; i++) {
        int n = poly_lengths[i];
        if (n < 2) { off += n; continue; }
        fprintf(f, "G0 X%.3f Y%.3f\n", polys[off].x, polys[off].y);
        fprintf(f, "M3 ; laser on\nG4 P0.5\n");
        for (int j = 1; j < n; j++)
            fprintf(f, "G1 X%.3f Y%.3f F%.0f\n", polys[off+j].x, polys[off+j].y, feed_xy);
        fprintf(f, "M5 ; laser off\n");
        off += n;
    }
    fprintf(f, "M5\nM30\n"); fclose(f); return true;
}

bool cam_laser_engrave(const char *path, V2 *poly, int n, float hatch_spacing, float feed_xy) {
    if (!path || n < 3) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; Laser engraving\nG21 G90 G17\nM4 S500\n");
    float mnx = poly[0].x, mny = poly[0].y, mxx = mnx, mxy = mny;
    for (int i = 1; i < n; i++) {
        if (poly[i].x < mnx) mnx = poly[i].x; if (poly[i].x > mxx) mxx = poly[i].x;
        if (poly[i].y < mny) mny = poly[i].y; if (poly[i].y > mxy) mxy = poly[i].y;
    }
    bool dir = true;
    for (float y = mny; y < mxy; y += hatch_spacing) {
        if (dir) fprintf(f, "G0 X%.3f Y%.3f\nM3\nG1 X%.3f F%.0f\nM5\n", mnx, y, mxx, feed_xy);
        else fprintf(f, "G0 X%.3f Y%.3f\nM3\nG1 X%.3f F%.0f\nM5\n", mxx, y, mnx, feed_xy);
        dir = !dir;
    }
    fprintf(f, "M30\n"); fclose(f); return true;
}

bool cam_waterjet_cut(const char *path, V2 *poly, int n, float pierce_time_s, float feed_xy) {
    if (!path || n < 2) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; Waterjet cutting\nG21 G90 G17\nG0 X%.3f Y%.3f\n", poly[0].x, poly[0].y);
    fprintf(f, "G4 P%.1f ; pierce\n", pierce_time_s);
    for (int i = 1; i < n; i++)
        fprintf(f, "G1 X%.3f Y%.3f F%.0f\n", poly[i].x, poly[i].y, feed_xy);
    fprintf(f, "G1 X%.3f Y%.3f\nM30\n", poly[0].x, poly[0].y); fclose(f); return true;
}

bool cam_plasma_cut(const char *path, V2 *poly, int n, float kerf_mm, float pierce_delay_s, float feed_xy) {
    if (!path || n < 2) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; Plasma cutting (kerf=%.2f)\nG21 G90 G17\n", kerf_mm);
    // simple: offset poly by kerf/2 outward (proceeds through original points)
    fprintf(f, "G0 X%.3f Y%.3f\n", poly[0].x + kerf_mm, poly[0].y);
    fprintf(f, "M3 ; torch on\nG4 P%.1f\n", pierce_delay_s);
    for (int i = 1; i < n; i++)
        fprintf(f, "G1 X%.3f Y%.3f F%.0f\n", poly[i].x + kerf_mm, poly[i].y + kerf_mm, feed_xy);
    fprintf(f, "G1 X%.3f Y%.3f\nM5\nM30\n", poly[0].x + kerf_mm, poly[0].y + kerf_mm); fclose(f); return true;
}

bool cam_3d_contour(const char *path, V3 *toolpath, int n_pts, float feed_xy, float safe_z) {
    if (!path || n_pts < 2) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; 3D contour\nG21 G90 G17\nG0 Z%.3f\nG0 X%.3f Y%.3f\n", safe_z, toolpath[0].x, toolpath[0].y);
    fprintf(f, "G1 Z%.3f F%.0f\n", toolpath[0].z, feed_xy);
    for (int i = 1; i < n_pts; i++)
        fprintf(f, "G1 X%.3f Y%.3f Z%.3f F%.0f\n", toolpath[i].x, toolpath[i].y, toolpath[i].z, feed_xy);
    fprintf(f, "G0 Z%.3f\nM30\n", safe_z); fclose(f); return true;
}

bool cam_adaptive_clear(const char *path, V2 *poly, int n, float target_z, float tool_dia, float stepover, float feed_xy, float feed_z, float safe_z) {
    if (!path || n < 3) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "; Adaptive clearing\nG21 G90 G17\nG0 Z%.3f\n", safe_z);
    // compute centroid
    float cx = 0, cy = 0;
    for (int i = 0; i < n; i++) { cx += poly[i].x; cy += poly[i].y; }
    cx /= (float)n; cy /= (float)n;
    // spiral outward from center
    float step = tool_dia * stepover;
    fprintf(f, "G0 X%.3f Y%.3f\nG1 Z%.3f F%.0f\n", cx, cy, target_z, feed_z);
    float r = step;
    int rings = 10;
    for (int i = 0; i < rings; i++) {
        for (int a = 0; a < 36; a++) {
            float ang = (float)a * 2.0f * (float)M_PI / 36.0f;
            fprintf(f, "G1 X%.3f Y%.3f F%.0f\n", cx + r * cosf(ang), cy + r * sinf(ang), feed_xy);
        }
        r += step;
    }
    fprintf(f, "G0 Z%.3f\nM30\n", safe_z); fclose(f); return true;
}
