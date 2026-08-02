#include "sweep.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static V3 rotate_profile(V2 p, V3 tang, V3 norm, V3 binorm, V3 pos) {
    return v3(pos.x + p.x*binorm.x + p.y*norm.x,
              pos.y + p.x*binorm.y + p.y*norm.y,
              pos.z + p.x*binorm.z + p.y*norm.z);
}

bool mesh_sweep(V2 *profile, int np, V3 *path, int npath, bool closed, CadMesh *out) {
    if (!profile || np < 3 || !path || npath < 2 || !out) return false;
    memset(out, 0, sizeof(*out)); out->valid = 1;
    int nv = np * npath + (closed ? np*2 : 0); // + cap vertices
    int nt = np * 2 * (npath - 1) + (closed ? (np-2)*2 : 0); // sides + caps
    out->nverts = nv; out->ntris = nt;
    out->pos = (float *)malloc(sizeof(float)*3*(size_t)nv);
    out->nrm = (float *)calloc((size_t)nv*3, sizeof(float));
    out->idx = (int *)malloc(sizeof(int)*3*(size_t)nt);

    // compute local frames at each path point
    V3 *tangents = (V3 *)malloc(sizeof(V3)*(size_t)npath);
    V3 *normals = (V3 *)malloc(sizeof(V3)*(size_t)npath);
    V3 *binormals = (V3 *)malloc(sizeof(V3)*(size_t)npath);

    for (int i = 0; i < npath; i++) {
        V3 tang;
        if (i == 0) tang = v3(path[1].x-path[0].x, path[1].y-path[0].y, path[1].z-path[0].z);
        else if (i == npath-1) tang = v3(path[i].x-path[i-1].x, path[i].y-path[i-1].y, path[i].z-path[i-1].z);
        else tang = v3(path[i+1].x-path[i-1].x, path[i+1].y-path[i-1].y, path[i+1].z-path[i-1].z);
        float tl = sqrtf(tang.x*tang.x+tang.y*tang.y+tang.z*tang.z);
        if (tl < 0.001f) tl = 1; tang.x/=tl; tang.y/=tl; tang.z/=tl;
        tangents[i] = tang;
        // binormal = cross(tang, up) where up = (0,0,1)
        V3 up = v3(0,0,1);
        if (fabsf(tang.z) > 0.99f) up = v3(1,0,0);
        binormals[i] = v3(tang.y*up.z-tang.z*up.y, tang.z*up.x-tang.x*up.z, tang.x*up.y-tang.y*up.x);
        float bl = sqrtf(binormals[i].x*binormals[i].x+binormals[i].y*binormals[i].y+binormals[i].z*binormals[i].z);
        if (bl<0.001f) bl=1; binormals[i].x/=bl; binormals[i].y/=bl; binormals[i].z/=bl;
        normals[i] = v3(binormals[i].y*tang.z-binormals[i].z*tang.y,
                        binormals[i].z*tang.x-binormals[i].x*tang.z,
                        binormals[i].x*tang.y-binormals[i].y*tang.x);
    }

    // place vertices
    for (int pi = 0; pi < npath; pi++)
        for (int vi = 0; vi < np; vi++) {
            V3 pt = rotate_profile(profile[vi], tangents[pi], normals[pi], binormals[pi], path[pi]);
            int idx = pi * np + vi;
            out->pos[idx*3]=pt.x; out->pos[idx*3+1]=pt.y; out->pos[idx*3+2]=pt.z;
        }

    // triangles: side walls
    int ti = 0;
    for (int pi = 0; pi < npath-1; pi++)
        for (int vi = 0; vi < np; vi++) {
            int vj = (vi+1)%np;
            int a = pi*np+vi, b = pi*np+vj, c = (pi+1)*np+vi, d = (pi+1)*np+vj;
            out->idx[ti*3]=a; out->idx[ti*3+1]=b; out->idx[ti*3+2]=c; ti++;
            out->idx[ti*3]=b; out->idx[ti*3+1]=d; out->idx[ti*3+2]=c; ti++;
        }

    // end caps if closed
    if (closed) {
        int cap_start = npath * np;
        // place cap vertices at start and end
        for (int vi = 0; vi < np; vi++) {
            out->pos[(cap_start+vi)*3] = path[0].x;
            out->pos[(cap_start+vi)*3+1] = path[0].y;
            out->pos[(cap_start+vi)*3+2] = path[0].z;
            out->pos[(cap_start+np+vi)*3] = path[npath-1].x;
            out->pos[(cap_start+np+vi)*3+1] = path[npath-1].y;
            out->pos[(cap_start+np+vi)*3+2] = path[npath-1].z;
        }
        // fan triangles
        for (int vi = 1; vi < np-1; vi++) {
            out->idx[ti*3]=cap_start; out->idx[ti*3+1]=cap_start+vi; out->idx[ti*3+2]=cap_start+vi+1; ti++;
            out->idx[ti*3]=cap_start+np; out->idx[ti*3+1]=cap_start+np+vi+1; out->idx[ti*3+2]=cap_start+np+vi; ti++;
        }
    }
    out->ntris = ti;
    free(tangents); free(normals); free(binormals);
    mesh_bbox(out);
    return true;
}
