#include "ui.h"
#include "../import/importer.h"
#include "../llm/ollama.h"
#include "../design/assistant.h"
#include "../design/joints.h"
#include "../design/co_design.h"
#include "../slicer/gcode.h"
#include "../slicer/farm.h"
#include "../slicer/calibration.h"
#include "../slicer/moonraker.h"
#include "../slicer/cam.h"
#include "../slicer/turning.h"
#include "../slicer/supports.h"
#include "../design/hollow.h"
#include "../design/loft.h"
#include "../design/pushroute.h"
#include "../analysis/erc.h"
#include "../import/gerber_out.h"
#include "../design/primitives.h"
#include "../design/meshrepair.h"
#include "../design/difroute.h"
#include "../design/xsection.h"
#include "../slicer/cam_ext.h"
#include "../design/exploded.h"
#include "../design/symedit.h"
#include "../design/fabdraw.h"
#include "../design/via_ext.h"
#include "../design/lengthtune.h"
#include "../analysis/thermal.h"
#include "../analysis/montecarlo.h"
#include "../slicer/bambu.h"
#include "../slicer/octoprint.h"
#include "../slicer/energy.h"
#include "../design/designblock.h"
#include "../design/lattice.h"
#include "../design/section3d.h"
#include "../design/libmgr.h"
#include "../analysis/signalint.h"
#include "../analysis/fatigue.h"
#include "../analysis/curvature.h"
#include "../design/sweep.h"
#include "../design/balloons.h"
#include "../design/webcam.h"
#include "../slicer/primetower.h"
#include "../slicer/toollib.h"
#include "../slicer/sla.h"
#include "../analysis/modal.h"
#include "../analysis/nonlinear.h"
#include "../design/annotations.h"
#include "../import/mfg_out.h"
#include "../import/pdfout.h"
#include "../design/bus.h"
#include "../analysis/powerint.h"
#include "../slicer/farmdash.h"
#include "../slicer/cam5axis.h"
#include "../design/multisheet.h"
#include "../design/version.h"
#include "../design/nethilite.h"
#include "../analysis/contact.h"
#include "../analysis/interact.h"
#include "../slicer/adaptiveinfill.h"
#include "../design/lib3d.h"
#include "../design/autosave.h"
#include "../slicer/creality.h"
#include "../design/scaletool.h"
#include "../design/webcamview.h"
#include "../slicer/sptblock.h"
#include "../design/finalmods.h"
#include "../slicer/printquality.h"
#include "../slicer/orcafeat.h"
#include "../design/sketcher.h"
#include "../design/freecadfeat.h"
#include "../analysis/ieda_eda.h"
#include "../design/kicadfeat.h"
#include "../design/kicad_draw.h"
#include "../analysis/real_algo.h"
#include "profui.h"
#include "canvas.h"
#include "multiview.h"
#include "../import/kicad_lib.h"
#include "../slicer/fusion_lib.h"
#include "../core/cloudsave.h"
#include "../core/autoupdate.h"
#include "../design/advance_feat.h"
#include "../design/v2_features.h"
#include "../design/finish_414.h"
#include "../design/final_feat.h"
#include "../analysis/spice.h"
#include "../analysis/drc.h"
#include "../analysis/pcbcalc.h"
#include "../analysis/mechcalc.h"
#include "../design/copperpour.h"
#include "../design/teardrop.h"
#include "../design/boolop.h"
#include "../design/meshops.h"
#include "../design/sheetmetal.h"
#include "../design/drawing2d.h"
#include "../core/util.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ---------- forward ----------
static void draw_schematic(App *app, Schematic *s, int x, int y, int w, int h);
static void draw_pcb(App *app, Pcb *pcb, int x, int y, int w, int h);
static void draw_3d(App *app, int x, int y, int w, int h);
static void draw_fit(App *app, FitReport *rep, int x, int y, int w, int h);
static void draw_design(App *app, int x, int y, int w, int h);
static void draw_assist(App *app, int x, int y, int w, int h);
static void draw_print(App *app, int x, int y, int w, int h);
static void draw_sidebar(App *app, int x, int y, int w, int h);
static void run_fit(App *app);
static void design_rebuild(App *app);
static void design_run_generative(App *app);
static void assist_submit(App *app);
static bool ui_button(const char *label, int x, int y, int w, int h);

// ---------- cached raylib models for 3D ----------
typedef struct { int gen; Model model; } CachedModel;
static CachedModel g_cache[64];
static int g_cache_n = 0;

static void cache_clear(void) {
    for (int i = 0; i < g_cache_n; i++) { UnloadModel(g_cache[i].model); g_cache[i].model = (Model){ 0 }; }
    g_cache_n = 0;
}

static Model *cache_get(App *app, int gen, CadModel *cm, int *out_ok) {
    if (out_ok) *out_ok = 0;
    if (!cm->mesh.valid || cm->mesh.nverts == 0) return NULL;
    for (int i = 0; i < g_cache_n; i++) if (g_cache[i].gen == gen) return &g_cache[i].model;
    cache_clear();
    Mesh rl = { 0 };
    rl.vertexCount = cm->mesh.nverts;
    rl.triangleCount = cm->mesh.ntris;
    rl.vertices = (float *)cm->mesh.pos;
    rl.normals = (float *)cm->mesh.nrm;
    rl.texcoords = NULL;
    // index conversion: raylib uses unsigned short
    if (cm->mesh.ntris > 0 && cm->mesh.idx) {
        unsigned short *idx = (unsigned short *)malloc(sizeof(unsigned short) * 3 * (size_t)cm->mesh.ntris);
        for (int i = 0; i < cm->mesh.ntris * 3; i++) idx[i] = (unsigned short)cm->mesh.idx[i];
        rl.indices = idx;
    }
    UploadMesh(&rl, false);
    Model model = LoadModelFromMesh(rl);
    free((void *)rl.indices);
    if (g_cache_n < 64) { g_cache[g_cache_n].gen = gen; g_cache[g_cache_n].model = model; g_cache_n++; }
    if (out_ok) *out_ok = 1;
    return &g_cache[g_cache_n - 1];
}

// ================= lifecycle =================
static void farm_seed_demo(Farm *f) {
    farm_add_printer(f, "Voron 2.4", 4, "PLA");
    farm_add_printer(f, "Prusa MK4", 2, "PETG");
    farm_add_printer(f, "Bambu X1C", 4, "ABS");
    farm_add_printer(f, "Rat Rig VC3", 4, "TPU");
    farm_add_printer(f, "Ender-3 V2", 1, "PLA");
    farm_add_spool(f, "PLA", "White", 250.0f);
    farm_add_spool(f, "PLA", "Black", 330.0f);
    farm_add_spool(f, "PETG", "Blue", 200.0f);
    farm_add_spool(f, "ABS", "Red", 150.0f);
    farm_add_spool(f, "TPU", "Green", 100.0f);
    farm_add_job(f, "Gear Set 40T", 4, 3, "PLA", 0);
    farm_add_job(f, "Bracket v2", 2, 2, "PETG", 1);
    farm_add_job(f, "Enclosure Top", 1, 1, "ABS", 2);
    farm_add_job(f, "Fan Duct", 3, 2, "PETG", 1);
    farm_add_job(f, "Cable Chain", 2, 1, "TPU", 3);
    farm_add_job(f, "Desk Mount", 1, 3, "PLA", 0);
    farm_add_job(f, "Camera Arm", 2, 2, "PETG", 1);
    farm_add_job(f, "Vent Grill", 5, 1, "ABS", 2);
    farm_assign_job(f, 0, 0);
    farm_assign_job(f, 1, 1);
    farm_assign_job(f, 2, 2);
    snprintf(f->jobs[0].status, sizeof(f->jobs[0].status), "running");
    snprintf(f->jobs[0].assigned_printer, sizeof(f->jobs[0].assigned_printer), "Voron 2.4");
    f->printers[0].busy = 1; f->printers[0].hours_running = 142.0f; f->printers[0].total_jobs_done = 28;
    snprintf(f->printers[0].status, sizeof(f->printers[0].status), "busy");
    snprintf(f->printers[0].current_job_id, sizeof(f->printers[0].current_job_id), f->jobs[0].id);
    snprintf(f->jobs[1].status, sizeof(f->jobs[1].status), "running");
    snprintf(f->jobs[1].assigned_printer, sizeof(f->jobs[1].assigned_printer), "Prusa MK4");
    f->printers[1].busy = 1; f->printers[1].hours_running = 87.0f; f->printers[1].total_jobs_done = 15;
    snprintf(f->printers[1].status, sizeof(f->printers[1].status), "busy");
    f->printers[2].hours_running = 310.0f; f->printers[2].total_jobs_done = 62;
    snprintf(f->jobs[2].status, sizeof(f->jobs[2].status), "done");
    f->jobs[2].finished = 1;
}

void app_init(App *app) {
    memset(app, 0, sizeof(*app));
    project_init(&app->proj, "Untitled", "");
    app->mode = UI_SCH;
    app->zoom = 1.0f;
    app->cam_dist = 300.0f;
    app->pitch = -30.0f;
    app->status = str_dup("ready");
    app->chat_log = str_dup("-- Volt360 assistant --\nType a request, e.g. 'add a box 40x30x10'.\n");
    app->assist_status = str_dup("offline mode (Ollama optional)");
    app->sel_mat = 0;
    app->gen_xi = 100;
    app->gen_yi = 50;
    app->gen.nx = 32;
    app->gen.ny = 20;
    app->gen.volfrac = 0.4f;
    app->gen.iters = 50;
    app->gen.fix_left = 1;
    app->gen.load_xf = 1.0f;
    app->gen.load_yf = 0.5f;
    // slicer / co-design defaults
    slice_settings_default(&app->slice_cfg);
    co_enclosure_defaults(&app->enc_params);
    app->encl_ready = false;
    app->last_gcode[0] = '\0';
    slice_settings_default(&app->slice_cfg);
    slice_settings_load(&app->slice_cfg, "build\\slice_settings.json");
    farm_init(&app->farm, "build\\farm");
    app->pcb_fp_sel = -1;
    app->pcb_fp_drag = 0;
    snprintf(app->printer_ip, sizeof(app->printer_ip), "%s", "localhost:7125");
    if (app->farm.n_printers == 0) farm_seed_demo(&app->farm);
}

void app_free(App *app) {
    project_free(&app->proj);
    free(app->folder);
    free(app->status);
    free(app->chat_log);
    free(app->assist_status);
    fit_report_free(&app->fit);
    gen_design_free(&app->gen);
    farm_free(&app->farm);
}

bool app_open_folder(App *app, const char *dir) {
    free(app->folder);
    app->folder = str_dup(dir);
    import_result_init(&(ImportResult){0});
    ImportResult r = importer_import_dir(&app->proj, dir);
    app->cad_gen++;
    cache_clear();
    char msg[512];
    snprintf(msg, sizeof(msg), "imported %s: %d comps, %d symbols, %d footprints, %d cad, %d schematics, %d pcbs, %d docs",
        r.ok ? "OK" : "FAIL", r.ncomponents, r.nsymbols, r.nfootprints, r.ncad, r.nsch, r.npcb, r.ndocs);
    free(app->status);
    app->status = str_dup(r.ok ? msg : (r.message[0] ? r.message : "import failed"));
    app->fit_dirty = true;
    return r.ok != 0;
}

// ---------- llm helpers ----------
static void ui_extract_docs(App *app) {
    if (app->proj.docs.len == 0) {
        free(app->status); app->status = str_dup("no datasheets to parse");
        return;
    }
    if (!ollama_ping(app->proj.llm_url)) {
        free(app->status); app->status = str_dup("Ollama not reachable (set env / config)");
        return;
    }
    int made = 0;
    for (int i = 0; i < app->proj.docs.len; i++) {
        Doc *d = &app->proj.docs.v[i];
        if (d->parsed || !d->text || !d->text[0]) continue;
        Component c;
        if (ollama_extract_component(app->proj.llm_url, app->proj.llm_model, d->text, d->id, &c)) {
            arr_push(app->proj.components, c);
            d->parsed = 1;
            made++;
        }
    }
    char msg[256];
    snprintf(msg, sizeof(msg), "parsed %d component(s) via %s", made, app->proj.llm_model);
    free(app->status); app->status = str_dup(msg);
    app->fit_dirty = true;
}

// ================= run =================
void app_frame(App *app) {
    float dt = GetFrameTime();
    autosave_tick(&app->proj);
    // check for updates once at startup
    static int update_checked = 0;
    if (!update_checked) { update_checked = 1;
        char *up = autoupdate_run("sbilkus1", "volt360");
        prof_console_write(up); free(up);
    }
    Vector2 mouse = GetMousePosition();
        // mouse drag to pan canvas views
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) && app->mode != UI_3D) {
            app->pan.x += GetMouseDelta().x / app->zoom;
            app->pan.y += GetMouseDelta().y / app->zoom;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) && app->mode == UI_3D) {
            app->yaw -= GetMouseDelta().x * 0.5f;
            app->pitch -= GetMouseDelta().y * 0.5f;
        }
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            if (app->mode == UI_3D) app->cam_dist *= (1.0f - wheel * 0.1f);
            else app->zoom *= (1.0f + wheel * 0.1f);
            if (app->zoom < 0.1f) app->zoom = 0.1f;
            if (app->cam_dist < 20) app->cam_dist = 20;
            if (app->cam_dist > 2000) app->cam_dist = 2000;
        }
        // mode switching
        if (IsKeyPressed(KEY_ONE)) { app->mode = UI_SCH; app->fit_dirty = true; }
        if (IsKeyPressed(KEY_TWO)) { app->mode = UI_PCB; app->fit_dirty = true; }
        if (IsKeyPressed(KEY_THREE)) { app->mode = UI_3D; }
        if (IsKeyPressed(KEY_FOUR)) { app->mode = UI_FIT; run_fit(app); }
        if (IsKeyPressed(KEY_FIVE)) { app->mode = UI_DESIGN; }
        if (IsKeyPressed(KEY_SIX)) { app->mode = UI_ASSIST; }
        if (IsKeyPressed(KEY_SEVEN)) { app->mode = UI_PRINT; }
        if (IsKeyPressed(KEY_F5)) { ui_extract_docs(app); }
        if (IsKeyPressed(KEY_SLASH) || IsKeyPressed(KEY_F1)) app->show_help = !app->show_help;
        if (app->show_help && IsKeyPressed(KEY_ESCAPE)) app->show_help = false;
        // M: measure toggle in 3D
        if (IsKeyPressed(KEY_M) && app->mode == UI_3D) {
            app->measure_mode = !app->measure_mode;
            if (!app->measure_mode) app->meas_step = 0;
            else app->meas_step = 0;
        }
        if (IsKeyPressed(KEY_W) && app->mode == UI_3D) app->wireframe_3d = !app->wireframe_3d;
        if (IsKeyPressed(KEY_G)) { grid_toggle(); char ms[32]; snprintf(ms, sizeof(ms), "Grid: %.2f mm", grid_size_current()); free(app->status); app->status = str_dup(ms); }
        if (IsKeyPressed(KEY_ZERO)) { app->multiview_mode = !app->multiview_mode; free(app->status); app->status = app->multiview_mode ? str_dup("Multi-view: EDA+CAD+Slicer+Farm") : str_dup("Single view"); }
        if (IsKeyPressed(KEY_R) && app->mode == UI_3D) {
            int ai = assembly_demo_robot(&app->proj);
            free(app->status);
            app->status = str_dup(ai >= 0 ? "built demo assembly with revolute joint (press R again to rebuild)" : "need 2+ cad models for the joint demo");
        }
        // F key: zoom to selected model in 3D
        if (IsKeyPressed(KEY_F) && app->mode == UI_3D && app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) {
            CadModel *cm = &app->proj.cad_models.v[app->sel_cad];
            if (cm->mesh.valid) {
                float cx = (cm->mesh.bmin.x + cm->mesh.bmax.x) * 0.5f;
                float cy = (cm->mesh.bmin.y + cm->mesh.bmax.y) * 0.5f;
                float cz = (cm->mesh.bmin.z + cm->mesh.bmax.z) * 0.5f;
                float dx = cm->mesh.bmax.x - cm->mesh.bmin.x;
                float dy = cm->mesh.bmax.y - cm->mesh.bmin.y;
                float dz = cm->mesh.bmax.z - cm->mesh.bmin.z;
                float size = (dx > dy) ? (dx > dz ? dx : dz) : (dy > dz ? dy : dz);
                app->pan.x = cx; app->pan.y = cy;
                app->cam_dist = size * 2.5f;
                if (app->cam_dist < 20) app->cam_dist = 20;
            }
        }
        // open folder via drag+drop
        if (IsFileDropped()) {
            FilePathList files = LoadDroppedFiles();
            if (files.count > 0 && DirectoryExists(files.paths[0])) {
                app_open_folder(app, files.paths[0]);
            }
            UnloadDroppedFiles(files);
        }
        if (app->fit_dirty && (app->mode == UI_FIT)) { run_fit(app); }

        BeginDrawing();
        ClearBackground((Color){ 28, 30, 34, 255 });
        int sw = GetScreenWidth(), sh = GetScreenHeight();

        // Multi-view tiled layout
        static MultiView g_mv; static int g_mv_init = 0;
        static EjectAnim g_eject; static LoopQueue g_loopq;
        if (!g_mv_init) { multiview_init(&g_mv, sw, sh); eject_anim_init(&g_eject); loopqueue_init(&g_loopq); g_mv_init = 1; }

        if (app->multiview_mode) {
            multiview_handle_input(&g_mv, sw, sh);
            // draw each panel frame
            for (int pi = 0; pi < 4; pi++) multiview_draw_frame(&g_mv, pi, sw, sh);
            // EDA panel content
            Panel *p0 = multiview_get_panel(&g_mv, PANEL_EDA);
            if (p0 && !p0->minimized) {
                CanvasView cv = canvas_view_create(p0->x+2, p0->y+24, p0->w-4, p0->h-26);
                cv.zoom = app->zoom; cv.pan = app->pan;
                canvas_eda_background(&cv); canvas_eda_origin(&cv);
            }
            // CAD panel content
            Panel *p1 = multiview_get_panel(&g_mv, PANEL_CAD);
            if (p1 && !p1->minimized) {
                canvas_cad_workbench_tabs(p1->x+4, p1->y+28, p1->w-8);
                canvas_cad_feature_tree(app->feats, app->nfeats, app->sel_feat, p1->x+4, p1->y+50, p1->w-8, p1->h-56);
            }
            // Slicer panel content
            Panel *p2 = multiview_get_panel(&g_mv, PANEL_SLICER);
            if (p2 && !p2->minimized) {
                loopqueue_tick(&g_loopq, &g_eject, GetFrameTime());
                eject_anim_draw(&g_eject, p2->x+4, p2->y+24, p2->w-8, p2->h-26, 200, 200);
            }
            // Farm panel content
            Panel *p3 = multiview_get_panel(&g_mv, PANEL_FARM);
            if (p3 && !p3->minimized) {
                loopqueue_draw(&g_loopq, p3->x+4, p3->y+24, p3->w-8, p3->h-26);
                Farm *f = &app->farm;
                char buf[128];
                snprintf(buf,sizeof(buf),"Printers: %d | Jobs: %d | Conveyor: %s", f->n_printers, f->n_jobs, g_loopq.conveyor?"ON":"OFF");
                DrawText(buf, p3->x+8, p3->y+p3->h-22, 12, GRAY);
            }
            // status bar
            DrawRectangle(0, sh-24, sw, 24, (Color){ 20, 22, 26, 255 });
            DrawText(app->status?app->status:"", 8, sh-18, 13, LIGHTGRAY);
            char mvbuf[64]; snprintf(mvbuf,sizeof(mvbuf),"Multi-view: EDA+CAD+Slicer+Farm | 0:single | G:grid | F1:help");
            DrawText(mvbuf, sw-MeasureText(mvbuf,12)-8, sh-18, 12, DARKGRAY);
            EndDrawing();
            return;
        }

        // Normal single-view layout
        // Menu bar
        prof_menu_bar(0, 0, sw, 24);
        // Toolbar
        prof_toolbar_draw(app, 0, 24, sw, 26);

        // Layout: sidebar | canvas | properties
        int sbw = 250, propw = 200;
        int top_y = 50, bot_y = sh - 56;
        int cv_y = top_y, cv_h = sh - 56 - 100; // canvas
        int con_y = cv_y + cv_h, con_h = 100; // console

        draw_sidebar(app, 0, top_y, sbw, bot_y - top_y);
        prof_splitter_drag(&sbw, sbw, top_y, 180, 400);

        int vx = sbw + 4, vy = top_y + 4, vw = sw - sbw - propw - 12, vh = cv_h - 4;
        DrawRectangle(vx - 2, vy - 2, vw + 4, vh + 4, (Color){ 40, 42, 48, 255 });
        // draw per-mode canvas background
        CanvasView cv = canvas_view_create(vx, vy, vw, vh);
        cv.zoom = app->zoom; cv.pan = app->pan; cv.grid_size = grid_size_current();
        canvas_render(app, &cv);
        // canvas area mode switch
        switch (app->mode) {
            case UI_SCH: {
                Schematic *s = (app->sel_sch < app->proj.schematics.len) ? &app->proj.schematics.v[app->sel_sch] : NULL;
                if (s) {
                    draw_schematic(app, s, vx, vy + 36, vw, vh - 36);
                    if (ui_button("Simulate (DC)", vx + 4, vy + 4, 120, 26)) {
                        SpiceResult *sr = spice_solve_dc(s);
                        if (sr) { char *txt = spice_result_text(sr); free(app->status); app->status = txt; spice_result_free(sr); }
                        else { free(app->status); app->status = str_dup("SPICE: no valid circuit found (need R + V components)"); }
                    }
                    if (ui_button("ERC", vx + 132, vy + 4, 48, 26)) {
                        ErcReport er = erc_check(s);
                        char st[256]; int off = 0;
                        for (int e = 0; e < er.nissues && off < 250; e++)
                            off += snprintf(st + off, sizeof(st) - off, "%s | ", er.issues[e].message);
                        free(app->status); app->status = str_dup(st);
                        erc_report_free(&er);
                    }
    if (ui_button("MC", vx + 188, vy + 4, 42, 26)) {
        char *mc = spice_monte_carlo(s, 5.0f, 20);
        free(app->status); app->status = mc;
    }
    if (ui_button("Annot", vx + 236, vy + 4, 56, 26))
        { int c = annotations_auto_assign(s); char ms[64]; snprintf(ms, sizeof(ms), "annotated %d refs", c); free(app->status); app->status = str_dup(ms); }
    if (ui_button("Bus", vx + 298, vy + 4, 46, 26))
        { char *bs = bus_auto_detect(s); free(app->status); app->status = bs; }
    if (ui_button("Eye", vx + 350, vy + 4, 42, 26))
        { char *ey = eye_diagram_report(10.0f, 30.0f, 5.0f, 50.0f); free(app->status); app->status = ey; }
    if (ui_button("Hier", vx + 398, vy + 4, 44, 26))
        { char *hr = sheet_hierarchy_report(&app->proj); free(app->status); app->status = hr; }
    if (ui_button("Tut", vx + 448, vy + 4, 40, 26))
        { char *tt = tutorial_text(); free(app->status); app->status = tt; }
    if (ui_button("Intr", vx + 494, vy + 4, 46, 26))
        { char *ic = interact_list_components(s); free(app->status); app->status = ic; }
    if (ui_button("CvPcb", vx + 546, vy + 4, 52, 26))
        { int n = cvpcb_auto_assign(&app->proj); char ms[64]; snprintf(ms, sizeof(ms), "CvPcb: %d footprints assigned", n); free(app->status); app->status = str_dup(ms); }
    if (ui_button("Net", vx + 604, vy + 4, 40, 26))
        { netlist_export_all(s, "build"); free(app->status); app->status = str_dup("netlists exported (PADS+Allegro+KiCad)"); }
    if (ui_button("Val", vx + 650, vy + 4, 38, 26))
        { char *vc = validator_check(&app->proj); free(app->status); app->status = vc; }
                } else DrawText("drop a folder with .kicad_sch/.sch files (or press 1)", vx + 20, vy + 20, 16, GRAY);
                break;
            }
            case UI_PCB: {
                Pcb *pcb = (app->sel_pcb < app->proj.pcbs.len) ? &app->proj.pcbs.v[app->sel_pcb] : NULL;
                if (pcb) {
                    // footprint editing: select, drag, rotate, delete
                    Vector2 m2 = GetMousePosition();
                    float ft_rect_half = 8.0f * app->zoom;
                    bool in_view = m2.x >= vx && m2.x <= vx + vw && m2.y >= vy && m2.y <= vy + vh;
                    float vcx = vx + vw * 0.5f, vcy = vy + vh * 0.5f;
                    // handle de-select
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && in_view && app->pcb_fp_sel >= 0 && !app->pcb_fp_drag) {
                        int hit = -1;
                        for (int i = pcb->nfps - 1; i >= 0; i--) {
                            float sx = vcx + (pcb->fps[i].pos.x - app->pan.x) * app->zoom;
                            float sy = vcy + (pcb->fps[i].pos.y - app->pan.y) * app->zoom;
                            if (m2.x >= sx - ft_rect_half && m2.x <= sx + ft_rect_half &&
                                m2.y >= sy - ft_rect_half && m2.y <= sy + ft_rect_half) { hit = i; break; }
                        }
                        if (hit >= 0) { app->pcb_fp_sel = hit; app->pcb_fp_drag = 1; app->pcb_fp_start = pcb->fps[hit].pos; }
                        else { app->pcb_fp_sel = -1; }
                    } else if (app->pcb_fp_sel < 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && in_view) {
                        // first click to select
                        for (int i = pcb->nfps - 1; i >= 0; i--) {
                            float sx = vcx + (pcb->fps[i].pos.x - app->pan.x) * app->zoom;
                            float sy = vcy + (pcb->fps[i].pos.y - app->pan.y) * app->zoom;
                            if (m2.x >= sx - ft_rect_half && m2.x <= sx + ft_rect_half &&
                                m2.y >= sy - ft_rect_half && m2.y <= sy + ft_rect_half) {
                                app->pcb_fp_sel = i; app->pcb_fp_drag = 1; app->pcb_fp_start = pcb->fps[i].pos; break;
                            }
                        }
                    }
                    // drag
                    if (app->pcb_fp_sel >= 0 && app->pcb_fp_drag) {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                            V2 delta = { GetMouseDelta().x / app->zoom, GetMouseDelta().y / app->zoom };
                            pcb->fps[app->pcb_fp_sel].pos.x += delta.x;
                            pcb->fps[app->pcb_fp_sel].pos.y += delta.y;
                            free(app->status);
                            char dbg[128]; snprintf(dbg, sizeof(dbg), "FP %s @ (%.1f, %.1f)", pcb->fps[app->pcb_fp_sel].ref ? pcb->fps[app->pcb_fp_sel].ref : "?", pcb->fps[app->pcb_fp_sel].pos.x, pcb->fps[app->pcb_fp_sel].pos.y);
                            app->status = str_dup(dbg);
                        } else {
                            app->pcb_fp_drag = 0;
                            app->fit_dirty = true;
                        }
                    }
                    // rotate selected footprint (R key)
                    if (app->pcb_fp_sel >= 0 && IsKeyPressed(KEY_R)) {
                        pcb->fps[app->pcb_fp_sel].rotation = fmodf(pcb->fps[app->pcb_fp_sel].rotation + 90.0f, 360.0f);
                        free(app->status); app->status = str_dup("footprint rotated 90");
                        app->fit_dirty = true;
                    }
                    // delete selected footprint (X key)
                    if (app->pcb_fp_sel >= 0 && IsKeyPressed(KEY_X)) {
                        // shift remaining elements down
                        for (int k = app->pcb_fp_sel + 1; k < pcb->nfps; k++) pcb->fps[k - 1] = pcb->fps[k];
                        pcb->nfps--;
                        int idx = app->pcb_fp_sel;
                        app->pcb_fp_sel = -1; app->pcb_fp_drag = 0; app->pcb_fp_start = (V2){0};
                        free(app->status); app->status = str_dup("footprint deleted");
                        pcb->fps = (FpInst *)realloc(pcb->fps, sizeof(FpInst) * (size_t)pcb->nfps);
                        app->fit_dirty = true;
                    }
                    // ESC deselect
                    if (app->pcb_fp_sel >= 0 && IsKeyPressed(KEY_ESCAPE)) { app->pcb_fp_sel = -1; app->pcb_fp_drag = 0; }
                    // track routing mode
                    if (IsKeyPressed(KEY_T)) {
                        app->route_mode = !app->route_mode;
                        app->route_has_start = false;
                        free(app->status);
                        app->status = app->route_mode ? str_dup("routing mode: click to place track (ESC to cancel)") : str_dup("routing cancelled");
                    }
                    if (app->route_mode && IsKeyPressed(KEY_ESCAPE)) { app->route_mode = false; app->route_has_start = false; free(app->status); app->status = str_dup("routing cancelled"); }
                    if (app->route_mode && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && in_view) {
                        // convert screen to world coords
                        float wx = (m2.x - vcx) / app->zoom + app->pan.x;
                        float wy = (m2.y - vcy) / app->zoom + app->pan.y;
                        if (!app->route_has_start) {
                            app->route_start = v2(wx, wy);
                            app->route_has_start = true;
                            free(app->status); app->status = str_dup("routing: click endpoint");
                        } else {
                            // add track
                            Track tr = { v2(app->route_start.x, app->route_start.y), v2(wx, wy), 0.5f, NULL };
                            pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track) * (size_t)(pcb->ntracks + 1));
                            pcb->tracks[pcb->ntracks] = tr;
                            pcb->ntracks++;
                            app->route_start = v2(wx, wy); // chain from endpoint
                            free(app->status); app->status = str_dup("track added (click next, ESC to finish)");
                            app->fit_dirty = true;
                        }
                    }
                    // enclosure buttons
                    CadModel *enc = co_find_enclosure(&app->proj, pcb->id);
                    if (!enc) {
                        if (ui_button("Generate Enclosure", vx + 4, vy + 4, 170, 26)) {
                            int ai = co_design_enclosure(&app->proj, pcb->id, &app->enc_params);
                            if (ai >= 0) { app->encl_ready = true; app->cad_gen++; free(app->status); app->status = str_dup("enclosure generated"); }
                            else { free(app->status); app->status = str_dup("enclosure gen failed"); }
                        }
                    } else {
                        DrawText("Enclosure ready", vx + 4, vy + 8, 14, GREEN);
                        if (ui_button("Print", vx + 152, vy + 4, 56, 26)) {
                            unsigned long tck = (unsigned long)clock();
                            snprintf(app->last_gcode, sizeof(app->last_gcode), "build\\pcb_print_%lu.gcode", tck);
                            bool ok = co_design_print_enclosure(&app->proj, pcb->id, &app->slice_cfg, app->last_gcode);
                            free(app->status);
                            app->status = ok ? str_dup("enclosure printed!") : str_dup("print failed");
                        }
                    }
                    if (ui_button("DRC", vx + 220, vy + 4, 48, 26)) {
                        DrcReport dr = drc_check(pcb, 0.15f, 0.2f, 0.15f);
                        char st[512]; int off = 0;
                        for (int d = 0; d < dr.nissues && off < 500; d++)
                            off += snprintf(st + off, sizeof(st) - off, "%s | ", dr.issues[d].message);
                        free(app->status); app->status = str_dup(st);
                        drc_report_free(&dr);
                    }
                    if (ui_button("Calc", vx + 274, vy + 4, 48, 26)) {
                        float w = pcbcalc_track_width(1.0f, 1.0f, 10.0f);
                        float c = pcbcalc_current_capacity(0.5f, 1.0f, 10.0f);
                        char st[128]; snprintf(st, sizeof(st), "PCB Calc: 0.5mm track = %.2fA | 1A needs %.2fmm width (1oz, 10C rise)", c, w);
                        free(app->status); app->status = str_dup(st);
                    }
                    if (ui_button("Tdrop", vx + 328, vy + 4, 56, 26))
                        { int n = teardrop_add(pcb, 2.0f, 0.5f); free(app->status); app->status = str_dup(n > 0 ? "teardrops added" : "no pads found"); }
                    if (ui_button("Pour", vx + 390, vy + 4, 48, 26))
                        { int n = copper_pour_ground_plane(pcb, 2.0f, 1.5f, 3.0f); char ms[64]; snprintf(ms, sizeof(ms), "copper pour: %d segments", n); free(app->status); app->status = str_dup(ms); }
                    if (ui_button("Gerber", vx + 444, vy + 4, 54, 26))
                        { gerber_write_all(pcb, "build"); free(app->status); app->status = str_dup("Gerber/drill files written to build/"); }
                    if (ui_button("FabDw", vx + 504, vy + 4, 54, 26))
                        { fabdraw_export_manufacturing(pcb, "build"); free(app->status); app->status = str_dup("fab drawing + gerber exported"); }
                    if (ui_button("MFG", vx + 564, vy + 4, 48, 26))
                        { mfg_export_all(pcb, "build"); free(app->status); app->status = str_dup("P&P + IPC356 + Gerber exported"); }
                    if (ui_button("PDF", vx + 618, vy + 4, 42, 26))
                        { pdf_export_drawing(pcb, "build\\board.pdf"); free(app->status); app->status = str_dup("PDF exported"); }
                    if (ui_button("DXF", vx + 666, vy + 4, 42, 26))
                        { dxf_export_pcb(pcb, "build\\board.dxf"); free(app->status); app->status = str_dup("DXF exported"); }
                    if (ui_button("PwrI", vx + 714, vy + 4, 46, 26))
                        { char *pr = powerint_report(pcb); free(app->status); app->status = pr; }
                    if (ui_button("Nets", vx + 766, vy + 4, 46, 26))
                        { char *nr = nethilite_nets_report(pcb); free(app->status); app->status = nr; }
                    if (ui_button("Therm", vx + 564, vy + 4, 56, 26))
                        { char *tr = thermal_report(pcb, 25.0f); free(app->status); app->status = tr; }
                    if (ui_button("Via+", vx + 626, vy + 4, 46, 26))
                        { via_add_typed(pcb, v2(50,50), 0.8f, 1.6f, 0, 1, "vcc"); free(app->status); app->status = str_dup("blind via added"); }
                    if (ui_button("SigI", vx + 678, vy + 4, 46, 26))
                        { char *sr = si_report(pcb); free(app->status); app->status = sr; }
                    if (ui_button("Blk+", vx + 730, vy + 4, 48, 26))
                        { DesignBlock *b = block_save(pcb, v2(40,40), 20, 20, "block1"); block_restore(pcb, b, v2(60,40)); block_free(b); free(app->status); app->status = str_dup("design block saved/restored"); }
                    if (ui_button("SA", vx + 784, vy + 4, 36, 26))
                        { int m = place_simulated_annealing(pcb, 100, 0.1f, 0.95f, 50); char ms[64]; snprintf(ms, sizeof(ms), "SA placement: %d moves", m); free(app->status); app->status = str_dup(ms); }
                    if (ui_button("A*", vx + 826, vy + 4, 34, 26))
                        { V2 path[256]; int n = route_astar(pcb, v2(40,40), v2(60,60), 0.5f, 0.2f, path, 256); char ms[64]; snprintf(ms, sizeof(ms), "A* route: %d waypoints", n); free(app->status); app->status = str_dup(ms); }
                    if (ui_button("CTS", vx + 866, vy + 4, 40, 26))
                        { V2 sinks[4] = {v2(45,55),v2(55,55),v2(45,45),v2(55,45)}; int a = cts_build_htree(pcb, v2(50,50), sinks, 4, 0.15f); char ms[64]; snprintf(ms, sizeof(ms), "H-tree: %d segments", a); free(app->status); app->status = str_dup(ms); }
                    if (ui_button("FP", vx + 912, vy + 4, 34, 26))
                        { float bw[3]={20,15,10}, bh[3]={15,10,10}; V2 pos[3]; floorplan_bstar(bw,bh,3,pos,100,50); char ms[64]; snprintf(ms, sizeof(ms), "Floorplan: 3 blocks placed"); free(app->status); app->status = str_dup(ms); }
                    if (ui_button("Buf", vx + 952, vy + 4, 36, 26))
                        { V2 bp = timing_optimal_buffer_pos(v2(0,0), v2(100,0), 50); char ms[64]; snprintf(ms, sizeof(ms), "Buffer at (%.0f,%.0f)", bp.x, bp.y); free(app->status); app->status = str_dup(ms); }
                    if (ui_button("DRC+", vx + 994, vy + 4, 46, 26)) {
                        DRCViolation v[32]; int n = drc_enhanced_check(pcb, 0.15f, 0.1f, 0.15f, v, 32);
                        char ms[256]; snprintf(ms, sizeof(ms), "DRC+: %d violations", n);
                        free(app->status); app->status = str_dup(ms);
                    }
                    if (ui_button("PG", vx + 1046, vy + 4, 34, 26))
                        { char *pg = power_grid_analysis(pcb, 3.3f, 2.0f); free(app->status); app->status = pg; }
                    if (ui_button("PS", vx + 1086, vy + 4, 34, 26))
                        { int s = pushshove_route(pcb, v2(50,48), v2(58,52), 0.2f, 0.15f, 20); char ms[32]; snprintf(ms, sizeof(ms), "Push: %d shoved", s); free(app->status); app->status = str_dup(ms); }
                    if (ui_button("Rte", vx + 1126, vy + 4, 38, 26))
                        { int s = pcb_route_interactive(pcb, v2(48,48), v2(60,60), 0.2f, 0.15f, 50); char ms[64]; snprintf(ms, sizeof(ms), "Interactive route: %d shoved", s); free(app->status); app->status = str_dup(ms); }
                    if (ui_button("CG", vx + 1126, vy + 4, 34, 26)) {
                        Schematic *s = (app->sel_sch < app->proj.schematics.len) ? &app->proj.schematics.v[app->sel_sch] : NULL;
                        if (s) { ConnNode nodes[256]; int nc = conngraph_build(s, nodes, 256); char ms[64]; snprintf(ms, sizeof(ms), "ConnGraph: %d nets", nc); free(app->status); app->status = str_dup(ms); }
                    }
                    if (ui_button("PushRt", vx + 504, vy + 4, 58, 26))
                        { pushroute_add(pcb, v2(45,45), v2(55,55), 0.3f, 0.2f); free(app->status); app->status = str_dup("push-route test track added"); }
                    if (ui_button("DifRt", vx + 568, vy + 4, 50, 26))
                        { difroute_add_pair(pcb, v2(40,48), v2(60,48), v2(40,52), v2(60,52), 0.3f, 0.5f); free(app->status); app->status = str_dup("differential pair added"); }
                    if (ui_button("Tune", vx + 624, vy + 4, 52, 26))
                        { float l = lengthtune_net_length(pcb, NULL); char ms[64]; snprintf(ms, sizeof(ms), "total length: %.1f mm (%.2f ns)", l, lengthtune_delay_ns(l)); free(app->status); app->status = str_dup(ms); }
                    draw_pcb(app, pcb, vx, vy + 36, vw, vh - 36);
                } else DrawText("drop a folder with .kicad_pcb/.brd files (or press 2)", vx + 20, vy + 20, 16, GRAY);
                break;
            }
            case UI_3D: {
                draw_3d(app, vx, vy, vw, vh);
                CadModel *cm3 = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len)
                                  ? &app->proj.cad_models.v[app->sel_cad] : NULL;
                if (cm3 && ui_button("Print This Model", vx + 4, vy + 4, 160, 28)) {
                    unsigned long tck = (unsigned long)clock();
                    snprintf(app->last_gcode, sizeof(app->last_gcode), "build\\3dprint_%lu.gcode", tck);
                    bool ok = co_design_print_cad(&app->proj, cm3->id, &app->slice_cfg, app->last_gcode);
                    free(app->status);
                    app->status = ok ? str_dup("CAD model printed!") : str_dup("print failed");
                }
                if (ui_button("Explode 2x", vx + 170, vy + 4, 100, 28)) {
                    if (app->proj.assemblies.len > 0) {
                        assembly_explode(&app->proj.assemblies.v[0], 2.0f);
                        free(app->status); app->status = str_dup("assembly exploded 2x");
                    }
                }
                if (ui_button("Baln", vx + 276, vy + 4, 50, 28)) {
                    if (app->proj.assemblies.len > 0) {
                        char *b = balloons_text(&app->proj.assemblies.v[0]);
                        free(app->status); app->status = b;
                    }
                }
                if (ui_button("Cont", vx + 332, vy + 4, 50, 28)) {
                    if (app->proj.assemblies.len > 0) {
                        char *cr = contact_report(&app->proj.assemblies.v[0], &app->proj);
                        free(app->status); app->status = cr;
                    }
                }
                break;
            }
            case UI_FIT: draw_fit(app, &app->fit, vx, vy, vw, vh); break;
            case UI_DESIGN: draw_design(app, vx, vy, vw, vh); break;
            case UI_ASSIST: draw_assist(app, vx, vy, vw, vh); break;
            case UI_PRINT: draw_print(app, vx, vy, vw, vh); break;
        }
        // Properties panel (right)
        prof_properties_draw(app, sw - propw, top_y, propw, bot_y - top_y - con_h);
        // Console panel (bottom)
        prof_console_draw(sbw + 4, con_y, sw - sbw - propw - 8, con_h);
        // Layers panel (right bottom)
        prof_layers_draw(app, sw - propw, con_y, propw, con_h);

        // status bar
        DrawRectangle(0, sh - 28, sw, 28, (Color){ 20, 22, 26, 255 });
        DrawText(app->status ? app->status : "", 8, sh - 20, 14, LIGHTGRAY);
        if (app->mode == UI_SCH || app->mode == UI_PCB) {
            char cbuf[64]; snprintf(cbuf, sizeof(cbuf), "X:%.1f Y:%.1f | Grid:%.1f", 0.0, 0.0, grid_size_current());
            DrawText(cbuf, sw - MeasureText(cbuf, 12) - 10, sh - 38, 12, (Color){160,170,180,255});
        }
        const char *modes = "1:SCH 2:PCB 3:3D 4:FIT 5:DESIGN 6:ASSIST 7:PRINT F5:Parse R:Joint-demo";
        DrawText(modes, GetScreenWidth() - MeasureText(modes, 14) - 10, GetScreenHeight() - 20, 14, DARKGRAY);
        // dashboard summary line
        if (autosave_seconds_since_save() > 300)
            { char *ds = dashboard_summary(&app->proj, &app->farm); DrawText(ds, 8, GetScreenHeight() - 44, 12, DARKGRAY); free(ds); }
        // help overlay
        if (app->show_help) {
            int sw = GetScreenWidth(), sh = GetScreenHeight();
            DrawRectangle(0, 0, sw, sh, (Color){ 0, 0, 0, 200 });
            int mx = sw / 2 - 340, my = 40;
            DrawRectangle(mx - 4, my - 4, 688, sh - 88, (Color){ 24, 26, 34, 255 });
            DrawRectangleLines(mx - 4, my - 4, 688, sh - 88, (Color){ 80, 85, 100, 255 });
            DrawText("Keyboard Shortcuts", mx + 12, my + 8, 20, WHITE);
            DrawText("Press ? / Esc to close", mx + 500, my + 12, 14, DARKGRAY);
            my += 36;
            const char *lines[] = {
                "1  Schematic     2D canvas view of schematics",
                "2  PCB            2D canvas view of boards  (click footprint to drag)",
                "3  3D View        Orbit/zoom 3D view  (click model to select)",
                "4  Fit Report     Assembly + PCB clearance checks",
                "5  Design         Parametric design + generative topology opt",
                "6  Assistant      Natural-language part creation",
                "7  Print Center   Enclosure gen + Slicer + Print Farm",
                "",
                "Global:",
                "  ? / F1   This help overlay            F5   Parse datasheets (Ollama)",
                "  R        Build joint demo (3D mode)   F     Zoom to selected model (3D)",
                "",
                "Navigation:",
                "  Middle-drag   Pan canvas (2D) / Orbit (3D)",
                "  Mouse wheel   Zoom canvas (2D) / Camera distance (3D)",
                "",
                "PCB Editing (mode 2):",
                "  Click footprint   Start dragging      Release   Place footprint",
                "",
                "3D View (mode 3):",
                "  Left-click model  Select              F key    Focus on selection",
                "",
                "Print Center (mode 7):",
                "  Generate Enclosure  Build enclosure for current PCB",
                "  Print Enclosure     Slice + emit G-code for enclosure",
                "  Print CAD Model     Slice + emit G-code for selected CAD",
                "  Save Settings       Persist slicer settings to disk",
                "",
                NULL
            };
            for (int i = 0; lines[i]; i++) {
                if (lines[i][0] == '\0') { my += 10; continue; }
                Color c = (lines[i][0] >= '1' && lines[i][0] <= '7') ? YELLOW : LIGHTGRAY;
                DrawText(lines[i], mx + 12, my, 13, c);
                my += 17;
            }
        }
        EndDrawing();
}

void app_run(App *app) {
    while (!WindowShouldClose()) app_frame(app);
}

// ================= sidebar =================
static void draw_section(const char *title, int count, int x, int y) {
    char buf[128];
    snprintf(buf, sizeof(buf), "%s (%d)", title, count);
    DrawText(buf, x + 8, y, 14, (Color){ 180, 190, 200, 255 });
}

static void draw_sidebar(App *app, int x, int y, int w, int h) {
    DrawRectangle(x, y, w, h, (Color){ 24, 26, 30, 255 });
    int py = y + 8;
    char buf[256];
    snprintf(buf, sizeof(buf), "Project: %s", app->proj.name);
    DrawText(buf, x + 8, py, 14, WHITE); py += 22;
    // search/filter
    DrawText("Search:", x + 8, py, 12, DARKGRAY);
    DrawText(app->sidebar_filter, x + 56, py, 13, app->sidebar_filter[0] ? WHITE : GRAY);
    // handle typing in sidebar
    if (IsKeyPressed(KEY_BACKSPACE) && app->sidebar_filter[0]) app->sidebar_filter[strlen(app->sidebar_filter)-1] = '\0';
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 126) {
            int l = (int)strlen(app->sidebar_filter);
            if (l < 62) { app->sidebar_filter[l] = (char)key; app->sidebar_filter[l+1] = '\0'; }
        }
        key = GetCharPressed();
    }
    py += 20;
    const char *flt = app->sidebar_filter;
    int flt_active = flt[0] ? 1 : 0;
    // helper macro for filtered display
    #define SIDEBAR_ITEM(name_str, idx, sel_field) \
        if (!flt_active || strstr(name_str, flt)) { \
            DrawText(name_str, x + 16, py, 13, (sel_field) == idx ? YELLOW : GRAY); py += 16; \
        }
    if (app->proj.docs.len || app->proj.components.len || app->proj.schematics.len || app->proj.pcbs.len || app->proj.cad_models.len) {
        // docs
        draw_section("Datasheets", app->proj.docs.len, x, py); py += 16;
        for (int i = 0; i < app->proj.docs.len && py < y + h; i++)
            { const char *n = app->proj.docs.v[i].name; SIDEBAR_ITEM(n, i, app->sel_doc) }
        draw_section("Components", app->proj.components.len, x, py); py += 16;
        for (int i = 0; i < app->proj.components.len && py < y + h; i++)
            { const char *n = app->proj.components.v[i].name; SIDEBAR_ITEM(n, i, app->sel_comp) }
        draw_section("Schematics", app->proj.schematics.len, x, py); py += 16;
        for (int i = 0; i < app->proj.schematics.len && py < y + h; i++)
            { const char *n = app->proj.schematics.v[i].name; SIDEBAR_ITEM(n, i, app->sel_sch) }
        draw_section("PCBs", app->proj.pcbs.len, x, py); py += 16;
        for (int i = 0; i < app->proj.pcbs.len && py < y + h; i++)
            { const char *n = app->proj.pcbs.v[i].name; SIDEBAR_ITEM(n, i, app->sel_pcb) }
        draw_section("CAD Models", app->proj.cad_models.len, x, py); py += 16;
        for (int i = 0; i < app->proj.cad_models.len && py < y + h; i++)
            { const char *n = app->proj.cad_models.v[i].name ? app->proj.cad_models.v[i].name : "?"; SIDEBAR_ITEM(n,i,app->sel_cad) }
    } else {
        DrawText("Drag & drop a folder", x + 8, py, 13, GRAY); py += 18;
        DrawText("with .kicad_* / .sch", x + 8, py, 13, GRAY); py += 18;
        DrawText("/ .brd / .stl / .step", x + 8, py, 13, GRAY); py += 18;
        DrawText("/ .csv / .docx files", x + 8, py, 13, GRAY); py += 18;
        DrawText("to import it.", x + 8, py, 13, GRAY);
    }
}

// ================= schematic 2D =================
static V2 to_screen(V2 p, App *app, int x, int y, int w, int h) {
    return (V2){ x + w * 0.5f + (p.x - app->pan.x) * app->zoom,
                 y + h * 0.5f + (p.y - app->pan.y) * app->zoom };
}

static void draw_schematic(App *app, Schematic *s, int x, int y, int w, int h) {
    // draw grid (light lines at grid spacing)
    float gs = grid_size_current();
    if (gs * app->zoom >= 4.0f) {
        float ox = fmodf(app->pan.x, gs) * app->zoom;
        float oy = fmodf(app->pan.y, gs) * app->zoom;
        for (float gx = -ox; gx < w; gx += gs * app->zoom) DrawLine(x+(int)gx,y,x+(int)gx,y+h,(Color){50,52,56,255});
        for (float gy = -oy; gy < h; gy += gs * app->zoom) DrawLine(x,y+(int)gy,x+w,y+(int)gy,(Color){50,52,56,255});
    }
    // junction dots
    V2 junctions[256]; int nj = junctions_find(s, junctions, 256, 5.0f);
    // instances as rectangles with pin markers
    for (int i = 0; i < s->ninsts; i++) {
        SymInst *si = &s->insts[i];
        V2 sc = to_screen(si->pos, app, x, y, w, h);
        float rw = 20 * app->zoom, rh = 14 * app->zoom;
        DrawRectangleLines((int)(sc.x-rw),(int)(sc.y-rh),(int)(rw*2),(int)(rh*2),(Color){100,180,255,255});
        if (si->ref) DrawText(si->ref, (int)(sc.x-rw+2), (int)(sc.y-rh), (int)(10*app->zoom > 8 ? 10*app->zoom : 8), WHITE);
        if (si->value && si->value[0]) DrawText(si->value, (int)(sc.x-rw+2), (int)(sc.y+rh-4), (int)(8*app->zoom > 6 ? 8*app->zoom : 6), GRAY);
        // pin dots on left/right
        int npins = 2; if (si->symbol) { Symbol *sym = project_find_symbol(&app->proj, si->symbol); if (sym) npins = sym->npins; }
        for (int p = 0; p < npins && p < 8; p++) {
            int py = (int)(sc.y - rh + (p+1)*app->zoom*6);
            DrawCircle((int)(sc.x - rw), py, 2.5f * app->zoom > 1 ? 2.5f * app->zoom : 1, GREEN);
        }
    }
    for (int i = 0; i < s->nwires; i++) {
        V2 a = to_screen(s->wires[i].a, app, x, y, w, h);
        V2 b = to_screen(s->wires[i].b, app, x, y, w, h);
        DrawLineEx((Vector2){a.x,a.y}, (Vector2){b.x,b.y}, 2.0f, GREEN);
    }
    for (int i = 0; i < nj; i++) {
        V2 jp = to_screen(junctions[i], app, x, y, w, h);
        DrawCircleV((Vector2){jp.x,jp.y}, 4.0f, GREEN);
        DrawCircleV((Vector2){jp.x,jp.y}, 2.5f, (Color){28,30,34,255});
    }
    for (int i = 0; i < s->nlabels; i++) {
        V2 sc = to_screen(s->labels[i].pos, app, x, y, w, h);
        DrawText(s->labels[i].name, (int)sc.x+4, (int)sc.y-8, 12, YELLOW);
    }
}

// ================= pcb 2D =================
static void draw_pcb(App *app, Pcb *pcb, int x, int y, int w, int h) {
    grid_draw_kiCad_style(x, y, w, h, app->zoom, app->pan, grid_size_current());
    // ratsnest lines
    V2 rfrom[256], rto[256]; int rn = ratsnest_compute(pcb, rfrom, rto, 256);
    for (int i = 0; i < rn; i++) {
        V2 a = to_screen(rfrom[i], app, x, y, w, h);
        V2 b = to_screen(rto[i], app, x, y, w, h);
        DrawLineEx((Vector2){a.x,a.y}, (Vector2){b.x,b.y}, 1.0f, (Color){255,255,255,80});
    }
    // board outline
    LayerColor lc = layer_color_get(5); // Edge.Cuts
    for (int i = 0; i < pcb->noutline; i++) {
        V2 a = to_screen(pcb->outline[i], app, x, y, w, h);
        V2 b = to_screen(pcb->outline[(i + 1) % pcb->noutline], app, x, y, w, h);
        DrawLineEx((Vector2){a.x,a.y}, (Vector2){b.x,b.y}, 3.0f, (Color){lc.r,lc.g,lc.b,lc.a});
    }
    // tracks with copper color
    LayerColor cu = layer_color_get(0); // F.Cu
    for (int i = 0; i < pcb->ntracks; i++) {
        V2 a = to_screen(pcb->tracks[i].a, app, x, y, w, h);
        V2 b = to_screen(pcb->tracks[i].b, app, x, y, w, h);
        DrawLineEx((Vector2){a.x,a.y}, (Vector2){b.x,b.y}, fmaxf(pcb->tracks[i].width * app->zoom, 2), (Color){cu.r,cu.g,cu.b,cu.a});
    }
    // vias
    for (int i = 0; i < pcb->nvias; i++) {
        V2 sc = to_screen(pcb->vias[i].pos, app, x, y, w, h);
        via_draw(sc, pcb->vias[i].outer, pcb->vias[i].drill, (int)sc.x, (int)sc.y, app->zoom);
    }
    // footprints / pads
    for (int i = 0; i < pcb->nfps; i++) {
        FpInst *f = &pcb->fps[i];
        V2 sc = to_screen(f->pos, app, x, y, w, h);
        bool sel = (app->pcb_fp_sel == i);
        Color fc = sel ? YELLOW : (Color){cu.r,cu.g,cu.b,255};
        float rw = 16 * app->zoom, rh = 16 * app->zoom;
        if (f->rotation != 0 && f->rotation != 180) { float t = rw; rw = rh; rh = t; }
        DrawRectangleLines((int)(sc.x - rw * 0.5f), (int)(sc.y - rh * 0.5f), (int)rw, (int)rh, fc);
        if (sel) DrawRectangleLines((int)(sc.x - rw * 0.5f - 2), (int)(sc.y - rh * 0.5f - 2), (int)(rw + 4), (int)(rh + 4), YELLOW);
        char buf[128];
        snprintf(buf, sizeof(buf), "%s", f->ref ? f->ref : "?");
        DrawText(buf, (int)sc.x + 6, (int)sc.y - 10, 12, WHITE);
        if (sel && f->rotation != 0) {
            char rb[32]; snprintf(rb, sizeof(rb), "%.0f", f->rotation);
            DrawText(rb, (int)sc.x + 6, (int)sc.y + 2, 10, YELLOW);
        }
    }
    // route preview
    if (app->route_mode && app->route_has_start) {
        V2 rp = to_screen(app->route_start, app, x, y, w, h);
        Vector2 mp = GetMousePosition();
        DrawLine((int)rp.x, (int)rp.y, (int)mp.x, (int)mp.y, YELLOW);
        DrawCircleV((Vector2){ rp.x, rp.y }, 4, YELLOW);
    }
}

// ================= 3D =================
static float ray_tri_hit(Ray ray, Vector3 v0, Vector3 v1, Vector3 v2) {
    Vector3 e1 = Vector3Subtract(v1, v0);
    Vector3 e2 = Vector3Subtract(v2, v0);
    Vector3 h = Vector3CrossProduct(ray.direction, e2);
    float a = Vector3DotProduct(e1, h);
    if (fabsf(a) < 1e-6f) return -1.0f;
    float f = 1.0f / a;
    Vector3 s = Vector3Subtract(ray.position, v0);
    float u = f * Vector3DotProduct(s, h);
    if (u < 0.0f || u > 1.0f) return -1.0f;
    Vector3 q = Vector3CrossProduct(s, e1);
    float v = f * Vector3DotProduct(ray.direction, q);
    if (v < 0.0f || u + v > 1.0f) return -1.0f;
    float t = f * Vector3DotProduct(e2, q);
    return t > 1e-6f ? t : -1.0f;
}

static Vector3 v3_mul_mat(Vector3 v, const float *m) {
    return (Vector3){
        m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12],
        m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13],
        m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14]
    };
}

static void draw_3d(App *app, int x, int y, int w, int h) {
    Camera3D cam = { 0 };
    cam.position = (Vector3){ 0, 0, 0 };
    cam.target = (Vector3){ app->pan.x, app->pan.y, 0 };
    cam.up = (Vector3){ 0, 1, 0 };
    cam.fovy = 45.0f;
    float rad_y = app->yaw * DEG2RAD;
    float rad_x = app->pitch * DEG2RAD;
    cam.position.x = cam.target.x + app->cam_dist * cosf(rad_x) * sinf(rad_y);
    cam.position.y = cam.target.y + app->cam_dist * sinf(rad_x);
    cam.position.z = cam.target.z + app->cam_dist * cosf(rad_x) * cosf(rad_y);

    // 3D click-to-select raycast
    Vector2 mpos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        mpos.x >= x && mpos.x <= x + w && mpos.y >= y && mpos.y <= y + h) {
        Ray ray = GetMouseRay(mpos, cam);
        float best_t = 1e30f; int best_ci = -1;
        // test assembly item models
        for (int ai = 0; ai < app->proj.assemblies.len; ai++) {
            Assembly *as = &app->proj.assemblies.v[ai];
            for (int j = 0; j < as->nitems; j++) {
                CadModel *cm = as->items[j].part_id ? project_find_cad(&app->proj, as->items[j].part_id) : NULL;
                if (!cm || !cm->mesh.valid) continue;
                float wm[16]; joint_world(as, j, wm);
                int nv = cm->mesh.nverts, nt = cm->mesh.ntris;
                float *pos = cm->mesh.pos; int *idx = cm->mesh.idx;
                for (int t = 0; t < nt; t++) {
                    Vector3 v0 = { pos[idx[t*3+0]*3], pos[idx[t*3+0]*3+1], pos[idx[t*3+0]*3+2] };
                    Vector3 v1 = { pos[idx[t*3+1]*3], pos[idx[t*3+1]*3+1], pos[idx[t*3+1]*3+2] };
                    Vector3 v2 = { pos[idx[t*3+2]*3], pos[idx[t*3+2]*3+1], pos[idx[t*3+2]*3+2] };
                    float ht = ray_tri_hit(ray, v3_mul_mat(v0, wm), v3_mul_mat(v1, wm), v3_mul_mat(v2, wm));
                    if (ht > 0 && ht < best_t) {
                        best_t = ht;
                        for (int k = 0; k < app->proj.cad_models.len; k++)
                            if (strcmp(app->proj.cad_models.v[k].id, cm->id) == 0) { best_ci = k; break; }
                    }
                }
            }
        }
        // test loose cad models
        for (int i = 0; i < app->proj.cad_models.len; i++) {
            CadModel *cm = &app->proj.cad_models.v[i];
            if (!cm->mesh.valid) continue;
            int placed = 0;
            for (int ai = 0; ai < app->proj.assemblies.len && !placed; ai++)
                for (int j = 0; j < app->proj.assemblies.v[ai].nitems; j++)
                    if (app->proj.assemblies.v[ai].items[j].part_id &&
                        strcmp(app->proj.assemblies.v[ai].items[j].part_id, cm->id) == 0) { placed = 1; break; }
            if (placed) continue;
            int nt = cm->mesh.ntris; float *pos = cm->mesh.pos; int *idx = cm->mesh.idx;
            for (int t = 0; t < nt; t++) {
                Vector3 v0 = { pos[idx[t*3+0]*3], pos[idx[t*3+0]*3+1], pos[idx[t*3+0]*3+2] };
                Vector3 v1 = { pos[idx[t*3+1]*3], pos[idx[t*3+1]*3+1], pos[idx[t*3+1]*3+2] };
                Vector3 v2 = { pos[idx[t*3+2]*3], pos[idx[t*3+2]*3+1], pos[idx[t*3+2]*3+2] };
                float ht = ray_tri_hit(ray, v0, v1, v2);
                if (ht > 0 && ht < best_t) { best_t = ht; best_ci = i; }
            }
        }
        if (best_ci >= 0) app->sel_cad = best_ci;
    }

    // distance measurement tool
    if (app->measure_mode && app->mode == UI_PRINT) {} // stub guard
    if (app->measure_mode) {
        Vector2 mm = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mm.x >= x && mm.x <= x + w && mm.y >= y && mm.y <= y + h) {
            Ray ray2 = GetMouseRay(mm, cam);
            float t2 = -ray2.position.y / ray2.direction.y;
            if (t2 > 0) {
                V3 pt = v3(ray2.position.x + t2 * ray2.direction.x,
                          ray2.position.y + t2 * ray2.direction.y,
                          ray2.position.z + t2 * ray2.direction.z);
                if (app->meas_step == 0) { app->meas_a = pt; app->meas_step = 1; }
                else { app->meas_b = pt; app->meas_step = 2; }
            }
        }
        if (app->meas_step == 2 && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) app->meas_step = 0;
    }

    BeginScissorMode(x, y, w, h);
    DrawGrid(20, 10.0f);
    BeginMode3D(cam);
    // axis crosshair at origin
    float ax_len = 15.0f;
    DrawLine3D((Vector3){0,0,0}, (Vector3){ax_len,0,0}, RED);
    DrawLine3D((Vector3){0,0,0}, (Vector3){0,ax_len,0}, GREEN);
    DrawLine3D((Vector3){0,0,0}, (Vector3){0,0,ax_len}, BLUE);
    // assemblies (with joints) render first
    for (int ai = 0; ai < app->proj.assemblies.len; ai++) {
        Assembly *as = &app->proj.assemblies.v[ai];
        if (as->animated) assembly_tick(as, GetFrameTime());
        for (int i = 0; i < as->nitems; i++) {
            PlaceItem *it = &as->items[i];
            CadModel *cm = it->part_id ? project_find_cad(&app->proj, it->part_id) : NULL;
            if (!cm) continue;
            Model *m = cache_get(app, app->cad_gen, cm, NULL);
            if (!m) continue;
            float world[16];
            joint_world(as, i, world);
            bool is_sel = false;
            for (int k = 0; k < app->proj.cad_models.len; k++)
                if (k == app->sel_cad && strcmp(app->proj.cad_models.v[k].id, cm->id) == 0) is_sel = true;
            rlPushMatrix();
            rlMultMatrixf(world);
            Color ac = is_sel ? (Color){ 255, 200, 100, 255 } : (Color){ 190, 150, 90, 255 };
            if (app->wireframe_3d) DrawModelWires(*m, (Vector3){ 0, 0, 0 }, 1.0f, ac);
            else DrawModel(*m, (Vector3){ 0, 0, 0 }, 1.0f, ac);
            rlPopMatrix();
        }
    }
    // loose cad models (skip ones referenced by an assembly item)
    for (int i = 0; i < app->proj.cad_models.len; i++) {
        CadModel *cm = &app->proj.cad_models.v[i];
        int placed = 0;
        for (int ai = 0; ai < app->proj.assemblies.len && !placed; ai++)
            for (int j = 0; j < app->proj.assemblies.v[ai].nitems; j++)
                if (app->proj.assemblies.v[ai].items[j].part_id &&
                    strcmp(app->proj.assemblies.v[ai].items[j].part_id, cm->id) == 0) { placed = 1; break; }
        if (placed) continue;
        Model *m = cache_get(app, app->cad_gen, cm, NULL);
        if (!m) continue;
        bool is_sel = (i == app->sel_cad);
        Color mc = is_sel ? (Color){ 255, 220, 140, 255 } : (Color){ 140, 170, 220, 255 };
        if (app->wireframe_3d) DrawModelWires(*m, (Vector3){ 0, 0, 0 }, 1.0f, mc);
        else DrawModel(*m, (Vector3){ 0, 0, 0 }, 1.0f, mc);
        Vector3 lo = { cm->mesh.bmin.x, cm->mesh.bmin.y, cm->mesh.bmin.z };
        Vector3 hi = { cm->mesh.bmax.x, cm->mesh.bmax.y, cm->mesh.bmax.z };
        DrawBoundingBox((BoundingBox){ lo, hi }, is_sel ? YELLOW : GREEN);
    }
    // measurement line
    if (app->measure_mode && app->meas_step >= 1) {
        Vector3 la = { app->meas_a.x, app->meas_a.y, app->meas_a.z };
        DrawSphere(la, 1.5f, (app->meas_step == 1) ? YELLOW : GREEN);
        if (app->meas_step == 2) {
            Vector3 lb = { app->meas_b.x, app->meas_b.y, app->meas_b.z };
            DrawSphere(lb, 1.5f, GREEN);
            DrawLine3D(la, lb, YELLOW);
        }
    }
    EndMode3D();
    EndScissorMode();
    // measurement distance overlay
    if (app->measure_mode && app->meas_step == 2) {
        float dist = sqrtf(
            (app->meas_a.x - app->meas_b.x) * (app->meas_a.x - app->meas_b.x) +
            (app->meas_a.y - app->meas_b.y) * (app->meas_a.y - app->meas_b.y) +
            (app->meas_a.z - app->meas_b.z) * (app->meas_a.z - app->meas_b.z));
        char db[64]; snprintf(db, sizeof(db), "Distance: %.1f mm  (right-click to clear)", dist);
        DrawText(db, x + 8, y + 8, 14, YELLOW);
    } else if (app->measure_mode) {
        DrawText("M: measure  (click ground, right-click clear)", x + 8, y + 8, 14, YELLOW);
    }
    // model info overlay
    if (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) {
        CadModel *cm = &app->proj.cad_models.v[app->sel_cad];
        if (cm->mesh.valid) {
            char bi[256];
            snprintf(bi, sizeof(bi), "%s  %d verts, %d tris  bbox: %.0fx%.0fx%.0f",
                     cm->name ? cm->name : "?", cm->mesh.nverts, cm->mesh.ntris,
                     cm->mesh.bmax.x - cm->mesh.bmin.x,
                     cm->mesh.bmax.y - cm->mesh.bmin.y,
                     cm->mesh.bmax.z - cm->mesh.bmin.z);
            DrawText(bi, x + 8, y + h - 22, 13, YELLOW);
        }
    }
}

// ================= fit =================
static void run_fit(App *app) {
    fit_report_free(&app->fit);
    // analyze selected pcb, else first pcb; assembly if any
    Pcb *pcb = (app->sel_pcb < app->proj.pcbs.len) ? &app->proj.pcbs.v[app->sel_pcb] : (app->proj.pcbs.len ? &app->proj.pcbs.v[0] : NULL);
    if (pcb) fit_analyze_pcb(&app->proj, pcb, &app->fit);
    if (app->proj.assemblies.len) fit_analyze_assembly(&app->proj, &app->proj.assemblies.v[0], &app->fit);
    app->fit_dirty = false;
}

static void draw_fit(App *app, FitReport *rep, int x, int y, int w, int h) {
    DrawText("Fit Analysis (press 4 to refresh)", x + 12, y + 12, 16, WHITE);
    int py = y + 40;
    for (int i = 0; i < rep->nissues && py < y + h; i++) {
        FitIssue *it = &rep->issues[i];
        Color c = it->level == FIT_ERROR ? RED : (it->level == FIT_WARNING ? ORANGE : GREEN);
        char buf[512];
        snprintf(buf, sizeof(buf), "[%s] %s", it->level == FIT_ERROR ? "ERR" : (it->level == FIT_WARNING ? "WARN" : " OK "), it->message);
        DrawText(buf, x + 12, py, 14, c);
        py += 20;
    }
    if (rep->nissues == 0) {
        DrawText("No issues. Import a folder with a PCB / assembly first.", x + 12, py, 14, GRAY);
    }
}

// ================= parametric design workspace =================
// simple immediate-mode button; returns true when clicked
static bool ui_button(const char *label, int x, int y, int w, int h) {
    Vector2 m = GetMousePosition();
    bool hover = m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h;
    DrawRectangle(x, y, w, h, hover ? (Color){ 70, 90, 130, 255 } : (Color){ 55, 60, 75, 255 });
    DrawRectangleLines(x, y, w, h, (Color){ 120, 130, 150, 255 });
    DrawText(label, x + 6, y + (h - 14) / 2, 14, WHITE);
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void design_add_feature(App *app, int type) {
    if (app->nfeats >= 32) return;
    DesignFeature *f = &app->feats[app->nfeats++];
    memset(f, 0, sizeof(*f));
    f->type = type;
    f->segments = 24;
    f->material_id = app->sel_mat;
    switch (type) {
        case FEAT_BOX: f->w = 40; f->h = 40; f->d = 40; break;
        case FEAT_CYLINDER: f->w = 20; f->h = 40; f->d = 20; break;
        case FEAT_SPHERE: f->w = 20; f->h = 20; f->d = 20; break;
    }
    app->sel_feat = app->nfeats - 1;
    design_rebuild(app);
}

static void design_delete_feature(App *app) {
    if (app->nfeats <= 0) return;
    if (app->sel_feat < app->nfeats - 1) {
        memmove(&app->feats[app->sel_feat], &app->feats[app->sel_feat + 1],
                sizeof(DesignFeature) * (size_t)(app->nfeats - app->sel_feat - 1));
    }
    app->nfeats--;
    if (app->sel_feat >= app->nfeats) app->sel_feat = app->nfeats - 1;
    design_rebuild(app);
}

static void design_duplicate_feature(App *app) {
    if (app->nfeats >= 32 || app->sel_feat < 0 || app->sel_feat >= app->nfeats) return;
    app->feats[app->nfeats] = app->feats[app->sel_feat];
    app->feats[app->nfeats].x += 20.0f; // offset to make it visible
    app->nfeats++;
    app->sel_feat = app->nfeats - 1;
    design_rebuild(app);
}

// rebuild the whole cad_models list from design features
static void design_rebuild(App *app) {
    // drop previously generated design:xxx models
    for (int i = (int)app->proj.cad_models.len - 1; i >= 0; i--) {
        CadModel *cm = &app->proj.cad_models.v[i];
        if (cm->source_file && strcmp(cm->source_file, "design:parametric") == 0) {
            cad_free_contents(cm);
            for (int j = i; j < (int)app->proj.cad_models.len - 1; j++)
                app->proj.cad_models.v[j] = app->proj.cad_models.v[j + 1];
            app->proj.cad_models.len--;
        }
    }
    if (app->nfeats > 0) {
        float cursor = 0;
        for (int i = 0; i < app->nfeats; i++) {
            DesignFeature f = app->feats[i];
            f.x = cursor;
            CadMesh mesh;
            if (!feature_make_mesh(&f, f.material_id, &mesh)) continue;
            char name[128];
            const char *t = f.type == FEAT_BOX ? "box" : (f.type == FEAT_CYLINDER ? "cyl" : "sphere");
            snprintf(name, sizeof(name), "design:%s-%d", t, i + 1);
            CadModel cm; memset(&cm, 0, sizeof(cm));
            cm.id = str_dup(make_id());
            cm.name = str_dup(name);
            cm.source_file = str_dup("design:parametric");
            cm.mesh = mesh;
            arr_push(app->proj.cad_models, cm);
            cursor += (f.type == FEAT_BOX ? f.w : 2.0f * f.w) + 3.0f;
        }
    }
    app->cad_gen++;
    cache_clear();
    app->fit_dirty = true;
    app->sel_cad = app->proj.cad_models.len ? app->proj.cad_models.len - 1 : 0;
}

static void design_export_stl(App *app) {
    if (app->proj.cad_models.len == 0) {
        free(app->status); app->status = str_dup("no cad models to export");
        return;
    }
    char path[512];
    snprintf(path, sizeof(path), "%sdesign_export.stl", app->proj.path && app->proj.path[0] ? app->proj.path : "build\\");
    CadModel *cm = &app->proj.cad_models.v[app->sel_cad < app->proj.cad_models.len ? app->sel_cad : 0];
    bool ok = mesh_write_stl(path, &cm->mesh);
    char msg[512];
    snprintf(msg, sizeof(msg), ok ? "STL exported: %s" : "STL export FAILED: %s", path);
    free(app->status); app->status = str_dup(msg);
}

static void design_export_bom(App *app) {
    char *bom = project_bom_text(&app->proj);
    char path[512];
    snprintf(path, sizeof(path), "%sbom.txt", app->proj.path && app->proj.path[0] ? app->proj.path : "build\\");
    FILE *f = fopen(path, "w");
    if (f) { fputs(bom, f); fclose(f); }
    char msg[512];
    snprintf(msg, sizeof(msg), f ? "BOM exported: %s" : "BOM export FAILED: %s", path);
    free(app->status); app->status = str_dup(msg);
    free(bom);
}

static void design_run_generative(App *app) {
    GenDesign *g = &app->gen;
    if (g->dens) { gen_design_free(g); memset(g, 0, sizeof(*g)); }
    if (g->nx < 4) g->nx = 32;
    if (g->ny < 4) g->ny = 20;
    if (g->volfrac <= 0 || g->volfrac > 1) g->volfrac = 0.4f;
    if (!g->fix_left && !g->fix_bottom) g->fix_left = 1;
    if (g->load_xf <= 0) g->load_xf = 1.0f;
    if (g->load_yf < 0) g->load_yf = 0.5f;
    if (g->iters <= 0) g->iters = 50;
    bool ok = gen_design_run(g);
    if (!ok) {
        free(app->status); app->status = str_dup("generative design failed");
        return;
    }
    CadMesh mesh;
    if (gen_design_mesh(g, 2.0f, 6.0f, &mesh)) {
        CadModel cm; memset(&cm, 0, sizeof(cm));
        cm.id = str_dup(make_id());
        cm.name = str_dup("design:topopt");
        cm.source_file = str_dup("design:topopt");
        cm.mesh = mesh;
        arr_push(app->proj.cad_models, cm);
        app->cad_gen++;
        cache_clear();
        app->gen_has_result = true;
        free(app->status);
        char msg[128];
        snprintf(msg, sizeof(msg), "topology optimization done (%dx%d)", g->nx, g->ny);
        app->status = str_dup(msg);
    } else {
        free(app->status); app->status = str_dup("topopt ran but produced no mesh");
    }
    app->fit_dirty = true;
}

static void design_draw_slider(App *app, const char *label, int x, int *y, int w, int *ival, int lo, int hi) {
    DrawText(label, x, *y, 14, LIGHTGRAY);
    *y += 18;
    float t = (float)(*ival - lo) / (float)(hi - lo);
    DrawRectangle(x, *y, w, 6, (Color){ 60, 60, 70, 255 });
    DrawRectangle(x, *y, (int)(w * t), 6, (Color){ 100, 160, 220, 255 });
    DrawText(TextFormat("%d", *ival), x + w + 8, *y - 4, 14, WHITE);
    Vector2 m = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && m.y >= *y - 6 && m.y <= *y + 10 && m.x >= x && m.x <= x + w + 40) {
        float nt = (m.x - x) / (float)w;
        int nv = lo + (int)(nt * (hi - lo));
        if (nv < lo) nv = lo; if (nv > hi) nv = hi;
        *ival = nv;
    }
    *y += 18;
}

static void design_draw_float(App *app, const char *label, int x, int *y, int w, float *val, float lo, float hi) {
    DrawText(label, x, *y, 14, LIGHTGRAY);
    *y += 18;
    int ival = (int)((*val - lo) / (hi - lo) * 100.0f);
    float t = (float)ival / 100.0f;
    DrawRectangle(x, *y, w, 6, (Color){ 60, 60, 70, 255 });
    DrawRectangle(x, *y, (int)(w * t), 6, (Color){ 220, 170, 90, 255 });
    DrawText(TextFormat("%.2f", *val), x + w + 8, *y - 4, 14, WHITE);
    Vector2 m = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && m.y >= *y - 6 && m.y <= *y + 10 && m.x >= x && m.x <= x + w + 40) {
        float nt = (m.x - x) / (float)w;
        if (nt < 0) nt = 0; if (nt > 1) nt = 1;
        *val = lo + nt * (hi - lo);
    }
    *y += 18;
}

static void draw_design(App *app, int x, int y, int w, int h) {
    DrawText("Design Workspace (parametric + generative)", x + 12, y + 10, 18, WHITE);
    int py = y + 40;

    // feature add bar
    if (ui_button("+ Box", x + 12, py, 70, 26)) design_add_feature(app, FEAT_BOX);
    if (ui_button("+ Cylinder", x + 90, py, 90, 26)) design_add_feature(app, FEAT_CYLINDER);
    if (ui_button("+ Sphere", x + 188, py, 80, 26)) design_add_feature(app, FEAT_SPHERE);
    if (ui_button("Delete", x + 276, py, 70, 26)) design_delete_feature(app);
    if (ui_button("Dup", x + 356, py, 46, 26)) design_duplicate_feature(app);
    if (ui_button("Bool", x + 410, py, 50, 26)) {
        // boolean op on first two features
        if (app->nfeats >= 2) {
            DesignFeature *a = &app->feats[0], *b = &app->feats[1];
            V3 amin = v3(a->x - a->w*0.5f, a->y - a->d*0.5f, a->z - a->h*0.5f);
            V3 amax = v3(a->x + a->w*0.5f, a->y + a->d*0.5f, a->z + a->h*0.5f);
            V3 bmin = v3(b->x - b->w*0.5f, b->y - b->d*0.5f, b->z - b->h*0.5f);
            V3 bmax = v3(b->x + b->w*0.5f, b->y + b->d*0.5f, b->z + b->h*0.5f);
            bool valid; V3 ir = bool_intersect_bbox(amin, amax, bmin, bmax, &valid);
            char ms[128]; snprintf(ms, sizeof(ms), "Intersect: %s  center=(%.0f,%.0f,%.0f)", valid ? "valid" : "empty", ir.x, ir.y, ir.z);
            free(app->status); app->status = str_dup(ms);
        } else { free(app->status); app->status = str_dup("Need 2+ features for boolean ops"); }
    }
    if (ui_button("Export STL", x + 466, py, 90, 26)) design_export_stl(app);
    if (ui_button("Export BOM", x + 562, py, 90, 26)) design_export_bom(app);
    if (ui_button("STEP", x + 658, py, 56, 26)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            step_export(&cm->mesh, "build\\model.stp");
            free(app->status); app->status = str_dup("STEP file exported to build\\model.stp");
        }
    }
    if (ui_button("AllOut", x + 720, py, 58, 26))
        { char *ea = export_all_formats(&app->proj, "build"); free(app->status); app->status = ea; }
    py += 32;
    if (ui_button("Cone", x + 12, py, 58, 22)) {
        CadMesh cn; memset(&cn, 0, sizeof(cn)); cn.valid = 1;
        mesh_cone(&cn, v3(0,0,15), 15, 30, 24);
        CadModel nc; memset(&nc, 0, sizeof(nc)); nc.id = str_dup(make_id()); nc.name = str_dup("cone"); nc.mesh = cn;
        arr_push(app->proj.cad_models, nc); app->cad_gen++; cache_clear();
    }
    if (ui_button("Torus", x + 76, py, 54, 22)) {
        CadMesh tr; memset(&tr, 0, sizeof(tr)); tr.valid = 1;
        mesh_torus(&tr, v3(0,0,30), 25, 8, 24, 12);
        CadModel nc; memset(&nc, 0, sizeof(nc)); nc.id = str_dup(make_id()); nc.name = str_dup("torus"); nc.mesh = tr;
        arr_push(app->proj.cad_models, nc); app->cad_gen++; cache_clear();
    }
    if (ui_button("Wedge", x + 136, py, 60, 22)) {
        CadMesh wd; memset(&wd, 0, sizeof(wd)); wd.valid = 1;
        mesh_wedge(&wd, v3(0,0,10), v3(40,30,20));
        CadModel nc; memset(&nc, 0, sizeof(nc)); nc.id = str_dup(make_id()); nc.name = str_dup("wedge"); nc.mesh = wd;
        arr_push(app->proj.cad_models, nc); app->cad_gen++; cache_clear();
    }
    if (ui_button("Pyramid", x + 202, py, 68, 22)) {
        CadMesh py; memset(&py, 0, sizeof(py)); py.valid = 1;
        mesh_pyramid(&py, v3(0,0,15), 30, 30, 30);
        CadModel nc; memset(&nc, 0, sizeof(nc)); nc.id = str_dup(make_id()); nc.name = str_dup("pyramid"); nc.mesh = py;
        arr_push(app->proj.cad_models, nc); app->cad_gen++; cache_clear();
    }
    if (ui_button("Lattic", x + 276, py, 56, 22)) {
        CadMesh lt; mesh_lattice_grid(v3(-20,-20,0), v3(20,20,30), 10, 1, 4, &lt);
        CadModel nc; memset(&nc, 0, sizeof(nc)); nc.id = str_dup(make_id()); nc.name = str_dup("lattice"); nc.mesh = lt;
        arr_push(app->proj.cad_models, nc); app->cad_gen++; cache_clear();
    }
    if (ui_button("Sect", x + 338, py, 48, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            ClipPlane cp = { v3(0,0,0), v3(0,0,1), 10 };
            char *r = section3d_report(&cm->mesh, cp); free(app->status); app->status = r;
        }
    }
    if (ui_button("Libs", x + 392, py, 48, 22))
        { char *lr = lib_list_report(&app->proj); free(app->status); app->status = lr; }
    if (ui_button("Lib3D", x + 446, py, 56, 22))
        { lib3d_create(&app->proj, "default"); lib3d_add_model(&app->proj, "default", app->proj.cad_models.len>0?app->proj.cad_models.v[0].id:""); char *lr = lib3d_list_report(&app->proj); free(app->status); app->status = lr; }
    if (ui_button("Surf", x + 508, py, 48, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { char *sf = surface_finish_report(&cm->mesh, 0.2f); free(app->status); app->status = sf; }
    }
    if (ui_button("Comm", x + 562, py, 58, 22))
        { char *ce = community_export_presets(&app->proj); free(app->status); app->status = ce; }
    if (ui_button("Scl", x + 626, py, 42, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { mesh_scale(&cm->mesh, 1.5f, 1.5f, 1.5f); app->cad_gen++; cache_clear(); free(app->status); app->status = str_dup("scaled 1.5x"); }
    }
    if (ui_button("Solid", x + 674, py, 48, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            CadMesh so; if (mesh_to_solid(&cm->mesh, &so, 2.0f)) {
                CadModel nc; memset(&nc, 0, sizeof(nc)); nc.id = str_dup(make_id()); nc.name = str_dup("solid"); nc.mesh = so;
                arr_push(app->proj.cad_models, nc); app->cad_gen++; cache_clear();
            }
        }
    }
    if (ui_button("Repair", x + 276, py, 60, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            int d = mesh_remove_degenerate(&cm->mesh, 0.001f);
            int f = mesh_fix_normals(&cm->mesh);
            char ms[64]; snprintf(ms, sizeof(ms), "repair: %d deg, %d normals fixed", d, f);
            free(app->status); app->status = str_dup(ms);
        } else { free(app->status); app->status = str_dup("Select a CAD model first"); }
    }
    if (ui_button("XSec", x + 342, py, 52, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { char *xs = mesh_xsection_text(&cm->mesh, (cm->mesh.bmin.z + cm->mesh.bmax.z)*0.5f); free(app->status); app->status = xs; }
    }
    if (ui_button("SymEd", x + 400, py, 56, 22)) {
        V2 pp[4] = { v2(-10,-10), v2(10,-10), v2(10,10), v2(-10,10) };
        const char *pn[4] = { "IN+", "IN-", "OUT", "GND" };
        symedit_create(&app->proj, "OpAmp", 20, 20, pp, pn, 4);
        free(app->status); app->status = str_dup("OpAmp symbol created");
    }
    py += 28;

    // timeline
    DrawText("Feature timeline:", x + 12, py, 14, LIGHTGRAY); py += 18;
    for (int i = 0; i < app->nfeats && py < y + h; i++) {
        DesignFeature *f = &app->feats[i];
        char buf[160];
        if (f->type == FEAT_BOX) snprintf(buf, sizeof(buf), "[%d] box %.0fx%.0fx%.0f", i + 1, f->w, f->h, f->d);
        else if (f->type == FEAT_CYLINDER) snprintf(buf, sizeof(buf), "[%d] cylinder r%.0f h%.0f", i + 1, f->w, f->h);
        else snprintf(buf, sizeof(buf), "[%d] sphere r%.0f", i + 1, f->w);
        Vector2 m = GetMousePosition();
        bool rowh = m.x >= x + 12 && m.x <= x + 300 && m.y >= py && m.y <= py + 16;
        if (rowh && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            app->sel_feat = i;
            app->sel_cad = i;
        }
        Color c = (app->sel_feat == i) ? YELLOW : (rowh ? (Color){ 220, 220, 230, 255 } : GRAY);
        DrawText(buf, x + 16, py, 13, c); py += 16;
    }
    if (app->nfeats == 0) {
        DrawText("No features yet - click + Box / + Cylinder / + Sphere", x + 16, py, 13, GRAY); py += 16;
    }
    py += 10;

    // position & rotation nudge for selected feature
    if (app->sel_feat >= 0 && app->sel_feat < app->nfeats) {
        DesignFeature *f = &app->feats[app->sel_feat];
        DrawText("Transform:", x + 12, py, 14, LIGHTGRAY); py += 18;
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "X:%.0f", f->x);
        DrawText(tmp, x + 16, py + 4, 13, WHITE);
        if (ui_button("<", x + 70, py, 24, 22)) { f->x -= 10.0f; design_rebuild(app); }
        if (ui_button(">", x + 96, py, 24, 22)) { f->x += 10.0f; design_rebuild(app); }
        snprintf(tmp, sizeof(tmp), "Y:%.0f", f->y);
        DrawText(tmp, x + 136, py + 4, 13, WHITE);
        if (ui_button("<", x + 190, py, 24, 22)) { f->y -= 10.0f; design_rebuild(app); }
        if (ui_button(">", x + 216, py, 24, 22)) { f->y += 10.0f; design_rebuild(app); }
        snprintf(tmp, sizeof(tmp), "Z:%.0f", f->z);
        DrawText(tmp, x + 256, py + 4, 13, WHITE);
        if (ui_button("<", x + 310, py, 24, 22)) { f->z -= 10.0f; design_rebuild(app); }
        if (ui_button(">", x + 336, py, 24, 22)) { f->z += 10.0f; design_rebuild(app); }
        snprintf(tmp, sizeof(tmp), "Rx:%.0f", f->rx);
        DrawText(tmp, x + 380, py + 4, 13, WHITE);
        if (ui_button("<", x + 436, py, 24, 22)) { f->rx -= 15.0f; design_rebuild(app); }
        if (ui_button(">", x + 462, py, 24, 22)) { f->rx += 15.0f; design_rebuild(app); }
        snprintf(tmp, sizeof(tmp), "Ry:%.0f", f->ry);
        DrawText(tmp, x + 500, py + 4, 13, WHITE);
        if (ui_button("<", x + 556, py, 24, 22)) { f->ry -= 15.0f; design_rebuild(app); }
        if (ui_button(">", x + 582, py, 24, 22)) { f->ry += 15.0f; design_rebuild(app); }
        snprintf(tmp, sizeof(tmp), "Rz:%.0f", f->rz);
        DrawText(tmp, x + 620, py + 4, 13, WHITE);
        if (ui_button("<", x + 676, py, 24, 22)) { f->rz -= 15.0f; design_rebuild(app); }
        if (ui_button(">", x + 702, py, 24, 22)) { f->rz += 15.0f; design_rebuild(app); }
        py += 28;
    }

    // mass properties of the selected feature
    if (app->sel_feat >= 0 && app->sel_feat < app->nfeats) {
        DesignFeature *f = &app->feats[app->sel_feat];
        CadMesh mesh;
        if (feature_make_mesh(f, f->material_id, &mesh)) {
            CadMaterial *mat = material_lib_get(app->sel_mat);
            float vol = 0, mass = 0; V3 cg = { 0 };
            mesh_mass_props(&mesh, mat->density, &vol, &mass, &cg);
            DrawText("Mass properties (selected feature):", x + 12, py, 14, LIGHTGRAY); py += 18;
            char buf[160];
            snprintf(buf, sizeof(buf), "material: %s  (%.2f g/cm3)", mat->name, mat->density);
            DrawText(buf, x + 16, py, 13, WHITE); py += 16;
            snprintf(buf, sizeof(buf), "volume: %.1f mm3   mass: %.2f g", vol, mass);
            DrawText(buf, x + 16, py, 13, WHITE); py += 16;
            snprintf(buf, sizeof(buf), "centroid: (%.1f, %.1f, %.1f) mm", cg.x, cg.y, cg.z);
            DrawText(buf, x + 16, py, 13, WHITE); py += 16;
            material_free(mat);
            mesh_free(&mesh);
        }
    }
    py += 10;

    // quick mechanical calculator
    DrawText("Mech Calc:", x + 12, py, 14, LIGHTGRAY); py += 18;
    if (ui_button("Beam (center L)", x + 12, py, 112, 22)) {
        float d, s;
        if (app->sel_feat >= 0 && app->sel_feat < app->nfeats) {
            DesignFeature *f = &app->feats[app->sel_feat];
            CadMaterial *mat = material_lib_get(app->sel_mat);
            beam_bending_center(f->d > 0 ? f->d : 40, f->w > 0 ? f->w : 10, f->h > 0 ? f->h : 10, 10.0f, mat->young_gpa, &d, &s);
            char ms[128]; snprintf(ms, sizeof(ms), "Beam %s: deflect=%.4f mm, stress=%.1f MPa", mat->name, d, s);
            free(app->status); app->status = str_dup(ms);
            material_free(mat);
        } else {
            float d2, s2; beam_bending_center(50, 10, 10, 10, 70, &d2, &s2);
            char ms[128]; snprintf(ms, sizeof(ms), "50x10x10mm Al beam: deflect=%.4f mm, stress=%.1f MPa", d2, s2);
            free(app->status); app->status = str_dup(ms);
        }
    }
    if (ui_button("Cantilever", x + 132, py, 90, 22)) {
        float d, s; beam_bending_cantilever(50, 10, 10, 10, 70, &d, &s);
        char ms[128]; snprintf(ms, sizeof(ms), "Cantilever 50x10x10 Al: deflect=%.4f mm, stress=%.1f MPa", d, s);
        free(app->status); app->status = str_dup(ms);
    }
    if (ui_button("Buckling", x + 230, py, 80, 22)) {
        float Pc = column_buckling_load(100, 10, 10, 70, 1);
        char ms[128]; snprintf(ms, sizeof(ms), "Column L=100 W=10 H=10: Pcr=%.0f N", Pc);
        free(app->status); app->status = str_dup(ms);
    }
    py += 28;
    if (ui_button("Modal", x + 12, py, 60, 22))
        { char *mr = modal_report("Demo", 70.0f, 2700.0f, 100.0f, 10.0f, 10.0f); free(app->status); app->status = mr; }
    if (ui_button("NonLin", x + 78, py, 60, 22))
        { char *nr = nonlinear_report("Demo", 1000.0f, 500.0f, 0.1f, 2000.0f); free(app->status); app->status = nr; }
    if (ui_button("SheetMetal", x + 144, py, 100, 22)) {
        char fb[512]; float K = sheet_k_factor_estimate(1.5f, 1.0f);
        float BA = sheet_bend_allowance(1.5f, 1.0f, K, 90);
        sheet_flat_pattern_text(100, 50, 20, 20, 1.5f, 1.0f, K, 90, fb, sizeof(fb));
        free(app->status); app->status = str_dup(fb);
    }
    if (ui_button("2D Draw", x + 120, py, 80, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm) { char *dr = drawing2d_from_mesh(&cm->mesh, cm->name); free(app->status); app->status = dr; }
        else { free(app->status); app->status = str_dup("Select a CAD model first"); }
    }
    if (ui_button("BOM", x + 208, py, 50, 22)) {
        char *bom = drawing2d_bom_sheet(&app->proj, app->proj.name);
        free(app->status); app->status = bom;
    }
    py += 28;
    // model operations row
    if (ui_button("Mirror", x + 12, py, 60, 22)) {
        if (app->sel_feat >= 0 && app->sel_feat < app->nfeats) {
            app->feats[app->sel_feat].x = -app->feats[app->sel_feat].x;
            design_rebuild(app);
            free(app->status); app->status = str_dup("mirrored X");
        }
    }
    if (ui_button("ArrayX3", x + 78, py, 64, 22)) {
        if (app->sel_feat >= 0 && app->sel_feat < app->nfeats && app->nfeats < 30) {
            for (int a = 1; a < 3; a++) {
                app->feats[app->nfeats] = app->feats[app->sel_feat];
                app->feats[app->nfeats].x += a * app->feats[app->sel_feat].w;
                app->nfeats++;
            }
            design_rebuild(app);
            free(app->status); app->status = str_dup("array X3");
        }
    }
    if (ui_button("Hollow", x + 148, py, 60, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            CadMesh hol; mesh_hollow(&cm->mesh, &hol, 2.0f);
            CadModel nc; memset(&nc, 0, sizeof(nc)); nc.id = str_dup(make_id()); nc.name = str_dup("hollow");
            nc.mesh = hol; arr_push(app->proj.cad_models, nc); app->cad_gen++; cache_clear();
            free(app->status); app->status = str_dup("hollow model created");
        } else { free(app->status); app->status = str_dup("Select a CAD model first"); }
    }
    if (ui_button("Loft", x + 214, py, 50, 22)) {
        V2 pa[4] = { v2(0,0), v2(30,0), v2(30,20), v2(0,20) };
        V2 pb[4] = { v2(-5,-5), v2(35,-5), v2(35,25), v2(-5,25) };
        CadMesh lo; if (mesh_loft(pa, pb, 4, 0, 30, &lo)) {
            CadModel nc; memset(&nc, 0, sizeof(nc)); nc.id = str_dup(make_id()); nc.name = str_dup("loft"); nc.mesh = lo;
            arr_push(app->proj.cad_models, nc); app->cad_gen++; cache_clear();
        }
    }
    if (ui_button("Sweep", x + 270, py, 60, 22)) {
        V2 prof[4] = { v2(-5,-5), v2(5,-5), v2(5,5), v2(-5,5) };
        V3 path[3] = { v3(0,0,0), v3(20,10,15), v3(40,0,30) };
        CadMesh sw; if (mesh_sweep(prof, 4, path, 3, true, &sw)) {
            CadModel nc; memset(&nc, 0, sizeof(nc)); nc.id = str_dup(make_id()); nc.name = str_dup("sweep"); nc.mesh = sw;
            arr_push(app->proj.cad_models, nc); app->cad_gen++; cache_clear();
        }
    }
    if (ui_button("Fatig", x + 336, py, 52, 22))
        { char *fr = fatigue_report("Demo", 200.0f, 50.0f, 500.0f, 10000); free(app->status); app->status = fr; }
    if (ui_button("Curve", x + 394, py, 54, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { char *cr = curvature_report(&cm->mesh); free(app->status); app->status = cr; }
    }
    py += 28;

    // material picker
    DrawText("Materials:", x + 12, py, 14, LIGHTGRAY); py += 18;
    int nmats = material_lib_count();
    for (int i = 0; i < nmats && py < y + h; i++) {
        CadMaterial *mat = material_lib_get(i);
        Vector2 m = GetMousePosition();
        bool rowh = m.x >= x + 12 && m.x <= x + 300 && m.y >= py && m.y <= py + 16;
        if (rowh && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { app->sel_mat = i; }
        char buf[160];
        snprintf(buf, sizeof(buf), "%.2f g/cm3", mat->density);
        Color c = (app->sel_mat == i) ? YELLOW : (rowh ? (Color){ 220, 220, 230, 255 } : GRAY);
        DrawText(mat->name, x + 16, py, 13, c);
        DrawText(buf, x + 200, py, 13, c);
        py += 16;
        material_free(mat);
    }

    // right side: generative design panel
    int gx = x + w - 300, gy = y + 40;
    DrawRectangle(gx - 8, gy - 8, 300, h - 60, (Color){ 30, 32, 40, 255 });
    DrawText("Generative Design (SIMP)", gx, gy, 14, LIGHTGRAY); gy += 20;
    design_draw_slider(app, "Grid X", gx, &gy, 180, &app->gen.nx, 16, 64);
    design_draw_slider(app, "Grid Y", gx, &gy, 180, &app->gen.ny, 12, 40);
    design_draw_float(app, "Volume frac", gx, &gy, 180, &app->gen.volfrac, 0.15f, 0.7f);
    design_draw_slider(app, "Load pos X", gx, &gy, 180, &app->gen_xi, 0, 100);
    design_draw_slider(app, "Load pos Y", gx, &gy, 180, &app->gen_yi, 0, 100);
    if (ui_button("Run Topology Opt", gx, gy, 190, 28)) {
        app->gen.load_xf = app->gen_xi / 100.0f;
        app->gen.load_yf = app->gen_yi / 100.0f;
        design_run_generative(app);
        app->mode = UI_3D;
        app->cam_dist = 60.0f;
    }
    gy += 40;
    if (app->gen_has_result) {
        DrawText("Result pushed to CAD Models (3D).", gx, gy, 13, GREEN); gy += 16;
        DrawText("Press 3 to view, 5 to tweak & re-run.", gx, gy, 13, GRAY);
    }
}

// ================= assistant panel =================
static void assist_submit(App *app) {
    if (app->chat_input[0] == '\0') return;
    // append user msg to log
    Buf b; buf_init(&b);
    if (app->chat_log) { buf_append_str(&b, app->chat_log); free(app->chat_log); }
    buf_appendf(&b, "you> %s\n", app->chat_input);

    DesignFeature feat;
    char *reply = NULL;
    char kind = assistant_offline_handle(app->chat_input, 0, &feat, &reply);
    if (kind == 'f') {
        // actually create the feature in the design workspace
        if (app->nfeats < 32) {
            DesignFeature *f = &app->feats[app->nfeats++];
            *f = feat;
            f->material_id = app->sel_mat;
            f->segments = 24;
            f->x = 0;
            app->sel_feat = app->nfeats - 1;
            design_rebuild(app);
        }
        buf_appendf(&b, "design> %s\n", reply ? reply : "created");
        free(reply);
        free(app->status); app->status = str_dup("assistant created a parametric part");
    } else if (kind == 'h') {
        buf_appendf(&b, "design> %s\n", reply ? reply : "?");
        free(reply);
    } else {
        // try Ollama for a real answer; fall back to offline reply
        bool got = false;
        if (ollama_ping(app->proj.llm_url)) {
            char *out = ollama_chat(app->proj.llm_url, app->proj.llm_model, app->chat_input);
            if (out) {
                buf_appendf(&b, "assist> %s\n", out);
                free(out);
                got = true;
            }
        }
        if (!got) {
            buf_appendf(&b, "assist> %s\n", reply ? reply : "(no design intent parsed)");
            if (reply) free(reply);
            free(app->status); app->status = str_dup("assistant: offline reply (no Ollama)");
        } else {
            free(reply);
            free(app->status); app->status = str_dup("assistant: replied via Ollama");
        }
    }
    app->chat_log = str_dup(buf_cstr(&b));
    buf_free(&b);
    app->chat_input[0] = '\0';
}

// ================= print / co-design / farm =================
static int g_slice_layer = 0;
static SliceResult g_last_sr = {0};

static void draw_print(App *app, int x, int y, int w, int h) {
    DrawText("Print Center (slicer + enclosure + farm)", x + 12, y + 10, 18, WHITE);
    int py = y + 40;

    // -- enclosure section --
    DrawText("Enclosure:", x + 12, py, 15, LIGHTGRAY); py += 20;
    Pcb *pcb = (app->sel_pcb >= 0 && app->sel_pcb < app->proj.pcbs.len)
                 ? &app->proj.pcbs.v[app->sel_pcb] : NULL;
    int epy = py;
    if (ui_button("Wall-", x + 12, epy, 48, 24) && app->enc_params.wall_thickness > 0.6f)
        app->enc_params.wall_thickness -= 0.5f;
    if (ui_button("Wall+", x + 62, epy, 48, 24))
        app->enc_params.wall_thickness += 0.5f;
    char buf[256];
    snprintf(buf, sizeof(buf), "Wall=%.1f", app->enc_params.wall_thickness);
    DrawText(buf, x + 116, epy + 4, 14, LIGHTGRAY);
    if (ui_button("H+", x + 200, epy, 36, 24)) app->enc_params.wall_height += 1.0f;
    if (ui_button("H-", x + 238, epy, 36, 24) && app->enc_params.wall_height > 1.0f) app->enc_params.wall_height -= 1.0f;
    snprintf(buf, sizeof(buf), "Ht=%.0f", app->enc_params.wall_height);
    DrawText(buf, x + 280, epy + 4, 14, LIGHTGRAY);
    if (ui_button("Clr+", x + 340, epy, 36, 24)) app->enc_params.clearance += 0.5f;
    if (ui_button("Clr-", x + 378, epy, 36, 24) && app->enc_params.clearance > 0.5f) app->enc_params.clearance -= 0.5f;
    snprintf(buf, sizeof(buf), "Clr=%.1f", app->enc_params.clearance);
    DrawText(buf, x + 420, epy + 4, 14, LIGHTGRAY);
    snprintf(buf, sizeof(buf), "Floor=%.1f  Standoff=%.0f", app->enc_params.floor_thickness, app->enc_params.standoff_h);
    DrawText(buf, x + 12, epy + 28, 13, GRAY);

    py = epy + 52;
    CadModel *enc = NULL;
    if (pcb) {
        snprintf(buf, sizeof(buf), "PCB: %s", pcb->name ? pcb->name : "?");
        DrawText(buf, x + 12, py, 14, WHITE); py += 20;
        if (ui_button("Generate Enclosure", x + 12, py, 170, 28)) {
            CadModel *existing = co_find_enclosure(&app->proj, pcb->id);
            if (!existing) {
                int ai = co_design_enclosure(&app->proj, pcb->id, &app->enc_params);
                if (ai >= 0) {
                    app->encl_ready = true;
                    app->cad_gen++;
                    free(app->status);
                    app->status = str_dup("enclosure generated");
                } else {
                    free(app->status);
                    app->status = str_dup("enclosure generation failed");
                }
            } else {
                app->encl_ready = true;
                free(app->status);
                app->status = str_dup("enclosure already exists");
            }
        }
        py += 32;
        enc = co_find_enclosure(&app->proj, pcb->id);
        if (enc) {
            snprintf(buf, sizeof(buf), "Enclosure: %d verts %d tris (bbox %.0fx%.0f)", enc->mesh.nverts, enc->mesh.ntris,
                     enc->mesh.bmax.x - enc->mesh.bmin.x, enc->mesh.bmax.y - enc->mesh.bmin.y);
            DrawText(buf, x + 12, py, 13, GREEN); py += 20;
            app->encl_ready = true;
        }
    } else {
        DrawText("No PCB loaded — import a .kicad_pcb first", x + 12, py, 13, GRAY); py += 20;
    }
    py += 8;

    // -- slicer settings summary --
    DrawText("Slicer Settings:", x + 12, py, 15, LIGHTGRAY); py += 18;
    SliceSettings *sc = &app->slice_cfg;
    snprintf(buf, sizeof(buf), "Layer %.2fmm  Line %.2fmm  Perimeters %d  Infill %d%%  Angle %.0f  Speed %.0f mm/s",
             sc->layer_height, sc->line_width, sc->perimeters, sc->infill_density, sc->infill_angle, sc->print_speed);
    DrawText(buf, x + 12, py, 13, GRAY); py += 18;
    snprintf(buf, sizeof(buf), "Nozzle: %dC  Bed: %dC  Travel: %.0f  Retract: %.1fmm",
             sc->hotend_temp, sc->bed_temp, sc->travel_speed, sc->retract_mm);
    DrawText(buf, x + 12, py, 13, GRAY); py += 18;

    int set_y = py;
    if (ui_button("Layer+", x + 12, set_y, 60, 24)) sc->layer_height = sc->layer_height < 0.5f ? sc->layer_height + 0.05f : sc->layer_height;
    if (ui_button("Layer-", x + 76, set_y, 60, 24)) sc->layer_height = sc->layer_height > 0.05f ? sc->layer_height - 0.05f : sc->layer_height;
    snprintf(buf, sizeof(buf), "%.2f", sc->layer_height);
    DrawText(buf, x + 144, set_y + 4, 14, WHITE);

    if (ui_button("Inf+", x + 200, set_y, 50, 24)) sc->infill_density = sc->infill_density < 100 ? sc->infill_density + 5 : sc->infill_density;
    if (ui_button("Inf-", x + 254, set_y, 50, 24)) sc->infill_density = sc->infill_density > 5 ? sc->infill_density - 5 : sc->infill_density;
    snprintf(buf, sizeof(buf), "%d%%", sc->infill_density);
    DrawText(buf, x + 312, set_y + 4, 14, WHITE);

    if (ui_button("Per+", x + 380, set_y, 50, 24)) sc->perimeters = sc->perimeters < 10 ? sc->perimeters + 1 : sc->perimeters;
    if (ui_button("Per-", x + 434, set_y, 50, 24)) sc->perimeters = sc->perimeters > 1 ? sc->perimeters - 1 : sc->perimeters;
    snprintf(buf, sizeof(buf), "%d per", sc->perimeters);
    DrawText(buf, x + 492, set_y + 4, 14, WHITE);
    py = set_y + 34;
    // infill pattern + ironing row
    static const char *pats[] = {"Grid","Grid2","Hex","Conc","Gyroid","Light","Cubic"};
    int pat = sc->infill_pattern; if (pat < 0) pat = 0; if (pat > 6) pat = 6;
    snprintf(buf, sizeof(buf), "Pat:%s", pats[pat]);
    if (ui_button(buf, x + 12, py, 60, 24)) { sc->infill_pattern = (sc->infill_pattern + 1) % 7; }
    if (ui_button(sc->ironing ? "Iron:ON" : "Iron:OFF", x + 78, py, 72, 24)) sc->ironing = !sc->ironing;
    if (ui_button(sc->adaptive_layer ? "Adapt:ON" : "Adapt:OFF", x + 156, py, 80, 24)) sc->adaptive_layer = !sc->adaptive_layer;
    if (ui_button("Supports", x + 242, py, 74, 24)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            SliceResult sr; memset(&sr, 0, sizeof(sr));
            if (slice_mesh(&cm->mesh, sc, &sr)) {
                supports_generate(&sr, sc, 45.0f);
                gcode_emit(&sr, sc, "build\\print_support.gcode");
                slice_result_free(&sr);
                free(app->status); app->status = str_dup("support-enabled gcode saved");
            } else { free(app->status); app->status = str_dup("slice failed"); }
        } else { free(app->status); app->status = str_dup("Select a CAD model first"); }
    }
    if (ui_button("PTower", x + 320, py, 64, 24))
        { char *pr = primetower_report(sc->nozzle_diameter, 4, 50.0f); free(app->status); app->status = pr; }
    if (ui_button("SLA", x + 390, py, 46, 24))
        { char *ar = additive_report("SLA", 50.0f, 150.0f); free(app->status); app->status = ar; }
    if (ui_button("TLib", x + 440, py, 48, 24))
        { char *tl = toollib_report(); free(app->status); app->status = tl; }
    if (ui_button("Save Settings", x + 540, py, 100, 24)) {
        slice_settings_save(sc, "build\\slice_settings.json");
        free(app->status);
        app->status = str_dup("slice settings saved");
    }
    py = py + 30;

    // -- calibration tests --
    DrawText("Calibration:", x + 12, py, 15, LIGHTGRAY); py += 20;
    int cal_y = py;
    #define CAL(fn, label, ...) do { \
        if (ui_button(label, x + 12 + (int)(w * 0.22f), cal_y, 140, 24)) { \
            char cp[256]; snprintf(cp, sizeof(cp), "build\\cal_%s.gcode", #fn); \
            fn(cp, __VA_ARGS__); \
            free(app->status); app->status = str_dup(#fn " saved to " cp); \
        } \
    } while(0)
    int bwcol = (int)(w * 0.22f);
    if (ui_button("Temp Tower", x + 12, cal_y, bwcol, 24))
        calib_temp_tower("build\\cal_temp.gcode", sc->bed_temp, 190, 230, 10, sc->layer_height, sc->line_width, sc->print_speed, sc->travel_speed, sc->retract_mm, sc->filament_dia);
    if (ui_button("Flow Rate", x + 18 + bwcol, cal_y, bwcol, 24))
        calib_flow_rate("build\\cal_flow.gcode", sc->bed_temp, sc->hotend_temp, sc->layer_height, sc->line_width, sc->print_speed, sc->travel_speed, sc->retract_mm, sc->filament_dia, 0.85f, 1.15f, 0.05f);
    if (ui_button("Pressure Adv", x + 24 + bwcol * 2, cal_y, bwcol, 24))
        calib_pressure_advance("build\\cal_pa.gcode", sc->bed_temp, sc->hotend_temp, 0.2f, sc->line_width, sc->print_speed, sc->travel_speed, sc->retract_mm, sc->filament_dia, 0.0f, 0.15f, 0.005f, true);
    if (ui_button("Max Flow", x + 30 + bwcol * 3, cal_y, bwcol, 24))
        calib_max_flow("build\\cal_maxflow.gcode", sc->bed_temp, sc->hotend_temp, 0.2f, sc->line_width, 20.0f, 100.0f, 10.0f, sc->travel_speed, sc->retract_mm, sc->filament_dia);
    cal_y += 28;
    if (ui_button("Input Shaper", x + 12, cal_y, bwcol, 24))
        calib_input_shaper("build\\cal_shaper.gcode", sc->bed_temp, sc->hotend_temp, sc->layer_height, sc->line_width, sc->print_speed, sc->travel_speed, sc->retract_mm, sc->filament_dia, 30.0f, 80.0f, 5.0f, 15.0f, 15.0f, 40.0f);
    if (ui_button("VFA Tower", x + 18 + bwcol, cal_y, bwcol, 24))
        calib_vfa_tower("build\\cal_vfa.gcode", sc->bed_temp, sc->hotend_temp, sc->layer_height, sc->line_width, 20.0f, 100.0f, 10.0f, sc->travel_speed, sc->retract_mm, sc->filament_dia, 15.0f, 15.0f, 40.0f);
    cal_y += 28;
    bwcol = (int)(w * 0.14f);
    if (ui_button("Retract", x + 12, cal_y, 62, 22))
        calib_retraction("build\\cal_retract.gcode", sc->bed_temp, sc->hotend_temp, 0.2f, sc->line_width, sc->print_speed, sc->travel_speed, sc->filament_dia, 0.2f, 2.0f, 0.3f);
    if (ui_button("Bridge", x + 80, cal_y, 62, 22))
        calib_bridge("build\\cal_bridge.gcode", sc->bed_temp, sc->hotend_temp, 0.2f, sc->line_width, sc->print_speed, sc->travel_speed, sc->filament_dia, 50.0f);
    if (ui_button("Toler", x + 148, cal_y, 56, 22))
        calib_tolerance("build\\cal_toler.gcode", sc->bed_temp, sc->hotend_temp, 0.2f, sc->line_width, sc->print_speed, sc->travel_speed, sc->filament_dia, 0.1f, 1.0f);
    if (ui_button("Overhang", x + 210, cal_y, 74, 22))
        calib_overhang("build\\cal_overhang.gcode", sc->bed_temp, sc->hotend_temp, 0.2f, sc->line_width, sc->print_speed, sc->travel_speed, sc->filament_dia);
    if (ui_button("Speed", x + 290, cal_y, 60, 22))
        calib_speed("build\\cal_speed.gcode", sc->bed_temp, sc->hotend_temp, 0.2f, sc->line_width, sc->print_speed, sc->travel_speed, sc->filament_dia, 20.0f, 150.0f, 15.0f);
    py = cal_y + 30;

    // -- print buttons --
    DrawText("Print:", x + 12, py, 15, LIGHTGRAY); py += 20;
    unsigned long tck = (unsigned long)clock();
    snprintf(app->last_gcode, sizeof(app->last_gcode), "build\\print_%lu.gcode", tck);

    if (pcb && enc) {
        if (ui_button("Print Enclosure", x + 12, py, 150, 30)) {
            bool ok = co_design_print_enclosure(&app->proj, pcb->id, sc, app->last_gcode);
            free(app->status);
            app->status = ok ? str_dup("enclosure printed!") : str_dup("print failed");
        }
    }
    if (ui_button("Print CAD Model", x + 176, py, 150, 30)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len)
                         ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm) {
            bool ok = co_design_print_cad(&app->proj, cm->id, sc, app->last_gcode);
            free(app->status);
            app->status = ok ? str_dup("CAD model printed!") : str_dup("print failed (no CAD selected?)");
        } else {
            free(app->status);
            app->status = str_dup("no CAD model selected — click a CAD model in sidebar");
        }
    }
    if (ui_button("Auto-Orient", x + 340, py, 110, 30)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len)
                         ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            float rx, ry, rz;
            if (calib_auto_orient(&cm->mesh, &rx, &ry, &rz)) {
                char ms[128];
                snprintf(ms, sizeof(ms), "Auto-orient: rotate Rx=%.0f Ry=%.0f Rz=%.0f (apply in Design workspace)", rx, ry, rz);
                free(app->status); app->status = str_dup(ms);
            } else { free(app->status); app->status = str_dup("Auto-orient failed: mesh too small"); }
        } else { free(app->status); app->status = str_dup("Select a CAD model first"); }
    }
    if (ui_button("Split", x + 460, py, 60, 30)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len)
                         ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            int nc = mesh_component_count(&cm->mesh);
            char ms[128]; snprintf(ms, sizeof(ms), "Model has %d connected component(s)", nc);
            free(app->status); app->status = str_dup(ms);
        } else { free(app->status); app->status = str_dup("Select a CAD model first"); }
    }
    if (ui_button("Turn", x + 526, py, 54, 30)) {
        TurnTool tt; turn_tool_defaults(&tt);
        turn_cyl("build\\turn_cyl.ngc", &tt, 20.0f, 15.0f, 50.0f, 3);
        free(app->status); app->status = str_dup("Turning toolpath saved to build\\turn_cyl.ngc");
    }
    if (app->last_gcode[0]) {
        snprintf(buf, sizeof(buf), "Output: %s", app->last_gcode);
        DrawText(buf, x + 12, py + 38, 12, DARKGRAY);
    }
    py = py + 62;
    // slicer stats panel + layer preview
    if (g_last_sr.nlayers > 0) {
        canvas_slicer_stats_panel(&g_last_sr, sc, x + w - 210, py - 40);
        g_slice_layer = canvas_slicer_layer_slider(x + 12, py + 8, w - 240, g_last_sr.nlayers);
        CanvasView pv = canvas_view_create(x + 12, py + 28, w - 240, 80);
        canvas_slicer_layer_preview(&g_last_sr, g_slice_layer, &pv);
    }

    // -- print farm dashboard --
    DrawText("Print Farm:", x + 12, py, 15, LIGHTGRAY); py += 20;
    Farm *f = &app->farm;
    int fb_x = x + 12, fb_w = w - 24;

    // action bar
    int act_y = py;
    if (ui_button("+ Printer", fb_x, act_y, 80, 24)) farm_add_printer(f, "Printer", 4, "");
    if (ui_button("+ Job", fb_x + 86, act_y, 60, 24)) farm_add_job(f, "Job", 1, 1, "", 0);
    if (ui_button("+ Spool", fb_x + 152, act_y, 62, 24)) farm_add_spool(f, "PLA", "White", 330.0f);
    if (ui_button("Assign", fb_x + 220, act_y, 60, 24)) {
        for (int j = 0; j < f->n_jobs; j++) if (!f->jobs[j].finished) farm_assign_job(f, j, -1);
    }
    if (ui_button("Scan", fb_x + 286, act_y, 52, 24)) farm_scan_store(f);
    if (ui_button("Save", fb_x + 344, act_y, 48, 24)) farm_save(f);
    if (ui_button("Load", fb_x + 398, act_y, 46, 24)) farm_load(f, "build\\farm");
    if (ui_button("Rebuild", fb_x + 450, act_y, 80, 24))
        { farm_free(f); farm_init(f, "build\\farm"); farm_seed_demo(f); farm_save(f); }
    act_y += 28;
    if (ui_button("All Idle", fb_x, act_y, 72, 22)) farm_select_all_printers(f, false);
    if (ui_button("All Busy", fb_x + 78, act_y, 72, 22)) farm_select_all_printers(f, true);
    if (ui_button("Pause All", fb_x + 156, act_y, 72, 22)) farm_batch_pause(f);
    if (ui_button("Resume All", fb_x + 234, act_y, 78, 22)) farm_batch_resume(f);
    if (ui_button("Maint", fb_x + 318, act_y, 52, 22)) {
        char mr[512]; int d = farm_check_maintenance(f, mr, sizeof(mr));
        if (d == 0) snprintf(mr, sizeof(mr), "All maintenance up to date");
        free(app->status); app->status = str_dup(mr);
        // auto-add demo maintenance if empty
        if (f->n_maint == 0) {
            farm_add_maintenance(f, "Voron 2.4", 100, "lubricate Z rods");
            farm_add_maintenance(f, "Bambu X1C", 200, "replace nozzle");
        }
    }
    if (ui_button("Filament", fb_x + 376, act_y, 68, 22)) {
        char fr[256]; int c = farm_low_filament_alerts(f, 50.0f, fr, sizeof(fr));
        if (c == 0) snprintf(fr, sizeof(fr), "All spools above 50m");
        free(app->status); app->status = str_dup(fr);
    }
    if (ui_button("Energy", fb_x + 450, act_y, 58, 22)) {
        float cost = energy_cost_total(f, 0.12f, 150.0f);
        char *fr = farm_failure_report(f);
        char ms[512]; snprintf(ms, sizeof(ms), "Energy: $%.2f | %s", cost, fr);
        free(fr); free(app->status); app->status = str_dup(ms);
    }
    if (ui_button("Util", fb_x + 514, act_y, 46, 22))
        { char *ur = farm_utilization_report(f); free(app->status); app->status = ur; }
    if (ui_button("Eff", fb_x + 566, act_y, 40, 22))
        { char *er = farm_efficiency_report(f); free(app->status); app->status = er; }
    if (ui_button("Cap", fb_x + 612, act_y, 40, 22))
        { char *cr = farm_capacity_report(f); free(app->status); app->status = cr; }
    if (ui_button("Resv", fb_x + 658, act_y, 44, 22))
        { char *rr = farm_reservations_report(f); free(app->status); app->status = rr; }
    if (ui_button("Pred", fb_x + 708, act_y, 46, 22))
        { char *pr = predictive_maintenance_report(f); free(app->status); app->status = pr; }
    if (ui_button("Eject", fb_x + 760, act_y, 46, 22)) {
        EjectProfile ep; eject_profile_default(&ep, "PLA");
        char *er = eject_profile_report(&ep); free(app->status); app->status = er;
    }
    if (ui_button("Fav", fb_x + 812, act_y, 40, 22)) {
        if (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len)
            favorites_add(&app->proj, "cad", app->proj.cad_models.v[app->sel_cad].id);
        char *fl = favorites_list(&app->proj); free(app->status); app->status = fl;
    }
    if (ui_button("Web", fb_x + 858, act_y, 42, 22))
        { char *ws = webcam_preview_status(); free(app->status); app->status = ws; }
    if (ui_button("Cnr", fb_x + 906, act_y, 42, 22)) {
        float cr = corner_relief_diameter(1.0f, 2.0f);
        char ms[64]; snprintf(ms, sizeof(ms), "Corner relief: %.1f mm", cr);
        free(app->status); app->status = str_dup(ms);
    }
    if (ui_button("Net", fb_x + 954, act_y, 40, 22))
        { char *ns = netprinter_status_widget(f); free(app->status); app->status = ns; }
    if (ui_button("Log", fb_x + 1000, act_y, 38, 22)) {
        PrintLog pl = {"demo", "Voron 2.4", "PLA", 5000, 3600, 1};
        printlog_add(f, pl);
        char *pr = printlog_report(f); free(app->status); app->status = pr;
    }
    // second row
    act_y += 28;
    if (ui_button("Convey", fb_x, act_y, 64, 22))
        { char *cg = conveyor_eject_gcode("PEI"); free(app->status); app->status = cg; }
    if (ui_button("Robot", fb_x + 70, act_y, 56, 22))
        { char *rg = robot_eject_gcode(v3(100,100,5), 10); free(app->status); app->status = rg; }
    if (ui_button("BkUp", fb_x + 132, act_y, 50, 22))
        { char *bu = project_backup(&app->proj); free(app->status); app->status = bu; }
    if (ui_button("BOM$", fb_x + 188, act_y, 54, 22))
        { char *bc = bom_with_cost(&app->proj, 0.50f); free(app->status); app->status = bc; }
    if (ui_button("Live", fb_x + 248, act_y, 46, 22))
        { farm_live_refresh(f); free(app->status); app->status = str_dup("farm refreshed"); }
    if (ui_button("Sheet", fb_x + 300, act_y, 52, 22)) {
        if (app->proj.schematics.len > 1) { app->sel_sch = (app->sel_sch + 1) % app->proj.schematics.len; app->mode = UI_SCH; }
        char ms[64]; snprintf(ms, sizeof(ms), "sheet %d/%d", app->sel_sch+1, app->proj.schematics.len);
        free(app->status); app->status = str_dup(ms);
    }
    if (ui_button("Hist", fb_x + 358, act_y, 46, 22))
        { char *sh = status_history_report(); free(app->status); app->status = sh; }
    if (ui_button("Trend", fb_x + 410, act_y, 52, 22))
        { char *ta = trend_analytics_report(); free(app->status); app->status = ta; }
    if (ui_button("Mat", fb_x + 468, act_y, 42, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            RenderProps rp = render_material_for_model(cm, "Aluminum");
            char ms[128]; snprintf(ms, sizeof(ms), "Material: rgb(%.0f,%.0f,%.0f) rough=%.1f metal=%.1f", rp.r*255, rp.g*255, rp.b*255, rp.rough, rp.metal);
            free(app->status); app->status = str_dup(ms);
        }
    }
    if (ui_button("Qual", fb_x + 516, act_y, 44, 22))
        { int q = quality_score(NULL, sc); char ms[32]; snprintf(ms, sizeof(ms), "Quality score: %d/100", q > 0 ? q : 70); free(app->status); app->status = str_dup(ms); }
    if (ui_button("Prof", fb_x + 566, act_y, 44, 22))
        { char *pl = profile_list_text(); free(app->status); app->status = pl; }
    if (ui_button("Fail", fb_x + 616, act_y, 44, 22)) {
        char *fa = failure_action(detect_nozzle_clog(1, 0.5f), detect_layer_shift(v2(0,0), v2(1,0), 0.5f), detect_warping(100, 70));
        free(app->status); app->status = fa;
    }
    if (ui_button("Cool", fb_x + 666, act_y, 44, 22)) {
        int fan = cooling_for_overhang(45.0f);
        char ms[32]; snprintf(ms, sizeof(ms), "Fan: %d (45deg overhang)", fan);
        free(app->status); app->status = str_dup(ms);
    }
    if (ui_button("Brim", fb_x + 716, act_y, 44, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            SliceResult sr; memset(&sr, 0, sizeof(sr));
            if (slice_mesh(&cm->mesh, sc, &sr) && sr.nlayers > 0) {
                V2 sq[4] = { v2(cm->mesh.bmin.x,cm->mesh.bmin.y), v2(cm->mesh.bmax.x,cm->mesh.bmin.y), v2(cm->mesh.bmax.x,cm->mesh.bmax.y), v2(cm->mesh.bmin.x,cm->mesh.bmax.y) };
                brim_add_mouse_ears(&sr.layers[0], sq, 4, 10.0f, 4, sc->line_width);
                gcode_emit(&sr, sc, "build\\print_brim.gcode"); slice_result_free(&sr);
                free(app->status); app->status = str_dup("brim added, gcode saved");
            }
        }
    }
    if (ui_button("Tree", fb_x + 766, act_y, 44, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) {
            SliceResult sr; memset(&sr, 0, sizeof(sr));
            if (slice_mesh(&cm->mesh, sc, &sr)) { supports_organic_tree(&sr, sc, 45, 3); gcode_emit(&sr, sc, "build\\print_organic.gcode"); slice_result_free(&sr); free(app->status); app->status = str_dup("organic tree supports added"); }
        }
    }
    if (ui_button("PBR", fb_x + 816, act_y, 40, 22)) {
        PBRMaterial mat; pbr_material_preset(&mat, "copper");
        HDREnvironment env; hdr_environment_preset(&env, "studio");
        char *rr = pbr_render_report(NULL, &mat, &env); free(app->status); app->status = rr;
    }
    if (ui_button("Cam", fb_x + 862, act_y, 42, 22))
        { char *ws = webcam_status_text(); free(app->status); app->status = ws; }
    if (ui_button("RFID", fb_x + 910, act_y, 44, 22))
        { char *rs = rfid_simulate_scan(); free(app->status); app->status = rs; }
    if (ui_button("Paint", fb_x + 960, act_y, 50, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { int tris[64]; int n = paint_support_region(&cm->mesh, v2(0,0), 20, 1, (V2){0,0}, tris, 64); char *pr = paint_region_report(n, "support"); free(app->status); app->status = pr; }
    }
    if (ui_button("Face", fb_x + 1016, act_y, 44, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { int tris[64]; int n = paint_visible_tris(&cm->mesh, v3(0,0,1), tris, 64); char ms[32]; snprintf(ms,sizeof(ms),"%d faces visible",n); free(app->status); app->status = str_dup(ms); }
    }
    if (ui_button("Cost", fb_x + 1066, act_y, 44, 22))
        { char *cr = cost_report(f); free(app->status); app->status = cr; }
    if (ui_button("Grp", fb_x + 1116, act_y, 36, 22))
        { int idx[3]={0,1,2}; farm_group_create(f,"Main",idx,3); char *gr = farm_group_report(f,g_groups,g_ng>0?g_ng:1); free(app->status); app->status = gr; }
    if (ui_button("BusB", fb_x + 1158, act_y, 44, 22))
        { const char *sigs[4]={"D0","D1","D2","D3"}; Schematic *s=app->proj.schematics.len>0?&app->proj.schematics.v[0]:NULL; if(s){bus_create_with_signals(s,"DATA",sigs,4);free(app->status);app->status=str_dup("bus DATA[0:3] created");}}
    if (ui_button("GDT", fb_x + 1208, act_y, 38, 22))
        { char *gd=gdt_feature_frame("Boss",25.0f,0.1f,0.05f,"A"); free(app->status); app->status = gd; }
    if (ui_button("BOpt", fb_x + 1252, act_y, 46, 22))
        { char *bo = bom_optimize(&app->proj); free(app->status); app->status = bo; }
    if (ui_button("MFG+", fb_x + 1304, act_y, 48, 22)) {
        Pcb *pcb = (app->sel_pcb >= 0 && app->sel_pcb < app->proj.pcbs.len) ? &app->proj.pcbs.v[app->sel_pcb] : NULL;
        if (pcb) { char *mo = manufacturing_optimize(pcb, 10); free(app->status); app->status = mo; }
    }
    if (ui_button("Meas", fb_x + 1358, act_y, 46, 22))
        { MeasurePoint a={0,0},b={30,40}; char *mr = measure_report(a,b); free(app->status); app->status = mr; }
    if (ui_button("XPrb", fb_x + 1410, act_y, 46, 22)) {
        Pcb *pcb = (app->sel_pcb >= 0 && app->sel_pcb < app->proj.pcbs.len) ? &app->proj.pcbs.v[app->sel_pcb] : NULL;
        const char *ref = pcb && pcb->nfps>0 && pcb->fps[0].ref ? pcb->fps[0].ref : "R1";
        char *xp = crossprobe_highlight(&app->proj, ref); free(app->status); app->status = xp;
    }
    if (ui_button("Splt", fb_x + 1462, act_y, 42, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { int n = object_split_into_project(&app->proj, &cm->mesh); char ms[32]; snprintf(ms,sizeof(ms),"Split into %d parts",n); free(app->status); app->status = str_dup(ms); app->cad_gen++; cache_clear(); }
    }
    if (ui_button("Maint", fb_x + 1510, act_y, 48, 22))
        { maint_log_event(f,"Voron 2.4","lubricate Z rods",142.0f); char *mh = maint_history_report(f); free(app->status); app->status = mh; }
    if (ui_button("FW", fb_x + 1564, act_y, 34, 22))
        { char *fw = firmware_update_status("Voron 2.4","klipper-v0.12"); free(app->status); app->status = fw; }
    if (ui_button("Stk", fb_x + 1604, act_y, 40, 22))
        { char *stk = stackup_default_4layer(); free(app->status); app->status = stk; }
    if (ui_button("AI", fb_x + 1650, act_y, 32, 22)) {
        Pcb *pcb = (app->sel_pcb >= 0 && app->sel_pcb < app->proj.pcbs.len) ? &app->proj.pcbs.v[app->sel_pcb] : NULL;
        if (pcb) { char *ae = ai_explain_design(pcb); free(app->status); app->status = ae; }
    }
    if (ui_button("T-S", fb_x + 1688, act_y, 36, 22))
        { char *ts = tspline_stub(); free(app->status); app->status = ts; }
    if (ui_button("5Axis", fb_x + 862, act_y, 50, 22)) {
        V3 pts[5] = {v3(10,10,0),v3(30,20,-2),v3(50,30,-5),v3(70,20,-2),v3(90,10,0)};
        V3 nrms[5] = {v3(0,0,1),v3(0,0,1),v3(0,0,1),v3(0,0,1),v3(0,0,1)};
        AxisPoint tp[256]; int n = cam5_simultaneous(pts,nrms,5,6.0f,1.0f,tp,256,"build\\cam_5axis_simul.ngc");
        char *rp = cam5_toolpath_report(tp,n); free(app->status); app->status = rp;
    }
    if (ui_button("Arc", fb_x + 816, act_y, 40, 22))
        { char *ag = arc_gcode(100,100,120,100,20,false,500); free(app->status); app->status = ag; }
    if (ui_button("Mesh", fb_x + 862, act_y, 44, 22))
        { char *bm = bed_mesh_gcode("klipper", 0, 350, 0, 350, 5); free(app->status); app->status = bm; }
    // third row: FreeCAD/iEDA features
    act_y += 28;
    if (ui_button("Sktch", fb_x, act_y, 54, 22)) {
        Sketch *sk = sketch_create("demo");
        sketch_add_line(sk, v2(0,0), v2(50,0)); sketch_add_line(sk, v2(50,0), v2(50,30));
        sketch_add_line(sk, v2(50,30), v2(0,30)); sketch_add_line(sk, v2(0,30), v2(0,0));
        sketch_add_horizontal(sk, 0); sketch_add_vertical(sk, 1);
        sketch_solve(sk); char *sr = sketch_report(sk); free(app->status); app->status = sr; sketch_free(sk);
    }
    if (ui_button("Page", fb_x + 60, act_y, 46, 22))
        { char *pl = pagelayout_title_block("KiCad Project", "User", "2026-08-02", "Rev B", 297, 210); free(app->status); app->status = pl; }
    if (ui_button("LibTB", fb_x + 112, act_y, 52, 22))
        { char *lt = libtable_get_default(); free(app->status); app->status = lt; }
    if (ui_button("IDF", fb_x + 170, act_y, 38, 22)) {
        Pcb *pcb = (app->sel_pcb >= 0 && app->sel_pcb < app->proj.pcbs.len) ? &app->proj.pcbs.v[app->sel_pcb] : NULL;
        if (pcb) { idf_export_board(pcb, "build\\board.idf"); free(app->status); app->status = str_dup("IDF exported"); }
    }
    if (ui_button("DPair", fb_x + 214, act_y, 50, 22)) {
        Pcb *pcb = (app->sel_pcb >= 0 && app->sel_pcb < app->proj.pcbs.len) ? &app->proj.pcbs.v[app->sel_pcb] : NULL;
        if (pcb) { diffpair_route_interactive(pcb, v2(40,48), v2(40,52), v2(60,48), v2(60,52), 0.2f, 0.3f); free(app->status); app->status = str_dup("diff pair routed"); }
    }
    if (ui_button("Adapt", fb_x + 270, act_y, 52, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { float ah[256]; int n = adaptive_layer_heights(&cm->mesh, 0.2f, 0.05f, 0.3f, ah, 256); char ms[64]; snprintf(ms, sizeof(ms), "Adaptive: %d layers (%.2f-%.2fmm)", n, ah[0], ah[n>1?n-1:0]); free(app->status); app->status = str_dup(ms); }
    }
    if (ui_button("DOF", fb_x + 328, act_y, 42, 22)) {
        Sketch *sk = sketch_create("tmp"); sketch_add_line(sk, v2(0,0), v2(50,0)); sketch_add_line(sk, v2(50,0), v2(50,30));
        sketch_add_horizontal(sk, 0); sketch_add_vertical(sk, 1);
        int dof = sketch_dof_count(sk); char ms[32]; snprintf(ms, sizeof(ms), "DOF: %d", dof);
        free(app->status); app->status = str_dup(ms); sketch_free(sk);
    }
    if (ui_button("Brdg", fb_x + 376, act_y, 46, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { float ang = surface_angle_at_z(&cm->mesh, (cm->mesh.bmin.z+cm->mesh.bmax.z)*0.5f); char ms[32]; snprintf(ms, sizeof(ms), "Angle@mid: %.0f deg", ang); free(app->status); app->status = str_dup(ms); }
    }
    if (ui_button("Deps", fb_x + 428, act_y, 48, 22)) {
        FeatureDep deps[32]; feature_dep_graph(app->feats, app->nfeats, deps, 32);
        char *dr = feature_dep_report(app->feats, app->nfeats, deps, app->nfeats); free(app->status); app->status = dr;
    }
    if (ui_button("Templ", fb_x + 482, act_y, 52, 22))
        { template_apply(&app->proj, TMPL_ARDUINO_UNO); free(app->status); app->status = str_dup("Arduino Uno template applied"); }
    if (ui_button("Lib", fb_x + 540, act_y, 38, 22)) {
        static LibComponent g_lib[16]; static int g_nlib=0;
        if (g_nlib==0) {
            g_lib[g_nlib] = *libcomponent_create(&app->proj, "R_0805","Resistor_SMD:R_0805_2012Metric","",""); g_nlib++;
            g_lib[g_nlib] = *libcomponent_create(&app->proj, "C_0805","Capacitor_SMD:C_0805_2012Metric","",""); g_nlib++;
            g_lib[g_nlib] = *libcomponent_create(&app->proj, "LED_0805","LED_SMD:LED_0805_2012Metric","",""); g_nlib++;
            libcomponent_merge(&g_lib[0], &g_lib[1]); // merge first two as test
        }
        char *lr = lib_browser_report(g_lib, g_nlib); free(app->status); app->status = lr;
    }
    if (ui_button("3D", fb_x + 584, act_y, 34, 22))
        { int l = kicad_link_3d_models(&app->proj, "build\\3d_models"); char ms[32]; snprintf(ms,sizeof(ms),"3D linked: %d",l); free(app->status); app->status = str_dup(ms); }
    if (ui_button("FTL", fb_x + 624, act_y, 40, 22))
        { char *tl = fusion_tool_list_all(); free(app->status); app->status = tl; }
    if (ui_button("FMat", fb_x + 670, act_y, 46, 22))
        { char *ml = fusion_material_list_all(); free(app->status); app->status = ml; }
    if (ui_button("Mach", fb_x + 722, act_y, 48, 22))
        { char *mc = fusion_machine_list_all(); free(app->status); app->status = mc; }
    if (ui_button("Rec", fb_x + 776, act_y, 40, 22))
        { char *rc = fusion_cam_recommendations("Aluminum 6061", "pocket"); free(app->status); app->status = rc; }
    if (ui_button("Save", fb_x + 822, act_y, 44, 22))
        { fusion_library_save("C:\\Users\\realc\\AppData\\Roaming\\Autodesk\\CAM360\\libraries\\Local\\Library.json"); free(app->status); app->status = str_dup("Fusion library saved"); }
    if (ui_button("Web", fb_x + 872, act_y, 40, 22))
        { website_generate("build"); free(app->status); app->status = str_dup("Website generated at build/index.html — deploy to GitHub Pages!"); }
    if (ui_button("Cloud", fb_x + 918, act_y, 50, 22)) {
        CloudSaveConfig cs; cloudsave_defaults(&cs);
        char *cl = cloudsave_list(&cs); free(app->status); app->status = cl;
    }
    if (ui_button("FEM", fb_x + 60, act_y, 42, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { char *fr = fem_static_analysis(&cm->mesh, 70, 0.33f, NULL, 0, NULL, 0); free(app->status); app->status = fr; }
        else { char *fr = fem_modal_analysis(NULL, 70, 0.33f, 2700, 3); free(app->status); app->status = fr; }
    }
    if (ui_button("Draw", fb_x + 108, act_y, 44, 22)) {
        CadModel *cm = (app->sel_cad >= 0 && app->sel_cad < app->proj.cad_models.len) ? &app->proj.cad_models.v[app->sel_cad] : NULL;
        if (cm && cm->mesh.valid) { char *tv = techdraw_all_views(&cm->mesh, cm->name); free(app->status); app->status = tv; }
    }
    if (ui_button("Robot", fb_x + 158, act_y, 52, 22)) {
        DHParam dh[6] = {{0,0,0,0},{100,0,0,0},{100,0,0,0},{0,0,0,0},{0,50,0,0},{0,20,0,0}};
        float a[6] = {0,0.5f,-0.3f,0,1.0f,0};
        char *rr = robot_kinematics_report(dh,6,a); free(app->status); app->status = rr;
    }
    if (ui_button("PyAPI", fb_x + 216, act_y, 54, 22))
        { char *pa = python_api_docs(); free(app->status); app->status = pa; }
    if (ui_button("Timing", fb_x + 276, act_y, 56, 22)) {
        Pcb *pcb = (app->sel_pcb >= 0 && app->sel_pcb < app->proj.pcbs.len) ? &app->proj.pcbs.v[app->sel_pcb] : NULL;
        if (pcb) { char *tr = timing_report(pcb, 100); free(app->status); app->status = tr; }
    }
    if (ui_button("OCCT", fb_x + 338, act_y, 46, 22))
        { char *oc = opencascade_compat_notes(); free(app->status); app->status = oc; }
    if (ui_button("Sheet", fb_x + 390, act_y, 52, 22))
        { char *ss = spreadsheet_create_sample(); free(app->status); app->status = ss; }
    int right = fb_x + fb_w - 4;
    DrawText("click jobs/spools/orders in sidebar to select", right - 240, act_y + 4, 12, DARKGRAY);
    py = act_y + 30;

    // left: printers
    int col_w = fb_w / 2 - 4;
    DrawRectangle(fb_x, py, col_w, 1, DARKGRAY);
    DrawText("Printers:", fb_x, py + 2, 13, LIGHTGRAY); py += 18;
    for (int i = 0; i < f->n_printers && py < y + h - 30; i++) {
        FarmPrinter *pr = &f->printers[i];
        Color stc = pr->busy ? ORANGE : GREEN;
        if (strcmp(pr->status, "error") == 0) stc = RED;
        if (strcmp(pr->status, "offline") == 0) stc = GRAY;
        snprintf(buf, sizeof(buf), "[%d] %s", i + 1, pr->name);
        DrawText(buf, fb_x + 4, py, 12, WHITE);
        snprintf(buf, sizeof(buf), "%s", pr->status);
        DrawText(buf, fb_x + 156, py, 12, stc);
        snprintf(buf, sizeof(buf), "jobs:%d hrs:%.0f slots:%d", pr->total_jobs_done, pr->hours_running, pr->tool_slots);
        DrawText(buf, fb_x + 210, py, 11, GRAY);
        if (pr->filament_type[0]) { snprintf(buf, sizeof(buf), "FIL:%s", pr->filament_type); DrawText(buf, fb_x + 380, py, 11, GRAY); }
        if (ui_button("Toggle", fb_x + col_w - 56, py - 1, 52, 14)) {
            if (pr->busy) { pr->busy = 0; snprintf(pr->status, sizeof(pr->status), "idle"); snprintf(pr->current_job_id, sizeof(pr->current_job_id), ""); }
            else { pr->busy = 1; snprintf(pr->status, sizeof(pr->status), "busy"); }
        }
        py += 14;
    }
    if (f->n_printers == 0) { DrawText("no printers", fb_x + 8, py, 12, GRAY); py += 14; }

    // right: jobs + orders + spools
    int ry_start = py - (f->n_printers > 0 ? f->n_printers : 1) * 14 - 26;
    if (ry_start < act_y + 30) ry_start = act_y + 30;
    int rx = fb_x + col_w + 8, ry = ry_start;
    DrawRectangle(rx, ry, col_w, 1, DARKGRAY);
    int nq = 0, nrun = 0, ndone = 0, nfail = 0;
    for (int j = 0; j < f->n_jobs; j++) {
        if (strcmp(f->jobs[j].status, "queued") == 0) nq++;
        else if (strcmp(f->jobs[j].status, "running") == 0) nrun++;
        else if (strcmp(f->jobs[j].status, "done") == 0) ndone++;
        else if (strcmp(f->jobs[j].status, "failed") == 0) nfail++;
    }
    snprintf(buf, sizeof(buf), "Jobs: %d q / %d run / %d done / %d fail (%d total)",
             nq, nrun, ndone, nfail, f->n_jobs);
    DrawText(buf, rx, ry + 2, 13, LIGHTGRAY); ry += 18;
    for (int i = 0; i < f->n_jobs && ry < y + h - 30; i++) {
        FarmJob *jb = &f->jobs[i];
        Color jc = GRAY;
        if (strcmp(jb->status, "running") == 0) jc = ORANGE;
        if (strcmp(jb->status, "done") == 0) jc = GREEN;
        if (strcmp(jb->status, "failed") == 0) jc = RED;
        snprintf(buf, sizeof(buf), "[%d] %s", i + 1, jb->name);
        DrawText(buf, rx + 4, ry, 12, WHITE);
        snprintf(buf, sizeof(buf), "%s", jb->status);
        DrawText(buf, rx + 160, ry, 12, jc);
        int bx = rx + col_w - 160;
        char rs[8]; snprintf(rs, sizeof(rs), "Run");
        if (ui_button(rs, bx, ry - 1, 36, 14)) {
            snprintf(jb->status, sizeof(jb->status), "running");
            jb->finished = 0;
            for (int p = 0; p < f->n_printers; p++) if (f->printers[p].busy && strcmp(f->printers[p].current_job_id, jb->id) == 0) continue;
            // start all queued jobs on idle printers
            for (int p = 0; p < f->n_printers && jb->finished == 0; p++)
                if (!f->printers[p].busy) { f->printers[p].busy = 1; snprintf(f->printers[p].status, sizeof(f->printers[p].status), "busy"); snprintf(f->printers[p].current_job_id, sizeof(f->printers[p].current_job_id), jb->id); snprintf(jb->assigned_printer, sizeof(jb->assigned_printer), f->printers[p].name); break; }
        }
        char ds[8]; snprintf(ds, sizeof(ds), "Done");
        if (ui_button(ds, bx + 40, ry - 1, 38, 14)) {
            snprintf(jb->status, sizeof(jb->status), "done"); jb->finished = 1;
            for (int p = 0; p < f->n_printers; p++) if (strcmp(f->printers[p].current_job_id, jb->id) == 0)
                { f->printers[p].busy = 0; f->printers[p].total_jobs_done++; snprintf(f->printers[p].status, sizeof(f->printers[p].status), "idle"); snprintf(f->printers[p].current_job_id, sizeof(f->printers[p].current_job_id), ""); break; }
        }
        char fs[8]; snprintf(fs, sizeof(fs), "Fail");
        if (ui_button(fs, bx + 82, ry - 1, 34, 14)) {
            snprintf(jb->status, sizeof(jb->status), "failed"); jb->finished = 1;
            for (int p = 0; p < f->n_printers; p++) if (strcmp(f->printers[p].current_job_id, jb->id) == 0)
                { f->printers[p].busy = 0; snprintf(f->printers[p].status, sizeof(f->printers[p].status), "idle"); snprintf(f->printers[p].current_job_id, sizeof(f->printers[p].current_job_id), ""); break; }
        }
        if (jb->assigned_printer[0]) { snprintf(buf, sizeof(buf), "-> %s", jb->assigned_printer); DrawText(buf, rx + 260, ry, 11, GRAY); }
        ry += 14;
    }
    if (f->n_jobs == 0) { DrawText("no jobs", rx + 4, ry, 12, GRAY); ry += 14; }
    ry += 2;
    snprintf(buf, sizeof(buf), "Orders: %d | Spools: %d | Files: %d", f->n_orders, f->n_spools, f->n_files);
    DrawText(buf, rx, ry, 12, LIGHTGRAY);

    // -- network printing --
    int nw_y = (ry > py ? ry : py) + 8;
    if (nw_y < py + 100) nw_y = py + 100;
    DrawText("Network (Moonraker):", x + 12, nw_y, 15, LIGHTGRAY); nw_y += 18;
    DrawText(app->printer_ip, x + 16, nw_y, 13, WHITE);
    if (ui_button("Status", x + 220, nw_y - 1, 60, 22)) {
        char st[64]; int sc = mr_status("localhost", 7125, st, sizeof(st));
        char ms[128]; snprintf(ms, sizeof(ms), "Printer %s (HTTP %d)", st, sc);
        free(app->status); app->status = str_dup(ms);
    }
    if (ui_button("Pause", x + 284, nw_y - 1, 54, 22)) mr_pause("localhost", 7125);
    if (ui_button("Resume", x + 342, nw_y - 1, 60, 22)) mr_resume("localhost", 7125);
    if (ui_button("Cancel", x + 406, nw_y - 1, 56, 22)) mr_cancel("localhost", 7125);
    if (ui_button("Send Gcode", x + 466, nw_y - 1, 90, 22) && app->last_gcode[0]) {
        mr_upload_and_print("localhost", 7125, app->last_gcode, app->last_gcode);
        free(app->status); app->status = str_dup("gcode sent to printer");
    }
    if (ui_button("USB", x + 562, nw_y - 1, 42, 22) && app->last_gcode[0]) {
        usb_print("COM3", app->last_gcode);
        free(app->status); app->status = str_dup("sent via USB");
    }
    if (ui_button("Octo", x + 610, nw_y - 1, 48, 22)) {
        char st[64]; octo_status("localhost", 5000, "APIKEY", st, sizeof(st));
        char ms[128]; snprintf(ms, sizeof(ms), "OctoPrint: %s", st);
        free(app->status); app->status = str_dup(ms);
    }
    nw_y += 28;

    // -- CAM: CNC milling --
    DrawText("CAM: CNC Milling", x + 12, nw_y, 15, LIGHTGRAY); nw_y += 20;
    CamSettings cam; cam_defaults(&cam);
    if (ui_button("Face 50x50", x + 12 + 0, nw_y, 100, 24))
        { cam_face("build\\cam_face.ngc", &cam, 10, 10, 50, 50, -1.0f); free(app->status); app->status = str_dup("face toolpath saved"); }
    if (ui_button("Profile 50x50", x + 12 + 106, nw_y, 110, 24)) {
        V2 sq[4] = { v2(10,10), v2(60,10), v2(60,60), v2(10,60) };
        cam_profile("build\\cam_profile.ngc", &cam, sq, 4, -2.0f);
        free(app->status); app->status = str_dup("profile toolpath saved");
    }
    if (ui_button("Pocket 50x50", x + 12 + 222, nw_y, 110, 24)) {
        V2 sq[4] = { v2(10,10), v2(60,10), v2(60,60), v2(10,60) };
        cam_pocket("build\\cam_pocket.ngc", &cam, sq, 4, -2.0f);
        free(app->status); app->status = str_dup("pocket toolpath saved");
    }
    if (ui_button("Drill 4 holes", x + 12 + 338, nw_y, 100, 24)) {
        V2 hls[4] = { v2(20,20), v2(50,20), v2(50,50), v2(20,50) };
        cam_drill("build\\cam_drill.ngc", &cam, hls, 4, -2.0f, 0.5f);
        free(app->status); app->status = str_dup("drill toolpath saved");
    }
    nw_y += 28;
    if (pcb) {
        if (ui_button("PCB Isolate", x + 12, nw_y, 120, 24)) {
            cam_pcb_isolate("build\\cam_pcb_isolate.ngc", &cam, pcb, 0.2f, -0.1f);
            free(app->status); app->status = str_dup("PCB isolation toolpath saved");
        }
        if (ui_button("PCB Outline", x + 138, nw_y, 120, 24)) {
            cam_pcb_outline("build\\cam_pcb_outline.ngc", &cam, pcb, -2.0f, 4, 3.0f);
            free(app->status); app->status = str_dup("PCB outline toolpath saved");
        }
    }
    nw_y += 28;
    if (ui_button("LaserCut", x + 12, nw_y, 84, 22)) {
        V2 sq[4] = { v2(10,10), v2(50,10), v2(50,30), v2(10,30) };
        V2 *pls[1] = { sq }; int pln[1] = { 4 };
        cam_laser_cut("build\\cam_laser.ngc", pls, pln, 1, 500.0f);
        free(app->status); app->status = str_dup("laser cut saved");
    }
    if (ui_button("Engrave", x + 102, nw_y, 72, 22)) {
        V2 sq[4] = { v2(10,10), v2(50,10), v2(50,30), v2(10,30) };
        cam_laser_engrave("build\\cam_engrave.ngc", sq, 4, 1.0f, 800.0f);
        free(app->status); app->status = str_dup("engrave saved");
    }
    if (ui_button("Waterjet", x + 180, nw_y, 74, 22)) {
        V2 sq[4] = { v2(10,10), v2(50,10), v2(50,30), v2(10,30) };
        cam_waterjet_cut("build\\cam_waterjet.ngc", sq, 4, 3.0f, 300.0f);
        free(app->status); app->status = str_dup("waterjet saved");
    }
    if (ui_button("Plasma", x + 260, nw_y, 62, 22)) {
        V2 sq[4] = { v2(10,10), v2(50,10), v2(50,30), v2(10,30) };
        cam_plasma_cut("build\\cam_plasma.ngc", sq, 4, 0.3f, 2.0f, 600.0f);
        free(app->status); app->status = str_dup("plasma saved");
    }
    if (ui_button("3DCont", x + 328, nw_y, 62, 22)) {
        V3 tp[3] = { v3(10,10,0), v3(30,20,-2), v3(50,10,0) };
        cam_3d_contour("build\\cam_3dcont.ngc", tp, 3, 400.0f, 5.0f);
        free(app->status); app->status = str_dup("3D contour saved");
    }
    if (ui_button("Adaptive", x + 396, nw_y, 74, 22)) {
        V2 sq[4] = { v2(10,10), v2(50,10), v2(50,40), v2(10,40) };
        cam_adaptive_clear("build\\cam_adaptive.ngc", sq, 4, -2.0f, 3.0f, 0.4f, 600.0f, 200.0f, 5.0f);
        free(app->status); app->status = str_dup("adaptive clearing saved");
    }
    nw_y += 28;
    if (ui_button("4-Axis", x + 12, nw_y, 64, 22))
        { cam4_rotary("build\\cam_4axis.ngc", 20.0f, 50.0f, 500.0f, 12); free(app->status); app->status = str_dup("4-axis rotary saved"); }
    if (ui_button("5-Axis", x + 82, nw_y, 56, 22))
        { cam5_contour("build\\cam_5axis.ngc", 400.0f); free(app->status); app->status = str_dup("5-axis contour saved"); }
    if (ui_button("MillTurn", x + 144, nw_y, 68, 22))
        { cam_millturn("build\\cam_millturn.ngc", 30.0f, -2.0f, 300.0f); free(app->status); app->status = str_dup("mill-turn saved"); }
    if (ui_button("Probe", x + 218, nw_y, 56, 22))
        { cam_probe_grid("build\\cam_probe.ngc", 0, 0, 50, 50, 5.0f, -3.0f, 5, 5); free(app->status); app->status = str_dup("probe grid saved"); }
}

static void draw_assist(App *app, int x, int y, int w, int h) {
    DrawText("AI Assistant (natural-language parametric design)", x + 12, y + 10, 18, WHITE);
    // transcript
    DrawRectangle(x + 8, y + 40, w - 16, h - 110, (Color){ 30, 32, 40, 255 });
    int py = y + 46;
    const char *t = app->chat_log ? app->chat_log : "";
    int n = (int)strlen(t);
    int start = n > 0 ? n : 0;
    // simple scrolling: show last portion
    (void)start;
    char line[512];
    int li = 0, lstart = 0;
    // draw from the beginning (log is small)
    for (int i = 0; i <= n && py < y + h - 80; i++) {
        if (i == n || t[i] == '\n') {
            int len = i - lstart;
            if (len >= (int)sizeof(line)) len = sizeof(line) - 1;
            memcpy(line, t + lstart, (size_t)len);
            line[len] = '\0';
            Color c = (strncmp(line, "you>", 4) == 0) ? (Color){ 180, 200, 255, 255 } : WHITE;
            DrawText(line, x + 14, py, 13, c);
            py += 16;
            lstart = i + 1;
            li++;
        }
    }
    if (li == 0) DrawText("(no messages)", x + 14, py, 13, GRAY);
    // input box
    int iy = y + h - 60;
    DrawRectangle(x + 8, iy, w - 16, 28, (Color){ 50, 52, 62, 255 });
    DrawRectangleLines(x + 8, iy, w - 16, 28, (Color){ 120, 130, 150, 255 });
    DrawText(app->chat_input[0] ? app->chat_input : "type a request...", x + 14, iy + 7, 14, app->chat_input[0] ? WHITE : GRAY);

    // text input via raylib
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 126 && strlen(app->chat_input) < sizeof(app->chat_input) - 1) {
            size_t len = strlen(app->chat_input);
            app->chat_input[len] = (char)key;
            app->chat_input[len + 1] = '\0';
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && strlen(app->chat_input) > 0) {
        app->chat_input[strlen(app->chat_input) - 1] = '\0';
    }
    if (IsKeyPressed(KEY_ENTER)) assist_submit(app);
}
