#pragma once
// ui.h - Volt360 application UI shell (raylib)
#include "../model/project.h"
#include "../analysis/fit.h"
#include "../design/design.h"
#include "../design/gen_design.h"
#include "../slicer/slicer.h"
#include "../design/co_design.h"
#include "../slicer/farm.h"
#include "../core/undoredo.h"
#include "shortcuts.h"
#include "pcb3d.h"
#include "sch_canvas.h"
#include "diff_view.h"

typedef enum { UI_SCH, UI_PCB, UI_3D, UI_FIT, UI_DESIGN, UI_ASSIST, UI_PRINT, UI_DIFF } UIMode;

typedef struct {
    Project proj;
    UIMode mode;
    int sel_sch;        // index into proj.schematics
    int sel_pcb;        // index into proj.pcbs
    int sel_cad;        // index into proj.cad_models
    int sel_doc;        // index into proj.docs
    int sel_comp;       // index into proj.components
    char *folder;       // last opened folder
    char *status;       // status bar text (malloc'd)
    // 3D
    float yaw, pitch;
    float cam_dist;
    // canvas view
    V2 pan;
    float zoom;
    // fit report for current selection
    FitReport fit;
    bool fit_dirty;
    int cad_gen;        // bumped when cad models reload; 3D cache keys on this
    // parametric design workspace
    DesignFeature feats[32];
    int nfeats;
    int sel_feat;       // selected feature in timeline
    int sel_mat;        // selected material index
    // generative design
    GenDesign gen;
    bool gen_has_result;
    int gen_xi;  // slider int 0..100 → load_xf
    int gen_yi;  // slider int 0..100 → load_yf
    // assistant panel
    char *chat_log;     // accumulated transcript
    char chat_input[256];
    char *assist_status;
    // slicer / print / co-design
    SliceSettings slice_cfg;
    EnclosureParams enc_params;
    bool encl_ready;        // enclosure generated for current PCB
    char last_gcode[512];   // path to last generated gcode
    Farm farm;              // print farm backend (64 printers, 256 jobs, 16 orders)
    // pcb interactive editing
    int pcb_fp_sel;         // selected footprint index (-1 if none)
    int pcb_fp_drag;        // 1 if actively dragging selected footprint
    V2 pcb_fp_start;        // world-space position at drag start
    bool show_help;         // keyboard shortcuts overlay
    // 3D measurement tool
    bool measure_mode;      // M toggles measurement mode
    V3 meas_a, meas_b;     // two measurement points
    int meas_step;          // 0=idle, 1=picked A, 2=picked B
    // network printing
    char printer_ip[64];    // Moonraker IP:port
    bool wireframe_3d;      // W toggles wireframe mode in 3D
    bool route_mode;        // T toggles track routing mode in PCB
    V2 route_start;         // first point of route
    bool route_has_start;   // whether first point is placed
    char sidebar_filter[64];
    int multiview_mode;     // 0=single, 1=tiled 4-panel layout // sidebar search filter text (empty = show all)
    UndoStack undo_stack;
    ShortcutMap shortcuts;
    Pcb3DView pcb3d_view;
    bool pcb_3d;
    SchCanvas sch_canvas;
    DiffView diff_view;
} App;

void app_init(App *app);
void app_free(App *app);
bool app_open_folder(App *app, const char *dir);
void app_frame(App *app); // single frame of the UI (needs window open)
void app_run(App *app); // main loop; expects raylib initialized
