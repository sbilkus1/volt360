#pragma once
#include "../model/project.h"
#include <raylib.h>

typedef enum { DIFF_LAYER_ALL, DIFF_LAYER_COPPER, DIFF_LAYER_SILK, DIFF_LAYER_OUTLINE } DiffLayer;

typedef enum { DIFF_NONE, DIFF_UNCHANGED, DIFF_ADDED, DIFF_REMOVED, DIFF_MODIFIED } DiffStatus;

typedef struct {
    int a_idx;
    int b_idx;
    DiffStatus status;
} TrackDiff;

typedef struct {
    int a_idx;
    int b_idx;
    DiffStatus status;
} ViaDiff;

typedef struct {
    Pcb *pcb_a;
    Pcb *pcb_b;
    int added_count;
    int removed_count;
    int modified_count;
    DiffLayer layer;
    int highlight_track;
    Rectangle viewport;
    /* internal */
    TrackDiff *track_diffs;
    int n_track_diffs;
    ViaDiff *via_diffs;
    int n_via_diffs;
    int total_diffs;
} DiffView;

void diff_view_init(DiffView *dv);
void diff_view_compare(DiffView *dv, const Pcb *a, const Pcb *b);
void diff_view_render(DiffView *dv);
void diff_view_cycle(DiffView *dv);
const char *diff_view_summary(DiffView *dv);
