#include "statusbar.h"
#include <string.h>
#include <stdio.h>
#include <raylib.h>

static Color g_bg      = { 22, 24, 28, 255 };
static Color g_border  = { 60, 60, 60, 255 };
static Color g_text    = { 180, 180, 185, 255 };
static Color g_text2   = { 120, 120, 125, 255 };

void statusbar_init(StatusBar *b, int x, int y, int w, int h) {
    memset(b, 0, sizeof(*b));
    b->rect = (Rectangle){ (float)x, (float)y, (float)w, (float)h };
    b->bg = g_bg;
    b->text = g_text;
    b->divider = g_border;
    b->left_text[0] = '\0';
    b->right_text[0] = '\0';
    strncpy(b->left_text, "Ready", 255);
    strncpy(b->right_text, "X:0.0 Y:0.0 Z:0.0 | Grid:1.0 | mm", 255);
}

void statusbar_set_left(StatusBar *b, const char *text) {
    strncpy(b->left_text, text ? text : "", 255);
    b->left_text[255] = '\0';
}

void statusbar_set_right(StatusBar *b, const char *text) {
    strncpy(b->right_text, text ? text : "", 255);
    b->right_text[255] = '\0';
}

void statusbar_render(StatusBar *b) {
    int x = (int)b->rect.x;
    int y = (int)b->rect.y;
    int w = (int)b->rect.width;
    int h = (int)b->rect.height;

    DrawRectangle(x, y, w, h, b->bg);
    DrawLine(x, y, x + w, y, b->divider);

    /* left side */
    DrawText(b->left_text, x + 8, y + 4, 13, b->text);

    /* right side */
    if (b->right_text[0]) {
        int rw = MeasureText(b->right_text, 12);
        DrawText(b->right_text, x + w - rw - 8, y + 5, 12, g_text2);
    }
}
