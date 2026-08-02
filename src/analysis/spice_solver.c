#include "spice_solver.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

static int spice_max(int a, int b) { return a > b ? a : b; }

/* Gaussian elimination with partial pivoting.
 * A is n*n, b is n, x is output n. Returns 1 on success. */
static int gaussian_elim(double *A, double *b, double *x, int n) {
    double *M;
    int col, row, j, pivot;
    double pv, diag, factor;
    int result = 1;

    M = (double *)malloc(sizeof(double) * (size_t)n * (size_t)(n + 1));
    if (!M) return 0;

    for (row = 0; row < n; row++) {
        for (col = 0; col < n; col++) M[row * (n + 1) + col] = A[row * n + col];
        M[row * (n + 1) + n] = b[row];
    }

    for (col = 0; col < n; col++) {
        pivot = col;
        pv = fabs(M[col * (n + 1) + col]);
        for (row = col + 1; row < n; row++) {
            double v = fabs(M[row * (n + 1) + col]);
            if (v > pv) { pivot = row; pv = v; }
        }
        if (pv < 1e-14) { result = 0; break; }
        if (pivot != col) {
            for (j = 0; j <= n; j++) {
                double t = M[col * (n + 1) + j];
                M[col * (n + 1) + j] = M[pivot * (n + 1) + j];
                M[pivot * (n + 1) + j] = t;
            }
        }
        diag = M[col * (n + 1) + col];
        for (j = col; j <= n; j++) M[col * (n + 1) + j] /= diag;
        for (row = col + 1; row < n; row++) {
            factor = M[row * (n + 1) + col];
            for (j = col; j <= n; j++) M[row * (n + 1) + j] -= factor * M[col * (n + 1) + j];
        }
    }

    if (result) {
        for (row = n - 1; row >= 0; row--) {
            double sum = 0.0;
            for (j = row + 1; j < n; j++) sum += M[row * (n + 1) + j] * x[j];
            x[row] = M[row * (n + 1) + n] - sum;
        }
    }

    free(M);
    return result;
}

void spice_circuit_init(SpiceCircuit *c) {
    memset(c, 0, sizeof(*c));
}

int spice_add_resistor(SpiceCircuit *c, int n1, int n2, double r, const char *name) {
    if (c->nelems >= SPICE_MAX_ELEMS) return -1;
    {
        SpiceElement *e = &c->elems[c->nelems++];
        e->type = SPICE_RESISTOR;
        e->n1 = n1;
        e->n2 = n2;
        e->value = r;
        if (name) { strncpy(e->name, name, 31); e->name[31] = '\0'; }
        else { snprintf(e->name, sizeof(e->name), "R%d", c->nelems); }
    }
    c->nnodes = spice_max(c->nnodes, spice_max(n1, n2) + 1);
    return c->nelems - 1;
}

int spice_add_vsource(SpiceCircuit *c, int nplus, int nminus, double v, const char *name) {
    if (c->nelems >= SPICE_MAX_ELEMS) return -1;
    {
        SpiceElement *e = &c->elems[c->nelems++];
        e->type = SPICE_VSOURCE;
        e->n1 = nplus;
        e->n2 = nminus;
        e->value = v;
        if (name) { strncpy(e->name, name, 31); e->name[31] = '\0'; }
        else { snprintf(e->name, sizeof(e->name), "V%d", c->nelems); }
    }
    c->nnodes = spice_max(c->nnodes, spice_max(nplus, nminus) + 1);
    return c->nelems - 1;
}

int spice_add_isource(SpiceCircuit *c, int nplus, int nminus, double i, const char *name) {
    if (c->nelems >= SPICE_MAX_ELEMS) return -1;
    {
        SpiceElement *e = &c->elems[c->nelems++];
        e->type = SPICE_ISOURCE;
        e->n1 = nplus;
        e->n2 = nminus;
        e->value = i;
        if (name) { strncpy(e->name, name, 31); e->name[31] = '\0'; }
        else { snprintf(e->name, sizeof(e->name), "I%d", c->nelems); }
    }
    c->nnodes = spice_max(c->nnodes, spice_max(nplus, nminus) + 1);
    return c->nelems - 1;
}

int spice_add_ground(SpiceCircuit *c, int node) {
    if (c->nelems >= SPICE_MAX_ELEMS) return -1;
    {
        SpiceElement *e = &c->elems[c->nelems++];
        e->type = SPICE_GROUND;
        e->n1 = node;
        e->n2 = -1;
        e->value = 0.0;
        snprintf(e->name, sizeof(e->name), "GND");
    }
    c->ngnd = node;
    c->nnodes = spice_max(c->nnodes, node + 1);
    return 0;
}

bool spice_dc_solve(SpiceCircuit *c, double *voltages, double *currents, int max_iter) {
    int N, n_vsrc;
    int dim;
    double *A, *b, *x;
    int i, gi;
    int ok;

    (void)max_iter;
    if (!c || !voltages || !currents) return false;

    N = c->nnodes;
    n_vsrc = 0;
    for (i = 0; i < c->nelems; i++)
        if (c->elems[i].type == SPICE_VSOURCE) n_vsrc++;

    dim = N + n_vsrc;
    if (dim < 2 || dim > 128) return false;

    A = (double *)calloc((size_t)(dim * dim), sizeof(double));
    b = (double *)calloc((size_t)dim, sizeof(double));
    if (!A || !b) { free(A); free(b); return false; }

    gi = N;
    for (i = 0; i < c->nelems; i++) {
        SpiceElement *e = &c->elems[i];
        if (e->type == SPICE_RESISTOR) {
            double G, R;
            int r1, r2;
            R = e->value;
            if (R < 1e-6) R = 1e-6;
            G = 1.0 / R;
            r1 = e->n1;
            r2 = e->n2;
            if (r1 >= 0) { A[r1 * dim + r1] += G; if (r2 >= 0) A[r1 * dim + r2] -= G; }
            if (r2 >= 0) { A[r2 * dim + r2] += G; if (r1 >= 0) A[r2 * dim + r1] -= G; }
        } else if (e->type == SPICE_VSOURCE) {
            int np, nm;
            np = e->n1;
            nm = e->n2;
            if (gi < dim) {
                if (np > N - 1) np = N - 1;
                if (nm > N - 1) nm = N - 1;
                if (np >= 0) {
                    A[np * dim + gi] = 1.0;
                    A[gi * dim + np] = 1.0;
                }
                if (nm >= 0) {
                    A[nm * dim + gi] = -1.0;
                    A[gi * dim + nm] = -1.0;
                }
                b[gi] = e->value;
                gi++;
            }
        } else if (e->type == SPICE_ISOURCE) {
            int np, nm;
            np = e->n1;
            nm = e->n2;
            if (np >= 0) b[np] -= e->value;
            if (nm >= 0) b[nm] += e->value;
        }
    }

    for (i = 0; i < N; i++)
        if (fabs(A[i * dim + i]) < 1e-12) A[i * dim + i] = 1e-12;

    x = (double *)malloc(sizeof(double) * (size_t)dim);
    if (!x) { free(A); free(b); return false; }

    ok = gaussian_elim(A, b, x, dim);

    if (ok) {
        int v_idx;
        for (i = 0; i < N; i++) voltages[i] = x[i];
        for (i = 0; i < c->nelems; i++) currents[i] = 0.0;
        v_idx = 0;
        for (i = 0; i < c->nelems; i++) {
            SpiceElement *e = &c->elems[i];
            if (e->type == SPICE_RESISTOR) {
                int n1 = e->n1, n2 = e->n2;
                double v1 = (n1 >= 0) ? voltages[n1] : 0.0;
                double v2 = (n2 >= 0) ? voltages[n2] : 0.0;
                currents[i] = (v1 - v2) / e->value;
            } else if (e->type == SPICE_VSOURCE) {
                if (v_idx < dim - N) currents[i] = x[N + v_idx];
                v_idx++;
            }
        }
    }

    free(x); free(A); free(b);
    return ok ? true : false;
}
