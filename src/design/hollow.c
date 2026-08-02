#include "hollow.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool mesh_hollow(CadMesh *in, CadMesh *out, float wall_thickness_mm) {
    if (!in || !out || !in->valid || wall_thickness_mm <= 0) return false;
    memset(out, 0, sizeof(*out));
    out->valid = 1;
    out->nverts = in->nverts;
    out->ntris = in->ntris;
    out->pos = (float *)malloc(sizeof(float) * 3 * (size_t)in->nverts);
    out->nrm = (float *)malloc(sizeof(float) * 3 * (size_t)in->nverts);
    out->idx = (int *)malloc(sizeof(int) * 3 * (size_t)in->ntris);

    // offset each vertex inward along its normal
    for (int i = 0; i < in->nverts; i++) {
        float nx = in->nrm ? in->nrm[i * 3] : 0;
        float ny = in->nrm ? in->nrm[i * 3 + 1] : 0;
        float nz = in->nrm ? in->nrm[i * 3 + 2] : 0;
        float len = sqrtf(nx*nx + ny*ny + nz*nz);
        if (len < 0.001f) len = 1.0f;
        out->pos[i * 3] = in->pos[i * 3] - nx / len * wall_thickness_mm;
        out->pos[i * 3 + 1] = in->pos[i * 3 + 1] - ny / len * wall_thickness_mm;
        out->pos[i * 3 + 2] = in->pos[i * 3 + 2] - nz / len * wall_thickness_mm;
        out->nrm[i * 3] = in->nrm ? in->nrm[i * 3] : 0;
        out->nrm[i * 3 + 1] = in->nrm ? in->nrm[i * 3 + 1] : 0;
        out->nrm[i * 3 + 2] = in->nrm ? in->nrm[i * 3 + 2] : 0;
    }
    memcpy(out->idx, in->idx, sizeof(int) * 3 * (size_t)in->ntris);
    mesh_bbox(out);
    return true;
}

bool mesh_decimate(CadMesh *in, CadMesh *out, float target_ratio) {
    // simple decimation: skip every N-th triangle
    if (!in || !out || !in->valid || target_ratio <= 0 || target_ratio >= 1.0f) return false;
    memset(out, 0, sizeof(*out));
    int keep = (int)(in->ntris * target_ratio);
    if (keep < 1) keep = 1;
    int step = in->ntris / keep;
    if (step < 2) step = 2;
    out->valid = 1;
    out->ntris = keep;
    out->nverts = in->nverts;
    out->pos = (float *)malloc(sizeof(float) * 3 * (size_t)in->nverts);
    out->nrm = (float *)malloc(sizeof(float) * 3 * (size_t)in->nverts);
    out->idx = (int *)malloc(sizeof(int) * 3 * (size_t)keep);
    memcpy(out->pos, in->pos, sizeof(float) * 3 * (size_t)in->nverts);
    if (in->nrm) memcpy(out->nrm, in->nrm, sizeof(float) * 3 * (size_t)in->nverts);
    int ki = 0;
    for (int i = 0; i < in->ntris && ki < keep; i += step) {
        out->idx[ki * 3] = in->idx[i * 3];
        out->idx[ki * 3 + 1] = in->idx[i * 3 + 1];
        out->idx[ki * 3 + 2] = in->idx[i * 3 + 2];
        ki++;
    }
    out->ntris = ki;
    mesh_bbox(out);
    return true;
}
