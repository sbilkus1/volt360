#include "gen_design.h"
#include "../core/util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================= SIMP 2D plane-stress solver =================

#define G_NU 0.3
#define G_PEN 3.0
#define G_EMIN 1e-3
#define G_MOVE 0.2
#define G_ETA 0.5

typedef struct {
    int nx, ny;          // elements
    int nnx, nny;        // nodes = nx+1, ny+1
    int ndof;            // nnx*nny*2
    double *K;           // CSR values
    int *col;            // CSR col idx
    int *row;            // CSR row start
    int nnz;
    double *f;           // load
    double *u;
} FEA;

static void fea_free(FEA *s) {
    free(s->K); free(s->col); free(s->row); free(s->f); free(s->u);
    memset(s, 0, sizeof(*s));
}

// 8x8 element stiffness for unit E, plane stress (bi-unit quad, 2x2 gauss)
static void elem_k(double ke[8][8]) {
    const double gp[2] = { -0.5773502691896257, 0.5773502691896257 };
    memset(ke, 0, sizeof(double) * 64);
    double D[3][3];
    double c = 1.0 / (1.0 - G_NU * G_NU);
    D[0][0] = c; D[0][1] = c * G_NU; D[0][2] = 0;
    D[1][0] = c * G_NU; D[1][1] = c; D[1][2] = 0;
    D[2][0] = 0; D[2][1] = 0; D[2][2] = c * (1.0 - G_NU) / 2.0;
    for (int gi = 0; gi < 2; gi++) for (int gj = 0; gj < 2; gj++) {
        double xi = gp[gi], eta = gp[gj];
        double N[4];
        N[0] = 0.25 * (1 - xi) * (1 - eta);
        N[1] = 0.25 * (1 + xi) * (1 - eta);
        N[2] = 0.25 * (1 + xi) * (1 + eta);
        N[3] = 0.25 * (1 - xi) * (1 + eta);
        double dN[4][2]; // dN/dxi, dN/deta
        dN[0][0] = -0.25 * (1 - eta); dN[0][1] = -0.25 * (1 - xi);
        dN[1][0] =  0.25 * (1 - eta); dN[1][1] = -0.25 * (1 + xi);
        dN[2][0] =  0.25 * (1 + eta); dN[2][1] =  0.25 * (1 + xi);
        dN[3][0] = -0.25 * (1 + eta); dN[3][1] =  0.25 * (1 - xi);
        // jacobian: element spans unit square -> J = 0.5*I
        double invj = 2.0, detj = 0.25;
        double B[3][8];
        memset(B, 0, sizeof(B));
        for (int a = 0; a < 4; a++) {
            double bx = invj * dN[a][0], by = invj * dN[a][1];
            B[0][2 * a] = bx; B[1][2 * a + 1] = by; B[2][2 * a] = by; B[2][2 * a + 1] = bx;
        }
        double w = 1.0; // (2x2 gauss weights are all 1.0)
        for (int p = 0; p < 8; p++) for (int q = 0; q < 8; q++) {
            double sum = 0;
            for (int r = 0; r < 3; r++) for (int s = 0; s < 3; s++) sum += B[r][p] * D[r][s] * B[s][q];
            ke[p][q] += sum * detj * w;
        }
    }
}

// assemble global CSR stiffness from density field
static int fea_assemble(FEA *s, const double *dens) {
    int nnx = s->nnx, nny = s->nny;
    s->ndof = nnx * nny * 2;
    // per-row column lists: each node connects to at most 3x3 nodes -> 18 columns
    int *rowcnt = (int *)calloc((size_t)s->ndof, sizeof(int));
    int *rowcols = (int *)calloc((size_t)s->ndof * 18, sizeof(int));
    double ke[8][8];
    elem_k(ke);
    int n[4], dofs[8];
    for (int ey = 0; ey < s->ny; ey++) {
        for (int ex = 0; ex < s->nx; ex++) {
            n[0] = ex + ey * nnx;
            n[1] = ex + 1 + ey * nnx;
            n[2] = ex + 1 + (ey + 1) * nnx;
            n[3] = ex + (ey + 1) * nnx;
            for (int a = 0; a < 4; a++) { dofs[2 * a] = n[a] * 2; dofs[2 * a + 1] = n[a] * 2 + 1; }
            for (int p = 0; p < 8; p++) {
                int row = dofs[p];
                int *list = rowcols + row * 18;
                for (int q = 0; q < 8; q++) {
                    int c = dofs[q];
                    int found = -1;
                    for (int k = 0; k < rowcnt[row]; k++) if (list[k] == c) { found = k; break; }
                    if (found < 0) { list[rowcnt[row]++] = c; }
                }
            }
        }
    }
    // prefix sum -> row offsets
    s->row = (int *)malloc(sizeof(int) * (size_t)(s->ndof + 1));
    s->row[0] = 0;
    for (int i = 0; i < s->ndof; i++) s->row[i + 1] = s->row[i] + rowcnt[i];
    int nnz = s->row[s->ndof];
    s->K = (double *)calloc((size_t)nnz, sizeof(double));
    s->col = (int *)malloc(sizeof(int) * (size_t)nnz);
    // fill columns + values
    int *fill = (int *)calloc((size_t)s->ndof, sizeof(int));
    for (int ey = 0; ey < s->ny; ey++) {
        for (int ex = 0; ex < s->nx; ex++) {
            int e = ex + ey * s->nx;
            double rho = dens[e];
            double E = G_EMIN + (1.0 - G_EMIN) * pow(rho, G_PEN);
            n[0] = ex + ey * nnx;
            n[1] = ex + 1 + ey * nnx;
            n[2] = ex + 1 + (ey + 1) * nnx;
            n[3] = ex + (ey + 1) * nnx;
            for (int a = 0; a < 4; a++) { dofs[2 * a] = n[a] * 2; dofs[2 * a + 1] = n[a] * 2 + 1; }
            for (int p = 0; p < 8; p++) {
                int row = dofs[p];
                int base = s->row[row];
                for (int q = 0; q < 8; q++) {
                    int c = dofs[q];
                    int found = -1;
                    for (int k = 0; k < fill[row]; k++) if (s->col[base + k] == c) { found = base + k; break; }
                    if (found < 0) {
                        found = base + fill[row];
                        s->col[found] = c;
                        fill[row]++;
                    }
                    s->K[found] += ke[p][q] * E;
                }
            }
        }
    }
    free(fill);
    free(rowcnt);
    free(rowcols);
    return nnz;
}

// conjugate gradient for SPD
static void cg_solve(FEA *s, double tol, int maxit) {
    int n = s->ndof;
    double *r = (double *)calloc((size_t)n, sizeof(double));
    double *p = (double *)calloc((size_t)n, sizeof(double));
    double *Ap = (double *)calloc((size_t)n, sizeof(double));
    // initial guess u=0, r = f - K*0 = f
    memcpy(r, s->f, sizeof(double) * (size_t)n);
    memcpy(p, s->f, sizeof(double) * (size_t)n);
    double rho = 0, rho_new = 0;
    for (int i = 0; i < n; i++) rho += r[i] * r[i];
    if (rho < tol) { free(r); free(p); free(Ap); return; }
    for (int it = 0; it < maxit; it++) {
        // Ap = K*p (CSR)
        memset(Ap, 0, sizeof(double) * (size_t)n);
        for (int i = 0; i < n; i++) {
            double acc = 0;
            for (int k = s->row[i]; k < s->row[i + 1]; k++) acc += s->K[k] * p[s->col[k]];
            Ap[i] = acc;
        }
        double pAp = 0;
        for (int i = 0; i < n; i++) pAp += p[i] * Ap[i];
        if (pAp <= 1e-30) break;
        double alpha = rho / pAp;
        for (int i = 0; i < n; i++) { s->u[i] += alpha * p[i]; r[i] -= alpha * Ap[i]; }
        rho_new = 0;
        for (int i = 0; i < n; i++) rho_new += r[i] * r[i];
        if (rho_new < tol) break;
        double beta = rho_new / rho;
        for (int i = 0; i < n; i++) p[i] = r[i] + beta * p[i];
        rho = rho_new;
    }
    free(r); free(p); free(Ap);
}

// ================= public API =================

bool gen_design_run(GenDesign *gd) {
    if (!gd || gd->nx < 4 || gd->ny < 4) return false;
    int nx = gd->nx, ny = gd->ny;
    gd->dens = (double *)malloc(sizeof(double) * (size_t)(nx * ny));
    for (int i = 0; i < nx * ny; i++) gd->dens[i] = gd->volfrac > 0 ? gd->volfrac : 0.4;
    int nnx = nx + 1, nny = ny + 1;

    FEA s; memset(&s, 0, sizeof(s));
    s.nx = nx; s.ny = ny; s.nnx = nnx; s.nny = nny;
    s.ndof = nnx * nny * 2;
    s.f = (double *)calloc((size_t)s.ndof, sizeof(double));
    s.u = (double *)calloc((size_t)s.ndof, sizeof(double));

    // load node near right edge
    int lx = (int)(gd->load_xf * nx); if (lx < 0) lx = 0; if (lx > nx) lx = nx;
    int ly = (int)(gd->load_yf * ny); if (ly < 0) ly = 0; if (ly > ny) ly = ny;
    int load_node = lx + ly * nnx;
    s.f[load_node * 2 + 1] = -1.0; // downward

    // boundary: fixed left (and optionally bottom)
    int *fixed = (int *)calloc((size_t)s.ndof, sizeof(int));
    for (int iy = 0; iy < nny; iy++) {
        int node = iy * nnx;
        if (gd->fix_left) { fixed[node * 2] = 1; fixed[node * 2 + 1] = 1; }
    }
    if (gd->fix_bottom) {
        for (int ix = 0; ix < nnx; ix++) { fixed[ix * 2] = 1; fixed[ix * 2 + 1] = 1; }
    }
    int iters = gd->iters > 0 ? gd->iters : 80;

    for (int it = 0; it < iters; it++) {
        // assemble
        free(s.K); free(s.col); free(s.row);
        s.K = NULL; s.col = NULL; s.row = NULL;
        fea_assemble(&s, gd->dens);
        memset(s.u, 0, sizeof(double) * (size_t)s.ndof);
        // symmetric fixed-dof elimination: zero column d in every row, then set diagonal to 1
        for (int d = 0; d < s.ndof; d++) if (fixed[d]) {
            for (int i = 0; i < s.ndof; i++) {
                for (int k = s.row[i]; k < s.row[i + 1]; k++) {
                    if (s.col[k] == d) { s.K[k] = 0; break; }
                }
            }
            for (int k = s.row[d]; k < s.row[d + 1]; k++) if (s.col[k] == d) { s.K[k] = 1; break; }
            s.f[d] = 0;
        }
        cg_solve(&s, 1e-10, 4000);

        // compliance sensitivities
        double *sens = (double *)malloc(sizeof(double) * (size_t)(nx * ny));
        double ke[8][8];
        elem_k(ke);
        double tot_vol = 0;
        for (int ey = 0; ey < ny; ey++) for (int ex = 0; ex < nx; ex++) {
            int e = ex + ey * nx;
            int n[4] = { ex + ey * nnx, ex + 1 + ey * nnx, ex + 1 + (ey + 1) * nnx, ex + (ey + 1) * nnx };
            int dofs[8];
            for (int a = 0; a < 4; a++) { dofs[2 * a] = n[a] * 2; dofs[2 * a + 1] = n[a] * 2 + 1; }
            double ue[8];
            for (int a = 0; a < 8; a++) ue[a] = s.u[dofs[a]];
            double ueKue = 0;
            for (int p = 0; p < 8; p++) for (int q = 0; q < 8; q++) ueKue += ue[p] * ke[p][q] * ue[q];
            double rho = gd->dens[e];
            double dE = (1.0 - G_EMIN) * G_PEN * pow(rho, G_PEN - 1.0);
            // -dC/drho = (1-Emin)*p*rho^(p-1) * u^T k0 u   (positive -> element worth keeping)
            sens[e] = ueKue * dE;
            tot_vol += rho;
        }
        // bisection on lambda for volume constraint
        double vfrac = gd->volfrac > 0 ? gd->volfrac : 0.4;
        double l1 = 0, l2 = 1e9;
        double *rhs = (double *)malloc(sizeof(double) * (size_t)(nx * ny));
        for (int bi = 0; bi < 60; bi++) {
            double lam = 0.5 * (l1 + l2);
            double vol = 0;
            for (int e = 0; e < nx * ny; e++) {
                double L = sens[e] / (lam + 1e-12);
                double nv = gd->dens[e] * pow(L, G_ETA);
                nv = fmin(1.0, fmax(0.0, fmax(gd->dens[e] - G_MOVE, fmin(gd->dens[e] + G_MOVE, nv))));
                rhs[e] = nv;
                vol += nv;
            }
            if (vol / (nx * ny) > vfrac) l1 = lam; else l2 = lam;
        }
        double vol = 0;
        for (int e = 0; e < nx * ny; e++) { gd->dens[e] = rhs[e]; vol += rhs[e]; }
        free(sens); free(rhs);
    }
    free(fixed);
    fea_free(&s);
    return true;
}

void gen_design_free(GenDesign *gd) {
    if (!gd) return;
    free(gd->dens);
    gd->dens = NULL;
}

// ================= voxel mesh =================
static void vox_push_box(CadMesh *m, float cx, float cy, float cz, float sx, float sy, float sz) {
    float x0 = cx - sx / 2, x1 = cx + sx / 2;
    float y0 = cy - sy / 2, y1 = cy + sy / 2;
    float z0 = cz - sz / 2, z1 = cz + sz / 2;
    V3 c[8] = {
        v3(x0, y0, z0), v3(x1, y0, z0), v3(x1, y1, z0), v3(x0, y1, z0),
        v3(x0, y0, z1), v3(x1, y0, z1), v3(x1, y1, z1), v3(x0, y1, z1),
    };
    V3 n[6] = { v3(0,0,-1), v3(0,0,1), v3(0,-1,0), v3(0,1,0), v3(-1,0,0), v3(1,0,0) };
    // reuse design.c? no - inline quads
    int quad[6][4] = { {0,1,2,3}, {5,4,7,6}, {0,4,5,1}, {3,2,6,7}, {0,3,7,4}, {1,5,6,2} };
    for (int q = 0; q < 6; q++) {
        int base = m->nverts;
        m->pos = (float *)realloc(m->pos, sizeof(float) * 3 * (size_t)(m->nverts + 4));
        m->nrm = (float *)realloc(m->nrm, sizeof(float) * 3 * (size_t)(m->nverts + 4));
        for (int k = 0; k < 4; k++) {
            V3 v = c[quad[q][k]];
            m->pos[(base + k) * 3] = v.x; m->pos[(base + k) * 3 + 1] = v.y; m->pos[(base + k) * 3 + 2] = v.z;
            m->nrm[(base + k) * 3] = n[q].x; m->nrm[(base + k) * 3 + 1] = n[q].y; m->nrm[(base + k) * 3 + 2] = n[q].z;
        }
        m->idx = (int *)realloc(m->idx, sizeof(int) * 3 * (size_t)(m->ntris + 2));
        m->idx[m->ntris * 3] = base; m->idx[m->ntris * 3 + 1] = base + 1; m->idx[m->ntris * 3 + 2] = base + 2;
        m->idx[m->ntris * 3 + 3] = base; m->idx[m->ntris * 3 + 4] = base + 2; m->idx[m->ntris * 3 + 5] = base + 3;
        m->nverts += 4; m->ntris += 2;
    }
}

bool gen_design_mesh(GenDesign *gd, float cell_mm, float depth_mm, CadMesh *out) {
    if (!gd || !gd->dens || !out) return false;
    memset(out, 0, sizeof(*out));
    double thr = 0.45;
    for (int ey = 0; ey < gd->ny; ey++) for (int ex = 0; ex < gd->nx; ex++) {
        int e = ex + ey * gd->nx;
        if (gd->dens[e] < thr) continue;
        float cx = (ex + 0.5f) * cell_mm;
        float cy = (ey + 0.5f) * cell_mm;
        vox_push_box(out, cx, cy, depth_mm / 2, cell_mm * 1.02f, cell_mm * 1.02f, depth_mm);
    }
    if (out->nverts == 0) return false;
    mesh_bbox(out);
    return true;
}
