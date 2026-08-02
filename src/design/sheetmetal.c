#include "sheetmetal.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

float sheet_bend_allowance(float R, float T, float K, float angle) {
    return (float)(M_PI / 180.0) * (R + K * T) * angle;
}

float sheet_bend_deduction(float R, float T, float K, float angle) {
    float ang_rad = angle * (float)(M_PI / 180.0);
    float OSB = (R + T) * tanf(ang_rad * 0.5f);
    float BA = sheet_bend_allowance(R, T, K, angle);
    return 2.0f * OSB - BA;
}

float sheet_flat_length(const float *lengths, const float *angles, int n_segments, float R, float T, float K) {
    if (n_segments < 2 || !lengths || !angles) return 0;
    float total = 0;
    for (int i = 0; i < n_segments; i++) total += lengths[i];
    for (int i = 0; i < n_segments - 1; i++)
        total = total - 2.0f * (R + T) + sheet_bend_allowance(R, T, K, fabsf(angles[i]));
    return total;
}

float sheet_min_flange(float R, float T) { return R + T * 4.0f; }

float sheet_k_factor_estimate(float R, float T) {
    if (T < 0.001f) return 0.5f;
    float ratio = R / T;
    if (ratio < 0.5f) return 0.33f;
    if (ratio < 1.5f) return 0.4f;
    if (ratio < 3.0f) return 0.45f;
    return 0.5f;
}

bool sheet_flat_pattern_text(float length, float width, float flange_l, float flange_r, float R, float T, float K, float angle,
                              char *buffer, int buf_size) {
    if (!buffer || buf_size < 256) return false;
    float BA = sheet_bend_allowance(R, T, K, angle);
    float ded = sheet_bend_deduction(R, T, K, angle);
    float flat_w = width;
    float flat_l = flange_l + flange_r + length - 2.0f * ded + 2.0f * BA;
    return snprintf(buffer, buf_size,
        "Sheet Metal Flat Pattern\n"
        "========================\n"
        "Finished: %.1f x %.1f mm (L=%04.1f W=%04.1f)\n"
        "Flat:     %.1f x %.1f mm\n"
        "Material: T=%.1f mm  R=%.1f mm K=%.2f\n"
        "Bend:     angle=%.0f  BA=%.2f  BD=%.2f\n"
        "Bend line 1: %.1f mm from edge\n"
        "Bend line 2: %.1f mm from edge\n",
        length, width, length, width,
        flat_l, flat_w,
        T, R, K, angle, BA, ded,
        flange_l - ded, flange_l + length - ded) > 0;
}
