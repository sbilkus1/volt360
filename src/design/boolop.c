#include "boolop.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static float clamp(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

V3 bool_intersect_bbox(V3 a_min, V3 a_max, V3 b_min, V3 b_max, bool *valid) {
    V3 r_min = v3(a_min.x > b_min.x ? a_min.x : b_min.x,
                  a_min.y > b_min.y ? a_min.y : b_min.y,
                  a_min.z > b_min.z ? a_min.z : b_min.z);
    V3 r_max = v3(a_max.x < b_max.x ? a_max.x : b_max.x,
                  a_max.y < b_max.y ? a_max.y : b_max.y,
                  a_max.z < b_max.z ? a_max.z : b_max.z);
    *valid = r_min.x < r_max.x && r_min.y < r_max.y && r_min.z < r_max.z;
    return r_min; // caller can compute max from min + dimensions
}

void bool_union_bbox(V3 a_min, V3 a_max, V3 b_min, V3 b_max, V3 *out_min, V3 *out_max) {
    out_min->x = a_min.x < b_min.x ? a_min.x : b_min.x;
    out_min->y = a_min.y < b_min.y ? a_min.y : b_min.y;
    out_min->z = a_min.z < b_min.z ? a_min.z : b_min.z;
    out_max->x = a_max.x > b_max.x ? a_max.x : b_max.x;
    out_max->y = a_max.y > b_max.y ? a_max.y : b_max.y;
    out_max->z = a_max.z > b_max.z ? a_max.z : b_max.z;
}

int bool_diff_box(V3 a_min, V3 a_max, V3 b_min, V3 b_max,
                  V3 *out_mins, V3 *out_maxs, int max_out) {
    // Clamp b to the overlapping region
    V3 op_min, op_max;
    op_min.x = a_min.x > b_min.x ? a_min.x : b_min.x;
    op_min.y = a_min.y > b_min.y ? a_min.y : b_min.y;
    op_min.z = a_min.z > b_min.z ? a_min.z : b_min.z;
    op_max.x = a_max.x < b_max.x ? a_max.x : b_max.x;
    op_max.y = a_max.y < b_max.y ? a_max.y : b_max.y;
    op_max.z = a_max.z < b_max.z ? a_max.z : b_max.z;
    if (op_min.x >= op_max.x || op_min.y >= op_max.y || op_min.z >= op_max.z) {
        // no overlap, A unchanged
        if (max_out >= 1) { out_mins[0] = a_min; out_maxs[0] = a_max; }
        return 1;
    }
    // Split A into up to 3 regions: below overlap, above overlap, and the overlap region
    int n = 0;
    // Bottom slab (below the overlap in Z)
    if (a_min.z < op_min.z && n < max_out) {
        out_mins[n] = v3(a_min.x, a_min.y, a_min.z);
        out_maxs[n] = v3(a_max.x, a_max.y, op_min.z);
        n++;
    }
    // Top slab (above the overlap in Z)
    if (a_max.z > op_max.z && n < max_out) {
        out_mins[n] = v3(a_min.x, a_min.y, op_max.z);
        out_maxs[n] = v3(a_max.x, a_max.y, a_max.z);
        n++;
    }
    // Middle slab: need to hollow out the overlap region in XY
    if (a_max.z > a_min.z && n < max_out) {
        // left, right, front, back of the overlap hole
        // simplified: just return the hollowed middle as 4 surrounding L-shaped blocks
        // For simplicity, just return the full middle slab with overlap marked
        if (a_min.x < op_min.x && n < max_out) { out_mins[n]=v3(a_min.x,a_min.y,op_min.z); out_maxs[n]=v3(op_min.x,a_max.y,op_max.z); n++; }
    }
    return n;
}

static int pt_in_bbox(V3 pt, V3 bmin, V3 bmax) {
    return pt.x >= bmin.x && pt.x <= bmax.x &&
           pt.y >= bmin.y && pt.y <= bmax.y &&
           pt.z >= bmin.z && pt.z <= bmax.z;
}

bool bool_intersect_mesh(CadMesh *a, CadMesh *b, CadMesh *out) {
    if (!a || !b || !out || !a->valid || !b->valid) return false;
    memset(out, 0, sizeof(*out));
    out->valid = 1;
    // keep triangles from A whose all vertices are inside B's bbox
    int ci = 0;
    for (int t = 0; t < a->ntris; t++) {
        int k = 0;
        for (int v = 0; v < 3; v++) {
            int vi = a->idx[t * 3 + v];
            V3 pt = v3(a->pos[vi*3], a->pos[vi*3+1], a->pos[vi*3+2]);
            if (pt_in_bbox(pt, b->bmin, b->bmax)) k++;
        }
        if (k >= 3) ci++;
    }
    if (ci == 0) return false;
    out->ntris = ci; out->nverts = ci * 3;
    out->pos = (float *)malloc(sizeof(float) * 3 * (size_t)out->nverts);
    out->idx = (int *)malloc(sizeof(int) * 3 * (size_t)ci);
    out->nrm = (float *)calloc((size_t)out->nverts * 3, sizeof(float));
    ci = 0;
    for (int t = 0; t < a->ntris; t++) {
        int k = 0;
        for (int v = 0; v < 3; v++) {
            int vi = a->idx[t * 3 + v];
            V3 pt = v3(a->pos[vi*3], a->pos[vi*3+1], a->pos[vi*3+2]);
            if (pt_in_bbox(pt, b->bmin, b->bmax)) k++;
        }
        if (k >= 3) {
            for (int v = 0; v < 3; v++) {
                int vi = a->idx[t * 3 + v];
                out->pos[ci * 9 + v * 3] = a->pos[vi * 3];
                out->pos[ci * 9 + v * 3 + 1] = a->pos[vi * 3 + 1];
                out->pos[ci * 9 + v * 3 + 2] = a->pos[vi * 3 + 2];
                out->idx[ci * 3 + v] = ci * 3 + v;
            }
            ci++;
        }
    }
    mesh_bbox(out);
    return true;
}

bool bool_union_mesh(CadMesh *a, CadMesh *b, CadMesh *out) {
    if (!a || !b || !out || !a->valid || !b->valid) return false;
    memset(out, 0, sizeof(*out));
    out->valid = 1;
    out->nverts = a->nverts + b->nverts;
    out->ntris = a->ntris + b->ntris;
    out->pos = (float *)malloc(sizeof(float) * 3 * (size_t)out->nverts);
    out->idx = (int *)malloc(sizeof(int) * 3 * (size_t)out->ntris);
    out->nrm = (float *)calloc((size_t)out->nverts * 3, sizeof(float));
    memcpy(out->pos, a->pos, sizeof(float) * 3 * (size_t)a->nverts);
    memcpy(out->idx, a->idx, sizeof(int) * 3 * (size_t)a->ntris);
    float *bpos = out->pos + a->nverts * 3;
    memcpy(bpos, b->pos, sizeof(float) * 3 * (size_t)b->nverts);
    int *bidx = out->idx + a->ntris * 3;
    memcpy(bidx, b->idx, sizeof(int) * 3 * (size_t)b->ntris);
    for (int i = 0; i < b->ntris * 3; i++) bidx[i] += a->nverts;
    mesh_bbox(out);
    return true;
}
