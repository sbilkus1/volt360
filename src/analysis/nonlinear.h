#pragma once
// nonlinear.h - Nonlinear geometric stiffness correction.
#include <stdbool.h>

// Geometric stiffness correction: effective stiffness considering axial load.
// k0: linear stiffness (N/m), P: axial load (N, +tension, -compression), L: length (m)
float nonlinear_effective_stiffness(float k0, float P, float L);

// Buckling load amplification factor for initial imperfection.
// P/Pcr: ratio of applied load to critical load
// imperfection_mm: initial geometric imperfection
float nonlinear_amplification_factor(float P_over_Pcr, float imperfection_mm, float thickness_mm);

// Large deflection correction for beam (approximate).
float nonlinear_beam_deflection(float linear_deflection, float load, float Pcr);

// Nonlinear analysis report.
char *nonlinear_report(const char *name, float k0, float P, float L, float Pcr);
