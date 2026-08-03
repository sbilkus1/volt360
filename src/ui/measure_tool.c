#include "measure_tool.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846f

void measure_init(MeasureTool *mt) {
    memset(mt, 0, sizeof(*mt));
    mt->state = MEASURE_IDLE;
    mt->active = 0;
    mt->snap_radius = 15.0f;
    mt->snap_idx = -1;
    mt->snap_type = 0;
}

void measure_activate(MeasureTool *mt) {
    mt->state = MEASURE_FIRST;
    mt->active = 1;
    mt->point_a = v2(0, 0);
    mt->point_b = v2(0, 0);
    mt->snap_idx = -1;
    mt->snap_type = 0;
}

void measure_deactivate(MeasureTool *mt) {
    mt->state = MEASURE_IDLE;
    mt->active = 0;
    mt->snap_idx = -1;
    mt->snap_type = 0;
}

static V2 screen_to_pcb(V2 screen, Rectangle vp, V2 pan, float zoom) {
    return v2((screen.x - vp.x - vp.width * 0.5f) / zoom + pan.x,
              (screen.y - vp.y - vp.height * 0.5f) / zoom + pan.y);
}

static V2 pcb_to_screen(V2 pcb, Rectangle vp, V2 pan, float zoom) {
    return v2(vp.x + vp.width * 0.5f + (pcb.x - pan.x) * zoom,
              vp.y + vp.height * 0.5f + (pcb.y - pan.y) * zoom);
}

static int snap_to_pad(const FpInst *fp, V2 mouse_pcb, float snap_mm) {
    return v2_dist(mouse_pcb, fp->pos) < snap_mm ? 1 : 0;
}

static int snap_to_via(const Via *via, V2 mouse_pcb, float snap_mm) {
    return v2_dist(mouse_pcb, via->pos) < snap_mm ? 1 : 0;
}

static int snap_to_track(const Track *track, V2 mouse_pcb, float snap_mm) {
    V2 mid = v2((track->a.x + track->b.x) * 0.5f, (track->a.y + track->b.y) * 0.5f);
    return v2_dist(mouse_pcb, mid) < snap_mm ? 1 : 0;
}

void measure_update(MeasureTool *mt, const Pcb *pcb, Rectangle viewport, V2 pan, float zoom) {
    if (!mt->active || !pcb) return;

    Vector2 rm = GetMousePosition();
    V2 mouse_screen = v2(rm.x, rm.y);
    V2 mouse_pcb = screen_to_pcb(mouse_screen, viewport, pan, zoom);
    mt->mouse_pos = mouse_pcb;

    float snap_mm = mt->snap_radius / (zoom > 0.01f ? zoom : 1.0f);
    mt->snap_idx = -1;
    mt->snap_type = 0;

    /* snap check - pads in footprints */
    for (int i = 0; i < pcb->nfps && mt->snap_idx < 0; i++) {
        if (snap_to_pad(&pcb->fps[i], mouse_pcb, snap_mm)) {
            mt->snap_idx = i;
            mt->snap_type = 1;
        }
    }

    /* snap check - vias */
    for (int i = 0; i < pcb->nvias && mt->snap_idx < 0; i++) {
        if (snap_to_via(&pcb->vias[i], mouse_pcb, snap_mm)) {
            mt->snap_idx = i;
            mt->snap_type = 2;
        }
    }

    /* snap check - tracks */
    for (int i = 0; i < pcb->ntracks && mt->snap_idx < 0; i++) {
        if (snap_to_track(&pcb->tracks[i], mouse_pcb, snap_mm)) {
            mt->snap_idx = i;
            mt->snap_type = 3;
        }
    }

    /* apply snap */
    if (mt->snap_idx >= 0) {
        if (mt->snap_type == 1) {
            mt->mouse_pos = pcb->fps[mt->snap_idx].pos;
        } else if (mt->snap_type == 2) {
            mt->mouse_pos = pcb->vias[mt->snap_idx].pos;
        } else if (mt->snap_type == 3) {
            Track *t = &pcb->tracks[mt->snap_idx];
            mt->mouse_pos = v2((t->a.x + t->b.x) * 0.5f, (t->a.y + t->b.y) * 0.5f);
        }
    }

    /* click handling */
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (mt->state == MEASURE_FIRST) {
            mt->point_a = mt->mouse_pos;
            mt->state = MEASURE_SECOND;
        } else if (mt->state == MEASURE_SECOND) {
            mt->point_b = mt->mouse_pos;
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        measure_deactivate(mt);
        measure_activate(mt);
    }

    /* restart measurement: if both points set, next click restarts */
    if (mt->state == MEASURE_SECOND && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        /* already handled above as second point pick */
    }
}

void measure_render(const MeasureTool *mt, Rectangle viewport, V2 pan, float zoom) {
    if (!mt->active) return;

    V2 screen_a = { 0, 0 };
    V2 screen_b = { 0, 0 };
    V2 screen_cursor = pcb_to_screen(mt->mouse_pos, viewport, pan, zoom);

    if (mt->state == MEASURE_SECOND) {
        screen_a = pcb_to_screen(mt->point_a, viewport, pan, zoom);
        screen_b = pcb_to_screen(mt->point_b, viewport, pan, zoom);
    } else if (mt->state == MEASURE_FIRST) {
        screen_b = screen_cursor;
    }

    /* dashed preview line from point_a to point_b or cursor */
    if (mt->state >= MEASURE_FIRST) {
        float len = v2_dist(screen_a, screen_b);
        V2 dir = v2(0, 0);
        if (len > 0.01f) {
            dir = v2((screen_b.x - screen_a.x) / len, (screen_b.y - screen_a.y) / len);
        }
        float dash_len = 6.0f;
        float gap = 4.0f;
        float pos = 0.0f;
        int drawing = 1;
        while (pos < len) {
            float seg = pos + dash_len;
            if (seg > len) seg = len;
            if (drawing) {
                DrawLine((int)(screen_a.x + dir.x * pos), (int)(screen_a.y + dir.y * pos),
                         (int)(screen_a.x + dir.x * seg), (int)(screen_a.y + dir.y * seg), YELLOW);
            }
            pos = seg;
            drawing = !drawing;
            if (drawing) pos += gap;
        }
    }

    /* point A */
    if (mt->state >= MEASURE_FIRST) {
        float radius = 6.0f;
        DrawCircleV((Vector2){ screen_a.x, screen_a.y }, radius, (Color){ 0, 220, 50, 200 });
        DrawCircleLines((int)screen_a.x, (int)screen_a.y, radius + 1, DARKGREEN);
        DrawText("A", (int)screen_a.x - 12, (int)screen_a.y - 22, 12, GREEN);
    }

    /* point B */
    if (mt->state == MEASURE_SECOND) {
        float radius = 6.0f;
        DrawCircleV((Vector2){ screen_b.x, screen_b.y }, radius, SKYBLUE);
        DrawCircleLines((int)screen_b.x, (int)screen_b.y, radius + 1, (Color){ 0, 120, 215, 255 });
        DrawText("B", (int)screen_b.x + 8, (int)screen_b.y - 22, 12, SKYBLUE);
    } else if (mt->state == MEASURE_FIRST) {
        DrawCircleV((Vector2){ screen_cursor.x, screen_cursor.y }, 4.0f, (Color){ 100, 200, 255, 180 });
    }

    /* snap ring */
    if (mt->snap_idx >= 0) {
        float ring_r = 8.0f;
        DrawCircleLines((int)screen_cursor.x, (int)screen_cursor.y, ring_r, ORANGE);
        DrawCircleLines((int)screen_cursor.x, (int)screen_cursor.y, ring_r + 1, ORANGE);
        const char *snap_label = "SNAP";
        if (mt->snap_type == 1) snap_label = "PAD";
        else if (mt->snap_type == 2) snap_label = "VIA";
        else if (mt->snap_type == 3) snap_label = "TRK";
        DrawText(snap_label, (int)screen_cursor.x + 10, (int)screen_cursor.y - 8, 10, ORANGE);
    }

    /* distance label at midpoint */
    if (mt->state == MEASURE_SECOND) {
        V2 mid = v2((screen_a.x + screen_b.x) * 0.5f, (screen_a.y + screen_b.y) * 0.5f);
        float dist = mt_measure_distance(mt);
        float angle = mt_measure_angle_deg(mt);
        V2 delta = v2_sub(mt->point_b, mt->point_a);
        char buf[128];
        snprintf(buf, sizeof(buf), "%.2f mm | %.1f deg", (double)dist, (double)angle);
        DrawText(buf, (int)mid.x + 6, (int)mid.y - 20, 13, YELLOW);

        snprintf(buf, sizeof(buf), "Dx: %.2fmm Dy: %.2fmm", (double)delta.x, (double)delta.y);
        DrawText(buf, (int)mid.x + 6, (int)mid.y - 4, 11, (Color){ 200, 200, 200, 255 });
    }

    /* status bar info at bottom of viewport */
    {
        float dist = 0.0f;
        float angle = 0.0f;
        V2 delta = { 0, 0 };
        if (mt->state == MEASURE_SECOND) {
            dist = mt_measure_distance(mt);
            angle = mt_measure_angle_deg(mt);
            delta = v2_sub(mt->point_b, mt->point_a);
        }
        char bar[256];
        snprintf(bar, sizeof(bar), "Distance: %.2f mm | Angle: %.1f deg | Dx: %.2fmm Dy: %.2fmm",
                 (double)dist, (double)angle, (double)delta.x, (double)delta.y);
        int bar_w = MeasureText(bar, 12);
        DrawRectangle((int)viewport.x, (int)(viewport.y + viewport.height - 22),
                      (int)viewport.width, 22, (Color){ 20, 22, 28, 220 });
        DrawText(bar, (int)(viewport.x + viewport.width - bar_w - 8),
                 (int)(viewport.y + viewport.height - 19), 12, LIGHTGRAY);
    }
}

float mt_measure_distance(const MeasureTool *mt) {
    return v2_dist(mt->point_a, mt->point_b);
}

float mt_measure_angle_deg(const MeasureTool *mt) {
    V2 delta = v2_sub(mt->point_b, mt->point_a);
    return atan2f(delta.y, delta.x) * (180.0f / (float)M_PI);
}

char *mt_measure_report(const MeasureTool *mt) {
    if (mt->state != MEASURE_SECOND) {
        return str_dup("Measurement not complete");
    }
    float dist = mt_measure_distance(mt);
    float angle = mt_measure_angle_deg(mt);
    V2 delta = v2_sub(mt->point_b, mt->point_a);
    Buf b; buf_init(&b);
    buf_appendf(&b, "Distance: %.2fmm, Angle: %.1f deg, Delta: X=%.2fmm Y=%.2fmm",
                (double)dist, (double)angle, (double)delta.x, (double)delta.y);
    return buf_cstr(&b);
}
