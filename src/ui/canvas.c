#include "canvas.h"
#include "../design/kicad_draw.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

CanvasView canvas_view_create(int x, int y, int w, int h) {
    CanvasView cv = {x,y,w,h,1.0f,{0,0},1.0f,1,1}; return cv;
}

// ===== EDA CANVAS =====
void canvas_eda_background(CanvasView *cv) {
    if (!cv || !cv->show_grid) return;
    float gs = cv->grid_size;
    if (gs * cv->zoom < 4.0f) return;
    float ox = fmodf(cv->pan.x, gs) * cv->zoom;
    float oy = fmodf(cv->pan.y, gs) * cv->zoom;
    Color gc = {50,52,56,255};
    for (float gx = -ox; gx < cv->w; gx += gs * cv->zoom) DrawLine(cv->x+(int)gx, cv->y, cv->x+(int)gx, cv->y+cv->h, gc);
    for (float gy = -oy; gy < cv->h; gy += gs * cv->zoom) DrawLine(cv->x, cv->y+(int)gy, cv->x+cv->w, cv->y+(int)gy, gc);
}

void canvas_eda_origin(CanvasView *cv) {
    if (!cv) return;
    float ox = cv->x + cv->w*0.5f - cv->pan.x * cv->zoom;
    float oy = cv->y + cv->h*0.5f - cv->pan.y * cv->zoom;
    DrawLine(cv->x, (int)oy, cv->x+cv->w, (int)oy, (Color){40,42,48,255}); // X axis
    DrawLine((int)ox, cv->y, (int)ox, cv->y+cv->h, (Color){40,42,48,255}); // Y axis
    // origin marker
    DrawCircle((int)ox, (int)oy, 4, (Color){100,160,220,255});
}

void canvas_eda_heatmap(Pcb *pcb, CanvasView *cv) {
    if (!pcb || !cv) return;
    // simple: highlight high-activity areas (where many tracks overlap)
    for (int i = 0; i < pcb->ntracks && i < pcb->ntracks; i++) {
        float cx = (pcb->tracks[i].a.x + pcb->tracks[i].b.x) * 0.5f;
        float cy = (pcb->tracks[i].a.y + pcb->tracks[i].b.y) * 0.5f;
        int sx = (int)(cv->x + cv->w*0.5f + (cx - cv->pan.x)*cv->zoom);
        int sy = (int)(cv->y + cv->h*0.5f + (cy - cv->pan.y)*cv->zoom);
        DrawRectangle(sx-1, sy-1, 3, 3, (Color){255,100,50,80});
    }
}

char *canvas_eda_context_menu(int mx, int my, Pcb *pcb, int sel_fp) {
    (void)mx; (void)my; (void)pcb; (void)sel_fp;
    return NULL; // returns action string when implemented with raylib mouse
}

// ===== CAD CANVAS =====
void canvas_cad_workbench_tabs(int x, int y, int w) {
    const char *tabs[] = {"Part","Part Design","Sketcher","TechDraw","FEM","Mesh"};
    for (int i = 0; i < 6; i++) {
        int tx = x + i * 80;
        DrawRectangle(tx, y-2, 76, 24, (Color){50,55,65,255});
        DrawText(tabs[i], tx+6, y+4, 12, GRAY);
    }
}

void canvas_cad_feature_tree(DesignFeature *feats, int n, int sel, int x, int y, int w, int h) {
    DrawRectangle(x, y, w, h, (Color){24,26,30,255});
    DrawText("Model Tree", x+8, y+6, 13, GRAY);
    for (int i = 0; i < n && y+24+i*16 < y+h; i++) {
        const char *tname = feats[i].type==0?"Box":feats[i].type==1?"Cylinder":"Sphere";
        char buf[64]; snprintf(buf,sizeof(buf)," [%d] %s",i+1,tname);
        DrawText(buf, x+12, y+24+i*16, 12, sel==i?YELLOW:GRAY);
    }
}

void canvas_cad_gizmo(V3 pos, int *drag_axis, int x, int y) {
    (void)pos; (void)drag_axis; (void)x; (void)y;
    // would draw 3-axis transform arrows
}

// ===== SLICER CANVAS =====
void canvas_slicer_build_plate(CanvasView *cv, float bed_x, float bed_y) {
    if (!cv) return;
    // gray plate background
    DrawRectangle(cv->x, cv->y, cv->w, cv->h, (Color){45,48,55,255});
    // plate outline
    int px = cv->x + 20, py = cv->y + 20, pw = cv->w - 40, ph = cv->h - 40;
    DrawRectangleLines(px, py, pw, ph, (Color){80,85,95,255});
    DrawText("Build Plate", px+8, py+4, 12, DARKGRAY);
    // ruler marks
    for (int i = 0; i < 6; i++) {
        DrawLine(px + pw*i/5, py-5, px + pw*i/5, py, (Color){80,85,95,255});
        char buf[16]; snprintf(buf, sizeof(buf), "%.0f", bed_x*i/5);
        DrawText(buf, px + pw*i/5 - 10, py-18, 10, GRAY);
    }
}

void canvas_slicer_model_arrange(SliceResult *sr, CanvasView *cv) {
    (void)sr; (void)cv;
    // draw model outlines on plate
}

void canvas_slicer_layer_preview(SliceResult *sr, int layer_idx, CanvasView *cv) {
    if (!sr || !cv || layer_idx < 0 || layer_idx >= sr->nlayers) return;
    SliceLayer *L = &sr->layers[layer_idx];
    // draw paths for this layer
    for (int j = 0; j < L->npaths; j++) {
        SlicePath *p = &L->paths[j];
        Color pc = p->kind == 0 ? RED : (p->solid ? ORANGE : BLUE);
        for (int k = 1; k < p->npts; k++)
            DrawLine((int)(cv->x + p->pts[k-1].x), (int)(cv->y + p->pts[k-1].y),
                     (int)(cv->x + p->pts[k].x), (int)(cv->y + p->pts[k].y), pc);
    }
}

void canvas_slicer_stats_panel(SliceResult *sr, const SliceSettings *cfg, int x, int y) {
    if (!sr || !cfg) return;
    DrawRectangle(x, y, 200, 100, (Color){30,32,38,255});
    DrawRectangleLines(x, y, 200, 100, (Color){60,62,68,255});
    char buf[128];
    snprintf(buf,sizeof(buf),"Filament: %.1f m", sr->filament_mm/1000.0f);
    DrawText(buf, x+8, y+6, 13, WHITE);
    snprintf(buf,sizeof(buf),"Time: %.0f min", sr->est_time_s/60.0f);
    DrawText(buf, x+8, y+24, 13, WHITE);
    snprintf(buf,sizeof(buf),"Layers: %d", sr->nlayers);
    DrawText(buf, x+8, y+42, 13, WHITE);
    snprintf(buf,sizeof(buf),"Cost: ~$%.2f", sr->filament_mm/1000.0f*0.03f);
    DrawText(buf, x+8, y+60, 13, GRAY);
}

int canvas_slicer_layer_slider(int x, int y, int w, int n_layers) {
    if (n_layers < 1) return 0;
    DrawRectangle(x, y, w, 6, (Color){60,60,70,255});
    Vector2 mp = GetMousePosition();
    static int layer = 0;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && mp.x >= x && mp.x <= x+w && mp.y >= y-6 && mp.y <= y+10) {
        float t = (mp.x - x) / (float)w;
        layer = (int)(t * n_layers);
        if (layer < 0) layer = 0; if (layer >= n_layers) layer = n_layers-1;
    }
    DrawRectangle(x + layer*w/n_layers, y-2, 4, 10, (Color){255,200,100,255});
    char buf[32]; snprintf(buf,sizeof(buf),"Layer %d/%d", layer+1, n_layers);
    DrawText(buf, x+w+8, y-4, 12, WHITE);
    return layer;
}

// ===== MIXED RENDERER =====
void canvas_render(App *app, CanvasView *cv) {
    if (!app || !cv) return;
    switch (app->mode) {
        case UI_SCH: case UI_PCB:
            canvas_eda_background(cv);
            canvas_eda_origin(cv);
            break;
        case UI_3D: case UI_DESIGN:
            // 3D renders separately, DESIGN uses its own draw function
            break;
        case UI_PRINT:
            canvas_slicer_build_plate(cv, 250, 250);
            break;
        default: break;
    }
}
