#pragma once
// spice.h - DC circuit solver (SPICE-style modified nodal analysis).
// Takes schematic data and solves for node voltages and branch currents.
#include <stdbool.h>
#include "../model/project.h"

typedef struct {
    int n_nodes;        // number of voltage nodes (0 = ground)
    double *v;          // node[n_nodes] voltages
    int n_branches;
    double *i;          // branch[n_branches] currents
    char **node_names;  // node[n_nodes] labels
} SpiceResult;

// Solve DC operating point for the given schematic.
// Returns NULL on failure; caller frees with spice_result_free.
SpiceResult *spice_solve_dc(Schematic *s);

void spice_result_free(SpiceResult *r);

// Convenience: returns a human-readable string (caller frees).
char *spice_result_text(SpiceResult *r);
