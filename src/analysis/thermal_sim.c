#include "thermal_sim.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DEFAULT_AMBIENT 25.0f
#define DEFAULT_CONVECTION 25.0f
#define DEFAULT_COPPER_OZ 1.0f
#define COPPER_K 400.0f
#define FR4_K 0.3f
#define COPPER_FACTOR_BASE (COPPER_K / FR4_K)

/* ---- internal helpers ---- */

static Color temp_to_color(float t) {
    /* map temperature to color stops:
       25C = BLUE, 40C = CYAN, 55C = GREEN, 70C = YELLOW, 85C = ORANGE, 100C+ = RED */
    float clamp_t = t < 25.0f ? 25.0f : (t > 100.0f ? 100.0f : t);
    float r, g, b, a = 255;

    if (clamp_t <= 40.0f) {
        float f = (clamp_t - 25.0f) / 15.0f;
        r = 0;
        g = f * 255.0f;
        b = 255.0f;
    } else if (clamp_t <= 55.0f) {
        float f = (clamp_t - 40.0f) / 15.0f;
        r = 0;
        g = 255.0f;
        b = 255.0f * (1.0f - f);
    } else if (clamp_t <= 70.0f) {
        float f = (clamp_t - 55.0f) / 15.0f;
        r = 255.0f * f;
        g = 255.0f;
        b = 0;
    } else if (clamp_t <= 85.0f) {
        float f = (clamp_t - 70.0f) / 15.0f;
        r = 255.0f;
        g = 255.0f * (1.0f - f);
        b = 0;
    } else {
        float f = (clamp_t - 85.0f) / 15.0f;
        r = 255.0f;
        g = 0;
        b = 0;
    }

    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;

    Color c;
    c.r = (unsigned char)r;
    c.g = (unsigned char)g;
    c.b = (unsigned char)b;
    c.a = (unsigned char)a;
    return c;
}

static int grid_idx(const ThermalGrid *tg, int x, int y) {
    return y * tg->w + x;
}

static int in_bounds(const ThermalGrid *tg, int x, int y) {
    return x >= 0 && x < tg->w && y >= 0 && y < tg->h;
}

/* ---- public API ---- */

void thermal_grid_init(ThermalGrid *tg, const Pcb *pcb, float cell_size) {
    int i, n;
    memset(tg, 0, sizeof(*tg));

    if (!pcb || !pcb->has_bbox) {
        tg->w = 10; tg->h = 10;
        tg->cell_size = cell_size > 0 ? cell_size : 1.0f;
        n = tg->w * tg->h;
        tg->grid = (float *)calloc((size_t)n, sizeof(float));
        tg->copper_factor = (float *)calloc((size_t)n, sizeof(float));
        tg->heat_source = (float *)calloc((size_t)n, sizeof(float));
        tg->source_temp = (float *)calloc((size_t)n, sizeof(float));
        tg->render_colors = (float *)calloc((size_t)n * 3, sizeof(float));
        for (i = 0; i < n; i++) tg->grid[i] = DEFAULT_AMBIENT;
        for (i = 0; i < n; i++) tg->copper_factor[i] = 1.0f;
        tg->ambient = DEFAULT_AMBIENT;
        tg->convection = DEFAULT_CONVECTION;
        tg->copper_weight = DEFAULT_COPPER_OZ;
        return;
    }

    tg->cell_size = cell_size > 0 ? cell_size : 1.0f;
    tg->w = (int)ceilf((pcb->bmax.x - pcb->bmin.x) / tg->cell_size) + 2;
    tg->h = (int)ceilf((pcb->bmax.y - pcb->bmin.y) / tg->cell_size) + 2;
    if (tg->w < 5) tg->w = 5;
    if (tg->h < 5) tg->h = 5;

    n = tg->w * tg->h;
    tg->grid = (float *)calloc((size_t)n, sizeof(float));
    tg->copper_factor = (float *)calloc((size_t)n, sizeof(float));
    tg->heat_source = (float *)calloc((size_t)n, sizeof(float));
    tg->source_temp = (float *)calloc((size_t)n, sizeof(float));
    tg->render_colors = (float *)calloc((size_t)n * 3, sizeof(float));

    for (i = 0; i < n; i++) tg->grid[i] = DEFAULT_AMBIENT;
    for (i = 0; i < n; i++) tg->copper_factor[i] = 1.0f;

    tg->ambient = DEFAULT_AMBIENT;
    tg->convection = DEFAULT_CONVECTION;
    tg->copper_weight = DEFAULT_COPPER_OZ;
    tg->min_temp = DEFAULT_AMBIENT;
    tg->max_temp = DEFAULT_AMBIENT;
    tg->hotspot = v2(0, 0);
}

void thermal_grid_free(ThermalGrid *tg) {
    if (!tg) return;
    free(tg->grid);
    free(tg->copper_factor);
    free(tg->heat_source);
    free(tg->source_temp);
    free(tg->render_colors);
    memset(tg, 0, sizeof(*tg));
}

void thermal_set_component(ThermalGrid *tg, V2 pos, float power, float theta_ja) {
    float r = 3.0f;
    int cx = (int)(pos.x / tg->cell_size);
    int cy = (int)(pos.y / tg->cell_size);
    int rx = (int)(r / tg->cell_size) + 1;
    int ry = (int)(r / tg->cell_size) + 1;
    int dx, dy;
    float source_temp = tg->ambient + power * theta_ja;

    for (dy = -ry; dy <= ry; dy++) {
        for (dx = -rx; dx <= rx; dx++) {
            int gx = cx + dx;
            int gy = cy + dy;
            if (!in_bounds(tg, gx, gy)) continue;
            float dist = sqrtf((float)(dx*dx + dy*dy)) * tg->cell_size;
            if (dist <= r) {
                int idx = grid_idx(tg, gx, gy);
                tg->heat_source[idx] = 1.0f;
                tg->source_temp[idx] = source_temp;
            }
        }
    }
}

void thermal_apply_copper(ThermalGrid *tg, const Pcb *pcb) {
    int i;

    if (!pcb) return;

    /* process tracks */
    for (i = 0; i < pcb->ntracks; i++) {
        Track *t = &pcb->tracks[i];
        float len = v2_dist(t->a, t->b);
        int steps = (int)(len / tg->cell_size) + 1;
        int s;
        for (s = 0; s <= steps; s++) {
            float f = steps > 0 ? (float)s / (float)steps : 0;
            V2 pt = v2_add(t->a, v2_scale(v2_sub(t->b, t->a), f));
            int gx = (int)(pt.x / tg->cell_size);
            int gy = (int)(pt.y / tg->cell_size);
            /* widen by track thickness */
            int hw = (int)((t->width * 0.5f) / tg->cell_size) + 1;
            int dx, dy;
            for (dy = -hw; dy <= hw; dy++) {
                for (dx = -hw; dx <= hw; dx++) {
                    int cx = gx + dx, cy = gy + dy;
                    if (!in_bounds(tg, cx, cy)) continue;
                    int idx = grid_idx(tg, cx, cy);
                    tg->copper_factor[idx] = COPPER_FACTOR_BASE;
                }
            }
        }
    }

    /* process zones */
    for (i = 0; i < pcb->nzones; i++) {
        Zone *z = &pcb->zones[i];
        if (z->npts < 3) continue;
        /* find bbox of zone */
        float zminx = z->pts[0].x, zmaxx = z->pts[0].x;
        float zminy = z->pts[0].y, zmaxy = z->pts[0].y;
        int j;
        for (j = 1; j < z->npts; j++) {
            if (z->pts[j].x < zminx) zminx = z->pts[j].x;
            if (z->pts[j].x > zmaxx) zmaxx = z->pts[j].x;
            if (z->pts[j].y < zminy) zminy = z->pts[j].y;
            if (z->pts[j].y > zmaxy) zmaxy = z->pts[j].y;
        }
        int gx0 = (int)(zminx / tg->cell_size);
        int gy0 = (int)(zminy / tg->cell_size);
        int gx1 = (int)(zmaxx / tg->cell_size) + 1;
        int gy1 = (int)(zmaxy / tg->cell_size) + 1;
        int gx, gy;
        for (gy = gy0; gy <= gy1; gy++) {
            for (gx = gx0; gx <= gx1; gx++) {
                if (!in_bounds(tg, gx, gy)) continue;
                V2 cell_center = v2((float)gx * tg->cell_size, (float)gy * tg->cell_size);
                /* point-in-polygon test for the zone */
                int inside = 0;
                int k, l;
                for (k = 0, l = z->npts - 1; k < z->npts; l = k++) {
                    V2 a = z->pts[l], b = z->pts[k];
                    if (((a.y > cell_center.y) != (b.y > cell_center.y)) &&
                        (cell_center.x < (b.x - a.x) * (cell_center.y - a.y) / (b.y - a.y) + a.x))
                        inside = !inside;
                }
                if (inside) {
                    int idx = grid_idx(tg, gx, gy);
                    tg->copper_factor[idx] = COPPER_FACTOR_BASE;
                }
            }
        }
    }
}

ThermalResult thermal_solve(ThermalGrid *tg, int max_iter, float tolerance) {
    ThermalResult result;
    int n = tg->w * tg->h;
    float *new_grid;
    int iter;
    float residual;
    float relaxation = 0.15f;
    float convection_coeff = tg->convection * 1e-6f;

    memset(&result, 0, sizeof(result));
    result.grid = *tg;

    if (n == 0 || max_iter <= 0) {
        result.success = 0;
        return result;
    }

    new_grid = (float *)calloc((size_t)n, sizeof(float));
    if (!new_grid) {
        result.success = 0;
        return result;
    }

    memcpy(new_grid, tg->grid, (size_t)n * sizeof(float));

    for (iter = 0; iter < max_iter; iter++) {
        residual = 0.0f;
        int x, y;

        for (y = 0; y < tg->h; y++) {
            for (x = 0; x < tg->w; x++) {
                int idx = grid_idx(tg, x, y);

                /* boundary cells: fixed at ambient */
                if (x == 0 || x == tg->w - 1 || y == 0 || y == tg->h - 1) {
                    new_grid[idx] = tg->ambient;
                    continue;
                }

                /* heat source cells: fixed temperature */
                if (tg->heat_source[idx] > 0.5f) {
                    new_grid[idx] = tg->source_temp[idx];
                    continue;
                }

                /* compute average of 4 neighbors weighted by copper factor */
                float sum_temp = 0.0f;
                float sum_weight = 0.0f;
                int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
                int d;

                for (d = 0; d < 4; d++) {
                    int nx = x + dirs[d][0];
                    int ny = y + dirs[d][1];
                    if (!in_bounds(tg, nx, ny)) continue;
                    int nidx = grid_idx(tg, nx, ny);
                    float w = 0.5f * (tg->copper_factor[idx] + tg->copper_factor[nidx]);
                    sum_temp += tg->grid[nidx] * w;
                    sum_weight += w;
                }

                if (sum_weight > 0.0f) {
                    float avg_neighbor = sum_temp / sum_weight;
                    float conduction = (avg_neighbor - tg->grid[idx]) *
                                       tg->copper_factor[idx] * relaxation;
                    float convection = (tg->grid[idx] - tg->ambient) *
                                       convection_coeff * relaxation;
                    float new_val = tg->grid[idx] + conduction - convection;

                    /* clamp to reasonable range */
                    if (new_val < -50.0f) new_val = -50.0f;
                    if (new_val > 300.0f) new_val = 300.0f;

                    new_grid[idx] = new_val;

                    float diff = fabsf(new_val - tg->grid[idx]);
                    if (diff > residual) residual = diff;
                }
            }
        }

        /* swap grids for next iteration */
        {
            float *tmp = tg->grid;
            tg->grid = new_grid;
            new_grid = tmp;
        }

        if (residual < tolerance) break;
    }

    /* find max/min temperatures and hotspot */
    tg->min_temp = 1e9f;
    tg->max_temp = -1e9f;
    {
        int i;
        for (i = 0; i < n; i++) {
            if (tg->grid[i] < tg->min_temp) tg->min_temp = tg->grid[i];
            if (tg->grid[i] > tg->max_temp) {
                tg->max_temp = tg->grid[i];
                tg->hotspot.x = (float)(i % tg->w) * tg->cell_size;
                tg->hotspot.y = (float)(i / tg->w) * tg->cell_size;
            }
        }
    }

    free(new_grid);
    result.grid = *tg;
    result.iterations = iter;
    result.convergence = residual;
    result.success = 1;
    return result;
}

void thermal_compute_colors(ThermalGrid *tg) {
    int i, n = tg->w * tg->h;
    for (i = 0; i < n; i++) {
        Color c = temp_to_color(tg->grid[i]);
        tg->render_colors[i * 3 + 0] = (float)c.r / 255.0f;
        tg->render_colors[i * 3 + 1] = (float)c.g / 255.0f;
        tg->render_colors[i * 3 + 2] = (float)c.b / 255.0f;
    }
}

Color thermal_get_color(const ThermalGrid *tg, float temp) {
    (void)tg;
    return temp_to_color(temp);
}

void thermal_render(const ThermalGrid *tg, Rectangle viewport, Vector2 pan, float zoom) {
    float origin_x = pan.x;
    float origin_y = pan.y;
    int step = 1;
    int x, y;
    int max_cells_to_draw = 2000;
    int cells_per_row = (tg->w + step - 1) / step;
    int cells_per_col = (tg->h + step - 1) / step;
    int total_cells = cells_per_row * cells_per_col;

    if (tg->w <= 0 || tg->h <= 0) return;

    /* downsample if too many cells */
    while (total_cells > max_cells_to_draw) {
        step++;
        cells_per_row = (tg->w + step - 1) / step;
        cells_per_col = (tg->h + step - 1) / step;
        total_cells = cells_per_row * cells_per_col;
    }

    for (y = 0; y < tg->h; y += step) {
        for (x = 0; x < tg->w; x += step) {
            int idx = grid_idx(tg, x, y);
            Color c = temp_to_color(tg->grid[idx]);
            float world_x = (float)x * tg->cell_size * zoom + origin_x + viewport.x;
            float world_y = (float)y * tg->cell_size * zoom + origin_y + viewport.y;
            float size = tg->cell_size * zoom * (float)step;
            if (size < 1.0f) size = 1.0f;
            DrawRectangle((int)world_x, (int)world_y, (int)(size + 0.5f), (int)(size + 0.5f), c);
        }
    }

    /* draw isothermal contour lines at 10C intervals */
    {
        float levels[] = {30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
        int nl = 8;
        Color contour_color = {220, 220, 220, 60};
        int li;
        for (li = 0; li < nl; li++) {
            float level = levels[li];
            for (y = 0; y < tg->h - 1; y++) {
                for (x = 0; x < tg->w - 1; x++) {
                    int i00 = grid_idx(tg, x, y);
                    int i10 = grid_idx(tg, x+1, y);
                    int i01 = grid_idx(tg, x, y+1);
                    float t00 = tg->grid[i00], t10 = tg->grid[i10], t01 = tg->grid[i01];

                    /* horizontal crossing */
                    if ((t00 < level && t10 >= level) || (t00 >= level && t10 < level)) {
                        float frac = (level - t00) / (t10 - t00 + 0.0001f);
                        float px = ((float)x + frac) * tg->cell_size * zoom + origin_x + viewport.x;
                        float py = (float)y * tg->cell_size * zoom + origin_y + viewport.y;
                        DrawPixel((int)px, (int)py, contour_color);
                    }
                    /* vertical crossing */
                    if ((t00 < level && t01 >= level) || (t00 >= level && t01 < level)) {
                        float frac = (level - t00) / (t01 - t00 + 0.0001f);
                        float px = (float)x * tg->cell_size * zoom + origin_x + viewport.x;
                        float py = ((float)y + frac) * tg->cell_size * zoom + origin_y + viewport.y;
                        DrawPixel((int)px, (int)py, contour_color);
                    }
                }
            }
        }
    }

    /* draw hotspot marker (red X) */
    {
        float hx = tg->hotspot.x * zoom + origin_x + viewport.x;
        float hy = tg->hotspot.y * zoom + origin_y + viewport.y;
        int hs = 6;
        Color red = {255, 0, 0, 255};
        DrawLine((int)(hx - hs), (int)(hy - hs), (int)(hx + hs), (int)(hy + hs), red);
        DrawLine((int)(hx + hs), (int)(hy - hs), (int)(hx - hs), (int)(hy + hs), red);
    }
}

void thermal_render_legend(Rectangle area) {
    int bar_width = 20;
    int bar_height = (int)area.height - 40;
    int bar_x = (int)area.x + (int)area.width - bar_width - 10;
    int bar_y = (int)area.y + 20;
    int y;
    int tick_count = 9;
    float tick_temps[] = {25.0f, 35.0f, 45.0f, 55.0f, 65.0f, 75.0f, 85.0f, 95.0f, 100.0f};
    int i;

    /* gradient bar from blue (bottom, 25C) to red (top, 100C) */
    for (y = 0; y < bar_height; y++) {
        float frac = (float)y / (float)bar_height;
        float temp = 100.0f - frac * 75.0f;
        Color c = temp_to_color(temp);
        DrawRectangle(bar_x, bar_y + y, bar_width, 1, c);
    }

    /* border */
    DrawRectangleLines(bar_x, bar_y, bar_width, bar_height, BLACK);

    /* tick marks and labels */
    for (i = 0; i < tick_count; i++) {
        float temp = tick_temps[i];
        float frac = (100.0f - temp) / 75.0f;
        if (frac < 0.0f) frac = 0.0f;
        if (frac > 1.0f) frac = 1.0f;
        int ty = bar_y + (int)(frac * (float)bar_height);
        DrawLine(bar_x - 5, ty, bar_x + bar_width + 5, ty, BLACK);
        {
            char label[16];
            snprintf(label, sizeof(label), "%.0fC", temp);
            DrawText(label, bar_x + bar_width + 8, ty - 7, 10, BLACK);
        }
    }
}

char *thermal_sim_report(const ThermalGrid *tg) {
    char buf[4096];
    int off = 0;
    float sum = 0.0f;
    int n = tg->w * tg->h;
    int i;
    int count = 0;

    if (!tg || n == 0) return str_dup("no thermal data");

    for (i = 0; i < n; i++) {
        sum += tg->grid[i];
        count++;
    }
    float avg = count > 0 ? sum / (float)count : tg->ambient;

    off += snprintf(buf + off, sizeof(buf) - off,
        "=== THERMAL SIMULATION ===\n"
        "Grid: %dx%d cells (%.1f mm/cell)\n"
        "Ambient: %.1f C, Convection: %.0f W/m2K\n"
        "Max: %.1f C at (%.0f, %.0f) mm\n"
        "Min: %.1f C\n"
        "Avg: %.1f C\n"
        "=================================\n",
        tg->w, tg->h, tg->cell_size,
        tg->ambient, tg->convection,
        tg->max_temp, tg->hotspot.x, tg->hotspot.y,
        tg->min_temp, avg);

    if (tg->max_temp > 100.0f) {
        off += snprintf(buf + off, sizeof(buf) - off,
            "WARNING: Hotspot exceeds 100C! Component at risk.\n");
    } else if (tg->max_temp > 85.0f) {
        off += snprintf(buf + off, sizeof(buf) - off,
            "CAUTION: Hotspot exceeds 85C. Consider thermal management.\n");
    } else {
        off += snprintf(buf + off, sizeof(buf) - off,
            "All temperatures within safe limits.\n");
    }

    return str_dup(buf);
}
