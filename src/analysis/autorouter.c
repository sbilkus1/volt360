#include "autorouter.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* ---------- helpers ---------- */

static void world_to_grid(const RouteGrid *g, V2 world, int *gx, int *gy) {
    *gx = (int)((world.x - g->origin.x) / g->cell_size + 0.5f);
    *gy = (int)((world.y - g->origin.y) / g->cell_size + 0.5f);
}

static V2 grid_to_world(const RouteGrid *g, int gx, int gy) {
    return v2(g->origin.x + (float)gx * g->cell_size,
              g->origin.y + (float)gy * g->cell_size);
}

static int clamp_int(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static int cell_idx(const RouteGrid *g, int x, int y) {
    return y * g->w + x;
}

static int is_in_bounds(const RouteGrid *g, int x, int y) {
    return x >= 0 && x < g->w && y >= 0 && y < g->h;
}

/* ---------- grid lifecycle ---------- */

void route_grid_init(RouteGrid *g, const Pcb *pcb, float cell_size) {
    if (!pcb || !pcb->has_bbox) {
        memset(g, 0, sizeof(*g));
        return;
    }
    g->cell_size = cell_size > 0.0f ? cell_size : 0.5f;
    g->via_cost = 2.0f;
    g->clearance = 1.0f;
    g->max_iterations = 5;

    float margin = 2.0f;
    g->origin.x = pcb->bmin.x - margin;
    g->origin.y = pcb->bmin.y - margin;
    g->w = (int)((pcb->bmax.x - pcb->bmin.x + 2.0f * margin) / g->cell_size) + 2;
    g->h = (int)((pcb->bmax.y - pcb->bmin.y + 2.0f * margin) / g->cell_size) + 2;
    if (g->w < 4) g->w = 4;
    if (g->h < 4) g->h = 4;

    size_t n = (size_t)g->w * (size_t)g->h;
    g->cells = (CellState *)calloc(n, sizeof(CellState));
    g->dist  = (int *)malloc(n * sizeof(int));
}

void route_grid_free(RouteGrid *g) {
    free(g->cells); g->cells = NULL;
    free(g->dist);  g->dist  = NULL;
    g->w = g->h = 0;
}

void route_grid_clear_path(RouteGrid *g) {
    if (!g->cells) return;
    size_t n = (size_t)g->w * (size_t)g->h;
    for (size_t i = 0; i < n; i++) {
        if (g->cells[i] == CELL_VIA) g->cells[i] = CELL_FREE;
    }
}

/* rasterise a circle of cells as obstacles */
static void mark_circle(RouteGrid *g, V2 center, float radius) {
    int radius_cells = (int)(radius / g->cell_size) + 2;
    int cx, cy;
    world_to_grid(g, center, &cx, &cy);
    for (int dy = -radius_cells; dy <= radius_cells; dy++) {
        for (int dx = -radius_cells; dx <= radius_cells; dx++) {
            int gx = cx + dx, gy = cy + dy;
            if (!is_in_bounds(g, gx, gy)) continue;
            float dcx = (float)dx * g->cell_size;
            float dcy = (float)dy * g->cell_size;
            if (sqrtf(dcx * dcx + dcy * dcy) <= radius + g->cell_size * g->clearance) {
                g->cells[cell_idx(g, gx, gy)] = CELL_OBSTACLE;
            }
        }
    }
}

/* rasterise a line segment with width onto the grid */
static void mark_line(RouteGrid *g, V2 a, V2 b, float width) {
    int half = (int)((width * 0.5f + g->cell_size * g->clearance) / g->cell_size) + 1;
    int ax, ay, bx, by;
    world_to_grid(g, a, &ax, &ay);
    world_to_grid(g, b, &bx, &by);

    int steps = abs(bx - ax) + abs(by - ay) + 1;
    if (steps < 1) steps = 1;
    for (int s = 0; s <= steps; s++) {
        float t = (float)s / (float)steps;
        int cx = ax + (int)((float)(bx - ax) * t + 0.5f);
        int cy = ay + (int)((float)(by - ay) * t + 0.5f);
        for (int dy = -half; dy <= half; dy++) {
            for (int dx = -half; dx <= half; dx++) {
                int gx = cx + dx, gy = cy + dy;
                if (is_in_bounds(g, gx, gy))
                    g->cells[cell_idx(g, gx, gy)] = CELL_OBSTACLE;
            }
        }
    }
}

void route_grid_mark_obstacles(RouteGrid *g, const Pcb *pcb) {
    if (!g->cells || !pcb) return;

    /* mark outside bbox as obstacle using board outline expanded margin */
    int cm = (int)(g->clearance);
    for (int y = 0; y < g->h; y++) {
        for (int x = 0; x < g->w; x++) {
            V2 w = grid_to_world(g, x, y);
            if (w.x < pcb->bmin.x - cm * g->cell_size ||
                w.x > pcb->bmax.x + cm * g->cell_size ||
                w.y < pcb->bmin.y - cm * g->cell_size ||
                w.y > pcb->bmax.y + cm * g->cell_size) {
                g->cells[cell_idx(g, x, y)] = CELL_OBSTACLE;
            }
        }
    }

    /* mark FpInst pads */
    for (int i = 0; i < pcb->nfps; i++) {
        mark_circle(g, pcb->fps[i].pos, 1.5f);
    }

    /* mark existing tracks */
    for (int i = 0; i < pcb->ntracks; i++) {
        route_grid_mark_track(g, &pcb->tracks[i]);
    }

    /* mark vias */
    for (int i = 0; i < pcb->nvias; i++) {
        mark_circle(g, pcb->vias[i].pos, pcb->vias[i].outer * 0.5f + 0.2f);
    }
}

void route_grid_mark_track(RouteGrid *g, const Track *t) {
    if (!g->cells || !t) return;
    mark_line(g, t->a, t->b, t->width > 0.0f ? t->width : 0.3f);
}

/* ---------- Lee's algorithm ---------- */

static const int lee_dx[4] = {  0,  0,  1, -1 };
static const int lee_dy[4] = { -1,  1,  0,  0 };

bool route_lee(RouteGrid *g, int sx, int sy, int tx, int ty,
               int *out_x, int *out_y, int *out_count, int max_segments)
{
    if (!g->cells || !out_x || !out_y || !out_count) return false;
    if (!is_in_bounds(g, sx, sy) || !is_in_bounds(g, tx, ty)) return false;
    if (g->cells[cell_idx(g, sx, sy)] == CELL_OBSTACLE) return false;
    if (g->cells[cell_idx(g, tx, ty)] == CELL_OBSTACLE) return false;

    size_t n = (size_t)g->w * (size_t)g->h;
    memset(g->dist, -1, n * sizeof(int));

    int *queue = (int *)malloc(n * sizeof(int));
    if (!queue) return false;
    int head = 0, tail = 0;

    int si = cell_idx(g, sx, sy);
    g->dist[si] = 0;
    queue[tail++] = si;

    int found = 0;
    while (head < tail) {
        int cur = queue[head++];
        int cx = cur % g->w;
        int cy = cur / g->w;
        int cd = g->dist[cur];

        if (cx == tx && cy == ty) {
            found = 1;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nx = cx + lee_dx[d];
            int ny = cy + lee_dy[d];
            if (!is_in_bounds(g, nx, ny)) continue;
            int ni = cell_idx(g, nx, ny);
            if (g->cells[ni] == CELL_OBSTACLE) continue;
            if (g->dist[ni] >= 0) continue;
            g->dist[ni] = cd + 1;
            queue[tail++] = ni;
        }
    }

    free(queue);

    if (!found) {
        *out_count = 0;
        return false;
    }

    /* backtrace */
    int max_path = max_segments > 0 ? max_segments : g->w + g->h;
    int *px = (int *)malloc((size_t)max_path * sizeof(int));
    int *py = (int *)malloc((size_t)max_path * sizeof(int));
    if (!px || !py) { free(px); free(py); *out_count = 0; return false; }

    int cx = tx, cy = ty;
    int path_len = 0;
    px[path_len] = cx; py[path_len] = cy; path_len++;

    while (!(cx == sx && cy == sy)) {
        int cd = g->dist[cell_idx(g, cx, cy)];
        if (cd <= 0) break;
        for (int d = 0; d < 4; d++) {
            int nx = cx + lee_dx[d];
            int ny = cy + lee_dy[d];
            if (!is_in_bounds(g, nx, ny)) continue;
            if (g->dist[cell_idx(g, nx, ny)] == cd - 1) {
                cx = nx; cy = ny;
                if (path_len < max_path) {
                    px[path_len] = cx; py[path_len] = cy; path_len++;
                }
                break;
            }
        }
    }

    /* reverse to source-first order */
    for (int i = 0; i < path_len / 2; i++) {
        int tmpx = px[i], tmpy = py[i];
        px[i] = px[path_len - 1 - i];
        py[i] = py[path_len - 1 - i];
        px[path_len - 1 - i] = tmpx;
        py[path_len - 1 - i] = tmpy;
    }

    /* collapse colinear points */
    int collapsed = 0;
    int *cx_arr = (int *)malloc((size_t)path_len * sizeof(int));
    int *cy_arr = (int *)malloc((size_t)path_len * sizeof(int));
    if (!cx_arr || !cy_arr) {
        free(px); free(py); free(cx_arr); free(cy_arr);
        *out_count = 0; return false;
    }

    cx_arr[0] = px[0]; cy_arr[0] = py[0]; collapsed = 1;
    for (int i = 1; i < path_len - 1; i++) {
        int pdx = px[i] - px[i - 1];
        int pdy = py[i] - py[i - 1];
        int ndx = px[i + 1] - px[i];
        int ndy = py[i + 1] - py[i];
        if (pdx == ndx && pdy == ndy) continue;
        cx_arr[collapsed] = px[i];
        cy_arr[collapsed] = py[i];
        collapsed++;
    }
    cx_arr[collapsed] = px[path_len - 1];
    cy_arr[collapsed] = py[path_len - 1];
    collapsed++;

    *out_count = collapsed < max_path ? collapsed : max_path;
    for (int i = 0; i < *out_count; i++) {
        out_x[i] = cx_arr[i];
        out_y[i] = cy_arr[i];
    }

    free(px); free(py); free(cx_arr); free(cy_arr);
    return *out_count > 0;
}

/* ---------- net extraction helpers ---------- */

typedef struct {
    char *net;
    V2 *pts;
    int npts;
    int cap;
} NetGroup;

static void netgroup_init(NetGroup *ng, const char *net) {
    ng->net = str_dup(net ? net : "");
    ng->pts = NULL;
    ng->npts = 0;
    ng->cap = 0;
}

static void netgroup_free(NetGroup *ng) {
    free(ng->net);
    free(ng->pts);
    ng->pts = NULL;
    ng->npts = 0;
    ng->cap = 0;
}

static void netgroup_add(NetGroup *ng, V2 pt) {
    if (ng->npts >= ng->cap) {
        ng->cap = ng->cap ? ng->cap * 2 : 16;
        ng->pts = (V2 *)realloc(ng->pts, (size_t)ng->cap * sizeof(V2));
    }
    ng->pts[ng->npts++] = pt;
}

static int netgroup_find(NetGroup *groups, int ngroups, const char *net) {
    for (int i = 0; i < ngroups; i++) {
        if (strcmp(groups[i].net, net) == 0) return i;
    }
    return -1;
}

static void extract_nets(Pcb *pcb, NetGroup **out_groups, int *out_ngroups) {
    int ngroups = 0;
    int cap = 0;
    NetGroup *groups = NULL;

    for (int i = 0; i < pcb->ntracks; i++) {
        const char *net = pcb->tracks[i].net ? pcb->tracks[i].net : "";
        int idx = netgroup_find(groups, ngroups, net);
        if (idx < 0) {
            if (ngroups >= cap) {
                cap = cap ? cap * 2 : 8;
                groups = (NetGroup *)realloc(groups, (size_t)cap * sizeof(NetGroup));
            }
            netgroup_init(&groups[ngroups], net);
            idx = ngroups++;
        }
        netgroup_add(&groups[idx], pcb->tracks[i].a);
        netgroup_add(&groups[idx], pcb->tracks[i].b);
    }

    /* assign FpInsts to nearest track net, or group by ref */
    for (int i = 0; i < pcb->nfps; i++) {
        V2 pos = pcb->fps[i].pos;
        const char *best_net = NULL;
        float best_dist = 3.0f;
        for (int j = 0; j < pcb->ntracks; j++) {
            if (!pcb->tracks[j].net) continue;
            float da = v2_dist(pos, pcb->tracks[j].a);
            float db = v2_dist(pos, pcb->tracks[j].b);
            float d = da < db ? da : db;
            if (d < best_dist) { best_dist = d; best_net = pcb->tracks[j].net; }
        }
        if (!best_net) best_net = pcb->fps[i].ref ? pcb->fps[i].ref : "net";
        int idx = netgroup_find(groups, ngroups, best_net);
        if (idx < 0) {
            if (ngroups >= cap) {
                cap = cap ? cap * 2 : 8;
                groups = (NetGroup *)realloc(groups, (size_t)cap * sizeof(NetGroup));
            }
            netgroup_init(&groups[ngroups], best_net);
            idx = ngroups++;
        }
        netgroup_add(&groups[idx], pos);
    }

    *out_groups = groups;
    *out_ngroups = ngroups;
}

/* ---------- autoroute ---------- */

RouteResult autoroute_all(Pcb *pcb, float cell_size, int max_ripup_iterations) {
    RouteResult result = {0};
    if (!pcb) return result;

    pcb_bbox(pcb);
    if (!pcb->has_bbox) return result;

    NetGroup *groups = NULL;
    int ngroups = 0;
    extract_nets(pcb, &groups, &ngroups);

    if (ngroups == 0) {
        /* no nets with tracks; just try to chain FpInsts */
        if (pcb->nfps >= 2) {
            if (ngroups >= 0) {
                groups = (NetGroup *)realloc(groups, sizeof(NetGroup));
                netgroup_init(&groups[0], "net");
                for (int i = 0; i < pcb->nfps; i++)
                    netgroup_add(&groups[0], pcb->fps[i].pos);
                ngroups = 1;
            }
        }
    }

    result.total_nets = ngroups;
    int max_path_points = 256;

    RouteGrid grid;
    route_grid_init(&grid, pcb, cell_size);
    route_grid_mark_obstacles(&grid, pcb);

    for (int iter = 0; iter < max_ripup_iterations; iter++) {
        int routed_this_pass = 0;
        int failed_this_pass = 0;

        /* sort nets by bounding box diagonal ascending (short nets first) */
        for (int gi = 0; gi < ngroups; gi++) {
            if (groups[gi].npts < 2) continue;

            /* spanning tree: connect each point to the "routed set" */
            int *routed = (int *)calloc((size_t)groups[gi].npts, sizeof(int));
            if (!routed) continue;
            routed[0] = 1; /* first point is the seed */

            for (int pi = 1; pi < groups[gi].npts; pi++) {
                V2 target = groups[gi].pts[pi];
                int best_src = -1;
                float best_mh = 1e9f;

                /* find closest routed point by manhattan grid distance */
                for (int ri = 0; ri < pi; ri++) {
                    if (!routed[ri]) continue;
                    V2 src = groups[gi].pts[ri];
                    float dh = fabsf(target.x - src.x) + fabsf(target.y - src.y);
                    if (dh < best_mh) { best_mh = dh; best_src = ri; }
                }
                if (best_src < 0) continue;

                V2 src = groups[gi].pts[best_src];
                int sx, sy, tx, ty;
                world_to_grid(&grid, src, &sx, &sy);
                world_to_grid(&grid, target, &tx, &ty);

                int *rx = (int *)malloc((size_t)max_path_points * sizeof(int));
                int *ry = (int *)malloc((size_t)max_path_points * sizeof(int));
                int nsegs = 0;

                if (route_lee(&grid, sx, sy, tx, ty, rx, ry, &nsegs, max_path_points)) {
                    /* add track for each segment */
                    for (int si = 0; si < nsegs - 1; si++) {
                        V2 pa = grid_to_world(&grid, rx[si], ry[si]);
                        V2 pb = grid_to_world(&grid, rx[si + 1], ry[si + 1]);
                        float d = v2_dist(pa, pb);
                        if (d < 0.01f) continue;

                        Track t = { pa, pb, 0.3f, groups[gi].net };
                        pcb->tracks = (Track *)realloc(pcb->tracks,
                            (size_t)(pcb->ntracks + 1) * sizeof(Track));
                        pcb->tracks[pcb->ntracks++] = t;
                        result.total_length += d;

                        /* mark new track as obstacle */
                        route_grid_mark_track(&grid, &pcb->tracks[pcb->ntracks - 1]);
                    }
                    routed[pi] = 1;
                    routed_this_pass++;
                    result.routed_nets++;
                } else {
                    failed_this_pass++;
                }

                free(rx); free(ry);
            }
            free(routed);
        }

        result.iterations = iter + 1;

        if (failed_this_pass == 0) break;

        /* ripup: remove longest 10% of tracks */
        if (pcb->ntracks > 10) {
            int to_rip = pcb->ntracks / 10;
            if (to_rip < 1) to_rip = 1;

            /* find longest tracks by index */
            typedef struct { int idx; float len; } TrackLen;
            TrackLen *tl = (TrackLen *)malloc((size_t)pcb->ntracks * sizeof(TrackLen));
            if (tl) {
                for (int i = 0; i < pcb->ntracks; i++) {
                    tl[i].idx = i;
                    tl[i].len = v2_dist(pcb->tracks[i].a, pcb->tracks[i].b);
                }
                /* simple sort by length descending */
                for (int i = 0; i < pcb->ntracks; i++) {
                    for (int j = i + 1; j < pcb->ntracks; j++) {
                        if (tl[j].len > tl[i].len) {
                            TrackLen tmp = tl[i]; tl[i] = tl[j]; tl[j] = tmp;
                        }
                    }
                }
                /* remove the longest */
                for (int r = 0; r < to_rip && r < pcb->ntracks; r++) {
                    int idx = tl[r].idx;
                    memmove(&pcb->tracks[idx], &pcb->tracks[idx + 1],
                            (size_t)(pcb->ntracks - idx - 1) * sizeof(Track));
                    pcb->ntracks--;
                    /* adjust tl indices */
                    for (int k = 0; k < pcb->ntracks; k++) {
                        if (tl[k].idx > idx) tl[k].idx--;
                    }
                }
                free(tl);
            }
        }

        /* rebuild grid free areas */
        route_grid_clear_path(&grid);
        memset(grid.cells, 0, (size_t)grid.w * (size_t)grid.h * sizeof(CellState));
        route_grid_mark_obstacles(&grid, pcb);
    }

    result.failed_nets = result.total_nets - result.routed_nets;
    if (result.routed_nets < 0) result.routed_nets = 0;
    result.success = (result.failed_nets == 0) ? 1 : 0;

    for (int i = 0; i < ngroups; i++) netgroup_free(&groups[i]);
    free(groups);
    route_grid_free(&grid);
    return result;
}

void autoroute_progress_report(const RouteResult *r) {
    if (!r) return;
    printf("[autorouter] nets: %d total, %d routed, %d failed\n",
           r->total_nets, r->routed_nets, r->failed_nets);
    printf("[autorouter] iterations: %d, length: %.1f mm, vias: %d\n",
           r->iterations, r->total_length, r->vias_placed);
    printf("[autorouter] result: %s\n", r->success ? "SUCCESS" : "FAILED");
}
