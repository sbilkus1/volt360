#include "curvature.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

float *mesh_curvature_mean(CadMesh *m) {
    if (!m || !m->valid || m->nverts < 1) return NULL;
    float *curv = (float *)calloc((size_t)m->nverts, sizeof(float));
    for (int i = 0; i < m->nverts; i++) {
        // approximate mean curvature from vertex normal variation
        float total = 0; int cnt = 0;
        float vx = m->pos[i*3], vy = m->pos[i*3+1], vz = m->pos[i*3+2];
        for (int j = 0; j < m->nverts && cnt < 10; j++) {
            if (i == j) continue;
            float dx = m->pos[j*3]-vx, dy = m->pos[j*3+1]-vy, dz = m->pos[j*3+2]-vz;
            float d = dx*dx+dy*dy+dz*dz;
            if (d < 10.0f) { total += sqrtf(d); cnt++; }
        }
        curv[i] = cnt > 0 ? total / (float)cnt : 0;
    }
    return curv;
}

float *mesh_curvature_gaussian(CadMesh *m) {
    if (!m || !m->valid || m->nverts < 1) return NULL;
    float *curv = (float *)calloc((size_t)m->nverts, sizeof(float));
    for (int t = 0; t < m->ntris; t++) {
        int i0=m->idx[t*3], i1=m->idx[t*3+1], i2=m->idx[t*3+2];
        float e1x=m->pos[i1*3]-m->pos[i0*3], e1y=m->pos[i1*3+1]-m->pos[i0*3+1], e1z=m->pos[i1*3+2]-m->pos[i0*3+2];
        float e2x=m->pos[i2*3]-m->pos[i0*3], e2y=m->pos[i2*3+1]-m->pos[i0*3+1], e2z=m->pos[i2*3+2]-m->pos[i0*3+2];
        float area = sqrtf((e1y*e2z-e1z*e2y)*(e1y*e2z-e1z*e2y)+(e1z*e2x-e1x*e2z)*(e1z*e2x-e1x*e2z)+(e1x*e2y-e1y*e2x)*(e1x*e2y-e1y*e2x))*0.5f;
        curv[i0] += area; curv[i1] += area; curv[i2] += area;
    }
    return curv;
}

float mesh_roughness_rms(CadMesh *m) {
    if (!m || !m->valid || m->ntris < 1) return 0;
    float sum_sq = 0;
    for (int t = 0; t < m->ntris; t++) {
        int i0=m->idx[t*3], i1=m->idx[t*3+1], i2=m->idx[t*3+2];
        float z0=m->pos[i0*3+2], z1=m->pos[i1*3+2], z2=m->pos[i2*3+2];
        float mean_z = (z0+z1+z2)/3.0f;
        sum_sq += (z0-mean_z)*(z0-mean_z) + (z1-mean_z)*(z1-mean_z) + (z2-mean_z)*(z2-mean_z);
    }
    return sqrtf(sum_sq / (float)(m->ntris*3));
}

char *curvature_report(CadMesh *m) {
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[1024]; int off = 0;
    float *mc = mesh_curvature_mean(m);
    float roughness = mesh_roughness_rms(m);
    float max_c = 0, avg_c = 0;
    if (mc) {
        for (int i = 0; i < m->nverts; i++) { avg_c += mc[i]; if (mc[i] > max_c) max_c = mc[i]; }
        avg_c /= (float)m->nverts;
        free(mc);
    }
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== SURFACE QUALITY REPORT ===\n"
        "Verts: %d  Tris: %d\n"
        "Max curvature: %.2f  Avg: %.2f\n"
        "RMS roughness: %.4f mm\n"
        "Quality: %s\n",
        m->nverts, m->ntris, max_c, avg_c, roughness,
        roughness < 0.01f ? "Excellent" : roughness < 0.1f ? "Good" : "Rough");
    return str_dup(buf);
}
