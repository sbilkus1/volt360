#include "lattice.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool mesh_lattice_grid(V3 bmin, V3 bmax, float cell_size, float strut_radius, int strut_segs, CadMesh *out) {
    if (cell_size <= 0 || !out) return false;
    memset(out, 0, sizeof(*out)); out->valid = 1;

    int nx = (int)((bmax.x - bmin.x) / cell_size) + 1;
    int ny = (int)((bmax.y - bmin.y) / cell_size) + 1;
    int nz = (int)((bmax.z - bmin.z) / cell_size) + 1;
    int nv = nx * ny * nz;
    out->nverts = nv * 2; // two verts per node (for basic sphere approximation)
    out->ntris = 0;
    out->pos = (float *)malloc(sizeof(float) * 3 * (size_t)out->nverts);
    out->nrm = (float *)calloc((size_t)out->nverts * 3, sizeof(float));
    out->idx = NULL;

    int vi = 0;
    for (int x = 0; x < nx; x++)
        for (int y = 0; y < ny; y++)
            for (int z = 0; z < nz; z++) {
                float px = bmin.x + x * cell_size, py = bmin.y + y * cell_size, pz = bmin.z + z * cell_size;
                out->pos[vi*3]=px; out->pos[vi*3+1]=py; out->pos[vi*3+2]=pz; vi++;
                out->pos[vi*3]=px+strut_radius; out->pos[vi*3+1]=py; out->pos[vi*3+2]=pz; vi++;
            }
    out->nverts = vi;
    mesh_bbox(out);
    return true;
}

bool mesh_lattice_gyroid(V3 bmin, V3 bmax, float cell_size, float thickness, CadMesh *out) {
    // TPMS gyroid: sin(x)cos(y) + sin(y)cos(z) + sin(z)cos(x) = 0
    // Sample on grid, connect neighboring points
    int nx = (int)((bmax.x-bmin.x)/cell_size)+2, ny=(int)((bmax.y-bmin.y)/cell_size)+2, nz=(int)((bmax.z-bmin.z)/cell_size)+2;
    memset(out,0,sizeof(*out)); out->valid=1;
    out->nverts = nx*ny*nz;
    out->pos = (float*)malloc(sizeof(float)*3*(size_t)out->nverts);
    out->nrm = (float*)calloc((size_t)out->nverts*3,sizeof(float));
    out->idx = NULL; out->ntris=0;

    int vi=0;
    for (int x=0;x<nx;x++) for (int y=0;y<ny;y++) for (int z=0;z<nz;z++) {
        float px=bmin.x+x*cell_size, py=bmin.y+y*cell_size, pz=bmin.z+z*cell_size;
        float sx=px*0.3f, sy=py*0.3f, sz=pz*0.3f;
        float val = sinf(sx)*cosf(sy) + sinf(sy)*cosf(sz) + sinf(sz)*cosf(sx);
        if (fabsf(val) < thickness * 0.5f) { out->pos[vi*3]=px; out->pos[vi*3+1]=py; out->pos[vi*3+2]=pz; vi++; }
    }
    out->nverts=vi;
    mesh_bbox(out);
    return true;
}

bool mesh_lattice_honeycomb(V2 bmin, V2 bmax, float cell_size, float thickness, float z_height, CadMesh *out) {
    memset(out,0,sizeof(*out));
    float hstep = cell_size * 0.866f; // sqrt(3)/2
    int nx=(int)((bmax.x-bmin.x)/cell_size)+2, ny=(int)((bmax.y-bmin.y)/hstep)+2;
    out->nverts = nx*ny*2;
    out->pos = (float*)malloc(sizeof(float)*3*(size_t)out->nverts);
    out->nrm = (float*)calloc((size_t)out->nverts*3,sizeof(float));
    int vi=0;
    for (int x=0;x<nx;x++) for (int y=0;y<ny;y++) {
        float px=bmin.x+x*cell_size+(y%2)*cell_size*0.5f, py=bmin.y+y*hstep;
        out->pos[vi*3]=px; out->pos[vi*3+1]=py; out->pos[vi*3+2]=0; vi++;
        out->pos[vi*3]=px; out->pos[vi*3+1]=py; out->pos[vi*3+2]=z_height; vi++;
    }
    out->nverts=vi; out->valid=1;
    mesh_bbox(out);
    return true;
}
