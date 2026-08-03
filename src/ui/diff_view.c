#include "diff_view.h"
#include <stdio.h>

#define BG_COLOR    (Color){30,30,30,255}
#define GREEN_DIFF  (Color){80,200,80,255}
#define RED_DIFF    (Color){220,80,80,255}
#define YELLOW_DIFF (Color){220,200,60,255}
#define GRAY_DIFF   (Color){120,120,120,255}
#define WHITE_HL    (Color){255,255,255,220}

void diff_view_init(DiffView *dv) {
    memset(dv, 0, sizeof(*dv));
    dv->layer = DIFF_LAYER_ALL;
    dv->highlight_track = -1;
}

static int track_similar(const Track *a, const Track *b) {
    float dx = a->a.x - b->a.x;
    float dy = a->a.y - b->a.y;
    float d1 = dx * dx + dy * dy;
    dx = a->b.x - b->b.x;
    dy = a->b.y - b->b.y;
    float d2 = dx * dx + dy * dy;
    float max_d = (d1 > d2) ? d1 : d2;
    return max_d < 100.0f;
}

static int via_similar(const Via *a, const Via *b) {
    float dx = a->pos.x - b->pos.x;
    float dy = a->pos.y - b->pos.y;
    float d = dx * dx + dy * dy;
    return d < 25.0f;
}

static int net_same(const Track *a, const Track *b) {
    if (!a->net && !b->net) return 1;
    if (!a->net || !b->net) return 0;
    return strcmp(a->net, b->net) == 0;
}

static int via_net_same(const Via *a, const Via *b) {
    if (!a->net && !b->net) return 1;
    if (!a->net || !b->net) return 0;
    return strcmp(a->net, b->net) == 0;
}

void diff_view_compare(DiffView *dv, const Pcb *a, const Pcb *b) {
    int i, j;
    free(dv->track_diffs); dv->track_diffs = NULL; dv->n_track_diffs = 0;
    free(dv->via_diffs); dv->via_diffs = NULL; dv->n_via_diffs = 0;
    dv->added_count = 0;
    dv->removed_count = 0;
    dv->modified_count = 0;
    dv->highlight_track = -1;
    dv->total_diffs = 0;

    dv->pcb_a = NULL;
    dv->pcb_b = NULL;

    if (!a && !b) return;
    if (!a) { a = b; b = NULL; }
    if (!b) { dv->pcb_a = NULL; dv->pcb_b = NULL; return; }

    dv->pcb_a = (Pcb *)a;
    dv->pcb_b = (Pcb *)b;

    /* compare tracks */
    {
        int *used_b = (int *)calloc((size_t)(b->ntracks > 0 ? b->ntracks : 1), sizeof(int));
        int cap = a->ntracks + b->ntracks + 8;
        dv->track_diffs = (TrackDiff *)malloc((size_t)cap * sizeof(TrackDiff));

        for (i = 0; i < a->ntracks; i++) {
            int matched = -1;
            for (j = 0; j < b->ntracks; j++) {
                if (used_b[j]) continue;
                if (net_same(&a->tracks[i], &b->tracks[j]) && track_similar(&a->tracks[i], &b->tracks[j])) {
                    matched = j;
                    break;
                }
            }
            if (matched >= 0) {
                used_b[matched] = 1;
                dv->track_diffs[dv->n_track_diffs].a_idx = i;
                dv->track_diffs[dv->n_track_diffs].b_idx = matched;
                dv->track_diffs[dv->n_track_diffs].status = DIFF_UNCHANGED;
                dv->n_track_diffs++;
            } else {
                /* check if same net but different position */
                int same_net = 0;
                for (j = 0; j < b->ntracks; j++) {
                    if (!used_b[j] && net_same(&a->tracks[i], &b->tracks[j])) {
                        same_net = 1;
                        used_b[j] = 1;
                        dv->track_diffs[dv->n_track_diffs].a_idx = i;
                        dv->track_diffs[dv->n_track_diffs].b_idx = j;
                        dv->track_diffs[dv->n_track_diffs].status = DIFF_MODIFIED;
                        dv->n_track_diffs++;
                        dv->modified_count++;
                        dv->total_diffs++;
                        break;
                    }
                }
                if (!same_net) {
                    dv->track_diffs[dv->n_track_diffs].a_idx = i;
                    dv->track_diffs[dv->n_track_diffs].b_idx = -1;
                    dv->track_diffs[dv->n_track_diffs].status = DIFF_REMOVED;
                    dv->n_track_diffs++;
                    dv->removed_count++;
                    dv->total_diffs++;
                }
            }
        }
        for (j = 0; j < b->ntracks; j++) {
            if (!used_b[j]) {
                dv->track_diffs[dv->n_track_diffs].a_idx = -1;
                dv->track_diffs[dv->n_track_diffs].b_idx = j;
                dv->track_diffs[dv->n_track_diffs].status = DIFF_ADDED;
                dv->n_track_diffs++;
                dv->added_count++;
                dv->total_diffs++;
            }
        }
        free(used_b);
    }

    /* compare vias */
    {
        int *used_b = (int *)calloc((size_t)(b->nvias > 0 ? b->nvias : 1), sizeof(int));
        int cap = a->nvias + b->nvias + 8;
        dv->via_diffs = (ViaDiff *)malloc((size_t)cap * sizeof(ViaDiff));

        for (i = 0; i < a->nvias; i++) {
            int matched = -1;
            for (j = 0; j < b->nvias; j++) {
                if (used_b[j]) continue;
                if (via_net_same(&a->vias[i], &b->vias[j]) && via_similar(&a->vias[i], &b->vias[j])) {
                    matched = j;
                    break;
                }
            }
            if (matched >= 0) {
                used_b[matched] = 1;
                dv->via_diffs[dv->n_via_diffs].a_idx = i;
                dv->via_diffs[dv->n_via_diffs].b_idx = matched;
                dv->via_diffs[dv->n_via_diffs].status = DIFF_UNCHANGED;
                dv->n_via_diffs++;
            } else {
                int same_net = 0;
                for (j = 0; j < b->nvias; j++) {
                    if (!used_b[j] && via_net_same(&a->vias[i], &b->vias[j])) {
                        same_net = 1;
                        used_b[j] = 1;
                        dv->via_diffs[dv->n_via_diffs].a_idx = i;
                        dv->via_diffs[dv->n_via_diffs].b_idx = j;
                        dv->via_diffs[dv->n_via_diffs].status = DIFF_MODIFIED;
                        dv->n_via_diffs++;
                        dv->modified_count++;
                        dv->total_diffs++;
                        break;
                    }
                }
                if (!same_net) {
                    dv->via_diffs[dv->n_via_diffs].a_idx = i;
                    dv->via_diffs[dv->n_via_diffs].b_idx = -1;
                    dv->via_diffs[dv->n_via_diffs].status = DIFF_REMOVED;
                    dv->n_via_diffs++;
                    dv->removed_count++;
                    dv->total_diffs++;
                }
            }
        }
        for (j = 0; j < b->nvias; j++) {
            if (!used_b[j]) {
                dv->via_diffs[dv->n_via_diffs].a_idx = -1;
                dv->via_diffs[dv->n_via_diffs].b_idx = j;
                dv->via_diffs[dv->n_via_diffs].status = DIFF_ADDED;
                dv->n_via_diffs++;
                dv->added_count++;
                dv->total_diffs++;
            }
        }
        free(used_b);
    }

    if (dv->total_diffs > 0) dv->highlight_track = 0;
}

static Color status_color(DiffStatus s) {
    switch (s) {
        case DIFF_ADDED:     return GREEN_DIFF;
        case DIFF_REMOVED:   return RED_DIFF;
        case DIFF_MODIFIED:  return YELLOW_DIFF;
        default:             return GRAY_DIFF;
    }
}

static void draw_pcb_mini(Pcb *pcb, int x, int y, int w, int h,
    TrackDiff *td, int ntd, ViaDiff *vd, int nvd, int highlight, int side_a) {
    int i;
    Pcb *p = (Pcb *)pcb;
    if (!p) {
        DrawText("(no PCB)", x + 4, y + 4, 12, GRAY);
        return;
    }

    /* compute bounding box */
    float min_x = 0, max_x = 100, min_y = 0, max_y = 100;
    if (p->ntracks > 0 || p->nvias > 0) {
        min_x = 1e9f; max_x = -1e9f; min_y = 1e9f; max_y = -1e9f;
        for (i = 0; i < p->ntracks; i++) {
            if (p->tracks[i].a.x < min_x) min_x = p->tracks[i].a.x;
            if (p->tracks[i].a.x > max_x) max_x = p->tracks[i].a.x;
            if (p->tracks[i].a.y < min_y) min_y = p->tracks[i].a.y;
            if (p->tracks[i].a.y > max_y) max_y = p->tracks[i].a.y;
            if (p->tracks[i].b.x < min_x) min_x = p->tracks[i].b.x;
            if (p->tracks[i].b.x > max_x) max_x = p->tracks[i].b.x;
            if (p->tracks[i].b.y < min_y) min_y = p->tracks[i].b.y;
            if (p->tracks[i].b.y > max_y) max_y = p->tracks[i].b.y;
        }
        for (i = 0; i < p->nvias; i++) {
            if (p->vias[i].pos.x < min_x) min_x = p->vias[i].pos.x;
            if (p->vias[i].pos.x > max_x) max_x = p->vias[i].pos.x;
            if (p->vias[i].pos.y < min_y) min_y = p->vias[i].pos.y;
            if (p->vias[i].pos.y > max_y) max_y = p->vias[i].pos.y;
        }
        if (min_x >= max_x) { min_x -= 10; max_x += 10; }
        if (min_y >= max_y) { min_y -= 10; max_y += 10; }
    }

    float world_w = max_x - min_x;
    float world_h = max_y - min_y;
    float pad = 20.0f;
    float scale_x = ((float)w - pad * 2) / (world_w > 0 ? world_w : 1);
    float scale_y = ((float)h - pad * 2) / (world_h > 0 ? world_h : 1);
    float scale = scale_x < scale_y ? scale_x : scale_y;

    /* board outline */
    DrawRectangleLines(x, y, w, h, (Color){60,60,66,255});
    DrawText(side_a ? "PCB A" : "PCB B", x + 4, y + 2, 10, (Color){150,150,160,255});

    /* draw tracks with color coding */
    for (i = 0; i < ntd; i++) {
        int ti = (side_a ? td[i].a_idx : td[i].b_idx);
        if (ti < 0) continue;
        if (ti >= p->ntracks) continue;
        Track *t = &p->tracks[ti];
        Color col = status_color(td[i].status);
        int is_hl = (i == highlight);
        if (is_hl) {
            int sx1 = x + (int)pad + (int)((t->a.x - min_x) * scale);
            int sy1 = y + (int)pad + (int)((t->a.y - min_y) * scale);
            int sx2 = x + (int)pad + (int)((t->b.x - min_x) * scale);
            int sy2 = y + (int)pad + (int)((t->b.y - min_y) * scale);
            DrawLine(sx1 - 1, sy1, sx2 - 1, sy2, WHITE_HL);
            DrawLine(sx1 + 1, sy1, sx2 + 1, sy2, WHITE_HL);
            DrawLine(sx1, sy1 - 1, sx2, sy2 - 1, WHITE_HL);
            DrawLine(sx1, sy1 + 1, sx2, sy2 + 1, WHITE_HL);
        }
        int sx1 = x + (int)pad + (int)((t->a.x - min_x) * scale);
        int sy1 = y + (int)pad + (int)((t->a.y - min_y) * scale);
        int sx2 = x + (int)pad + (int)((t->b.x - min_x) * scale);
        int sy2 = y + (int)pad + (int)((t->b.y - min_y) * scale);
        DrawLine(sx1, sy1, sx2, sy2, col);
    }

    /* draw vias */
    for (i = 0; i < nvd; i++) {
        int vi = (side_a ? vd[i].a_idx : vd[i].b_idx);
        if (vi < 0) continue;
        Color col = status_color(vd[i].status);
        int via_hl_idx = ntd + i;
        int is_hl = (via_hl_idx == highlight);
        int sx = x + (int)pad + (int)((p->vias[vi].pos.x - min_x) * scale);
        int sy = y + (int)pad + (int)((p->vias[vi].pos.y - min_y) * scale);
        DrawCircle(sx, sy, 4, col);
        if (is_hl) DrawCircleLines(sx, sy, 6, WHITE_HL);
    }
}

void diff_view_render(DiffView *dv) {
    float vx = dv->viewport.x;
    float vy = dv->viewport.y;
    float vw = dv->viewport.width;
    float vh = dv->viewport.height;

    DrawRectangle((int)vx, (int)vy, (int)vw, (int)vh, BG_COLOR);

    /* legend at top */
    int legend_y = (int)vy + 2;
    int legend_h = 18;
    DrawRectangle((int)vx + 4, legend_y, 12, legend_h, GREEN_DIFF);
    DrawText("Added", (int)vx + 20, legend_y + 2, 10, GREEN_DIFF);
    DrawRectangle((int)vx + 70, legend_y, 12, legend_h, RED_DIFF);
    DrawText("Removed", (int)vx + 86, legend_y + 2, 10, RED_DIFF);
    DrawRectangle((int)vx + 148, legend_y, 12, legend_h, YELLOW_DIFF);
    DrawText("Modified", (int)vx + 164, legend_y + 2, 10, YELLOW_DIFF);
    DrawRectangle((int)vx + 230, legend_y, 12, legend_h, GRAY_DIFF);
    DrawText("Unchanged", (int)vx + 246, legend_y + 2, 10, GRAY_DIFF);

    /* split view */
    float content_y = vy + 24;
    float content_h = vh - 50;
    float half_w = (vw - 8) * 0.5f;
    float left_x = vx + 2;
    float right_x = vx + 4 + half_w;

    draw_pcb_mini(dv->pcb_a, (int)left_x, (int)content_y, (int)half_w, (int)content_h,
        dv->track_diffs, dv->n_track_diffs, dv->via_diffs, dv->n_via_diffs,
        dv->highlight_track, 1);
    draw_pcb_mini(dv->pcb_b, (int)right_x, (int)content_y, (int)half_w, (int)content_h,
        dv->track_diffs, dv->n_track_diffs, dv->via_diffs, dv->n_via_diffs,
        dv->highlight_track, 0);

    /* status bar at bottom */
    float status_y = vy + vh - 24;
    DrawRectangle((int)vx, (int)status_y, (int)vw, 24, (Color){25,28,35,255});
    DrawRectangleLines((int)vx, (int)status_y, (int)vw, 24, (Color){60,60,66,255});
    char buf[128];
    snprintf(buf, sizeof(buf), "%d added, %d removed, %d modified | [I] next diff",
        dv->added_count, dv->removed_count, dv->modified_count);
    DrawText(buf, (int)vx + 8, (int)status_y + 4, 11, (Color){180,180,190,255});

    /* cycle hint */
    if (dv->total_diffs > 0) {
        char hl[32];
        snprintf(hl, sizeof(hl), "Diff %d/%d", dv->highlight_track + 1, dv->total_diffs);
        DrawText(hl, (int)(vx + vw - 120), (int)status_y + 4, 11, YELLOW_DIFF);
    }
}

void diff_view_cycle(DiffView *dv) {
    if (dv->total_diffs <= 0) return;
    dv->highlight_track = (dv->highlight_track + 1) % dv->total_diffs;
}

const char *diff_view_summary(DiffView *dv) {
    static char buf[128];
    snprintf(buf, sizeof(buf), "%d added, %d removed, %d modified",
        dv->added_count, dv->removed_count, dv->modified_count);
    return buf;
}
