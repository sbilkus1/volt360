#include "console.h"
#include <string.h>
#include <stdio.h>
#include <raylib.h>

static Color g_bg      = { 28, 30, 34, 255 };
static Color g_border  = { 60, 60, 60, 255 };
static Color g_text    = { 190, 190, 195, 255 };
static Color g_text2   = { 120, 120, 125, 255 };
static Color g_tab_act = { 65, 130, 255, 255 };
static Color g_tab_inact = { 50, 50, 55, 255 };

void console_init(ConsolePanel *c, int x, int y, int w, int h) {
    memset(c, 0, sizeof(*c));
    c->rect = (Rectangle){ (float)x, (float)y, (float)w, (float)h };
    c->bg = g_bg;
    c->border = g_border;
    c->text = g_text;
    c->tab_active = 0;
    c->scroll = 0;
    c->nlines = 0;
}

void console_log(ConsolePanel *c, const char *msg) {
    if (!msg || c->nlines >= CONSOLE_MAX_LINES) return;
    strncpy(c->lines[c->nlines], msg, 255);
    c->lines[c->nlines][255] = '\0';
    c->nlines++;
    c->scroll = c->nlines;
}

void console_clear(ConsolePanel *c) {
    c->nlines = 0;
    c->scroll = 0;
}

void console_render(ConsolePanel *c) {
    int x = (int)c->rect.x;
    int y = (int)c->rect.y;
    int w = (int)c->rect.width;
    int h = (int)c->rect.height;
    Vector2 mp = GetMousePosition();

    DrawRectangle(x, y, w, h, c->bg);
    DrawRectangleLines(x, y, w, h, c->border);

    /* tab bar */
    int tab_y = y + 2;
    const char *tabs[] = { "Output", "Layer View" };
    int tab_x = x + 4;
    for (int i = 0; i < 2; i++) {
        int tw = MeasureText(tabs[i], 12) + 16;
        Rectangle tr = { (float)tab_x, (float)tab_y, (float)tw, 20 };
        int hover = CheckCollisionPointRec(mp, tr);

        if (i == c->tab_active) {
            DrawRectangleRec(tr, g_tab_act);
            DrawText(tabs[i], tab_x + 8, tab_y + 4, 12, WHITE);
        } else {
            DrawRectangleRec(tr, hover ? (Color){ 70, 70, 75, 255 } : g_tab_inact);
            DrawText(tabs[i], tab_x + 8, tab_y + 4, 12, g_text2);
        }

        if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            c->tab_active = i;
        }
        tab_x += tw + 2;
    }

    DrawLine(x, y + 22, x + w, y + 22, c->border);

    /* content area */
    BeginScissorMode(x + 2, y + 24, w - 4, h - 26);

    int visible = (h - 28) / 14;
    if (visible < 1) visible = 1;
    int start = c->nlines - visible;
    if (start < 0) start = 0;

    /* mouse wheel scroll */
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        if (mp.x >= x && mp.x <= x + w && mp.y >= y && mp.y <= y + h) {
            c->scroll -= (int)(wheel * 3);
        }
    }

    int max_scroll = c->nlines - visible;
    if (max_scroll < 0) max_scroll = 0;
    if (c->scroll > max_scroll) c->scroll = max_scroll;
    if (c->scroll < 0) c->scroll = 0;

    start = c->scroll;

    if (c->tab_active == 0) {
        /* Output tab */
        for (int i = start; i < c->nlines && i < start + visible + 2; i++) {
            DrawText(c->lines[i], x + 6, y + 26 + (i - start) * 14, 11, c->text);
        }
        if (c->nlines == 0) {
            DrawText("Build output will appear here", x + 6, y + 30, 11, g_text2);
        }
    } else {
        /* Layer View tab */
        DrawText("Layer visualization - slice a model to preview", x + 6, y + 30, 11, g_text2);
    }

    EndScissorMode();
}
