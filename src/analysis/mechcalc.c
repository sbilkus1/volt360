#include "mechcalc.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

bool beam_bending_center(float length_mm, float width_mm, float height_mm,
                         float load_N, float E_GPa,
                         float *out_deflection_mm, float *out_stress_MPa) {
    if (length_mm <= 0 || width_mm <= 0 || height_mm <= 0 || load_N <= 0 || E_GPa <= 0) return false;
    float E = E_GPa * 1e3f; // GPa -> MPa
    float I = width_mm * height_mm * height_mm * height_mm / 12.0f; // mm^4
    float L = length_mm;
    if (out_deflection_mm) *out_deflection_mm = load_N * L * L * L / (48.0f * E * I);
    if (out_stress_MPa) *out_stress_MPa = load_N * L * height_mm / (4.0f * I);
    return true;
}

bool beam_bending_cantilever(float length_mm, float width_mm, float height_mm,
                              float load_N, float E_GPa,
                              float *out_deflection_mm, float *out_stress_MPa) {
    if (length_mm <= 0 || width_mm <= 0 || height_mm <= 0 || load_N <= 0 || E_GPa <= 0) return false;
    float E = E_GPa * 1e3f;
    float I = width_mm * height_mm * height_mm * height_mm / 12.0f;
    float L = length_mm;
    if (out_deflection_mm) *out_deflection_mm = load_N * L * L * L / (3.0f * E * I);
    if (out_stress_MPa) *out_stress_MPa = load_N * L * height_mm / (2.0f * I);
    return true;
}

bool plate_bending_uniform(float a_mm, float b_mm, float t_mm,
                            float pressure_kPa, float E_GPa, float nu,
                            float *out_deflection_mm, float *out_stress_MPa) {
    if (a_mm <= 0 || b_mm <= 0 || t_mm <= 0 || pressure_kPa <= 0 || E_GPa <= 0) return false;
    float E = E_GPa * 1e3f;
    float D = E * t_mm * t_mm * t_mm / (12.0f * (1.0f - nu * nu)); // flexural rigidity
    float p = pressure_kPa * 1e-3f; // kPa -> MPa
    float alpha = (a_mm < b_mm) ? (a_mm / b_mm) : (b_mm / a_mm);
    // approximate max deflection for simply supported rectangular plate
    float k = 0.00406f / (1.0f + alpha * alpha * alpha * alpha); // rough coeff
    float a4 = a_mm * a_mm * a_mm * a_mm;
    float b4 = b_mm * b_mm * b_mm * b_mm;
    float denom = 1.0f / (a4) + 1.0f / (b4);
    if (out_deflection_mm) *out_deflection_mm = k * p * a4 * b4 / (D * (a4 + b4)) * (a4 + b4) / (a4 * b4);
    // simplified: wmax = k * p * min(a,b)^4 / D
    float s = (a_mm < b_mm) ? a_mm : b_mm;
    if (out_deflection_mm) *out_deflection_mm = 0.00406f * p * s * s * s * s / D;
    if (out_stress_MPa) *out_stress_MPa = 0.75f * p * s * s / (t_mm * t_mm);
    return true;
}

float column_buckling_load(float length_mm, float width_mm, float height_mm,
                            float E_GPa, int end_condition) {
    if (length_mm <= 0 || width_mm <= 0 || height_mm <= 0 || E_GPa <= 0) return 0;
    float E = E_GPa * 1e3f;
    float I = width_mm * height_mm * height_mm * height_mm / 12.0f;
    float L = length_mm;
    float K = 1.0f;
    if (end_condition == 0) K = 2.0f; // free
    else if (end_condition == 1) K = 1.0f; // pinned-pinned
    else if (end_condition == 2) K = 0.7f; // fixed-pinned
    else if (end_condition == 3) K = 0.5f; // fixed-fixed
    float Le = K * L;
    return (float)(M_PI * M_PI * E * I / (Le * Le));
}
