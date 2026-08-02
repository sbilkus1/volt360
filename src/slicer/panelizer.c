#include "panelizer.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

static float pmin(float a, float b) { return a < b ? a : b; }
static float pmax(float a, float b) { return a > b ? a : b; }

void panel_config_default(PanelConfig *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    cfg->pcb_margin = 2.0f;
    cfg->frame_margin = 10.0f;
    cfg->vscore = 1;
    cfg->add_fiducials = 1;
    cfg->add_tooling_holes = 1;
    cfg->tab_width = 3.0f;
    cfg->mouse_bite_dia = 0.5f;
    cfg->mouse_bite_spacing = 1.0f;
    cfg->panel_cx = 0.0f;
    cfg->panel_cy = 0.0f;
}

void panel_result_free(PanelResult *r) {
    free(r->positions);
    free(r->vscore_lines);
    memset(r, 0, sizeof(*r));
}

int panelize_pcb(V2 pcb_min, V2 pcb_max, int qty, PanelConfig *cfg, PanelResult *out, int max_panels) {
    float pcb_w, pcb_h, step_x, step_y;
    int cols, rows;
    int ix, iy;
    int idx;

    if (qty <= 0 || max_panels <= 0 || !cfg || !out) return 0;
    if (qty > max_panels) qty = max_panels;

    pcb_w = pcb_max.x - pcb_min.x + cfg->pcb_margin * 2.0f;
    pcb_h = pcb_max.y - pcb_min.y + cfg->pcb_margin * 2.0f;
    step_x = pcb_w + cfg->pcb_margin;
    step_y = pcb_h + cfg->pcb_margin;

    {
        float aspect = step_x / step_y;
        cols = (int)ceil(sqrt((double)qty * aspect));
        if (cols < 1) cols = 1;
        rows = (qty + cols - 1) / cols;
        if (rows < 1) rows = 1;
        if (cfg->panel_cx > 0.0f && step_x > 0.0f) {
            cols = (int)pmax(1.0f, (cfg->panel_cx - cfg->frame_margin * 2.0f) / step_x);
        }
                if (cfg->panel_cy > 0.0f && step_y > 0.0f) {
            rows = (int)pmax(1.0f, (cfg->panel_cy - cfg->frame_margin * 2.0f) / step_y);
        }


        out->positions = (PanelPos *)calloc((size_t)max_panels, sizeof(PanelPos));
        if (!out->positions) return 0;
        out->npositions = 0;

        idx = 0;
        for (iy = 0; iy < rows && idx < qty; iy++) {
            for (ix = 0; ix < cols && idx < qty; ix++) {
                out->positions[idx].x = cfg->frame_margin + (float)ix * step_x - pcb_min.x + cfg->pcb_margin;
                out->positions[idx].y = cfg->frame_margin + (float)iy * step_y - pcb_min.y + cfg->pcb_margin;
                out->positions[idx].rotation = 0.0f;
                idx++;
            }
        }
        out->npositions = idx;

        out->panel_w = cfg->frame_margin * 2.0f + (float)cols * step_x - cfg->pcb_margin;
        out->panel_h = cfg->frame_margin * 2.0f + (float)rows * step_y - cfg->pcb_margin;

        if (cfg->vscore) {
            int max_lines = out->npositions * 2;
            out->vscore_lines = (VScoreLine *)calloc((size_t)max_lines, sizeof(VScoreLine));
            if (out->vscore_lines) {
                float half_gap = cfg->pcb_margin * 0.5f;
                float top, bot;
                out->nvscores = 0;
                for (ix = 1; ix < cols; ix++) {
                    float vx = cfg->frame_margin + (float)ix * step_x - half_gap - pcb_min.x + cfg->pcb_margin;
                    top = cfg->frame_margin - pcb_min.y + cfg->pcb_margin;
                    bot = top + (float)rows * step_y - cfg->pcb_margin;
                    if (out->nvscores < max_lines) {
                        out->vscore_lines[out->nvscores].x1 = vx;
                        out->vscore_lines[out->nvscores].y1 = top;
                        out->vscore_lines[out->nvscores].x2 = vx;
                        out->vscore_lines[out->nvscores].y2 = bot;
                        out->nvscores++;
                    }
                }
                for (iy = 1; iy < rows; iy++) {
                    float vy = cfg->frame_margin + (float)iy * step_y - half_gap - pcb_min.y + cfg->pcb_margin;
                    float lft = cfg->frame_margin - pcb_min.x + cfg->pcb_margin;
                    float rgt = lft + (float)cols * step_x - cfg->pcb_margin;
                    if (out->nvscores < max_lines) {
                        out->vscore_lines[out->nvscores].x1 = lft;
                        out->vscore_lines[out->nvscores].y1 = vy;
                        out->vscore_lines[out->nvscores].x2 = rgt;
                        out->vscore_lines[out->nvscores].y2 = vy;
                        out->nvscores++;
                    }
                }
            }
        }

        return out->npositions;
    }
}

void panel_bbox(PanelPos *positions, int n, float *w, float *h) {
    int i;
    float mnx, mny, mxx, mxy;
    if (n <= 0) { *w = 0; *h = 0; return; }
    mnx = positions[0].x; mny = positions[0].y;
    mxx = positions[0].x; mxy = positions[0].y;
    for (i = 1; i < n; i++) {
        if (positions[i].x < mnx) mnx = positions[i].x;
        if (positions[i].y < mny) mny = positions[i].y;
        if (positions[i].x > mxx) mxx = positions[i].x;
        if (positions[i].y > mxy) mxy = positions[i].y;
    }
    *w = mxx - mnx;
    *h = mxy - mny;
}

int panel_vscore_lines(PanelPos *positions, int n, float pcb_w, float pcb_h, float margin, VScoreLine *lines, int max_lines) {
    int i;
    (void)pcb_w; (void)pcb_h;
    if (!positions || n < 2 || !lines || max_lines <= 0) return 0;
    for (i = 0; i < n - 1; i++) {
        if (i >= max_lines) break;
        lines[i].x1 = positions[i].x;
        lines[i].y1 = positions[i].y;
        lines[i].x2 = positions[i + 1].x;
        lines[i].y2 = positions[i + 1].y;
    }
    return i;
}
