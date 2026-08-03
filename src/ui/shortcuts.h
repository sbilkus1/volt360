#pragma once
/* shortcuts.h - Keyboard shortcuts engine for Volt360
 * Maps standard CAD/EDA hotkeys to Volt360 actions using raylib key detection.
 */

#include <raylib.h>

typedef enum {
	/* View controls */
	SHORTCUT_PAN_LEFT, SHORTCUT_PAN_RIGHT, SHORTCUT_PAN_UP, SHORTCUT_PAN_DOWN,
	SHORTCUT_ZOOM_IN, SHORTCUT_ZOOM_OUT, SHORTCUT_ZOOM_FIT,
	SHORTCUT_VIEW_TOP, SHORTCUT_VIEW_FRONT, SHORTCUT_VIEW_RIGHT, SHORTCUT_VIEW_3D,
	SHORTCUT_VIEW_RESET,
	/* Edit */
	SHORTCUT_UNDO, SHORTCUT_REDO,
	SHORTCUT_COPY, SHORTCUT_PASTE, SHORTCUT_CUT,
	SHORTCUT_DELETE, SHORTCUT_SELECT_ALL,
	SHORTCUT_ESCAPE, SHORTCUT_ENTER,
	/* Tools */
	SHORTCUT_TOOL_MOVE, SHORTCUT_TOOL_ROTATE, SHORTCUT_TOOL_SCALE,
	SHORTCUT_TOOL_WIRE, SHORTCUT_TOOL_TRACK,
	SHORTCUT_TOOL_PLACE_VIA, SHORTCUT_TOOL_DIMENSION,
	SHORTCUT_TOOL_MEASURE,
	/* File */
	SHORTCUT_NEW, SHORTCUT_OPEN, SHORTCUT_SAVE, SHORTCUT_SAVE_AS,
	SHORTCUT_EXPORT, SHORTCUT_IMPORT,
	/* Tabs */
	SHORTCUT_SCHEMATIC, SHORTCUT_PCB, SHORTCUT_3D_VIEW, SHORTCUT_SLICER,
	SHORTCUT_FARM,
	/* Misc */
	SHORTCUT_GRID_TOGGLE, SHORTCUT_SNAP_TOGGLE,
	SHORTCUT_CONSOLE_TOGGLE, SHORTCUT_PROPERTIES_TOGGLE,
	SHORTCUT_FULLSCREEN,
	SHORTCUT_IMPORT_WEB, SHORTCUT_RUN_LUA,
	SHORTCUT_COUNT
} ShortcutAction;

typedef struct {
	ShortcutAction action;
	int key;
	int ctrl;
	int shift;
	int alt;
	const char *label;
	const char *desc;
} ShortcutBinding;

typedef struct {
	ShortcutBinding bindings[SHORTCUT_COUNT];
} ShortcutMap;

void shortcuts_init(ShortcutMap *map);
ShortcutAction shortcuts_poll(const ShortcutMap *map);
const ShortcutBinding *shortcuts_get(const ShortcutMap *map, ShortcutAction action);
