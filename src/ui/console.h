#pragma once
/* console.h - Console/bottom panel with tabs */
#include <stdbool.h>
#include <raylib.h>

#define CONSOLE_MAX_LINES 256

typedef struct {
    char lines[CONSOLE_MAX_LINES][256];
    int nlines;
    Rectangle rect;
    int scroll;
    Color bg;
    Color border;
    Color text;
    int tab_active;
} ConsolePanel;

void console_init(ConsolePanel *c, int x, int y, int w, int h);
void console_render(ConsolePanel *c);
void console_log(ConsolePanel *c, const char *msg);
void console_clear(ConsolePanel *c);
