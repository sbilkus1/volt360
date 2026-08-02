#include "treeview.h"
#include <string.h>
#include <stdio.h>
#include <raylib.h>

static Color g_bg   = { 35, 35, 38, 255 };
static Color g_text = { 200, 200, 200, 255 };
static Color g_sel  = { 55, 90, 140, 255 };
static Color g_border = { 60, 60, 60, 255 };
static Color g_text2 = { 140, 140, 145, 255 };

void tree_init(ProjectTree *t, int x, int y, int w, int h) {
    memset(t, 0, sizeof(*t));
    t->rect = (Rectangle){ (float)x, (float)y, (float)w, (float)h };
    t->bg = g_bg;
    t->text = g_text;
    t->select_bg = g_sel;
    t->scroll_offset = 0;
    t->nnodes = 0;
    t->root_idx = -1;
    t->sel_idx = -1;
    t->search[0] = '\0';

    /* create root node */
    t->root_idx = tree_add_node(t, -1, "Project");
    if (t->root_idx >= 0) {
        t->nodes[t->root_idx].expanded = 1;
        t->nodes[t->root_idx].depth = 0;
    }
}

int tree_add_node(ProjectTree *t, int parent_id, const char *name) {
    if (t->nnodes >= TREE_MAX_NODES) return -1;
    int id = t->nnodes;
    strncpy(t->nodes[id].label, name, 63);
    t->nodes[id].label[63] = '\0';
    t->nodes[id].expanded = 0;
    t->nodes[id].selected = 0;
    t->nodes[id].parent_idx = parent_id;
    t->nodes[id].nchildren = 0;
    t->nodes[id].id = id;

    if (parent_id >= 0 && parent_id < t->nnodes) {
        t->nodes[id].depth = t->nodes[parent_id].depth + 1;
        int nc = t->nodes[parent_id].nchildren;
        if (nc < 64) {
            t->nodes[parent_id].children[nc] = id;
            t->nodes[parent_id].nchildren++;
        }
    } else {
        t->nodes[id].depth = 0;
    }

    t->nnodes++;
    return id;
}

void tree_clear(ProjectTree *t) {
    t->nnodes = 0;
    t->root_idx = -1;
    t->sel_idx = -1;
    t->scroll_offset = 0;
    t->root_idx = tree_add_node(t, -1, "Project");
    if (t->root_idx >= 0) {
        t->nodes[t->root_idx].expanded = 1;
        t->nodes[t->root_idx].depth = 0;
    }
}

int tree_get_selected(ProjectTree *t) {
    return t->sel_idx;
}

static int tree_draw_node(ProjectTree *t, int node_id, int *py, int x, int w) {
    if (node_id < 0 || node_id >= t->nnodes) return 0;
    if (*py > (int)(t->rect.y + t->rect.height) + 50) return 0;
    if (*py < (int)t->rect.y - 50) { *py += 18; return 0; }

    int indent = t->nodes[node_id].depth * 16;
    int tx = x + 4 + indent;
    int item_h = 18;
    Vector2 mp = GetMousePosition();

    /* filter check */
    int show = 1;
    if (t->search[0]) {
        show = 0;
        if (strstr(t->nodes[node_id].label, t->search)) show = 1;
        /* also check children */
        for (int c = 0; c < t->nodes[node_id].nchildren && !show; c++) {
            int ci = t->nodes[node_id].children[c];
            if (strstr(t->nodes[ci].label, t->search)) show = 1;
        }
    }
    if (!show) { *py += item_h; return 0; }

    int draw_y = *py;
    if (draw_y > (int)(t->rect.y + t->rect.height)) return 0;

    Rectangle ir = { (float)tx, (float)draw_y, (float)(w - indent - 4), (float)item_h };
    int hover = CheckCollisionPointRec(mp, ir);
    int is_sel = (node_id == t->sel_idx);

    if (is_sel) {
        DrawRectangle(tx, draw_y, w - indent - 4, item_h, g_sel);
    } else if (hover) {
        DrawRectangle(tx, draw_y, w - indent - 4, item_h, (Color){ 50, 50, 55, 255 });
    }

    /* toggle expander */
    int has_children = (t->nodes[node_id].nchildren > 0);
    int ex = tx + 2;
    if (has_children) {
        const char *exp = t->nodes[node_id].expanded ? "-" : "+";
        DrawText(exp, ex, draw_y + 2, 12, g_text2);
    }
    DrawText(t->nodes[node_id].label, ex + 16, draw_y + 2, 13, is_sel ? WHITE : g_text);

    if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        /* check if clicked on expander */
        if (has_children && mp.x >= ex && mp.x <= ex + 12) {
            t->nodes[node_id].expanded = !t->nodes[node_id].expanded;
        } else {
            t->sel_idx = node_id;
        }
    }

    *py += item_h;

    /* children */
    if (t->nodes[node_id].expanded && has_children) {
        for (int c = 0; c < t->nodes[node_id].nchildren; c++) {
            tree_draw_node(t, t->nodes[node_id].children[c], py, x, w);
        }
    }

    return 0;
}

int tree_render(ProjectTree *t) {
    int x = (int)t->rect.x;
    int y = (int)t->rect.y;
    int w = (int)t->rect.width;
    int h = (int)t->rect.height;

    DrawRectangle(x, y, w, h, t->bg);
    DrawRectangleLines(x, y, w, h, g_border);

    /* header: "PROJECT" */
    DrawText("PROJECT BROWSER", x + 8, y + 4, 10, g_text2);
    DrawLine(x, y + 18, x + w, y + 18, g_border);

    /* search bar */
    int sy = y + 22;
    DrawRectangle(x + 4, sy, w - 8, 22, (Color){ 25, 25, 28, 255 });
    DrawRectangleLines(x + 4, sy, w - 8, 22, g_border);
    DrawText(t->search[0] ? t->search : "Search...", x + 10, sy + 4, 12, t->search[0] ? g_text : g_text2);

    /* handle search input (only when user clicks in search box) */
    static int editing_search = 0;
    Vector2 mp = GetMousePosition();
    Rectangle search_rect = { (float)(x + 4), (float)sy, (float)(w - 8), 22 };
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        editing_search = CheckCollisionPointRec(mp, search_rect);
    }
    if (editing_search) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 126) {
                int l = (int)strlen(t->search);
                if (l < 62) { t->search[l] = (char)key; t->search[l+1] = '\0'; }
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && t->search[0]) {
            t->search[strlen(t->search) - 1] = '\0';
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            t->search[0] = '\0';
            editing_search = 0;
        }
    }

    /* tree content */
    int py = y + 48;
    if (t->root_idx >= 0) tree_draw_node(t, t->root_idx, &py, x, w);

    return t->sel_idx;
}
