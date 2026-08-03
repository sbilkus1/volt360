#include "shortcuts.h"
#include <string.h>

void shortcuts_init(ShortcutMap *map) {
	int i = 0;
	memset(map, 0, sizeof(*map));

	/* View controls */
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_PAN_LEFT,    KEY_LEFT,     0, 0, 0, "Left",       "Pan Left" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_PAN_RIGHT,   KEY_RIGHT,    0, 0, 0, "Right",      "Pan Right" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_PAN_UP,      KEY_UP,       0, 0, 0, "Up",         "Pan Up" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_PAN_DOWN,    KEY_DOWN,     0, 0, 0, "Down",       "Pan Down" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_ZOOM_IN,     KEY_EQUAL,    1, 0, 0, "Ctrl+Plus",  "Zoom In" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_ZOOM_OUT,    KEY_MINUS,    1, 0, 0, "Ctrl+Minus", "Zoom Out" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_ZOOM_FIT,    KEY_F,        0, 0, 0, "F",          "Zoom Fit" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_VIEW_TOP,    KEY_KP_7,     0, 0, 0, "Numpad 7",   "View Top" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_VIEW_FRONT,  KEY_KP_1,     0, 0, 0, "Numpad 1",   "View Front" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_VIEW_RIGHT,  KEY_KP_3,     0, 0, 0, "Numpad 3",   "View Right" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_VIEW_3D,     KEY_KP_0,     0, 0, 0, "Numpad 0",   "View 3D" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_VIEW_RESET,  KEY_HOME,     0, 0, 0, "Home",       "Reset View" };

	/* Edit */
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_UNDO,        KEY_Z,        1, 0, 0, "Ctrl+Z",     "Undo" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_REDO,        KEY_Y,        1, 0, 0, "Ctrl+Y",     "Redo" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_COPY,        KEY_C,        1, 0, 0, "Ctrl+C",     "Copy" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_PASTE,       KEY_V,        1, 0, 0, "Ctrl+V",     "Paste" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_CUT,         KEY_X,        1, 0, 0, "Ctrl+X",     "Cut" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_DELETE,      KEY_DELETE,   0, 0, 0, "Del",        "Delete" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_SELECT_ALL,  KEY_A,        1, 0, 0, "Ctrl+A",     "Select All" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_ESCAPE,      KEY_ESCAPE,   0, 0, 0, "Esc",        "Cancel/Back" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_ENTER,       KEY_ENTER,    0, 0, 0, "Enter",      "Confirm" };

	/* Tools */
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_TOOL_MOVE,      KEY_M,     0, 0, 0, "M",          "Move Tool" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_TOOL_ROTATE,    KEY_R,     0, 0, 0, "R",          "Rotate Tool" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_TOOL_SCALE,     KEY_S,     0, 0, 0, "S",          "Scale Tool" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_TOOL_WIRE,      KEY_W,     0, 0, 0, "W",          "Wire Tool" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_TOOL_TRACK,     KEY_X,     0, 0, 0, "X",          "Track Tool" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_TOOL_PLACE_VIA, KEY_V,     0, 0, 0, "V",          "Place Via" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_TOOL_DIMENSION, KEY_D,     0, 0, 0, "D",          "Dimension Tool" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_TOOL_MEASURE,   KEY_Q,     0, 0, 0, "Q",          "Measure Tool" };

	/* File */
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_NEW,      KEY_N,  1, 0, 0, "Ctrl+N",     "New Project" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_OPEN,     KEY_O,  1, 0, 0, "Ctrl+O",     "Open Project" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_SAVE,     KEY_S,  1, 0, 0, "Ctrl+S",     "Save Project" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_SAVE_AS,  KEY_S,  1, 1, 0, "Ctrl+Shift+S","Save As" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_EXPORT,   KEY_E,  1, 0, 0, "Ctrl+E",     "Export" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_IMPORT,   KEY_I,  1, 0, 0, "Ctrl+I",     "Import" };

	/* Tabs */
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_SCHEMATIC, KEY_ONE,   0, 0, 0, "1",         "Schematic Tab" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_PCB,       KEY_TWO,   0, 0, 0, "2",         "PCB Tab" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_3D_VIEW,   KEY_THREE, 0, 0, 0, "3",         "3D View Tab" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_SLICER,    KEY_FOUR,  0, 0, 0, "4",         "Slicer Tab" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_FARM,      KEY_FIVE,  0, 0, 0, "5",         "Farm Tab" };

	/* Misc */
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_GRID_TOGGLE,         KEY_G,   0, 0, 0, "G",              "Toggle Grid" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_SNAP_TOGGLE,         KEY_S,   0, 1, 0, "Shift+S",        "Toggle Snap" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_CONSOLE_TOGGLE,      KEY_T,   1, 1, 0, "Ctrl+Shift+T",   "Toggle Console" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_PROPERTIES_TOGGLE,   KEY_P,   1, 1, 0, "Ctrl+Shift+P",   "Toggle Properties" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_FULLSCREEN,          KEY_F11, 0, 0, 0, "F11",            "Fullscreen" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_IMPORT_WEB,         KEY_I,   1, 1, 0, "Ctrl+Shift+I",  "Import from Web" };
	map->bindings[i++] = (ShortcutBinding){ SHORTCUT_RUN_LUA,            KEY_F5,  0, 0, 0, "F5",             "Run Lua Script" };
}

static int modifier_check(int ctrl, int shift, int alt) {
	int ctrl_down = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
	int shift_down = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
	int alt_down = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);

	if ((ctrl  && !ctrl_down)  || (!ctrl  && ctrl_down))  return 0;
	if ((shift && !shift_down) || (!shift && shift_down)) return 0;
	if ((alt   && !alt_down)   || (!alt   && alt_down))   return 0;
	return 1;
}

static int is_pan_action(ShortcutAction action) {
	return action == SHORTCUT_PAN_LEFT  || action == SHORTCUT_PAN_RIGHT ||
	       action == SHORTCUT_PAN_UP    || action == SHORTCUT_PAN_DOWN;
}

static int is_zoom_action(ShortcutAction action) {
	return action == SHORTCUT_ZOOM_IN || action == SHORTCUT_ZOOM_OUT;
}

ShortcutAction shortcuts_poll(const ShortcutMap *map) {
	int i;

	/* Check IsKeyPressed bindings first (most actions) */
	for (i = 0; i < SHORTCUT_COUNT; i++) {
		const ShortcutBinding *b = &map->bindings[i];
		if (b->key == 0) continue;
		if (is_pan_action(b->action) || is_zoom_action(b->action)) continue;
		if (!modifier_check(b->ctrl, b->shift, b->alt)) continue;
		if (IsKeyPressed(b->key)) return b->action;
	}

	/* Check continuous bindings (pan arrows, zoom) using IsKeyDown */
	for (i = 0; i < SHORTCUT_COUNT; i++) {
		const ShortcutBinding *b = &map->bindings[i];
		if (b->key == 0) continue;
		if (!is_pan_action(b->action) && !is_zoom_action(b->action)) continue;
		if (!modifier_check(b->ctrl, b->shift, b->alt)) continue;
		if (IsKeyDown(b->key)) return b->action;
	}

	return SHORTCUT_COUNT;
}

const ShortcutBinding *shortcuts_get(const ShortcutMap *map, ShortcutAction action) {
	if (action >= SHORTCUT_COUNT) return NULL;
	if (map->bindings[action].action != action) return NULL;
	return &map->bindings[action];
}
