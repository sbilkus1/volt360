#include "meshops.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct { int parent, rank; } UF;

static int uf_find(UF *u, int x) {
    while (u[x].parent != x) { u[x].parent = u[u[x].parent].parent; x = u[x].parent; }
    return x;
}
static void uf_union(UF *u, int a, int b) {
    int ra = uf_find(u, a), rb = uf_find(u, b);
    if (ra == rb) return;
    if (u[ra].rank < u[rb].rank) u[ra].parent = rb;
    else if (u[ra].rank > u[rb].rank) u[rb].parent = ra;
    else { u[rb].parent = ra; u[ra].rank++; }
}

static bool edge_eq(int *idx, int t1, int e1, int t2, int e2) {
    int a1 = idx[t1 * 3 + e1], b1 = idx[t1 * 3 + ((e1 + 1) % 3)];
    int a2 = idx[t2 * 3 + e2], b2 = idx[t2 * 3 + ((e2 + 1) % 3)];
    return (a1 == a2 && b1 == b2) || (a1 == b2 && b1 == a2);
}

int mesh_split_components(CadMesh *in, CadMesh **out_components) {
    if (!in || !in->valid || in->ntris < 1) return 0;
    int nt = in->ntris;
    UF *uf = (UF *)malloc(sizeof(UF) * (size_t)nt);
    for (int i = 0; i < nt; i++) { uf[i].parent = i; uf[i].rank = 0; }

    // union triangles sharing an edge
    for (int i = 0; i < nt; i++)
        for (int j = i + 1; j < nt; j++)
            for (int e1 = 0; e1 < 3; e1++)
                for (int e2 = 0; e2 < 3; e2++)
                    if (edge_eq(in->idx, i, e1, j, e2)) uf_union(uf, i, j);

    // group by component
    int *comp = (int *)malloc(sizeof(int) * (size_t)nt);
    int ncomp = 0;
    int *remap = (int *)malloc(sizeof(int) * (size_t)nt);
    for (int i = 0; i < nt; i++) remap[i] = -1;

    for (int i = 0; i < nt; i++) {
        int root = uf_find(uf, i);
        int ci = remap[root];
        if (ci < 0) { ci = ncomp++; remap[root] = ci; }
        comp[i] = ci;
    }

    // allocate output
    for (int c = 0; c < ncomp; c++) {
        CadMesh *out = (CadMesh *)calloc(1, sizeof(CadMesh));
        out->valid = 1;
        // count verts and tris in this component
        int ct = 0;
        int *vert_map = (int *)malloc(sizeof(int) * (size_t)in->nverts);
        for (int v = 0; v < in->nverts; v++) vert_map[v] = -1;
        int nv_out = 0;

        for (int t = 0; t < nt; t++) {
            if (comp[t] != c) continue;
            ct++;
            for (int c2 = 0; c2 < 3; c2++) {
                int vi = in->idx[t * 3 + c2];
                if (vert_map[vi] < 0) vert_map[vi] = nv_out++;
            }
        }

        out->nverts = nv_out;
        out->ntris = ct;
        out->pos = (float *)malloc(sizeof(float) * 3 * (size_t)nv_out);
        out->nrm = (float *)malloc(sizeof(float) * 3 * (size_t)nv_out);
        out->idx = (int *)malloc(sizeof(int) * 3 * (size_t)ct);

        for (int v = 0; v < in->nverts; v++) {
            int ov = vert_map[v];
            if (ov < 0) continue;
            out->pos[ov * 3] = in->pos[v * 3];
            out->pos[ov * 3 + 1] = in->pos[v * 3 + 1];
            out->pos[ov * 3 + 2] = in->pos[v * 3 + 2];
            out->nrm[ov * 3] = in->nrm ? in->nrm[v * 3] : 0;
            out->nrm[ov * 3 + 1] = in->nrm ? in->nrm[v * 3 + 1] : 0;
            out->nrm[ov * 3 + 2] = in->nrm ? in->nrm[v * 3 + 2] : 0;
        }

        int tri = 0;
        for (int t = 0; t < nt; t++) {
            if (comp[t] != c) continue;
            out->idx[tri * 3] = vert_map[in->idx[t * 3]];
            out->idx[tri * 3 + 1] = vert_map[in->idx[t * 3 + 1]];
            out->idx[tri * 3 + 2] = vert_map[in->idx[t * 3 + 2]];
            tri++;
        }

        mesh_bbox(out);
        // Move the struct into the output array
        out_components[c] = (CadMesh *)malloc(sizeof(CadMesh));
        memcpy(out_components[c], out, sizeof(CadMesh));
        free(out);
        free(vert_map);
    }

    free(comp); free(remap); free(uf);
    return ncomp;
}

// needed by mesh_largest_component
void mesh_copy(CadMesh *src, CadMesh *dst) {
    if (!src || !dst || !src->valid) return;
    memset(dst, 0, sizeof(*dst));
    dst->valid = 1; dst->nverts = src->nverts; dst->ntris = src->ntris;
    dst->pos = (float *)malloc(sizeof(float) * 3 * (size_t)src->nverts);
    dst->nrm = (float *)malloc(sizeof(float) * 3 * (size_t)src->nverts);
    dst->idx = (int *)malloc(sizeof(int) * 3 * (size_t)src->ntris);
    memcpy(dst->pos, src->pos, sizeof(float) * 3 * (size_t)src->nverts);
    memcpy(dst->nrm, src->nrm, sizeof(float) * 3 * (size_t)src->nverts);
    memcpy(dst->idx, src->idx, sizeof(int) * 3 * (size_t)src->ntris);
    dst->bmin = src->bmin; dst->bmax = src->bmax;
}

bool mesh_largest_component(CadMesh *in, CadMesh *out) {
    int n = mesh_component_count(in);
    if (n <= 0) return false;
    if (n == 1) { mesh_copy(in, out); return true; }
    CadMesh *comps = (CadMesh *)malloc(sizeof(CadMesh) * (size_t)n);
    int nc = mesh_split_components(in, comps);
    if (nc <= 0) { free(comps); return false; }
    int best = 0;
    for (int i = 1; i < nc; i++) if (comps[i].nverts > comps[best].nverts) best = i;
    mesh_copy(&comps[best], out);
    for (int i = 0; i < nc; i++) mesh_free(&comps[i]);
    free(comps);
    return true;
}

int mesh_component_count(CadMesh *in) {
    if (!in || !in->valid || in->ntris < 1) return 0;
    int nt = in->ntris;
    UF *uf = (UF *)malloc(sizeof(UF) * (size_t)nt);
    for (int i = 0; i < nt; i++) { uf[i].parent = i; uf[i].rank = 0; }
    for (int i = 0; i < nt; i++)
        for (int j = i + 1; j < nt; j++)
            for (int e1 = 0; e1 < 3; e1++)
                for (int e2 = 0; e2 < 3; e2++)
                    if (edge_eq(in->idx, i, e1, j, e2)) uf_union(uf, i, j);
    int ncomp = 0;
    int *seen = (int *)calloc((size_t)nt, sizeof(int));
    for (int i = 0; i < nt; i++) { int r = uf_find(uf, i); if (!seen[r]) { seen[r] = 1; ncomp++; } }
    free(uf); free(seen);
    return ncomp;
}

float mesh_size(CadMesh *in) {
    if (!in || !in->valid) return 0;
    float dx = in->bmax.x - in->bmin.x;
    float dy = in->bmax.y - in->bmin.y;
    float dz = in->bmax.z - in->bmin.z;
    return sqrtf(dx*dx + dy*dy + dz*dz);
}
