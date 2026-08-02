#pragma once
// signalint.h - Signal integrity analysis: impedance, crosstalk, reflections.
#include <stdbool.h>
#include "../model/project.h"

// Microstrip characteristic impedance (FR4, er=4.2).
// width_mm, height_mm (dielectric thickness above ground plane)
float si_microstrip_z0(float width_mm, float height_mm, float er);

// Stripline impedance (buried between two ground planes).
float si_stripline_z0(float width_mm, float height_mm, float er);

// Differential pair impedance (edge-coupled microstrip).
float si_diff_pair_z0(float width_mm, float spacing_mm, float height_mm, float er);

// Crosstalk estimate (near-end, as coupling coefficient 0-1).
float si_crosstalk_ne(float aggressor_length_mm, float victim_length_mm, float spacing_mm);

// Reflection coefficient for impedance mismatch.
float si_reflection_coeff(float z1, float z2);

// Full signal integrity report.
char *si_report(Pcb *pcb);
