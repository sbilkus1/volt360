#include "primitives.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

static void m_grow(CadMesh *m, int nv, int nt) {
    m->pos = (float *)realloc(m->pos, (size_t)(m->nverts + nv) * 3 * sizeof(float));
    m->nrm = (float *)realloc(m->nrm, (size_t)(m->nverts + nv) * 3 * sizeof(float));
    m->idx = (int *)realloc(m->idx, (size_t)(m->ntris + nt) * 3 * sizeof(int));
}
static void m_v(CadMesh *m, float x, float y, float z, float nx, float ny, float nz) {
    int i = m->nverts * 3;
    m->pos[i]=x; m->pos[i+1]=y; m->pos[i+2]=z;
    m->nrm[i]=nx; m->nrm[i+1]=ny; m->nrm[i+2]=nz;
    m->nverts++;
}
static void m_t(CadMesh *m, int a, int b, int c) {
    int i = m->ntris * 3;
    m->idx[i]=a; m->idx[i+1]=b; m->idx[i+2]=c;
    m->ntris++;
}

void mesh_cone(CadMesh *m, V3 center, float radius, float height, int segs) {
    if (segs < 3) segs = 16;
    m_grow(m, segs + 2, segs * 2);
    int base_start = m->nverts;
    float hz = height * 0.5f;
    for (int i = 0; i < segs; i++) {
        float a = (float)i * 2.0f * (float)M_PI / (float)segs;
        float x = center.x + radius * cosf(a), y = center.y + radius * sinf(a);
        float nx = cosf(a), ny = sinf(a);
        m_v(m, x, y, center.z - hz, nx, ny, 0);
    }
    int apex = m->nverts; m_v(m, center.x, center.y, center.z + hz, 0, 0, 1);
    int base_center = m->nverts; m_v(m, center.x, center.y, center.z - hz, 0, 0, -1);
    // side triangles
    for (int i = 0; i < segs; i++) {
        int i1 = base_start + i, i2 = base_start + (i + 1) % segs;
        m_t(m, i1, i2, apex);
        m_t(m, i2, i1, base_center);
    }
}

void mesh_torus(CadMesh *m, V3 center, float R, float r, int segs, int ring_segs) {
    if (segs < 4) segs = 16; if (ring_segs < 4) ring_segs = 8;
    m_grow(m, segs * ring_segs, segs * ring_segs * 2);
    int base = m->nverts;
    for (int i = 0; i < segs; i++) {
        float phi = (float)i * 2.0f * (float)M_PI / (float)segs;
        float cx = center.x + R * cosf(phi), cy = center.y + R * sinf(phi);
        for (int j = 0; j < ring_segs; j++) {
            float theta = (float)j * 2.0f * (float)M_PI / (float)ring_segs;
            float x = cx + r * cosf(theta) * cosf(phi);
            float y = cy + r * cosf(theta) * sinf(phi);
            float z = center.z + r * sinf(theta);
            m_v(m, x, y, z, cosf(theta)*cosf(phi), cosf(theta)*sinf(phi), sinf(theta));
        }
    }
    for (int i = 0; i < segs; i++) {
        int ni = (i + 1) % segs;
        for (int j = 0; j < ring_segs; j++) {
            int nj = (j + 1) % ring_segs;
            int a = base + i * ring_segs + j, b = base + ni * ring_segs + j;
            int c = base + i * ring_segs + nj, d = base + ni * ring_segs + nj;
            m_t(m, a, b, c); m_t(m, c, b, d);
        }
    }
}

void mesh_wedge(CadMesh *m, V3 c, V3 s) {
    m_grow(m, 6, 8);
    float hx = s.x * 0.5f, hy = s.y * 0.5f, hz = s.z * 0.5f;
    // vertices: 0-3 base, 4-5 top (triangle edge)
    float v[6][3] = {
        {c.x-hx, c.y-hy, c.z-hz}, {c.x+hx, c.y-hy, c.z-hz}, {c.x+hx, c.y+hy, c.z-hz}, {c.x-hx, c.y+hy, c.z-hz},
        {c.x-hx, c.y-hy, c.z+hz}, {c.x-hx, c.y+hy, c.z+hz}
    };
    for (int i = 0; i < 6; i++) m_v(m, v[i][0], v[i][1], v[i][2], 0, 1, 0);
    int b = m->nverts - 6;
    // base
    m_t(m,b+0,b+1,b+2); m_t(m,b+0,b+2,b+3);
    // side 1
    m_t(m,b+0,b+4,b+1); m_t(m,b+1,b+4,b+5); m_t(m,b+1,b+5,b+2);
    // side 2
    m_t(m,b+3,b+2,b+5); m_t(m,b+0,b+3,b+4); m_t(m,b+3,b+5,b+4);
}

void mesh_pyramid(CadMesh *m, V3 center, float base_w, float base_d, float height) {
    m_grow(m, 5, 6);
    float hx = base_w * 0.5f, hy = base_d * 0.5f, hz = height * 0.5f;
    int b = m->nverts;
    m_v(m, center.x-hx, center.y-hy, center.z-hz, 0,-1,0); // 0
    m_v(m, center.x+hx, center.y-hy, center.z-hz, 1,-1,0); // 1
    m_v(m, center.x+hx, center.y+hy, center.z-hz, 1,1,0);  // 2
    m_v(m, center.x-hx, center.y+hy, center.z-hz, -1,1,0); // 3
    m_v(m, center.x, center.y, center.z+hz, 0,0,1);         // 4 apex
    // base (2 tris)
    m_t(m,b+0,b+1,b+2); m_t(m,b+0,b+2,b+3);
    // sides
    m_t(m,b+0,b+1,b+4); m_t(m,b+1,b+2,b+4);
    m_t(m,b+2,b+3,b+4); m_t(m,b+3,b+0,b+4);
}
