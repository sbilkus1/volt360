#include "profui.h"
#include "../core/util.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static ProfTheme g_theme = {{28,30,34},{24,26,30},{220,225,235},{100,160,220},{255,200,100},{60,62,68},{55,60,75},{70,90,130}};
static int g_splitter_drag = 0;
static char g_console[16][128]; static int g_console_lines = 0;
static int g_console_scroll = 0;

ProfTheme prof_theme_dark(void) { return (ProfTheme){{28,30,34},{24,26,30},{220,225,235},{100,160,220},{255,200,100},{60,62,68},{55,60,75},{70,90,130}}; }
ProfTheme prof_theme_get(void) { return g_theme; }
void prof_theme_set(ProfTheme t) { g_theme = t; }

// ===== MENU BAR =====
static const char *g_menu_file[] = {"New|Ctrl+N","Open|Ctrl+O","Save|Ctrl+S","Save As...","-","Export All...","Import...","-","Exit|Alt+F4"};
static const char *g_menu_edit[] = {"Undo|Ctrl+Z","Redo|Ctrl+Y","-","Delete|Del","Select All|Ctrl+A"};
static const char *g_menu_view[] = {"Zoom In|+","Zoom Out|-","Zoom Fit|Home","-","Toggle Grid|G","3D Wireframe|W"};
static const char *g_menu_tools[] = {"DRC Check","ERC Check","SPICE Simulate","-","Calibration Suite","PCB Calculator"};
static const char *g_menu_help[] = {"About","Keyboard Shortcuts|?","-","Tutorial"};

static int g_active_menu = -1;
static int g_active_sub = -1;

MenuAction prof_menu_bar(int x, int y, int w, int h) {
    ProfTheme *t = &g_theme;
    DrawRectangle(x, y, w, h, (Color){t->panel[0],t->panel[1],t->panel[2],255});
    DrawLine(x, y+h-1, x+w, y+h-1, (Color){t->border[0],t->border[1],t->border[2],255});

    const char *menus[] = {" File ", " Edit ", " View ", " Tools ", " Help "};
    const char **submenus[] = {g_menu_file, g_menu_edit, g_menu_view, g_menu_tools, g_menu_help};
    int counts[] = {9, 5, 6, 6, 5};
    int mx = x + 4;

    for (int i = 0; i < 5; i++) {
        int mw = MeasureText(menus[i], 14) + 8;
        Vector2 mp = GetMousePosition();
        int hover = mp.x >= mx && mp.x <= mx+mw && mp.y >= y && mp.y <= y+h;
        if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) g_active_menu = (g_active_menu == i) ? -1 : i;
        DrawText(menus[i], mx+4, y+4, 14, g_active_menu==i ? YELLOW : (Color){t->text[0],t->text[1],t->text[2],255});
        if (g_active_menu == i) {
            int sy = y + h, sw = 180, sh = counts[i] * 22 + 4;
            DrawRectangle(mx, sy, sw, sh, (Color){t->panel[0],t->panel[1],t->panel[2],255});
            DrawRectangleLines(mx, sy, sw, sh, (Color){t->border[0],t->border[1],t->border[2],255});
            for (int j = 0; j < counts[i]; j++) {
                if (submenus[i][j][0] == '-') { DrawLine(mx+4, sy+4+j*22+10, mx+sw-4, sy+4+j*22+10, (Color){t->border[0],t->border[1],t->border[2],255}); continue; }
                bool sub_hover = mp.x >= mx && mp.x <= mx+sw && mp.y >= sy+4+j*22 && mp.y <= sy+4+(j+1)*22;
                DrawText(submenus[i][j], mx+8, sy+6+j*22, 14, sub_hover ? YELLOW : (Color){t->text[0],t->text[1],t->text[2],255});
                if (sub_hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { g_active_menu = -1; g_active_sub = j; }
            }
        }
        mx += mw;
    }
    return MENU_NONE;
}

// ===== TOOLBAR =====
void prof_toolbar_draw(App *app, int x, int y, int w, int h) {
    ProfTheme *t = &g_theme;
    DrawRectangle(x, y, w, h, (Color){t->panel[0],t->panel[1],t->panel[2],255});
    DrawLine(x, y+h-1, x+w, y+h-1, (Color){t->border[0],t->border[1],t->border[2],255});
    const char *mode_name = "Schematic";
    switch (app->mode) { case UI_PCB: mode_name="PCB Editor"; break; case UI_3D: mode_name="3D Viewer"; break;
        case UI_FIT: mode_name="Fit Analysis"; break; case UI_DESIGN: mode_name="Design Workspace"; break;
        case UI_PRINT: mode_name="Print Center"; break; default: break; }
    DrawText(mode_name, x+8, y+6, 14, YELLOW);
}

// ===== PROPERTIES PANEL =====
void prof_properties_draw(App *app, int x, int y, int w, int h) {
    ProfTheme *t = &g_theme;
    DrawRectangle(x, y, w, h, (Color){t->panel[0],t->panel[1],t->panel[2],255});
    DrawRectangleLines(x, y, w, h, (Color){t->border[0],t->border[1],t->border[2],255});
    DrawText("Properties", x+8, y+6, 14, (Color){t->text[0],t->text[1],t->text[2],255});
    int py = y + 26;
    // show selected item info
    if (app->mode == UI_PCB && app->pcb_fp_sel >= 0) {
        Pcb *pcb = (app->sel_pcb >= 0 && app->sel_pcb < app->proj.pcbs.len) ? &app->proj.pcbs.v[app->sel_pcb] : NULL;
        if (pcb && app->pcb_fp_sel < pcb->nfps) {
            FpInst *f = &pcb->fps[app->pcb_fp_sel];
            char buf[64];
            snprintf(buf, sizeof(buf), "Ref: %s", f->ref ? f->ref : "?"); DrawText(buf, x+8, py, 13, WHITE); py+=18;
            snprintf(buf, sizeof(buf), "Footprint: %s", f->footprint ? f->footprint : "?"); DrawText(buf, x+8, py, 13, GRAY); py+=18;
            snprintf(buf, sizeof(buf), "Pos: %.1f, %.1f", f->pos.x, f->pos.y); DrawText(buf, x+8, py, 13, GRAY); py+=18;
            snprintf(buf, sizeof(buf), "Rot: %.0f", f->rotation); DrawText(buf, x+8, py, 13, GRAY); py+=18;
        }
    } else if (app->mode == UI_3D && app->sel_cad >= 0) {
        DrawText("Selected model", x+8, py, 13, WHITE); py+=18;
    } else if (app->mode == UI_DESIGN && app->sel_feat >= 0 && app->sel_feat < app->nfeats) {
        DesignFeature *f = &app->feats[app->sel_feat];
        char buf[64];
        snprintf(buf, sizeof(buf), "Type: %s", f->type==0?"Box":f->type==1?"Cylinder":"Sphere"); DrawText(buf, x+8, py, 13, WHITE); py+=18;
        snprintf(buf, sizeof(buf), "W:%.0f H:%.0f D:%.0f", f->w, f->h, f->d); DrawText(buf, x+8, py, 13, GRAY); py+=18;
    }
}

// ===== CONSOLE PANEL =====
void prof_console_write(const char *msg) {
    if (!msg || g_console_lines >= 16) return;
    snprintf(g_console[g_console_lines], 128, "%s", msg);
    g_console_lines++;
}

void prof_console_draw(int x, int y, int w, int h) {
    ProfTheme *t = &g_theme;
    DrawRectangle(x, y, w, h, (Color){t->panel[0],t->panel[1],t->panel[2],255});
    DrawRectangleLines(x, y, w, h, (Color){t->border[0],t->border[1],t->border[2],255});
    DrawText("Output", x+8, y+6, 13, (Color){t->text[0],t->text[1],t->text[2],255});
    for (int i = 0; i < g_console_lines; i++)
        DrawText(g_console[i], x+8, y+24+i*16, 12, GRAY);
}

// ===== LAYER PANEL =====
void prof_layers_draw(App *app, int x, int y, int w, int h) {
    ProfTheme *t = &g_theme;
    DrawRectangle(x, y, w, h, (Color){t->panel[0],t->panel[1],t->panel[2],255});
    DrawRectangleLines(x, y, w, h, (Color){t->border[0],t->border[1],t->border[2],255});
    DrawText("Layers", x+8, y+6, 13, (Color){t->text[0],t->text[1],t->text[2],255});
    const char *lnames[] = {"F.Cu","B.Cu","F.SilkS","F.Mask","Edge.Cuts"};
    unsigned char lcolors[][3] = {{200,52,52},{50,180,50},{200,200,52},{200,52,200},{120,120,120}};
    for (int i = 0; i < 5 && i+1 < 5; i++) {
        DrawRectangle(x+12, y+26+i*20, 12, 12, (Color){lcolors[i][0],lcolors[i][1],lcolors[i][2],255});
        DrawText(lnames[i], x+30, y+24+i*20, 12, (Color){t->text[0],t->text[1],t->text[2],255});
    }
}

// ===== CONTEXT MENU =====
CtxAction prof_context_menu(int x, int y, const char **items, int n) {
    if (n < 1) return CTX_NONE;
    int w = 160, h = n * 22 + 4;
    DrawRectangle(x, y, w, h, (Color){g_theme.panel[0],g_theme.panel[1],g_theme.panel[2],255});
    DrawRectangleLines(x, y, w, h, (Color){g_theme.border[0],g_theme.border[1],g_theme.border[2],255});
    Vector2 mp = GetMousePosition();
    for (int i = 0; i < n; i++) {
        bool hover = mp.x >= x && mp.x <= x+w && mp.y >= y+4+i*22 && mp.y <= y+4+(i+1)*22;
        DrawText(items[i], x+8, y+6+i*22, 14, hover ? YELLOW : (Color){g_theme.text[0],g_theme.text[1],g_theme.text[2],255});
        if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return (CtxAction)(i+1);
    }
    return CTX_NONE;
}

// ===== SPLITTER =====
bool prof_splitter_drag(int *panel_w, int x, int y, int min_w, int max_w) {
    Vector2 mp = GetMousePosition();
    int hover = mp.x >= x-3 && mp.x <= x+3 && mp.y >= y;
    if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) g_splitter_drag = 1;
    if (g_splitter_drag) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            int nx = (int)mp.x;
            if (nx < min_w) nx = min_w; if (nx > max_w) nx = max_w;
            *panel_w = nx;
        } else g_splitter_drag = 0;
    }
    DrawRectangle(x-2, y, 4, GetScreenHeight(), hover ? (Color){g_theme.accent[0],g_theme.accent[1],g_theme.accent[2],255} : (Color){g_theme.border[0],g_theme.border[1],g_theme.border[2],255});
    return g_splitter_drag != 0;
}
