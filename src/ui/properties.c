#include "properties.h"
#include <string.h>
#include <stdio.h>
#include <raylib.h>

static Color g_bg      = { 35, 35, 38, 255 };
static Color g_border  = { 60, 60, 60, 255 };
static Color g_text    = { 200, 200, 200, 255 };
static Color g_label   = { 140, 140, 145, 255 };
static Color g_line_bg = { 42, 42, 46, 255 };

void props_init(PropertiesPanel *p, int x, int y, int w, int h, const char *title) {
    memset(p, 0, sizeof(*p));
    p->rect = (Rectangle){ (float)x, (float)y, (float)w, (float)h };
    p->bg = g_bg;
    p->border = g_border;
    p->text = g_text;
    p->label_color = g_label;
    strncpy(p->title, title, 63);
    p->title[63] = '\0';
    p->scroll_offset = 0;
    p->nfields = 0;
}

void props_clear(PropertiesPanel *p) {
    p->nfields = 0;
}

void props_add_field(PropertiesPanel *p, const char *label, const char *value, int readonly) {
    if (p->nfields >= PROP_MAX_FIELDS) return;
    int i = p->nfields;
    strncpy(p->fields[i].label, label, 63);
    p->fields[i].label[63] = '\0';
    strncpy(p->fields[i].value, value ? value : "", 127);
    p->fields[i].value[127] = '\0';
    p->fields[i].readonly = readonly;
    p->nfields++;
}

void props_set_title(PropertiesPanel *p, const char *title) {
    strncpy(p->title, title, 63);
    p->title[63] = '\0';
}

void props_render(PropertiesPanel *p) {
    int x = (int)p->rect.x;
    int y = (int)p->rect.y;
    int w = (int)p->rect.width;
    int h = (int)p->rect.height;

    DrawRectangle(x, y, w, h, p->bg);
    DrawRectangleLines(x, y, w, h, p->border);

    /* title */
    DrawText(p->title, x + 8, y + 4, 10, p->label_color);
    DrawLine(x, y + 18, x + w, y + 18, p->border);

    /* fields */
    int py = y + 24;
    for (int i = 0; i < p->nfields && py < y + h - 4; i++) {
        PropField *f = &p->fields[i];

        /* alternating row bg */
        if (i % 2 == 0) {
            DrawRectangle(x + 2, py, w - 4, 18, g_line_bg);
        }

        DrawText(f->label, x + 8, py + 2, 12, p->label_color);
        /* value - right aligned */
        if (f->value[0]) {
            int vw = MeasureText(f->value, 12);
            DrawText(f->value, x + w - vw - 8, py + 2, 12,
                f->readonly ? p->text : (Color){ 65, 130, 255, 255 });
        }

        py += 18;
    }

    if (p->nfields == 0) {
        DrawText("No selection", x + 12, py + 8, 13, p->label_color);
    }
}
