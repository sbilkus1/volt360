#include "xsection.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SL_EPS 1e-5f

int mesh_cross_section(CadMesh *m, float z, V2 ***out_polys, int **out_poly_lens) {
    if (!m || !m->valid || m->ntris < 1) { *out_polys = NULL; *out_poly_lens = NULL; return 0; }
    // collect intersection segments
    int maxseg = m->ntris * 2;
    V2 *pts = (V2 *)malloc(sizeof(V2) * (size_t)(maxseg * 2));
    int nseg = 0;

    for (int t = 0; t < m->ntris; t++) {
        float *v0 = &m->pos[m->idx[t*3]*3], *v1 = &m->pos[m->idx[t*3+1]*3], *v2 = &m->pos[m->idx[t*3+2]*3];
        float d[3] = { v0[2]-z, v1[2]-z, v2[2]-z };
        int above = (d[0]>SL_EPS)+(d[1]>SL_EPS)+(d[2]>SL_EPS);
        int below = (d[0]<-SL_EPS)+(d[1]<-SL_EPS)+(d[2]<-SL_EPS);
        if (above == 3 || below == 3) continue;
        float *va[3] = {v0,v1,v2};
        for (int i = 0; i < 3; i++) if (fabs(d[i]) <= SL_EPS) d[i] = SL_EPS;
        for (int i = 0; i < 3; i++) {
            int j = (i+1)%3;
            if ((d[i]>0) != (d[j]>0)) {
                float t_val = d[i] / (d[i]-d[j]);
                pts[nseg*2].x = va[i][0] + t_val*(va[j][0]-va[i][0]);
                pts[nseg*2].y = va[i][1] + t_val*(va[j][1]-va[i][1]);
                pts[nseg*2+1].x = pts[nseg*2].x; // placeholder for connection
                pts[nseg*2+1].y = pts[nseg*2].y;
                nseg++;
            }
        }
    }

    // group into loops (simple: just return all segments as one polygon)
    if (nseg < 3) { free(pts); *out_polys = NULL; *out_poly_lens = NULL; return 0; }

    V2 *poly = (V2 *)malloc(sizeof(V2) * (size_t)(nseg * 2));
    for (int i = 0; i < nseg; i++) { poly[i*2] = pts[i*2]; poly[i*2+1] = pts[i*2+1]; }

    *out_polys = (V2 **)malloc(sizeof(V2*));
    *out_poly_lens = (int *)malloc(sizeof(int));
    *out_polys[0] = poly;
    *out_poly_lens[0] = nseg * 2;
    free(pts);
    return 1;
}

char *mesh_xsection_text(CadMesh *m, float z) {
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[2048]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off, "Cross-section at Z=%.1f mm\n", z);
    off += snprintf(buf + off, sizeof(buf) - off, "BBox: X %.1f-%.1f  Y %.1f-%.1f\n",
        m->bmin.x, m->bmax.x, m->bmin.y, m->bmax.y);
    // quick intersection count
    int hits = 0;
    for (int t = 0; t < m->ntris; t++) {
        float *v0 = &m->pos[m->idx[t*3]*3];
        float *v2 = &m->pos[m->idx[t*3+2]*3];
        float z0 = v0[2], z2 = v2[2];
        if ((z0 <= z && z2 >= z) || (z2 <= z && z0 >= z)) hits++;
    }
    off += snprintf(buf + off, sizeof(buf) - off, "Triangles crossing: %d / %d\n", hits, m->ntris);
    return str_dup(buf);
}
