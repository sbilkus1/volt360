#pragma once
/* properties.h - Properties panel (right sidebar) */
#include <stdbool.h>
#include <raylib.h>

#define PROP_MAX_FIELDS 32

typedef struct {
    char label[64];
    char value[128];
    int readonly;
} PropField;

typedef struct {
    PropField fields[PROP_MAX_FIELDS];
    int nfields;
    Rectangle rect;
    int scroll_offset;
    Color bg;
    Color border;
    Color text;
    Color label_color;
    char title[64];
} PropertiesPanel;

void props_init(PropertiesPanel *p, int x, int y, int w, int h, const char *title);
void props_render(PropertiesPanel *p);
void props_clear(PropertiesPanel *p);
void props_add_field(PropertiesPanel *p, const char *label, const char *value, int readonly);
void props_set_title(PropertiesPanel *p, const char *title);
