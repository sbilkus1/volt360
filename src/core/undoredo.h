#pragma once
/* undoredo.h - undo/redo command stack for Volt360 */

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CMD_COMPONENT_MOVE, CMD_COMPONENT_DELETE, CMD_COMPONENT_ADD,
    CMD_WIRE_ADD, CMD_WIRE_DELETE, CMD_TRACK_MOVE, CMD_TRACK_DELETE, CMD_TRACK_ADD,
    CMD_VIA_ADD, CMD_VIA_DELETE, CMD_TEXT_EDIT,
    CMD_SLICE_SETTING, CMD_FARM_JOB,
    CMD_PROPERTY_CHANGE, CMD_CAD_TRANSFORM,
} CmdType;

typedef struct {
    CmdType type;
    char id[64];
    float old_x, old_y;
    float new_x, new_y;
    int old_val, new_val;
    char *old_text;
    char *new_text;
    void *old_data;
    void *new_data;
    size_t old_size, new_size;
} UndoCommand;

typedef struct {
    UndoCommand *stack;
    int capacity;
    int base;             /* oldest valid command index */
    int pos;              /* undo frontier (pos == head when all applied) */
    int head;             /* next write position (one past newest) */
    int count;            /* number of valid commands (base to head-1) */
    int group_level;
    int group_start;
} UndoStack;

void undo_init(UndoStack *s, int capacity);
void undo_free(UndoStack *s);
void undo_begin_group(UndoStack *s);
void undo_end_group(UndoStack *s);
void undo_push(UndoStack *s, const UndoCommand *cmd);
bool undo_step(UndoStack *s);
bool redo_step(UndoStack *s);
void undo_clear(UndoStack *s);
const char *undo_peek_desc(const UndoStack *s);
bool undo_can_undo(const UndoStack *s);
bool undo_can_redo(const UndoStack *s);
const UndoCommand *undo_current(const UndoStack *s);

#ifdef __cplusplus
}
#endif
