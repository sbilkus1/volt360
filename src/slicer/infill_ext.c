#include "infill_ext.h"
#include <math.h>
#include <stdlib.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

int infill_gyroid(V2 *poly, int n, float spacing, float angle, V2 *out, int maxseg) {
    if (n < 3 || spacing <= 0 || maxseg < 1) return 0;
    float mnx = poly[0].x, mny = poly[0].y, mxx = mnx, mxy = mny;
    for (int i = 1; i < n; i++) {
        if (poly[i].x < mnx) mnx = poly[i].x; if (poly[i].x > mxx) mxx = poly[i].x;
        if (poly[i].y < mny) mny = poly[i].y; if (poly[i].y > mxy) mxy = poly[i].y;
    }
    float rad = angle * (float)(M_PI / 180.0);
    float c = cosf(rad), s = sinf(rad);
    int cnt = 0;
    for (float y = mny; y < mxy && cnt < maxseg - 2; y += spacing * 0.5f) {
        for (float x = mnx; x < mxx && cnt < maxseg - 2; x += spacing) {
            float wave = sinf((x * c + y * s) * 0.3f) * cosf((-x * s + y * c) * 0.3f) * spacing * 0.4f;
            float rx = x * c - y * s, ry = x * s + y * c;
            // single line segment approximating the wave
            out[cnt].x = rx; out[cnt].y = ry + wave;
            out[cnt+1].x = rx + spacing; out[cnt+1].y = ry + wave;
            cnt += 2;
        }
    }
    return cnt;
}

int infill_lightning(V2 *poly, int n, float line_w, V2 *out, int maxseg) {
    if (n < 3 || maxseg < 2) return 0;
    float mnx = poly[0].x, mny = poly[0].y, mxx = mnx, mxy = mny;
    for (int i = 1; i < n; i++) {
        if (poly[i].x < mnx) mnx = poly[i].x; if (poly[i].x > mxx) mxx = poly[i].x;
        if (poly[i].y < mny) mny = poly[i].y; if (poly[i].y > mxy) mxy = poly[i].y;
    }
    float cx = (mnx + mxx) * 0.5f, cy = (mny + mxy) * 0.5f;
    int cnt = 0;
    float step = line_w * 8.0f;
    // radial lightning: lines from center to perimeter
    for (float a = 0; a < 6.283f && cnt < maxseg - 2; a += 0.5f) {
        float rx = cx + (mxx - mnx) * 0.45f * cosf(a);
        float ry = cy + (mxy - mny) * 0.45f * sinf(a);
        out[cnt].x = cx; out[cnt].y = cy;
        out[cnt+1].x = rx; out[cnt+1].y = ry;
        cnt += 2;
    }
    return cnt;
}

int infill_cubic(V2 *poly, int n, float spacing, int layer_idx, V2 *out, int maxseg) {
    if (n < 3 || spacing <= 0 || maxseg < 1) return 0;
    // cubic alternates angle every layer to create 3D lattice effect
    float angles[3] = { 0, 60, -60 };
    float deg = angles[layer_idx % 3];
    float rad = deg * (float)(M_PI / 180.0);
    float c = cosf(rad), s = sinf(rad);

    float mnx = poly[0].x, mny = poly[0].y, mxx = mnx, mxy = mny;
    for (int i = 1; i < n; i++) {
        if (poly[i].x < mnx) mnx = poly[i].x; if (poly[i].x > mxx) mxx = poly[i].x;
        if (poly[i].y < mny) mny = poly[i].y; if (poly[i].y > mxy) mxy = poly[i].y;
    }
    int cnt = 0;
    for (float d = mnx + mny; d < mxx + mxy && cnt < maxseg - 2; d += spacing * 0.7f) {
        float x0 = mnx, y0 = d - mnx;
        if (y0 < mny) { y0 = mny; x0 = d - mny; }
        float x1 = d - mxy, y1 = mxy;
        if (x1 < mnx) { x1 = mnx; y1 = d - mnx; }
        if (x0 >= mnx && x0 <= mxx && y0 >= mny && y0 <= mxy &&
            x1 >= mnx && x1 <= mxx && y1 >= mny && y1 <= mxy) {
            out[cnt].x = x0; out[cnt].y = y0;
            out[cnt+1].x = x1; out[cnt+1].y = y1;
            cnt += 2;
        }
    }
    return cnt;
}
