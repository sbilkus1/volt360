#include "mesh_import.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mesh_import_3mf(const char *path, Project *p) {
    (void)path; (void)p;
    // 3MF is a ZIP archive with XML model data.
    // Stub: would need ZIP extraction + XML parsing.
    return 0;
}

bool mesh_import_ply(const char *path, CadMesh *out) {
    if (!path || !out) return false;
    FILE *f = fopen(path, "rb");
    if (!f) return false;

    char line[256]; int nv = 0, nf = 0;
    int header = 1;
    while (header && fgets(line, sizeof(line), f)) {
        if (strncmp(line, "element vertex", 14) == 0) nv = atoi(line + 15);
        if (strncmp(line, "element face", 12) == 0) nf = atoi(line + 13);
        if (strncmp(line, "end_header", 10) == 0) header = 0;
    }
    if (nv <= 0 || nf <= 0) { fclose(f); return false; }

    memset(out, 0, sizeof(*out));
    out->valid = 1; out->nverts = nv; out->ntris = nf;
    out->pos = (float *)malloc(sizeof(float) * 3 * (size_t)nv);
    out->nrm = (float *)calloc((size_t)nv * 3, sizeof(float));
    out->idx = (int *)malloc(sizeof(int) * 3 * (size_t)nf);

    // read vertices
    for (int i = 0; i < nv; i++)
        fread(&out->pos[i*3], sizeof(float), 3, f);

    // read faces (each: count, v1, v2, v3)
    for (int i = 0; i < nf; i++) {
        unsigned char cnt;
        fread(&cnt, 1, 1, f);
        int vidx[3] = {0};
        fread(vidx, sizeof(int), cnt < 3 ? cnt : 3, f);
        out->idx[i*3]=vidx[0]; out->idx[i*3+1]=vidx[1]; out->idx[i*3+2]=vidx[2];
        if (cnt > 3) fseek(f, (cnt-3)*sizeof(int), SEEK_CUR); // skip extra verts
    }

    mesh_bbox(out);
    fclose(f);
    return true;
}

bool mesh_import_amf(const char *path, CadMesh *out) {
    (void)path; (void)out;
    // AMF is XML-based. Stub: would need XML parser.
    return false;
}
