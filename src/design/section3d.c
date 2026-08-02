#include "section3d.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

bool mesh_clip_plane(CadMesh *in, CadMesh *out, ClipPlane plane) {
    if (!in || !out || !in->valid) return false;
    memset(out, 0, sizeof(*out));
    out->valid = 1;
    int kept = 0;
    // count vertices on positive side
    for (int i = 0; i < in->nverts; i++) {
        float dx = in->pos[i*3] - plane.point.x;
        float dy = in->pos[i*3+1] - plane.point.y;
        float dz = in->pos[i*3+2] - plane.point.z;
        float d = dx*plane.normal.x + dy*plane.normal.y + dz*plane.normal.z;
        if (d >= -0.001f) kept++;
    }
    out->nverts = kept;
    out->pos = (float *)malloc(sizeof(float)*3*(size_t)kept);
    out->nrm = (float *)calloc((size_t)kept*3, sizeof(float));
    int *vmap = (int *)malloc(sizeof(int)*(size_t)in->nverts);
    int vi = 0;
    for (int i = 0; i < in->nverts; i++) {
        float dx = in->pos[i*3] - plane.point.x, dy = in->pos[i*3+1] - plane.point.y, dz = in->pos[i*3+2] - plane.point.z;
        if (dx*plane.normal.x + dy*plane.normal.y + dz*plane.normal.z >= -0.001f) {
            out->pos[vi*3]=in->pos[i*3]; out->pos[vi*3+1]=in->pos[i*3+1]; out->pos[vi*3+2]=in->pos[i*3+2];
            if (in->nrm) { out->nrm[vi*3]=in->nrm[i*3]; out->nrm[vi*3+1]=in->nrm[i*3+1]; out->nrm[vi*3+2]=in->nrm[i*3+2]; }
            vmap[i] = vi++;
        } else vmap[i] = -1;
    }
    // keep triangles where all 3 vertices survive
    int ct = 0;
    for (int t = 0; t < in->ntris; t++)
        if (vmap[in->idx[t*3]]>=0 && vmap[in->idx[t*3+1]]>=0 && vmap[in->idx[t*3+2]]>=0) ct++;
    out->ntris = ct;
    out->idx = (int *)malloc(sizeof(int)*3*(size_t)ct);
    ct = 0;
    for (int t = 0; t < in->ntris; t++)
        if (vmap[in->idx[t*3]]>=0 && vmap[in->idx[t*3+1]]>=0 && vmap[in->idx[t*3+2]]>=0) {
            out->idx[ct*3]=vmap[in->idx[t*3]]; out->idx[ct*3+1]=vmap[in->idx[t*3+1]]; out->idx[ct*3+2]=vmap[in->idx[t*3+2]]; ct++;
        }
    free(vmap);
    mesh_bbox(out);
    return true;
}

char *section3d_report(CadMesh *m, ClipPlane plane) {
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[512];
    int kept = 0;
    for (int i = 0; i < m->nverts; i++) {
        float d = (m->pos[i*3]-plane.point.x)*plane.normal.x + (m->pos[i*3+1]-plane.point.y)*plane.normal.y + (m->pos[i*3+2]-plane.point.z)*plane.normal.z;
        if (d >= -0.001f) kept++;
    }
    snprintf(buf, sizeof(buf), "Section plane: normal=(%.1f,%.1f,%.1f)  kept %d/%d verts, %d tris",
        plane.normal.x, plane.normal.y, plane.normal.z, kept, m->nverts, m->ntris);
    return str_dup(buf);
}
