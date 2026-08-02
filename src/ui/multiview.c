#include "multiview.h"
#include "../core/util.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// ===== MULTI-VIEW =====
void multiview_init(MultiView *mv, int sw, int sh) {
    memset(mv, 0, sizeof(*mv));
    mv->drag_panel = -1;
    mv->split_v = sw * 2 / 3;
    mv->split_h = sh / 2;
    const char *titles[] = {"EDA (SCH/PCB)", "CAD (3D/Design)", "Slicer (Print)", "Farm"};
    for (int i = 0; i < 4; i++) {
        mv->panels[i].type = (PanelType)i;
        snprintf(mv->panels[i].title, sizeof(mv->panels[i].title), "%s", titles[i]);
    }
    multiview_layout(mv, sw, sh);
}

void multiview_layout(MultiView *mv, int sw, int sh) {
    if (!mv) return;
    // top-left: EDA; top-right: CAD; bottom: Slicer+Farm
    int sv = mv->split_v, sh2 = mv->split_h;
    if (sv < 300) sv = 300; if (sv > sw - 300) sv = sw - 300;
    if (sh2 < 200) sh2 = 200; if (sh2 > sh - 200) sh2 = sh - 200;
    // EDA: top-left
    mv->panels[0].x = 0; mv->panels[0].y = 0; mv->panels[0].w = sv; mv->panels[0].h = sh2;
    // CAD: top-right
    mv->panels[1].x = sv; mv->panels[1].y = 0; mv->panels[1].w = sw - sv; mv->panels[1].h = sh2;
    // Slicer: bottom-left
    mv->panels[2].x = 0; mv->panels[2].y = sh2; mv->panels[2].w = sw/2; mv->panels[2].h = sh - sh2;
    // Farm: bottom-right
    mv->panels[3].x = sw/2; mv->panels[3].y = sh2; mv->panels[3].w = sw - sw/2; mv->panels[3].h = sh - sh2;
}

void multiview_draw_frame(MultiView *mv, int pi, int sw, int sh) {
    if (!mv || pi < 0 || pi >= 4) return;
    Panel *p = &mv->panels[pi];
    int th = 22;
    // title bar
    DrawRectangle(p->x, p->y, p->w, th, (Color){35,38,45,255});
    DrawText(p->title, p->x+6, p->y+3, 13, p->focused ? YELLOW : GRAY);
    // minimize/maximize buttons
    DrawText(p->minimized ? "[+]" : "[-]", p->x+p->w-30, p->y+3, 13, GRAY);
    // panel border
    DrawRectangleLines(p->x, p->y, p->w, p->h, (Color){50,53,60,255});
    // check for clicks
    Vector2 mp = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // title bar click = focus
        if (mp.x >= p->x && mp.x <= p->x+p->w-30 && mp.y >= p->y && mp.y <= p->y+th) {
            for (int i = 0; i < 4; i++) mv->panels[i].focused = 0;
            p->focused = 1;
        }
        // minimize button
        if (mp.x >= p->x+p->w-30 && mp.x <= p->x+p->w && mp.y >= p->y && mp.y <= p->y+th)
            p->minimized = !p->minimized;
    }
    // splitter drag
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (mp.x >= mv->split_v-3 && mp.x <= mv->split_v+3 && mp.y >= 0 && mp.y <= sh) mv->split_v = (int)mp.x;
        if (mp.y >= mv->split_h-3 && mp.y <= mv->split_h+3 && mp.x >= 0 && mp.x <= sw) mv->split_h = (int)mp.y;
    }
    // draw splitter lines
    DrawRectangle(mv->split_v-1, 0, 3, sh, (Color){50,53,60,255});
    DrawRectangle(0, mv->split_h-1, sw, 3, (Color){50,53,60,255});
}

void multiview_handle_input(MultiView *mv, int sw, int sh) {
    multiview_layout(mv, sw, sh);
}

Panel *multiview_get_panel(MultiView *mv, PanelType type) {
    for (int i = 0; i < 4; i++) if (mv->panels[i].type == type) return &mv->panels[i];
    return NULL;
}

// ===== LOOP AUTO-EJECTION ANIMATION =====
void eject_anim_init(EjectAnim *ea) {
    memset(ea, 0, sizeof(*ea));
    ea->part_x = 0.5f; ea->part_y = 0.3f;
}

void eject_anim_tick(EjectAnim *ea, float dt) {
    ea->phase_timer += dt;
    switch (ea->phase) {
        case 0: // printing
            if (ea->phase_timer > 3.0f) { ea->phase = 1; ea->phase_timer = 0; } break;
        case 1: // cooling
            if (ea->phase_timer > 2.0f) { ea->phase = 2; ea->phase_timer = 0; ea->conveyor_offset = 0; } break;
        case 2: // ejecting
            ea->conveyor_offset += dt * 0.3f;
            if (ea->conveyor_offset > 1.0f) { ea->phase = 3; ea->phase_timer = 0; ea->job_count++; } break;
        case 3: // clearing
            if (ea->phase_timer > 1.0f) { ea->phase = 0; ea->phase_timer = 0; } break;
    }
}

void eject_anim_draw(EjectAnim *ea, int x, int y, int w, int h, float bed_w, float bed_h) {
    (void)bed_w; (void)bed_h;
    // build plate
    int px = x + 20, py = y + h/2, pw = w - 40, ph = h/2 - 20;
    DrawRectangle(px, py, pw, ph, (Color){50,55,65,255});
    DrawRectangleLines(px, py, pw, ph, (Color){80,85,95,255});
    DrawText("Build Plate", px+4, py+2, 10, DARKGRAY);

    // part on plate (offset during ejection)
    int part_cx = px + (int)(pw * ea->part_x + ea->conveyor_offset * pw * 0.6f);
    int part_cy = py + (int)(ph * ea->part_y);
    int part_sz = 15;
    DrawRectangle(part_cx - part_sz, part_cy - part_sz, part_sz*2, part_sz*2, (Color){255,180,50,255});
    DrawRectangleLines(part_cx - part_sz, part_cy - part_sz, part_sz*2, part_sz*2, (Color){255,220,100,255});

    // conveyor belt (bottom)
    int cy = py + ph + 10;
    DrawRectangle(px, cy, pw, 8, (Color){40,45,55,255});
    // belt animation
    for (int i = 0; i < pw/10; i++) {
        int bx = px + (i*10 + (int)(ea->conveyor_offset * 100)) % pw;
        DrawRectangle(bx, cy-2, 6, 12, (Color){80,90,110,255});
    }
    DrawText("Conveyor", px+4, cy+10, 10, GRAY);

    // status text
    const char *phases[] = {"Printing...", "Cooling...", "Ejecting!", "Ready"};
    DrawText(phases[ea->phase], x+8, y+4, 14, YELLOW);

    // job counter
    char jc[32]; snprintf(jc, sizeof(jc), "Jobs: %d", ea->job_count);
    DrawText(jc, x+w-80, y+4, 13, GRAY);

    // part collection bin
    DrawRectangle(px + pw + 5, cy, 20, 30, (Color){50,55,65,255});
    DrawRectangleLines(px + pw + 5, cy, 20, 30, (Color){80,85,95,255});
    DrawText("Bin", px+pw+7, cy+10, 10, GRAY);
}

// ===== LOOP QUEUE =====
void loopqueue_init(LoopQueue *lq) { memset(lq, 0, sizeof(*lq)); lq->conveyor = 1; lq->continuous = 1; }

void loopqueue_add(LoopQueue *lq, const char *gcode, const char *fil, float t, int qty) {
    if (!lq || lq->n_jobs >= 32) return;
    LoopJob *j = &lq->jobs[lq->n_jobs++];
    snprintf(j->gcode_path, sizeof(j->gcode_path), "%s", gcode ? gcode : "");
    snprintf(j->filament, sizeof(j->filament), "%s", fil ? fil : "PLA");
    j->est_time_min = t; j->quantity = qty; j->completed = 0; j->status = 0;
}

void loopqueue_tick(LoopQueue *lq, EjectAnim *ea, float dt) {
    if (!lq || !ea) return;
    // check for completed ejection, start next job
    if (ea->phase == 0 && ea->phase_timer < 0.1f && lq->running >= 0) {
        for (int i = 0; i < lq->n_jobs; i++) {
            if (lq->jobs[i].status == 1) { // was printing
                lq->jobs[i].completed++;
                lq->total_printed++;
                lq->total_time_min += lq->jobs[i].est_time_min;
                if (lq->jobs[i].completed >= lq->jobs[i].quantity)
                    lq->jobs[i].status = 3;
                else
                    lq->jobs[i].status = 0;
                lq->running = -1;
            }
        }
        // find next queued job
        for (int i = 0; i < lq->n_jobs && lq->running < 0; i++)
            if (lq->jobs[i].status == 0) { lq->jobs[i].status = 1; lq->running = i; }
    }
    eject_anim_tick(ea, dt);
}

char *loopqueue_report(LoopQueue *lq) {
    if (!lq) return str_dup("no queue");
    char buf[512]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "Loop Queue: %d jobs | Printed: %d | %.0f min\n", lq->n_jobs, lq->total_printed, lq->total_time_min);
    for (int i = 0; i < lq->n_jobs; i++) {
        LoopJob *j = &lq->jobs[i];
        const char *st = j->status==0?"queued":j->status==1?"printing":j->status==2?"ejecting":"done";
        off += snprintf(buf+off, sizeof(buf)-off, "  [%d] %s x%d %s %s\n", i+1, j->filament, j->quantity, st, j->gcode_path);
    }
    return str_dup(buf);
}

void loopqueue_draw(LoopQueue *lq, int x, int y, int w, int h) {
    if (!lq) return;
    DrawText("Continuous Queue:", x+8, y+4, 13, LIGHTGRAY);
    for (int i = 0; i < lq->n_jobs && y+24+i*16 < y+h; i++) {
        LoopJob *j = &lq->jobs[i];
        Color c = j->status==0?GRAY:j->status==1?GREEN:j->status==2?ORANGE:DARKGRAY;
        char buf[64]; snprintf(buf,sizeof(buf),"[%d] %s x%d %.0fmin", i+1, j->filament, j->quantity, j->est_time_min);
        DrawText(buf, x+12, y+24+i*16, 12, c);
    }
    char buf[32]; snprintf(buf,sizeof(buf),"Total: %d parts", lq->total_printed);
    DrawText(buf, x+8, y+24+lq->n_jobs*16+4, 12, GRAY);
}
