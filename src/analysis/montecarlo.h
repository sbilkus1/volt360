#pragma once
// montecarlo.h - Monte Carlo analysis and parameter sweeps for SPICE.
#include <stdbool.h>
#include "../model/project.h"

// Run a parameter sweep: varies component value and reports result at each step.
// value_key: spec key to vary (e.g. "Resistance")
// start_val, end_val: range
// steps: number of steps
// Returns malloc'd text report.
char *spice_parameter_sweep(Schematic *s, const char *value_key, float start_val, float end_val, int steps);

// Run Monte Carlo analysis with random variations.
// tolerance_pct: percentage variation applied randomly to component values
// runs: number of MC runs
// Returns malloc'd text report.
char *spice_monte_carlo(Schematic *s, float tolerance_pct, int runs);
