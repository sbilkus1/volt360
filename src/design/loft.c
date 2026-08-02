#include "loft.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool mesh_loft(V2 *profile_a, V2 *profile_b, int n_verts, float z_a, float z_b, CadMesh *out) {
    if (!profile_a || !profile_b || !out || n_verts < 3) return false;
    memset(out, 0, sizeof(*out));
    out->valid = 1;
    // vertices: profile_a (n) + profile_b (n) = 2n vertices
    out->nverts = n_verts * 2;
    out->pos = (float *)malloc(sizeof(float) * 3 * (size_t)out->nverts);
    out->nrm = (float *)calloc((size_t)out->nverts * 3, sizeof(float));
    // side triangles: n*2 + top/bottom caps: (n-2)*2 = n*2 + 2n - 4 = 4n - 4
    out->ntris = n_verts * 4 - 4;
    out->idx = (int *)malloc(sizeof(int) * 3 * (size_t)out->ntris);

    for (int i = 0; i < n_verts; i++) {
        out->pos[i * 3] = profile_a[i].x; out->pos[i * 3 + 1] = profile_a[i].y; out->pos[i * 3 + 2] = z_a;
        out->pos[(n_verts + i) * 3] = profile_b[i].x; out->pos[(n_verts + i) * 3 + 1] = profile_b[i].y; out->pos[(n_verts + i) * 3 + 2] = z_b;
    }

    int ti = 0;
    // side walls
    for (int i = 0; i < n_verts; i++) {
        int i0 = i, i1 = (i + 1) % n_verts;
        int j0 = n_verts + i, j1 = n_verts + (i + 1) % n_verts;
        out->idx[ti * 3] = i0; out->idx[ti * 3 + 1] = i1; out->idx[ti * 3 + 2] = j0; ti++;
        out->idx[ti * 3] = i1; out->idx[ti * 3 + 1] = j1; out->idx[ti * 3 + 2] = j0; ti++;
    }
    // bottom cap (fan from vertex 0)
    for (int i = 1; i < n_verts - 1; i++) {
        out->idx[ti * 3] = 0; out->idx[ti * 3 + 1] = i; out->idx[ti * 3 + 2] = i + 1; ti++;
    }
    // top cap (fan from vertex n_verts)
    for (int i = 1; i < n_verts - 1; i++) {
        out->idx[ti * 3] = n_verts; out->idx[ti * 3 + 1] = n_verts + i + 1; out->idx[ti * 3 + 2] = n_verts + i; ti++;
    }

    mesh_bbox(out);
    return true;
}
