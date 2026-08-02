#pragma once
// multiview.h - Tiled multi-panel layout: EDA + CAD + Slicer simultaneously.
// Loop-style continuous printing with auto-ejection animation.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"
#include "../slicer/farm.h"
#include "../ui/ui.h"

// Panel types for tiled layout.
typedef enum { PANEL_EDA, PANEL_CAD, PANEL_SLICER, PANEL_FARM, PANEL_COUNT } PanelType;

// Panel state.
typedef struct {
    PanelType type;
    int x, y, w, h;
    int minimized; // 0=visible, 1=minimized to title bar
    int focused;
    char title[32];
} Panel;

// Multi-view layout: manages tiled panels.
typedef struct {
    Panel panels[PANEL_COUNT];
    int drag_panel;      // -1 = none, else panel index being dragged
    int drag_edge;       // 0=move, 1=right, 2=bottom, 3=corner
    int sx, sy;          // drag start position
    int split_v, split_h; // split positions
} MultiView;

void multiview_init(MultiView *mv, int sw, int sh);
void multiview_layout(MultiView *mv, int sw, int sh);
void multiview_draw_frame(MultiView *mv, int pi, int sw, int sh);
void multiview_handle_input(MultiView *mv, int sw, int sh);
Panel *multiview_get_panel(MultiView *mv, PanelType type);

// ===== LOOP-STYLE AUTO-EJECTION =====
// Animation state for conveyor/ejection visualization.
typedef struct {
    float part_x, part_y;        // part position on plate
    float conveyor_offset;       // belt animation offset (0-1)
    int phase;                   // 0=printing, 1=cooling, 2=ejecting, 3=clearing
    float phase_timer;
    int job_count;               // total jobs completed this session
} EjectAnim;

void eject_anim_init(EjectAnim *ea);
void eject_anim_tick(EjectAnim *ea, float dt);
void eject_anim_draw(EjectAnim *ea, int x, int y, int w, int h, float bed_w, float bed_h);

// ===== CONTINUOUS PRINTING QUEUE (LOOP-STYLE) =====
typedef struct {
    char gcode_path[256];
    char filament[32];
    float est_time_min;
    int quantity;
    int completed;
    int status; // 0=queued, 1=printing, 2=ejecting, 3=done
} LoopJob;

typedef struct {
    LoopJob jobs[32]; int n_jobs;
    int running;
    int total_printed;
    float total_time_min;
    int conveyor;    // 1=conveyor belt, 0=manual removal
    int continuous;  // 1=keep running, 0=stop when queue empty
} LoopQueue;

void loopqueue_init(LoopQueue *lq);
void loopqueue_add(LoopQueue *lq, const char *gcode, const char *filament, float time_min, int qty);
void loopqueue_tick(LoopQueue *lq, EjectAnim *ea, float dt);
char *loopqueue_report(LoopQueue *lq);
void loopqueue_draw(LoopQueue *lq, int x, int y, int w, int h);
