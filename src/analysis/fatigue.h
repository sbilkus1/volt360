#pragma once
// fatigue.h - Fatigue analysis: S-N curves, stress-life, Goodman diagram.
#include <stdbool.h>

// Compute cycles to failure using Basquin's equation: N = C * (stress_range)^(-m)
// stress_amp_MPa: alternating stress amplitude
// C, m: material constants (e.g. C=1e12, m=3 for steel)
float fatigue_cycles_basquin(float stress_amp, float C, float m);

// Goodman mean stress correction.
float fatigue_goodman_corrected(float stress_amp, float stress_mean, float UTS_MPa);

// Miner's rule: cumulative damage = sum(n_i / N_i). Returns damage fraction (>=1 = failure).
float fatigue_miner_damage(const float *stress_amps, const float *cycles, int n_levels, float C, float m);

// Estimate endurance limit from UTS (Mpa).
float fatigue_endurance_limit(float UTS_MPa);

// Full fatigue report for a component.
char *fatigue_report(const char *name, float stress_amp, float stress_mean, float UTS_MPa, int cycles_applied);
