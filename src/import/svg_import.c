#include "svg_import.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int svg_parse_path(const char *d, V2 **points) {
    if (!d || !points) return 0;
    int cap = 256, n = 0;
    V2 *pts = (V2 *)malloc(sizeof(V2) * (size_t)cap);
    float cx = 0, cy = 0, sx = 0, sy = 0;
    const char *p = d;
    while (*p) {
        while (*p == ' ' || *p == ',' || *p == '\n' || *p == '\r') p++;
        if (!*p) break;
        char cmd = 'L';
        if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')) { cmd = *p; p++; }
        float args[6]; int na = 0;
        while (*p && na < 6) {
            while (*p == ' ' || *p == ',') p++;
            if (*p == '-' || *p == '+' || (*p >= '0' && *p <= '9') || *p == '.') {
                args[na++] = (float)atof(p);
                while (*p && (*p == '-' || *p == '+' || (*p >= '0' && *p <= '9') || *p == '.')) p++;
            } else break;
        }
        if (na >= 2) {
            float nx = args[na-2], ny = args[na-1];
            if (cmd >= 'a') { nx += cx; ny += cy; } // relative
            if (cmd == 'M' || cmd == 'm') { sx = nx; sy = ny; cx = nx; cy = ny; }
            else if (cmd == 'L' || cmd == 'l' || cmd == 'H' || cmd == 'h' || cmd == 'V' || cmd == 'v') {
                if (n >= cap) { cap *= 2; pts = (V2 *)realloc(pts, sizeof(V2)*(size_t)cap); }
                pts[n++] = v2(cx, cy);
                pts[n++] = v2(nx, ny);
                cx = nx; cy = ny;
            } else if (cmd == 'C' || cmd == 'c' && na >= 6) {
                // cubic bezier: approximate with line segments
                for (int s = 1; s <= 4; s++) {
                    float t = s / 4.0f;
                    float t2 = t*t, t3 = t2*t;
                    float u = 1-t, u2 = u*u, u3 = u2*u;
                    float bx = u3*cx + 3*u2*t*args[0] + 3*u*t2*args[2] + t3*args[4];
                    float by = u3*cy + 3*u2*t*args[1] + 3*u*t2*args[3] + t3*args[5];
                    if (n >= cap) { cap *= 2; pts = (V2 *)realloc(pts, sizeof(V2)*(size_t)cap); }
                    pts[n++] = v2(cx, cy);
                    pts[n++] = v2(bx, by);
                    cx = bx; cy = by;
                }
            } else if (cmd == 'Z' || cmd == 'z') {
                if (n >= cap) { cap *= 2; pts = (V2 *)realloc(pts, sizeof(V2)*(size_t)cap); }
                pts[n++] = v2(cx, cy);
                pts[n++] = v2(sx, sy);
                cx = sx; cy = sy;
            }
        }
    }
    *points = pts;
    return n;
}

bool svg_import_pcb(const char *path, Pcb *out) {
    if (!path || !out) return false;
    FILE *f = fopen(path, "r"); if (!f) return false;
    char *data = NULL; size_t len = 0;
    fseek(f, 0, SEEK_END); len = (size_t)ftell(f); fseek(f, 0, SEEK_SET);
    data = (char *)malloc(len + 1); fread(data, 1, len, f); data[len] = '\0'; fclose(f);

    memset(out, 0, sizeof(*out));
    out->id = str_dup(make_id());
    const char *slash = strrchr(path, '/'); const char *bs = strrchr(path, '\\');
    if (bs && bs > slash) slash = bs;
    out->name = str_dup(slash ? slash + 1 : path);

    // find path data
    char *cur = data;
    while ((cur = strstr(cur, " d=\""))) {
        cur += 4;
        char *endq = strchr(cur, '"');
        if (!endq) break;
        char saved = *endq; *endq = '\0';
        V2 *pts; int n = svg_parse_path(cur, &pts);
        *endq = saved; cur = endq + 1;
        for (int i = 0; i < n-1; i += 2) {
            Track t; t.a = pts[i]; t.b = pts[i+1]; t.width = 0.1f; t.net = NULL;
            out->tracks = (Track *)realloc(out->tracks, sizeof(Track)*(size_t)(out->ntracks+1));
            out->tracks[out->ntracks++] = t;
        }
        free(pts);
    }
    free(data);
    pcb_bbox(out);
    return out->ntracks > 0;
}
