#include "ribbon.h"
#include <string.h>
#include <stdio.h>
#include <raylib.h>

static Color g_bg       = { 30, 30, 30, 255 };
static Color g_panel    = { 40, 40, 40, 255 };
static Color g_tab_active  = { 65, 130, 255, 255 };
static Color g_tab_inactive = { 50, 50, 50, 255 };
static Color g_text     = { 220, 220, 220, 255 };
static Color g_text2    = { 150, 150, 150, 255 };
static Color g_sep      = { 80, 80, 80, 255 };
static Color g_accent   = { 65, 130, 255, 255 };
static Color g_btn_hover = { 60, 60, 70, 255 };
static Color g_btn_press = { 50, 110, 220, 255 };

void ribbon_init(RibbonBar *r, int x, int y, int w) {
    memset(r, 0, sizeof(*r));
    r->rect = (Rectangle){ (float)x, (float)y, (float)w, (float)RIBBON_H };
    r->bg = g_bg;
    r->tab_active = g_tab_active;
    r->tab_inactive = g_tab_inactive;
    r->text_color = g_text;
    r->separator = g_sep;
    r->accent = g_accent;
    r->active_tab = 0;
    r->tab_clicked = false;
}

int ribbon_add_tab(RibbonBar *r, const char *name) {
    if (r->ntabs >= RIBBON_MAX_TABS) return -1;
    int id = r->ntabs;
    r->tabs[id].name = name;
    r->tabs[id].id = id;
    r->tabs[id].active = (id == 0);
    r->ntabs++;
    return id;
}

int ribbon_add_group(RibbonBar *r, int tab_id, const char *name) {
    if (r->ngroups >= RIBBON_MAX_GROUPS) return -1;
    if (tab_id < 0 || tab_id >= r->ntabs) return -1;
    int id = r->ngroups;
    r->groups[id].name = name;
    r->groups[id].tab_id = tab_id;
    r->groups[id].nbuttons = 0;
    r->groups[id].buttons = NULL;
    r->ngroups++;
    return id;
}

int ribbon_add_button(RibbonBar *r, int tab_id, int group_id, const char *label, const char *icon, void (*action)(void)) {
    if (r->nbuttons >= RIBBON_MAX_BUTTONS) return -1;
    if (group_id < 0 || group_id >= r->ngroups) return -1;
    int id = r->nbuttons;
    r->buttons[id].label = label;
    r->buttons[id].icon_text = icon;
    r->buttons[id].action = action;
    r->buttons[id].tab_id = tab_id;
    r->buttons[id].group_id = group_id;
    r->buttons[id].pressed = false;
    r->groups[group_id].nbuttons++;
    r->nbuttons++;
    return id;
}

void ribbon_set_active_tab(RibbonBar *r, int tab_id) {
    if (tab_id < 0 || tab_id >= r->ntabs) return;
    r->active_tab = tab_id;
}

int ribbon_get_clicked_button(RibbonBar *r) {
    int ret = -1;
    for (int i = 0; i < r->nbuttons; i++) {
        if (r->buttons[i].pressed) {
            r->buttons[i].pressed = false;
            ret = i;
            break;
        }
    }
    return ret;
}

int ribbon_render(RibbonBar *r) {
    Vector2 mp = GetMousePosition();
    int rx = (int)r->rect.x;
    int ry = (int)r->rect.y;
    int rw = (int)r->rect.width;
    int any_clicked = 0;

    /* background */
    DrawRectangle(rx, ry, rw, RIBBON_H, r->bg);

    /* tab bar */
    DrawRectangle(rx, ry, rw, RIBBON_TAB_H, g_panel);
    DrawLine(rx, ry + RIBBON_TAB_H, rx + rw, ry + RIBBON_TAB_H, g_sep);

    int tx = rx + 2;
    for (int i = 0; i < r->ntabs; i++) {
        int tw = MeasureText(r->tabs[i].name, 14) + 20;
        Rectangle tr = { (float)tx, (float)ry + 1, (float)tw, (float)RIBBON_TAB_H - 2 };
        bool hover = CheckCollisionPointRec(mp, tr);
        int active = (i == r->active_tab);

        if (active) {
            DrawRectangleRec(tr, g_tab_active);
            DrawText(r->tabs[i].name, tx + 10, ry + 7, 14, WHITE);
        } else if (hover) {
            DrawRectangleRec(tr, (Color){ 70, 70, 75, 255 });
            DrawText(r->tabs[i].name, tx + 10, ry + 7, 14, g_text);
        } else {
            DrawText(r->tabs[i].name, tx + 10, ry + 7, 14, g_text2);
        }

        if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            r->active_tab = i;
            r->tab_clicked = true;
        }
        tx += tw;
    }

    /* band area */
    DrawRectangle(rx, ry + RIBBON_TAB_H, rw, RIBBON_BAND_H, g_panel);

    /* layout groups for active tab */
    int gx = rx + 4;
    int gy = ry + RIBBON_TAB_H + 2;
    int button_w = RIBBON_BTN_SIZE;
    int button_h = RIBBON_BTN_SIZE;

    for (int g = 0; g < r->ngroups; g++) {
        RibbonGroup *grp = &r->groups[g];
        if (grp->tab_id != r->active_tab) continue;
        if (grp->nbuttons == 0) continue;

        /* count buttons in this group */
        int nbtns = 0;
        for (int b = 0; b < r->nbuttons; b++) {
            if (r->buttons[b].group_id == g) nbtns++;
        }

        int g_w = nbtns * (button_w + 4) + 8;
        if (gx + g_w > rx + rw - 4) {
            gx = rx + 4;
            gy += RIBBON_BAND_H / 2;
        }

        grp->x = gx;
        grp->y = gy;
        grp->w = g_w;

        /* group name */
        int name_w = MeasureText(grp->name, 10);
        DrawText(grp->name, gx + (g_w - name_w) / 2, gy + RIBBON_BAND_H - 14, 10, g_text2);

        int bx = gx + 2;
        int by = gy + 2;
        for (int b = 0; b < r->nbuttons; b++) {
            if (r->buttons[b].group_id != g) continue;
            RibbonButton *btn = &r->buttons[b];
            Rectangle br = { (float)bx, (float)by, (float)button_w, (float)button_h };
            bool hover = CheckCollisionPointRec(mp, br);

            if (hover && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                DrawRectangleRec(br, g_btn_press);
            } else if (hover) {
                DrawRectangleRec(br, g_btn_hover);
            }

            /* icon - draw 2-3 chars centered */
            if (btn->icon_text && btn->icon_text[0]) {
                int icon_w = MeasureText(btn->icon_text, 18);
                DrawText(btn->icon_text, bx + (button_w - icon_w) / 2, by + 4, 18,
                    hover ? WHITE : g_text);
            }

            /* label below icon */
            int label_w = MeasureText(btn->label, 10);
            DrawText(btn->label, bx + (button_w - label_w) / 2, by + 30, 10, g_text2);

            if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                btn->pressed = true;
                any_clicked = 1;
                if (btn->action) btn->action();
            }

            bx += button_w + 4;
        }

        /* separator line after group */
        gx += g_w + 4;
        DrawLine(gx - 2, gy + 2, gx - 2, gy + RIBBON_BAND_H - 16, g_sep);
    }

    return any_clicked;
}
