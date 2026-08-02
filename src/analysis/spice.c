#include "spice.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// component types detected from ref prefix
static int comp_type(const char *ref) {
    if (!ref) return 0;
    if (ref[0] == 'R' || ref[0] == 'r') return 'R';
    if (ref[0] == 'V' || ref[0] == 'v') return 'V';
    return 0;
}

// parse value (e.g. "10k" -> 10000, "4.7" -> 4.7, "1M" -> 1e6)
static double parse_value(const char *val) {
    if (!val) return 1.0;
    char buf[64]; int n = 0;
    while (*val && n < 62) { if (*val != ' ') buf[n++] = *val; val++; }
    buf[n] = '\0';
    double scale = 1.0;
    char suffix = 0;
    if (n > 0 && (buf[n-1] == 'k' || buf[n-1] == 'K' || buf[n-1] == 'm' || buf[n-1] == 'M' || buf[n-1] == 'u' || buf[n-1] == 'U')) { suffix = buf[n-1]; buf[n-1] = '\0'; }
    if (suffix == 'k' || suffix == 'K') scale = 1e3;
    else if (suffix == 'm') scale = 1e-3;
    else if (suffix == 'M') scale = 1e6;
    else if (suffix == 'u' || suffix == 'U') scale = 1e-6;
    double v = atof(buf);
    return v * scale;
}

// ---- union-find for wire nets ----
typedef struct { int parent; int rank; } UF;
static int uf_find(UF *uf, int x) {
    while (uf[x].parent != x) { uf[x].parent = uf[uf[x].parent].parent; x = uf[x].parent; }
    return x;
}
static void uf_union(UF *uf, int a, int b) {
    int ra = uf_find(uf, a), rb = uf_find(uf, b);
    if (ra == rb) return;
    if (uf[ra].rank < uf[rb].rank) uf[ra].parent = rb;
    else if (uf[ra].rank > uf[rb].rank) uf[rb].parent = ra;
    else { uf[rb].parent = ra; uf[ra].rank++; }
}

static bool within(V2 a, V2 b) { return fabs(a.x - b.x) < 1e-4f && fabs(a.y - b.y) < 1e-4f; }

// ---- Gaussian elimination with partial pivoting ----
static int gauss_solve(double *A, double *b, double *x, int n) {
    // augmented matrix
    double *M = (double *)malloc(sizeof(double) * n * (n + 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) M[i * (n + 1) + j] = A[i * n + j];
        M[i * (n + 1) + n] = b[i];
    }
    for (int col = 0; col < n; col++) {
        int pivot = col;
        double pv = fabs(M[col * (n + 1) + col]);
        for (int row = col + 1; row < n; row++) {
            if (fabs(M[row * (n + 1) + col]) > pv) { pivot = row; pv = fabs(M[row * (n + 1) + col]); }
        }
        if (pv < 1e-12) { free(M); return 0; }
        if (pivot != col) {
            for (int j = 0; j <= n; j++) { double t = M[col * (n + 1) + j]; M[col * (n + 1) + j] = M[pivot * (n + 1) + j]; M[pivot * (n + 1) + j] = t; }
        }
        double diag = M[col * (n + 1) + col];
        for (int j = col; j <= n; j++) M[col * (n + 1) + j] /= diag;
        for (int row = col + 1; row < n; row++) {
            double factor = M[row * (n + 1) + col];
            for (int j = col; j <= n; j++) M[row * (n + 1) + j] -= factor * M[col * (n + 1) + j];
        }
    }
    for (int i = n - 1; i >= 0; i--) {
        x[i] = M[i * (n + 1) + n];
        for (int j = i + 1; j < n; j++) x[i] -= M[i * (n + 1) + j] * x[j];
    }
    free(M);
    return 1;
}

// ---- netlist construction from schematic ----
typedef struct {
    int n_wires;     // number of wire segments
    V2 *w_a, *w_b;   // wire endpoints
    int n_names;     // named nodes
    V2 *name_pos; char **name_labels;
} WireList;

SpiceResult *spice_solve_dc(Schematic *s) {
    if (!s || s->ninsts < 1) return NULL;

    // 1. collect all connection points: component pins + wire endpoints + net labels
    int np = 0;
    // first pass: count
    int n_comps = 0;
    for (int i = 0; i < s->ninsts; i++) {
        int t = comp_type(s->insts[i].ref);
        if (t == 'R' || t == 'V') n_comps++;
    }
    if (n_comps < 1) return NULL;

    // total points: wire endpoints (2 per wire) + net labels
    int npts = s->nwires * 2 + s->nlabels + n_comps * 2;
    V2 *pts = (V2 *)malloc(sizeof(V2) * (size_t)npts);
    int *pt_comp = (int *)malloc(sizeof(int) * (size_t)npts); // -1 = wire, comp index
    int *pt_pin = (int *)malloc(sizeof(int) * (size_t)npts);  // pin 0 or 1
    char **pt_label = (char **)calloc((size_t)npts, sizeof(char *));

    // wire endpoints
    for (int i = 0; i < s->nwires; i++) {
        pts[np] = s->wires[i].a; pt_comp[np] = -1; pt_pin[np] = 0; np++;
        pts[np] = s->wires[i].b; pt_comp[np] = -1; pt_pin[np] = 0; np++;
    }
    // component pins
    for (int i = 0; i < s->ninsts; i++) {
        int t = comp_type(s->insts[i].ref);
        if (t != 'R' && t != 'V') continue;
        pts[np] = s->insts[i].pos; pt_comp[np] = i; pt_pin[np] = 0; np++;
        // approximate pin2 position (offset right for simplicity)
        pts[np] = v2(s->insts[i].pos.x + 20, s->insts[i].pos.y); pt_comp[np] = i; pt_pin[np] = 1; np++;
    }
    // net labels
    for (int i = 0; i < s->nlabels; i++) {
        pts[np] = s->labels[i].pos; pt_comp[np] = -2; pt_pin[np] = 0; pt_label[np] = s->labels[i].name; np++;
    }

    // 2. union-find to merge coincident points
    UF *uf = (UF *)malloc(sizeof(UF) * (size_t)npts);
    for (int i = 0; i < npts; i++) { uf[i].parent = i; uf[i].rank = 0; }
    for (int i = 0; i < npts; i++)
        for (int j = i + 1; j < npts; j++)
            if (within(pts[i], pts[j])) uf_union(uf, i, j);

    // 3. assign net IDs
    int *net_id = (int *)malloc(sizeof(int) * (size_t)npts);
    for (int i = 0; i < npts; i++) net_id[i] = -1;
    int GND = -1, next_net = 0;
    // find GND from labels
    for (int i = 0; i < npts; i++) {
        if (pt_label[i] && (strcmp(pt_label[i], "GND") == 0 || strcmp(pt_label[i], "0") == 0)) {
            int root = uf_find(uf, i);
            for (int j = 0; j < npts; j++) if (uf_find(uf, j) == root) net_id[j] = 0;
            GND = root;
            break;
        }
    }
    if (GND < 0) {
        // if no GND label, pick the first wire endpoint as ground
        if (s->nwires > 0) { int root = uf_find(uf, 0); for (int j = 0; j < npts; j++) if (uf_find(uf, j) == root) net_id[j] = 0; GND = root; }
    }
    if (GND < 0) { free(uf); free(pts); free(pt_comp); free(pt_pin); free(pt_label); free(net_id); return NULL; }

    // assign other nets
    for (int i = 0; i < npts; i++) {
        if (net_id[i] >= 0) continue;
        int root = uf_find(uf, i);
        if (root == GND) { net_id[i] = 0; continue; }
        // find if any node in this root already has a net
        int found = -1;
        for (int j = 0; j < i; j++) if (uf_find(uf, j) == root && net_id[j] >= 0) { found = net_id[j]; break; }
        if (found >= 0) net_id[i] = found;
        else { net_id[i] = ++next_net; for (int j = i + 1; j < npts; j++) if (uf_find(uf, j) == root) net_id[j] = next_net; }
    }
    int N = next_net; // nodes 1..N, 0 = GND

    // 4. count V-sources (need extra equations for MNA)
    int n_vsrc = 0;
    for (int i = 0; i < s->ninsts; i++) if (comp_type(s->insts[i].ref) == 'V') n_vsrc++;
    int dim = N + n_vsrc; // total MNA dimension

    // 5. build MNA matrix
    double *A = (double *)calloc((size_t)(dim * dim), sizeof(double));
    double *b = (double *)calloc((size_t)dim, sizeof(double));
    int vsrc_idx = 0;

    for (int i = 0; i < s->ninsts; i++) {
        int t = comp_type(s->insts[i].ref);
        if (t == 'R') {
            double R = parse_value(s->insts[i].value);
            if (R < 1e-6) R = 1e-6; // avoid division by zero
            double G = 1.0 / R;
            // find pin nodes
            int p1 = -1, p2 = -1;
            for (int k = 0; k < npts; k++) if (pt_comp[k] == i && pt_pin[k] == 0) p1 = net_id[k];
            for (int k = 0; k < npts; k++) if (pt_comp[k] == i && pt_pin[k] == 1) p2 = net_id[k];
            if (p1 < 0 || p2 < 0) continue;
            if (p1 > 0) { A[(p1-1) * dim + (p1-1)] += G; if (p2 > 0) A[(p1-1) * dim + (p2-1)] -= G; }
            if (p2 > 0) { A[(p2-1) * dim + (p2-1)] += G; if (p1 > 0) A[(p2-1) * dim + (p1-1)] -= G; }
        } else if (t == 'V') {
            double Vval = parse_value(s->insts[i].value);
            int p1 = -1, p2 = -1;
            for (int k = 0; k < npts; k++) if (pt_comp[k] == i && pt_pin[k] == 0) p1 = net_id[k];
            for (int k = 0; k < npts; k++) if (pt_comp[k] == i && pt_pin[k] == 1) p2 = net_id[k];
            if (p1 < 0 || p2 < 0) continue;
            int vi = N + vsrc_idx; // MNA row for this V-source
            vsrc_idx++;
            // stamp: +1/-1 for branch current equation
            if (p1 > 0) { A[(p1-1) * dim + vi] = 1.0; A[vi * dim + (p1-1)] = 1.0; }
            if (p2 > 0) { A[(p2-1) * dim + vi] = -1.0; A[vi * dim + (p2-1)] = -1.0; }
            if (p1 == 0) A[vi * dim + (N-1)] = 1.0; // ground side: node N doesn't exist, hack - simplify
            // RHS: voltage value
            b[vi] = Vval;
        }
    }

    // 6. ensure no singular matrix: add small conductance to GND
    for (int i = 0; i < N; i++) if (fabs(A[i * dim + i]) < 1e-12) A[i * dim + i] = 1e-12;

    // 7. solve
    double *x = (double *)malloc(sizeof(double) * (size_t)dim);
    int ok = gauss_solve(A, b, x, dim);

    // 8. build result
    SpiceResult *r = NULL;
    if (ok) {
        r = (SpiceResult *)calloc(1, sizeof(SpiceResult));
        r->n_nodes = N;
        r->v = (double *)malloc(sizeof(double) * (size_t)(N + 1));
        r->v[0] = 0; // GND
        for (int i = 0; i < N; i++) r->v[i + 1] = x[i];
        r->n_branches = n_vsrc;
        r->i = (double *)malloc(sizeof(double) * (size_t)(n_vsrc > 0 ? n_vsrc : 1));
        for (int i = 0; i < n_vsrc; i++) r->i[i] = x[N + i];
        r->node_names = (char **)calloc((size_t)(N + 1), sizeof(char *));
        r->node_names[0] = str_dup("GND");
        for (int i = 0; i < npts; i++) if (pt_label[i] && net_id[i] > 0 && !r->node_names[net_id[i]])
            r->node_names[net_id[i]] = str_dup(pt_label[i]);
        for (int i = 1; i <= N; i++) if (!r->node_names[i]) {
            char nn[16]; snprintf(nn, sizeof(nn), "N%d", i); r->node_names[i] = str_dup(nn);
        }
    }

    free(x); free(A); free(b);
    free(uf); free(pts); free(pt_comp); free(pt_pin); free(pt_label); free(net_id);
    return r;
}

void spice_result_free(SpiceResult *r) {
    if (!r) return;
    free(r->v); free(r->i);
    if (r->node_names) { for (int i = 0; i <= r->n_nodes; i++) free(r->node_names[i]); free(r->node_names); }
    free(r);
}

char *spice_result_text(SpiceResult *r) {
    if (!r) return str_dup("no result");
    char buf[4096]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off, "DC operating point:\n");
    for (int i = 0; i <= r->n_nodes; i++) {
        off += snprintf(buf + off, sizeof(buf) - off, "  %s: %+.4f V\n", r->node_names[i] ? r->node_names[i] : "?", r->v[i]);
    }
    for (int i = 0; i < r->n_branches; i++)
        off += snprintf(buf + off, sizeof(buf) - off, "  I(Vsrc%d): %+.4f A\n", i, r->i[i]);
    return str_dup(buf);
}
