#pragma once
/* ribbon.h - Fusion 360 style ribbon bar */
#include <stdbool.h>
#include <raylib.h>

#define RIBBON_MAX_TABS 8
#define RIBBON_MAX_GROUPS 64
#define RIBBON_MAX_BUTTONS 512
#define RIBBON_BTN_SIZE 48
#define RIBBON_TAB_H 28
#define RIBBON_BAND_H 70
#define RIBBON_H (RIBBON_TAB_H + RIBBON_BAND_H)

typedef struct {
    const char *name;
    int id;
    bool active;
} RibbonTab;

typedef struct {
    const char *label;
    const char *icon_text;
    void (*action)(void);
    bool pressed;
    int tab_id;
    int group_id;
} RibbonButton;

typedef struct {
    const char *name;
    RibbonButton *buttons;
    int nbuttons;
    int x, y, w;
    int tab_id;
} RibbonGroup;

typedef struct {
    RibbonTab tabs[RIBBON_MAX_TABS];
    RibbonGroup groups[RIBBON_MAX_GROUPS];
    RibbonButton buttons[RIBBON_MAX_BUTTONS];
    int ntabs;
    int ngroups;
    int nbuttons;
    int active_tab;
    Rectangle rect;
    Color bg;
    Color tab_active;
    Color tab_inactive;
    Color text_color;
    Color separator;
    Color accent;
    bool tab_clicked;
} RibbonBar;

void ribbon_init(RibbonBar *r, int x, int y, int w);
int ribbon_add_tab(RibbonBar *r, const char *name);
int ribbon_add_group(RibbonBar *r, int tab_id, const char *name);
int ribbon_add_button(RibbonBar *r, int tab_id, int group_id, const char *label, const char *icon, void (*action)(void));
int ribbon_render(RibbonBar *r);
void ribbon_set_active_tab(RibbonBar *r, int tab_id);
int ribbon_get_clicked_button(RibbonBar *r);
