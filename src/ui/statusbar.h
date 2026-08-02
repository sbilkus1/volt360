#pragma once
/* statusbar.h - Status bar at very bottom */
#include <stdbool.h>
#include <raylib.h>

typedef struct {
    Rectangle rect;
    char left_text[256];
    char right_text[256];
    Color bg;
    Color text;
    Color divider;
} StatusBar;

void statusbar_init(StatusBar *b, int x, int y, int w, int h);
void statusbar_render(StatusBar *b);
void statusbar_set_left(StatusBar *b, const char *text);
void statusbar_set_right(StatusBar *b, const char *text);
