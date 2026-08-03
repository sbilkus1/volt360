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

static int point_inside_mesh(V3 pt, CadMesh *m) {
    int hits = 0;
    float px = pt.x, py = pt.y, pz = pt.z;
    for (int t = 0; t < m->ntris; t++) {
        int i0 = m->idx[t*3], i1 = m->idx[t*3+1], i2 = m->idx[t*3+2];
        float v0x = m->pos[i0*3], v0y = m->pos[i0*3+1], v0z = m->pos[i0*3+2];
        float v1x = m->pos[i1*3], v1y = m->pos[i1*3+1], v1z = m->pos[i1*3+2];
        float v2x = m->pos[i2*3], v2y = m->pos[i2*3+1], v2z = m->pos[i2*3+2];
        float min_y = v0y, max_y = v0y;
        if (v1y < min_y) min_y = v1y; if (v2y < min_y) min_y = v2y;
        if (v1y > max_y) max_y = v1y; if (v2y > max_y) max_y = v2y;
        if (py < min_y || py >= max_y) continue;
        float min_x = v0x, max_x = v0x;
        if (v1x < min_x) min_x = v1x; if (v2x < min_x) min_x = v2x;
        if (v1x > max_x) max_x = v1x; if (v2x > max_x) max_x = v2x;
        if (px > max_x) continue;
        float e1x = v1x - v0x, e1y = v1y - v0y, e1z = v1z - v0z;
        float e2x = v2x - v0x, e2y = v2y - v0y, e2z = v2z - v0z;
        float nx = e1y*e2z - e1z*e2y;
        float ny = e1z*e2x - e1x*e2z;
        float nz = e1x*e2y - e1y*e2x;
        float len = sqrtf(nx*nx + ny*ny + nz*nz);
        if (len < 1e-10f) continue;
        nx /= len; ny /= len; nz /= len;
        float dx = e1x, dy = e1y, dz = e1z;
        float t_num = ny*(v0y - py) + nz*(v0z - pz);
        float t_den = ny*dy + nz*dz;
        if (fabsf(t_den) < 1e-10f) continue;
        float t = t_num / t_den;
        if (t < 0 || t > 1) continue;
        float hx = v0x + dx*t - px;
        if (hx < 0) continue;
        float ex = e2x, ey = e2y, ez = e2z;
        float s_den = ey*nz - ez*ny;
        if (fabsf(s_den) < 1e-10f) continue;
        float s_num = ey*(py - v0y - dy*t) - ex*(pz - v0z - dz*t);
        float s = s_num / s_den;
        if (s < 0 || s > 1 || t + s > 1) continue;
        hits++;
    }
    return hits % 2;
}

#define VOX_RES 64

bool bool_subtract_mesh(CadMesh *a, CadMesh *b, CadMesh *out) {
    if (!a || !b || !out || !a->valid || !b->valid) return false;
    memset(out, 0, sizeof(*out));

    V3 umin = v3_min(a->bmin, b->bmin);
    V3 umax = v3_max(a->bmax, b->bmax);
    float pad = 0.02f;
    umin.x -= pad; umin.y -= pad; umin.z -= pad;
    umax.x += pad; umax.y += pad; umax.z += pad;
    float sx = umax.x - umin.x, sy = umax.y - umin.y, sz = umax.z - umin.z;
    if (sx <= 0 || sy <= 0 || sz <= 0) return false;

    float cs = sx/(float)VOX_RES, cy_s = sy/(float)VOX_RES, cz_s = sz/(float)VOX_RES;
    int total = VOX_RES * VOX_RES * VOX_RES;
    unsigned char *grid = (unsigned char *)calloc((size_t)total, 1);
    if (!grid) return false;

    for (int iz = 0; iz < VOX_RES; iz++) {
        for (int iy = 0; iy < VOX_RES; iy++) {
            for (int ix = 0; ix < VOX_RES; ix++) {
                V3 pt = v3(umin.x + ((float)ix + 0.5f)*cs,
                           umin.y + ((float)iy + 0.5f)*cy_s,
                           umin.z + ((float)iz + 0.5f)*cz_s);
                if (point_inside_mesh(pt, a)) {
                    grid[(iz*VOX_RES + iy)*VOX_RES + ix] = 1;
                }
            }
        }
    }

    for (int iz = 0; iz < VOX_RES; iz++) {
        for (int iy = 0; iy < VOX_RES; iy++) {
            for (int ix = 0; ix < VOX_RES; ix++) {
                int idx = (iz*VOX_RES + iy)*VOX_RES + ix;
                if (!grid[idx]) continue;
                V3 pt = v3(umin.x + ((float)ix + 0.5f)*cs,
                           umin.y + ((float)iy + 0.5f)*cy_s,
                           umin.z + ((float)iz + 0.5f)*cz_s);
                if (point_inside_mesh(pt, b)) {
                    grid[idx] = 0;
                }
            }
        }
    }

    int nfaces = 0;
    for (int iz = 0; iz < VOX_RES; iz++) {
        for (int iy = 0; iy < VOX_RES; iy++) {
            for (int ix = 0; ix < VOX_RES; ix++) {
                int idx = (iz*VOX_RES + iy)*VOX_RES + ix;
                if (!grid[idx]) continue;
                if (ix == 0 || !grid[idx - 1]) nfaces++;
                if (ix == VOX_RES-1 || !grid[idx + 1]) nfaces++;
                if (iy == 0 || !grid[idx - VOX_RES]) nfaces++;
                if (iy == VOX_RES-1 || !grid[idx + VOX_RES]) nfaces++;
                if (iz == 0 || !grid[idx - VOX_RES*VOX_RES]) nfaces++;
                if (iz == VOX_RES-1 || !grid[idx + VOX_RES*VOX_RES]) nfaces++;
            }
        }
    }
    if (nfaces == 0) { free(grid); return false; }

    out->ntris = nfaces * 2;
    out->nverts = nfaces * 4;
    out->pos = (float *)malloc(sizeof(float) * 3 * (size_t)out->nverts);
    out->idx = (int *)malloc(sizeof(int) * 3 * (size_t)out->ntris);
    out->nrm = (float *)calloc((size_t)out->nverts * 3, sizeof(float));
    out->valid = 1;

    int vi = 0, ti = 0;
    for (int iz = 0; iz < VOX_RES; iz++) {
        for (int iy = 0; iy < VOX_RES; iy++) {
            for (int ix = 0; ix < VOX_RES; ix++) {
                int idx = (iz*VOX_RES + iy)*VOX_RES + ix;
                if (!grid[idx]) continue;
                float x0 = umin.x + (float)ix * cs;
                float x1 = x0 + cs;
                float y0 = umin.y + (float)iy * cy_s;
                float y1 = y0 + cy_s;
                float z0 = umin.z + (float)iz * cz_s;
                float z1 = z0 + cz_s;

                if (ix == 0 || !grid[idx - 1]) {
                    int b = vi;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z1; vi++;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z1; vi++;
                    out->idx[ti*3]=b; out->idx[ti*3+1]=b+1; out->idx[ti*3+2]=b+2; ti++;
                    out->idx[ti*3]=b; out->idx[ti*3+1]=b+2; out->idx[ti*3+2]=b+3; ti++;
                }
                if (ix == VOX_RES-1 || !grid[idx + 1]) {
                    int b = vi;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z1; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z1; vi++;
                    out->idx[ti*3]=b+2; out->idx[ti*3+1]=b+1; out->idx[ti*3+2]=b; ti++;
                    out->idx[ti*3]=b+3; out->idx[ti*3+1]=b+2; out->idx[ti*3+2]=b; ti++;
                }
                if (iy == 0 || !grid[idx - VOX_RES]) {
                    int b = vi;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z1; vi++;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z1; vi++;
                    out->idx[ti*3]=b+2; out->idx[ti*3+1]=b+1; out->idx[ti*3+2]=b; ti++;
                    out->idx[ti*3]=b+3; out->idx[ti*3+1]=b+2; out->idx[ti*3+2]=b; ti++;
                }
                if (iy == VOX_RES-1 || !grid[idx + VOX_RES]) {
                    int b = vi;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z1; vi++;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z1; vi++;
                    out->idx[ti*3]=b; out->idx[ti*3+1]=b+1; out->idx[ti*3+2]=b+2; ti++;
                    out->idx[ti*3]=b; out->idx[ti*3+1]=b+2; out->idx[ti*3+2]=b+3; ti++;
                }
                if (iz == 0 || !grid[idx - VOX_RES*VOX_RES]) {
                    int b = vi;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z0; vi++;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z0; vi++;
                    out->idx[ti*3]=b; out->idx[ti*3+1]=b+1; out->idx[ti*3+2]=b+2; ti++;
                    out->idx[ti*3]=b; out->idx[ti*3+1]=b+2; out->idx[ti*3+2]=b+3; ti++;
                }
                if (iz == VOX_RES-1 || !grid[idx + VOX_RES*VOX_RES]) {
                    int b = vi;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z1; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y0; out->pos[(vi)*3+2]=z1; vi++;
                    out->pos[(vi)*3]=x1; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z1; vi++;
                    out->pos[(vi)*3]=x0; out->pos[(vi)*3+1]=y1; out->pos[(vi)*3+2]=z1; vi++;
                    out->idx[ti*3]=b+2; out->idx[ti*3+1]=b+1; out->idx[ti*3+2]=b; ti++;
                    out->idx[ti*3]=b+3; out->idx[ti*3+1]=b+2; out->idx[ti*3+2]=b; ti++;
                }
            }
        }
    }
    out->nverts = vi;
    out->ntris = ti;
    free(grid);
    mesh_bbox(out);
    return true;
}
