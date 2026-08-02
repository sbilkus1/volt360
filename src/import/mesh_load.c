#include "importer.h"
#include <ctype.h>

#ifdef _MSC_VER
#define strtok_r strtok_s
#endif

// ---------------- STL (binary + ascii) ----------------
bool mesh_load_stl(const char *path, CadMesh *out) {
    memset(out, 0, sizeof(*out));
    size_t len = 0;
    char *data = file_read(path, &len);
    if (!data || len < 15) { free(data); return false; }
    ARRAY(float) verts; arr_init(verts);
    ARRAY(float) norms; arr_init(norms);
    ARRAY(int) idx; arr_init(idx);

    // ascii? starts with "solid" but binary can too... check: binary has 80-byte header; try to detect triangles count
    bool is_binary = false;
    if (len >= 84) {
        uint32_t ntris;
        memcpy(&ntris, data + 80, 4);
        if (ntris > 0 && 84 + (size_t)ntris * 50 <= len + 4) is_binary = true;
    }
    if (is_binary) {
        uint32_t ntris;
        memcpy(&ntris, data + 80, 4);
        const unsigned char *p = (const unsigned char *)data + 84;
        for (uint32_t t = 0; t < ntris; t++) {
            if (p + 50 > (const unsigned char *)data + len) break;
            float n[3], v[3][3];
            uint16_t attr;
            memcpy(n, p, 12); memcpy(v[0], p + 12, 12); memcpy(v[1], p + 24, 12); memcpy(v[2], p + 36, 12); memcpy(&attr, p + 48, 2);
            (void)attr;
            int base = verts.len / 3;
            for (int i = 0; i < 3; i++) {
                arr_push(verts, v[i][0]); arr_push(verts, v[i][1]); arr_push(verts, v[i][2]);
                arr_push(norms, n[0]); arr_push(norms, n[1]); arr_push(norms, n[2]);
                arr_push(idx, base + i);
            }
            p += 50;
        }
    } else {
        // ascii
        const char *p = data;
        while (*p && p < data + len) {
            char line[512];
            int i = 0;
            while (*p && *p != '\n' && i < 510 && p < data + len) line[i++] = *p++;
            if (*p == '\n') p++;
            line[i] = '\0';
            float n[3];
            if (sscanf(line, " facet normal %f %f %f", &n[0], &n[1], &n[2]) >= 3) {
                    // read vertices
                    float v[3][3];
                    for (int k = 0; k < 3; k++) {
                        // skip to "vertex"
                        const char *q = line;
                        while (*q && strncmp(q, "vertex", 6) != 0) { q++; if (!*q) break; }
                        // read next "vertex" line
                        char vline[512];
                        if (*q) {
                            // consume rest of current line (may contain vertex)
                            memcpy(vline, q, strlen(q) + 1);
                        } else {
                            // read next line
                            int j = 0;
                            while (*p && *p != '\n' && j < 510 && p < data + len) vline[j++] = *p++;
                            if (*p == '\n') p++;
                            vline[j] = '\0';
                        }
                        float x, y, z;
                        if (sscanf(vline, " vertex %f %f %f", &x, &y, &z) >= 3) {
                            v[k][0] = x; v[k][1] = y; v[k][2] = z;
                        } else { v[k][0] = v[k][1] = v[k][2] = 0; }
                    }
                    int base = verts.len / 3;
                    for (int k = 0; k < 3; k++) {
                        arr_push(verts, v[k][0]); arr_push(verts, v[k][1]); arr_push(verts, v[k][2]);
                        arr_push(norms, n[0]); arr_push(norms, n[1]); arr_push(norms, n[2]);
                        arr_push(idx, base + k);
                    }
                }
            }
        }
    free(data);
    if (verts.len == 0) { arr_free(verts); arr_free(norms); arr_free(idx); return false; }
    out->nverts = verts.len / 3;
    out->ntris = idx.len / 3;
    out->pos = verts.v;
    out->nrm = norms.v;
    out->idx = idx.v;
    mesh_bbox(out);
    return out->valid;
}

// ---------------- OBJ ----------------
bool mesh_load_obj(const char *path, CadMesh *out) {
    memset(out, 0, sizeof(*out));
    size_t len = 0;
    char *data = file_read(path, &len);
    if (!data) return false;
    ARRAY(float) verts; arr_init(verts);
    ARRAY(float) norms; arr_init(norms);
    ARRAY(int) idx; arr_init(idx);
    ARRAY(float) nv; arr_init(nv); // normals pool

    const char *p = data;
    while (p && *p && p < data + len) {
        char line[1024];
        int i = 0;
        while (*p && *p != '\n' && i < 1022 && p < data + len) line[i++] = *p++;
        if (*p == '\n') p++;
        line[i] = '\0';
        str_trim(line);
        if (!*line || line[0] == '#') continue;
        if (strncmp(line, "v ", 2) == 0) {
            float x, y, z;
            if (sscanf(line + 2, "%f %f %f", &x, &y, &z) >= 3) { arr_push(verts, x); arr_push(verts, y); arr_push(verts, z); }
        } else if (strncmp(line, "vn ", 3) == 0) {
            float x, y, z;
            if (sscanf(line + 3, "%f %f %f", &x, &y, &z) >= 3) { arr_push(nv, x); arr_push(nv, y); arr_push(nv, z); }
        } else if (line[0] == 'f') {
            // parse "f v/vt/vn v/vt/vn v/vt/vn" (quads -> two tris)
            char *toks[8];
            int ntoks = 0;
            char *copy = str_dup(line + 2);
            char *save = NULL;
            for (char *tk = strtok_r(copy, " ", &save); tk && ntoks < 8; tk = strtok_r(NULL, " ", &save)) {
                char *slash = strchr(tk, '/');
                if (slash) *slash = '\0';
                int vi = atoi(tk);
                toks[ntoks++] = tk;
                (void)vi;
            }
            int vlist[8];
            for (int k = 0; k < ntoks; k++) {
                char *slash = strchr(toks[k], '/');
                (void)slash;
                int vi = atoi(toks[k]);
                vlist[k] = (vi < 0) ? (verts.len / 3 + vi) : (vi - 1);
            }
            for (int k = 1; k + 1 < ntoks; k++) {
                arr_push(idx, vlist[0]); arr_push(idx, vlist[k]); arr_push(idx, vlist[k + 1]);
            }
            free(copy);
        }
    }
    free(data);
    if (verts.len < 9 || idx.len == 0) { arr_free(verts); arr_free(nv); arr_free(norms); arr_free(idx); return false; }
    out->nverts = verts.len / 3;
    out->ntris = idx.len / 3;
    out->pos = verts.v;
    out->idx = idx.v;
    if (nv.len == verts.len) {
        out->nrm = nv.v;
    } else {
        // compute flat normals
        for (int i = 0; i < out->nverts; i++) { arr_push(norms, 0); arr_push(norms, 0); arr_push(norms, 0); }
        for (int t = 0; t < out->ntris; t++) {
            int a = out->idx[t * 3], b = out->idx[t * 3 + 1], c = out->idx[t * 3 + 2];
            V3 va = v3(out->pos[a * 3], out->pos[a * 3 + 1], out->pos[a * 3 + 2]);
            V3 vb = v3(out->pos[b * 3], out->pos[b * 3 + 1], out->pos[b * 3 + 2]);
            V3 vc = v3(out->pos[c * 3], out->pos[c * 3 + 1], out->pos[c * 3 + 2]);
            V3 n = v3_cross(v3_sub(vb, va), v3_sub(vc, va));
            for (int k = 0; k < 3; k++) {
                int vi = out->idx[t * 3 + k];
                norms.v[vi * 3] += n.x; norms.v[vi * 3 + 1] += n.y; norms.v[vi * 3 + 2] += n.z;
            }
        }
        for (int i = 0; i < out->nverts; i++) {
            V3 n = v3(norms.v[i * 3], norms.v[i * 3 + 1], norms.v[i * 3 + 2]);
            float l = v3_len(n);
            if (l > 1e-6f) { n = v3_scale(n, 1.0f / l); }
            norms.v[i * 3] = n.x; norms.v[i * 3 + 1] = n.y; norms.v[i * 3 + 2] = n.z;
        }
        out->nrm = norms.v;
        arr_free(nv);
    }
    mesh_bbox(out);
    return out->valid;
}

bool mesh_load_any(const char *path, CadMesh *out) {
    const char *ext = path_ext(path);
    if (strcmp(ext, "stl") == 0) return mesh_load_stl(path, out);
    if (strcmp(ext, "obj") == 0) return mesh_load_obj(path, out);
    if (strcmp(ext, "step") == 0 || strcmp(ext, "stp") == 0) return mesh_load_step(path, out);
    return false;
}
