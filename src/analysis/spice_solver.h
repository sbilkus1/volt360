#pragma once
/* spice_solver.h - Enhanced standalone SPICE DC solver.
 * Uses MNA (Modified Nodal Analysis) with Gaussian elimination + partial pivoting.
 */
#include <stdbool.h>

#define SPICE_MAX_NODES  128
#define SPICE_MAX_ELEMS  256

typedef enum { SPICE_RESISTOR, SPICE_VSOURCE, SPICE_ISOURCE, SPICE_GROUND } SpiceElemType;

typedef struct {
    int type;
    int n1, n2;
    double value;
    char name[32];
} SpiceElement;

typedef struct {
    SpiceElement elems[SPICE_MAX_ELEMS];
    int nelems;
    int nnodes;
    int ngnd;
} SpiceCircuit;

/* Add elements to circuit */
int spice_add_resistor(SpiceCircuit *c, int n1, int n2, double r, const char *name);
int spice_add_vsource(SpiceCircuit *c, int nplus, int nminus, double v, const char *name);
int spice_add_isource(SpiceCircuit *c, int nplus, int nminus, double i, const char *name);
int spice_add_ground(SpiceCircuit *c, int node);

/* Solve DC operating point.
 * voltages[nnodes] - output node voltages
 * currents[nelems] - output branch currents
 * max_iter - reserved for future nonlinear support, pass 0
 */
bool spice_dc_solve(SpiceCircuit *c, double *voltages, double *currents, int max_iter);
