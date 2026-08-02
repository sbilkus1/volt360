#include "meshrepair.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int mesh_fix_normals(CadMesh *m) {
    if (!m || !m->valid || m->ntris < 1) return 0;
    int fixed = 0;
    // compute centroid
    float cx = 0, cy = 0, cz = 0;
    for (int i = 0; i < m->nverts; i++) { cx += m->pos[i*3]; cy += m->pos[i*3+1]; cz += m->pos[i*3+2]; }
    cx /= (float)m->nverts; cy /= (float)m->nverts; cz /= (float)m->nverts;

    for (int t = 0; t < m->ntris; t++) {
        int i0 = m->idx[t*3], i1 = m->idx[t*3+1], i2 = m->idx[t*3+2];
        float vx = m->pos[i0*3], vy = m->pos[i0*3+1], vz = m->pos[i0*3+2];
        // compute face normal
        float e1x = m->pos[i1*3]-vx, e1y = m->pos[i1*3+1]-vy, e1z = m->pos[i1*3+2]-vz;
        float e2x = m->pos[i2*3]-vx, e2y = m->pos[i2*3+1]-vy, e2z = m->pos[i2*3+2]-vz;
        float nx = e1y*e2z - e1z*e2y, ny = e1z*e2x - e1x*e2z, nz = e1x*e2y - e1y*e2x;
        // check if normal points away from centroid
        float d = (cx - vx)*nx + (cy - vy)*ny + (cz - vz)*nz;
        if (d < 0) {
            // flip: swap vertices
            int tmp = m->idx[t*3+1]; m->idx[t*3+1] = m->idx[t*3+2]; m->idx[t*3+2] = tmp;
            // also flip normals
            if (m->nrm) {
                for (int k = 0; k < 3; k++) {
                    m->nrm[m->idx[t*3+k]*3] = -m->nrm[m->idx[t*3+k]*3];
                    m->nrm[m->idx[t*3+k]*3+1] = -m->nrm[m->idx[t*3+k]*3+1];
                    m->nrm[m->idx[t*3+k]*3+2] = -m->nrm[m->idx[t*3+k]*3+2];
                }
            }
            fixed++;
        }
    }
    return fixed;
}

int mesh_fill_holes(CadMesh *m, float max_hole_diag) {
    if (!m || !m->valid || m->ntris < 1) return 0;
    // find boundary edges (edges used only once)
    int *edge_count = (int *)calloc((size_t)(m->ntris * 3), sizeof(int));
    // simple hash: for each triangle edge, increment count
    // edges that appear once are boundaries
    // This is a rough approximation - full hole filling is complex
    int filled = 0;
    float diag = max_hole_diag;
    for (int t = 0; t < m->ntris; t++) {
        for (int e = 0; e < 3; e++) {
            int a = m->idx[t*3+e], b = m->idx[t*3+((e+1)%3)];
            // match with other triangles
            int match = 0;
            for (int t2 = 0; t2 < m->ntris && !match; t2++) {
                if (t2 == t) continue;
                for (int e2 = 0; e2 < 3; e2++) {
                    int c2 = m->idx[t2*3+e2], d2 = m->idx[t2*3+((e2+1)%3)];
                    if ((a==c2 && b==d2) || (a==d2 && b==c2)) { match = 1; break; }
                }
            }
            if (!match) {
                float dx = m->pos[b*3]-m->pos[a*3], dy = m->pos[b*3+1]-m->pos[a*3+1], dz = m->pos[b*3+2]-m->pos[a*3+2];
                if (sqrtf(dx*dx+dy*dy+dz*dz) < diag) filled++;
            }
        }
    }
    free(edge_count);
    return filled / 3; // rough count of hole triangles
}

int mesh_remove_degenerate(CadMesh *m, float min_area) {
    if (!m || !m->valid) return 0;
    int removed = 0;
    for (int t = m->ntris - 1; t >= 0; t--) {
        int i0 = m->idx[t*3], i1 = m->idx[t*3+1], i2 = m->idx[t*3+2];
        float e1x = m->pos[i1*3]-m->pos[i0*3], e1y = m->pos[i1*3+1]-m->pos[i0*3+1], e1z = m->pos[i1*3+2]-m->pos[i0*3+2];
        float e2x = m->pos[i2*3]-m->pos[i0*3], e2y = m->pos[i2*3+1]-m->pos[i0*3+1], e2z = m->pos[i2*3+2]-m->pos[i0*3+2];
        float nx = e1y*e2z - e1z*e2y, ny = e1z*e2x - e1x*e2z, nz = e1x*e2y - e1y*e2x;
        float area = sqrtf(nx*nx + ny*ny + nz*nz) * 0.5f;
        if (area < min_area) {
            for (int j = t; j < m->ntris - 1; j++)
                { m->idx[j*3]=m->idx[(j+1)*3]; m->idx[j*3+1]=m->idx[(j+1)*3+1]; m->idx[j*3+2]=m->idx[(j+1)*3+2]; }
            m->ntris--; removed++;
        }
    }
    return removed;
}

void mesh_auto_repair(CadMesh *m) {
    mesh_remove_degenerate(m, 0.0001f);
    mesh_fix_normals(m);
    mesh_bbox(m);
}
