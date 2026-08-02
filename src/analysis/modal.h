#pragma once
// modal.h - Modal/natural frequency analysis for simple structures.
#include <stdbool.h>

// Compute natural frequency of a simply-supported beam (Hz).
// E_GPa: Young's modulus, I_mm4: area moment of inertia, L_mm: length, mass_kg: mass
float modal_beam_natural_freq(float E_GPa, float I_mm4, float L_mm, float mass_kg);

// Compute natural frequency of a cantilever beam.
float modal_cantilever_freq(float E_GPa, float I_mm4, float L_mm, float mass_kg);

// Compute first N natural frequencies of a rectangular plate.
// Fills freqs array (caller provides N elements).
int modal_plate_freqs(float a_mm, float b_mm, float t_mm, float E_GPa, float nu, float density, float *freqs, int N);

// Compute natural frequency of a spring-mass system.
float modal_spring_mass_freq(float k_N_mm, float mass_kg);

// Full modal analysis report.
char *modal_report(const char *name, float E_GPa, float density, float L_mm, float w_mm, float h_mm);
