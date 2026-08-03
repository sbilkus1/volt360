#include "undoredo.h"
#include "util.h"
#include <string.h>

static int wrap(const UndoStack *s, int idx)
{
    return (idx % s->capacity + s->capacity) % s->capacity;
}

static int dist(const UndoStack *s, int a, int b)
{
    int d = b - a;
    if (d < 0) d += s->capacity;
    return d;
}

static int add_wrap(const UndoStack *s, int a, int delta)
{
    return wrap(s, a + delta);
}

void undo_init(UndoStack *s, int capacity)
{
    s->capacity = capacity;
    s->stack = (UndoCommand *)calloc((size_t)capacity, sizeof(UndoCommand));
    s->base = 0;
    s->pos = 0;
    s->head = 0;
    s->count = 0;
    s->group_level = 0;
    s->group_start = 0;
}

static void undo_cmd_free(UndoCommand *c)
{
    free(c->old_text);
    free(c->new_text);
    free(c->old_data);
    free(c->new_data);
    c->old_text = NULL;
    c->new_text = NULL;
    c->old_data = NULL;
    c->new_data = NULL;
    c->old_size = 0;
    c->new_size = 0;
}

void undo_free(UndoStack *s)
{
    int i, n = s->count;
    for (i = 0; i < n; i++) {
        undo_cmd_free(&s->stack[wrap(s, s->base + i)]);
    }
    free(s->stack);
    s->stack = NULL;
    s->capacity = 0;
    s->base = 0;
    s->pos = 0;
    s->head = 0;
    s->count = 0;
}

void undo_begin_group(UndoStack *s)
{
    if (s->group_level == 0) {
        s->group_start = s->head;
    }
    s->group_level++;
}

void undo_end_group(UndoStack *s)
{
    int first, last, ngrp, i;

    if (s->group_level <= 0) return;
    s->group_level--;
    if (s->group_level > 0) return;

    ngrp = dist(s, s->group_start, s->head);
    if (ngrp <= 1) return;

    first = wrap(s, s->group_start);
    last = wrap(s, s->head - 1);

    /* merge: keep first command's old_*, use last command's new_* */
    undo_cmd_free(&s->stack[first]);

    s->stack[first].new_x = s->stack[last].new_x;
    s->stack[first].new_y = s->stack[last].new_y;
    s->stack[first].new_val = s->stack[last].new_val;
    s->stack[first].new_text = s->stack[last].new_text;
    s->stack[first].new_data = s->stack[last].new_data;
    s->stack[first].new_size = s->stack[last].new_size;

    s->stack[last].new_text = NULL;
    s->stack[last].new_data = NULL;
    s->stack[last].new_size = 0;

    /* free intermediate commands (between first and last) */
    for (i = 1; i < ngrp - 1; i++) {
        undo_cmd_free(&s->stack[wrap(s, s->group_start + i)]);
    }

    /* compact: shift tail to close the gap */
    {
        int tail_start = s->head;
        int tail_len = dist(s, tail_start, add_wrap(s, s->base, s->count));
        int gap = ngrp - 1;
        int j;
        for (j = 0; j < tail_len; j++) {
            int src = wrap(s, tail_start + j);
            int dst = wrap(s, s->group_start + 1 + j);
            s->stack[dst] = s->stack[src];
            memset(&s->stack[src], 0, sizeof(UndoCommand));
        }
    }

    s->head = wrap(s, s->group_start + 1);
    if (s->pos == wrap(s, s->head)) s->pos = s->head;
    s->count -= (ngrp - 1);
}

static void undo_cmd_copy(UndoCommand *dst, const UndoCommand *src)
{
    memset(dst, 0, sizeof(UndoCommand));
    dst->type = src->type;
    memcpy(dst->id, src->id, sizeof(src->id));
    dst->old_x = src->old_x;
    dst->old_y = src->old_y;
    dst->new_x = src->new_x;
    dst->new_y = src->new_y;
    dst->old_val = src->old_val;
    dst->new_val = src->new_val;
    dst->old_text = src->old_text ? str_dup(src->old_text) : NULL;
    dst->new_text = src->new_text ? str_dup(src->new_text) : NULL;
    dst->old_size = src->old_size;
    dst->new_size = src->new_size;
    if (src->old_data && src->old_size > 0) {
        dst->old_data = malloc(src->old_size);
        memcpy(dst->old_data, src->old_data, src->old_size);
    }
    if (src->new_data && src->new_size > 0) {
        dst->new_data = malloc(src->new_size);
        memcpy(dst->new_data, src->new_data, src->new_size);
    }
}

void undo_push(UndoStack *s, const UndoCommand *cmd)
{
    /* discard redo history: clear commands from pos to head-1 */
    while (s->pos != s->head) {
        int clear_idx;
        s->head = wrap(s, s->head - 1);
        clear_idx = s->head;
        undo_cmd_free(&s->stack[clear_idx]);
        memset(&s->stack[clear_idx], 0, sizeof(UndoCommand));
    }

    /* advance base if full */
    while (s->count >= s->capacity) {
        int oldest = s->base;
        undo_cmd_free(&s->stack[oldest]);
        memset(&s->stack[oldest], 0, sizeof(UndoCommand));
        s->base = wrap(s, s->base + 1);
        s->count--;
        /* adjust pos if it was pointing at the dropped command */
        if (s->pos == oldest) s->pos = s->base;
    }

    /* write new command at pos (which equals head since we cleared redo) */
    undo_cmd_free(&s->stack[s->pos]);
    undo_cmd_copy(&s->stack[s->pos], cmd);
    s->pos = wrap(s, s->pos + 1);
    s->head = s->pos;
    s->count++;
}

bool undo_step(UndoStack *s)
{
    if (s->pos == s->base) return false;
    s->pos = wrap(s, s->pos - 1);
    return true;
}

bool redo_step(UndoStack *s)
{
    if (s->pos == s->head) return false;
    s->pos = wrap(s, s->pos + 1);
    return true;
}

void undo_clear(UndoStack *s)
{
    int i;
    for (i = 0; i < s->count; i++) {
        undo_cmd_free(&s->stack[wrap(s, s->base + i)]);
        memset(&s->stack[wrap(s, s->base + i)], 0, sizeof(UndoCommand));
    }
    s->base = 0;
    s->pos = 0;
    s->head = 0;
    s->count = 0;
    s->group_level = 0;
    s->group_start = 0;
}

const char *undo_peek_desc(const UndoStack *s)
{
    static const char *names[] = {
        "Move Component", "Delete Component", "Add Component",
        "Add Wire", "Delete Wire", "Move Track", "Delete Track", "Add Track",
        "Add Via", "Delete Via", "Edit Text",
        "Slice Setting", "Farm Job",
        "Property Change", "CAD Transform"
    };
    if (undo_can_undo(s)) {
        int idx = wrap(s, s->pos - 1);
        unsigned int t = (unsigned int)s->stack[idx].type;
        if (t < sizeof(names) / sizeof(names[0])) return names[t];
    } else if (undo_can_redo(s)) {
        int idx = s->pos;
        unsigned int t = (unsigned int)s->stack[idx].type;
        if (t < sizeof(names) / sizeof(names[0])) return names[t];
    }
    return "";
}

bool undo_can_undo(const UndoStack *s)
{
    return s->pos != s->base;
}

bool undo_can_redo(const UndoStack *s)
{
    return s->pos != s->head;
}

const UndoCommand *undo_current(const UndoStack *s)
{
    if (undo_can_undo(s)) {
        return &s->stack[wrap(s, s->pos - 1)];
    }
    if (undo_can_redo(s)) {
        return &s->stack[s->pos];
    }
    return NULL;
}
