#pragma once
// profui.h - Professional CAD/EDA UI shell with menus, toolbars, panels.
#include <stdbool.h>
#include "../model/project.h"
#include "ui.h"

// Menu bar items
typedef enum { MENU_NONE, MENU_FILE_NEW, MENU_FILE_OPEN, MENU_FILE_SAVE, MENU_FILE_SAVEAS,
               MENU_FILE_EXPORT, MENU_FILE_IMPORT, MENU_FILE_EXIT,
               MENU_EDIT_UNDO, MENU_EDIT_REDO, MENU_EDIT_DELETE, MENU_EDIT_SELECT_ALL,
               MENU_VIEW_ZOOM_IN, MENU_VIEW_ZOOM_OUT, MENU_VIEW_ZOOM_FIT, MENU_VIEW_GRID,
               MENU_TOOLS_DRC, MENU_TOOLS_ERC, MENU_TOOLS_SPICE,
               MENU_HELP_ABOUT, MENU_HELP_SHORTCUTS } MenuAction;

// Draw professional menu bar at top of screen.
// Returns the selected menu action, or MENU_NONE.
MenuAction prof_menu_bar(int x, int y, int w, int h);

// Draw mode-specific toolbar.
void prof_toolbar_draw(App *app, int x, int y, int w, int h);

// Draw properties panel (right side) for selected items.
void prof_properties_draw(App *app, int x, int y, int w, int h);

// Draw output/console panel (bottom) for messages.
void prof_console_write(const char *msg);
void prof_console_draw(int x, int y, int w, int h);

// Draw layer manager panel.
void prof_layers_draw(App *app, int x, int y, int w, int h);

// Right-click context menu.
typedef enum { CTX_NONE, CTX_DELETE, CTX_PROPERTIES, CTX_ROTATE, CTX_MOVE, CTX_COPY, CTX_PASTE } CtxAction;
CtxAction prof_context_menu(int x, int y, const char **items, int n_items);

// Dock panel resizing handle.
bool prof_splitter_drag(int *panel_w, int x, int y, int min_w, int max_w);

// Theme colors for professional dark theme.
typedef struct {
    unsigned char bg[3], panel[3], text[3], accent[3], highlight[3], border[3];
    unsigned char btn[3], btn_hover[3];
} ProfTheme;

ProfTheme prof_theme_dark(void);
ProfTheme prof_theme_get(void);
void prof_theme_set(ProfTheme t);
