#include "supports.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

// simple 2D grid fill for a polygon
static void fill_poly_into_layer(SliceLayer *L, V2 *poly, int n, float spacing, float line_w) {
    if (n < 3 || spacing <= 0) return;
    float mnx = poly[0].x, mny = poly[0].y, mxx = mnx, mxy = mny;
    for (int i = 1; i < n; i++) {
        if (poly[i].x < mnx) mnx = poly[i].x; if (poly[i].x > mxx) mxx = poly[i].x;
        if (poly[i].y < mny) mny = poly[i].y; if (poly[i].y > mxy) mxy = poly[i].y;
    }
    // point-in-poly
    for (float y = mny + spacing; y < mxy; y += spacing) {
        for (float x = mnx + spacing; x < mxx; x += spacing) {
            int in = 0;
            for (int i = 0, j = n - 1; i < n; j = i++) {
                if (((poly[i].y > y) != (poly[j].y > y)) &&
                    (x < (poly[j].x - poly[i].x) * (y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x))
                    in = !in;
            }
            if (in) {
                V2 pts[2] = { v2(x - spacing * 0.3f, y), v2(x + spacing * 0.3f, y) };
                int ni = L->npaths++;
                L->paths = (SlicePath *)realloc(L->paths, sizeof(SlicePath) * (size_t)L->npaths);
                SlicePath *p = &L->paths[ni];
                p->kind = 2; // support type
                p->npts = 2; p->width = line_w; p->solid = 0;
                p->pts = (V2 *)malloc(sizeof(V2) * 2);
                p->pts[0] = pts[0]; p->pts[1] = pts[1];
            }
        }
    }
}

void supports_generate(SliceResult *sr, const SliceSettings *cfg, float overhang_angle_deg) {
    if (!sr || sr->nlayers < 2 || !cfg) return;
    float threshold = tanf(overhang_angle_deg * (float)(M_PI / 180.0));
    if (threshold < 0.01f) threshold = 1.0f; // 45 deg default

    for (int li = 1; li < sr->nlayers; li++) {
        SliceLayer *L_below = &sr->layers[li - 1];
        SliceLayer *L = &sr->layers[li];

        // find outer perimeters in both layers
        int n_above = 0, n_below = 0;
        V2 *above_poly = NULL, *below_poly = NULL;

        for (int j = 0; j < L->npaths; j++)
            if (L->paths[j].kind == 0 && L->paths[j].npts > n_above) { n_above = L->paths[j].npts; above_poly = L->paths[j].pts; }
        for (int j = 0; j < L_below->npaths; j++)
            if (L_below->paths[j].kind == 0 && L_below->paths[j].npts > n_below) { n_below = L_below->paths[j].npts; below_poly = L_below->paths[j].pts; }

        if (!above_poly || !below_poly || n_above < 3) continue;

        // compute overhang: where above layer extends beyond below layer
        // simplified: check how much above polygon extends past below bbox
        float bmnx = below_poly[0].x, bmny = below_poly[0].y, bmxx = bmnx, bmxy = bmny;
        for (int i = 1; i < n_below; i++) {
            if (below_poly[i].x < bmnx) bmnx = below_poly[i].x; if (below_poly[i].x > bmxx) bmxx = below_poly[i].x;
            if (below_poly[i].y < bmny) bmny = below_poly[i].y; if (below_poly[i].y > bmxy) bmxy = below_poly[i].y;
        }

        // collect overhang points of above polygon (outside below bbox by > threshold * layer_h)
        float margin_z = cfg->layer_height * threshold;
        V2 overhang[64]; int no = 0;
        for (int i = 0; i < n_above && no < 60; i++) {
            if (above_poly[i].x < bmnx - margin_z || above_poly[i].x > bmxx + margin_z ||
                above_poly[i].y < bmny - margin_z || above_poly[i].y > bmxy + margin_z) {
                overhang[no++] = above_poly[i];
            }
        }
        if (no >= 3) fill_poly_into_layer(L_below, overhang, no, cfg->line_width * 2.5f, cfg->line_width * 0.8f);
    }
}
