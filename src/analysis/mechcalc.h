#pragma once
// mechcalc.h - Simple mechanical calculators: beam bending, stress, deflection.
#include <stdbool.h>

// Beam bending: simply supported beam with center point load.
// Returns max deflection (mm) and max bending stress (MPa).
// length_mm, width_mm, height_mm: beam dimensions
// load_N: center point load in Newtons
// E_GPa: Young's modulus
// out_deflection_mm, out_stress_MPa: results (can be NULL)
bool beam_bending_center(float length_mm, float width_mm, float height_mm,
                         float load_N, float E_GPa,
                         float *out_deflection_mm, float *out_stress_MPa);

// Cantilever beam: fixed at one end, load at free end.
bool beam_bending_cantilever(float length_mm, float width_mm, float height_mm,
                              float load_N, float E_GPa,
                              float *out_deflection_mm, float *out_stress_MPa);

// Rectangular plate bending (simply supported, uniform pressure).
// a_mm, b_mm: plate dimensions, t_mm: thickness
// pressure_kPa: uniform pressure in kPa
bool plate_bending_uniform(float a_mm, float b_mm, float t_mm,
                            float pressure_kPa, float E_GPa, float nu,
                            float *out_deflection_mm, float *out_stress_MPa);

// Column buckling (Euler). Returns critical load in N.
float column_buckling_load(float length_mm, float width_mm, float height_mm,
                            float E_GPa, int end_condition); // 0=free, 1=pinned, 2=fixed-free, 3=fixed-fixed
