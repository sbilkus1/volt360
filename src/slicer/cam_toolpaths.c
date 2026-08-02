#include "cam_toolpaths.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

static float cam_fmin(float a, float b) { return a < b ? a : b; }
static float cam_fmax(float a, float b) { return a > b ? a : b; }

void campath_init(CamPath *p) {
    p->pts = NULL;
    p->npts = 0;
    p->cap = 0;
}

void campath_free(CamPath *p) {
    free(p->pts);
    p->pts = NULL;
    p->npts = 0;
    p->cap = 0;
}

void campath_add(CamPath *p, float x, float y, float z, int code, float feed) {
    if (p->npts >= p->cap) {
        p->cap = p->cap ? p->cap * 2 : 64;
        p->pts = (CamPoint *)realloc(p->pts, sizeof(CamPoint) * (size_t)p->cap);
    }
    p->pts[p->npts].x = x;
    p->pts[p->npts].y = y;
    p->pts[p->npts].z = z;
    p->pts[p->npts].code = code;
    p->pts[p->npts].feed = feed;
    p->npts++;
}

void campath_bbox(V2 *poly, int n, CamBBox *bb) {
    int i;
    if (n <= 0) { memset(bb, 0, sizeof(*bb)); return; }
    bb->mnx = poly[0].x; bb->mny = poly[0].y;
    bb->mxx = poly[0].x; bb->mxy = poly[0].y;
    for (i = 1; i < n; i++) {
        if (poly[i].x < bb->mnx) bb->mnx = poly[i].x;
        if (poly[i].y < bb->mny) bb->mny = poly[i].y;
        if (poly[i].x > bb->mxx) bb->mxx = poly[i].x;
        if (poly[i].y > bb->mxy) bb->mxy = poly[i].y;
    }
}

int cam_generate_face(CamPath *paths, int max_paths, CamTool *t, CamBBox *area, float depth) {
    float step, z, safe_z, retract_z;
    int dir;
    float yy;
    if (max_paths < 1 || !t || !area) return 0;
    step = t->diameter * t->stepover;
    if (step < 0.01f) step = 0.5f;
    safe_z = 5.0f;
    retract_z = 2.0f;
    campath_init(&paths[0]);
    z = 0.0f;
    while (z > depth) {
        z -= depth; z = cam_fmin(z, -depth);
        z = cam_fmax(z, depth);
        break;
    }
    {
        float dz = t->diameter * 0.25f;
        z = 0.0f;
        while (z > depth) {
            z -= dz;
            if (z < depth) z = depth;
            campath_add(&paths[0], area->mnx, area->mny, safe_z, 0, t->feedrate);
            campath_add(&paths[0], area->mnx, area->mny, z, 1, t->feedrate * 0.3f);
            dir = 1;
            for (yy = area->mny; yy < area->mxy; yy += step) {
                float top = cam_fmin(yy + step, area->mxy);
                if (dir) {
                    campath_add(&paths[0], area->mxx, yy, z, 1, t->feedrate);
                    campath_add(&paths[0], area->mxx, top, z, 1, t->feedrate);
                } else {
                    campath_add(&paths[0], area->mnx, top, z, 1, t->feedrate);
                    campath_add(&paths[0], area->mnx, yy, z, 1, t->feedrate);
                }
                dir = !dir;
            }
            campath_add(&paths[0], area->mnx, area->mny, retract_z, 0, t->feedrate);
        }
    }
    campath_add(&paths[0], area->mnx, area->mny, safe_z, 0, t->feedrate);
    return paths[0].npts > 0 ? 1 : 0;
}

int cam_generate_pocket(CamPath *paths, int max_paths, CamTool *t, V2 *poly, int npts, float depth) {
    CamBBox bb;
    float step, z, dx, dy, safe_z, retract_z;
    int pass_max;
    if (max_paths < 1 || !t || !poly || npts < 3) return 0;
    step = t->diameter * t->stepover;
    if (step < 0.01f) step = 0.5f;
    safe_z = 5.0f;
    retract_z = 2.0f;
    campath_bbox(poly, npts, &bb);
    dx = bb.mxx - bb.mnx;
    dy = bb.mxy - bb.mny;
    campath_init(&paths[0]);
    {
        float dz = t->diameter * 0.25f;
        z = 0.0f;
        while (z > depth) {
            int xi, yi;
            float cx, cy;
            int nx_passes, ny_passes;
            z -= dz;
            if (z < depth) z = depth;
            nx_passes = (int)(dx / step) + 1;
            ny_passes = (int)(dy / step) + 1;
            if (nx_passes > 200) nx_passes = 200;
            if (ny_passes > 200) ny_passes = 200;
            cx = (bb.mnx + bb.mxx) * 0.5f;
            cy = (bb.mny + bb.mxy) * 0.5f;
            campath_add(&paths[0], cx, cy, safe_z, 0, t->feedrate);
            campath_add(&paths[0], cx, cy, z, 1, t->feedrate * 0.3f);
            pass_max = nx_passes > ny_passes ? nx_passes : ny_passes;
            for (xi = 0; xi <= pass_max; xi++) {
                float nstep = step * (float)(xi + 1);
                float x0, x1, y0, y1;
                if (nstep > dx * 0.5f + step) break;
                x0 = cx - nstep; x1 = cx + nstep;
                y0 = cy - nstep; y1 = cy + nstep;
                if (x0 < bb.mnx) x0 = bb.mnx;
                if (x1 > bb.mxx) x1 = bb.mxx;
                if (y0 < bb.mny) y0 = bb.mny;
                if (y1 > bb.mxy) y1 = bb.mxy;
                if (xi % 2 == 0) {
                    campath_add(&paths[0], x0, y0, z, 1, t->feedrate);
                    campath_add(&paths[0], x1, y0, z, 1, t->feedrate);
                    campath_add(&paths[0], x1, y1, z, 1, t->feedrate);
                    campath_add(&paths[0], x0, y1, z, 1, t->feedrate);
                } else {
                    campath_add(&paths[0], x0, y1, z, 1, t->feedrate);
                    campath_add(&paths[0], x1, y1, z, 1, t->feedrate);
                    campath_add(&paths[0], x1, y0, z, 1, t->feedrate);
                    campath_add(&paths[0], x0, y0, z, 1, t->feedrate);
                }
            }
            for (yi = 0; yi < npts; yi++)
                campath_add(&paths[0], poly[yi].x, poly[yi].y, z, 1, t->feedrate);
            campath_add(&paths[0], poly[0].x, poly[0].y, z, 1, t->feedrate);
            campath_add(&paths[0], cx, cy, retract_z, 0, t->feedrate);
        }
    }
    campath_add(&paths[0], bb.mnx, bb.mny, safe_z, 0, t->feedrate);
    return paths[0].npts > 0 ? 1 : 0;
}

static void arc_points(float cx, float cy, float z, float r, float a0, float a1, int segs, float feed, CamPath *p, int is_leadout) {
    int i;
    float xe = cx + r * cosf(a1);
    float ye = cy + r * sinf(a1);
    for (i = 0; i <= segs; i++) {
        float t = (float)i / (float)segs;
        float a = a0 + (a1 - a0) * t;
        campath_add(p, cx + r * cosf(a), cy + r * sinf(a), z, 1, feed);
    }
}

int cam_generate_profile(CamPath *paths, int max_paths, CamTool *t, V2 *poly, int npts, float depth) {
    float z, safe_z, retract_z;
    float start_x, start_y;
    int i;
    float dx, dy, len_val, nx, ny, lead_in;
    if (max_paths < 1 || !t || !poly || npts < 3) return 0;
    safe_z = 5.0f;
    retract_z = 2.0f;
    campath_init(&paths[0]);
    start_x = poly[0].x;
    start_y = poly[0].y;
    z = 0.0f;
    {
        float dz = t->diameter * 0.25f;
        while (z > depth) {
            z -= dz;
            if (z < depth) z = depth;
            {
                dx = poly[1].x - poly[0].x;
                dy = poly[1].y - poly[0].y;
                len_val = sqrtf(dx * dx + dy * dy);
                if (len_val < 0.001f) len_val = 1.0f;
                nx = -dy / len_val;
                ny = dx / len_val;
                lead_in = t->diameter * 0.5f + 1.0f;
                campath_add(&paths[0], start_x + nx * lead_in, start_y + ny * lead_in, safe_z, 0, t->feedrate);
                campath_add(&paths[0], start_x + nx * lead_in, start_y + ny * lead_in, z, 1, t->feedrate * 0.3f);
                campath_add(&paths[0], start_x, start_y, z, 1, t->feedrate);
            }
            for (i = 1; i < npts; i++)
                campath_add(&paths[0], poly[i].x, poly[i].y, z, 1, t->feedrate);
            campath_add(&paths[0], start_x, start_y, z, 1, t->feedrate);
            {
                dx = poly[npts - 1].x - start_x;
                dy = poly[npts - 1].y - start_y;
                len_val = sqrtf(dx * dx + dy * dy);
                if (len_val < 0.001f) len_val = 1.0f;
                nx = -dy / len_val;
                ny = dx / len_val;
                lead_in = t->diameter * 0.5f + 1.0f;
                campath_add(&paths[0], start_x + nx * lead_in, start_y + ny * lead_in, z, 1, t->feedrate);
                campath_add(&paths[0], start_x + nx * lead_in, start_y + ny * lead_in, retract_z, 0, t->feedrate);
            }
            campath_add(&paths[0], start_x, start_y, retract_z, 0, t->feedrate);
        }
    }
    campath_add(&paths[0], start_x, start_y, safe_z, 0, t->feedrate);
    return paths[0].npts > 0 ? 1 : 0;
}

int cam_generate_drill(CamPath *paths, int max_paths, CamTool *t, V2 *holes, int nholes, float depth) {
    int i;
    float safe_z, retract_z;
    float z;
    if (max_paths < 1 || !t || !holes || nholes <= 0) return 0;
    safe_z = 5.0f;
    retract_z = 2.0f;
    campath_init(&paths[0]);
    for (i = 0; i < nholes; i++) {
        campath_add(&paths[0], holes[i].x, holes[i].y, safe_z, 0, t->feedrate);
        campath_add(&paths[0], holes[i].x, holes[i].y, retract_z, 0, t->feedrate);
        z = 0.0f;
        {
            float peck = depth > -1.0f ? -1.0f : depth;
            while (z > depth) {
                z += peck;
                if (z < depth) z = depth;
                campath_add(&paths[0], holes[i].x, holes[i].y, z, 1, t->feedrate * 0.3f);
                if (z > depth) campath_add(&paths[0], holes[i].x, holes[i].y, retract_z, 0, t->feedrate);
            }
        }
        campath_add(&paths[0], holes[i].x, holes[i].y, safe_z, 0, t->feedrate);
    }
    return paths[0].npts > 0 ? 1 : 0;
}
