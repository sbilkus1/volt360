#pragma once
/* panelizer.h - PCB panelization: compute optimal panel layout.
 * Supports V-score (straight cuts) and tab-routing with mouse bites.
 */
#include <stdbool.h>
#include "../model/project.h"

typedef struct {
    float pcb_margin;       /* mm between PCB edges (default 2.0) */
    float frame_margin;     /* mm from frame edge to PCB (default 10.0) */
    int vscore;             /* 1 = V-score lines, 0 = tab routing */
    int add_fiducials;      /* 1 = add fiducial marks */
    int add_tooling_holes;  /* 1 = add tooling holes */
    float tab_width;        /* mm tab width for tab routing (default 3.0) */
    float mouse_bite_dia;   /* mm diameter of mouse bite holes (default 0.5) */
    float mouse_bite_spacing; /* mm center-to-center (default 1.0) */
    float panel_cx;         /* max panel X extent (mm, 0 = auto) */
    float panel_cy;         /* max panel Y extent (mm, 0 = auto) */
} PanelConfig;

typedef struct {
    float x, y;             /* panel position offset in mm */
    float rotation;         /* rotation in degrees (0, 90, 180, 270) */
} PanelPos;

typedef struct {
    float x1, y1, x2, y2;   /* V-score line segment */
} VScoreLine;

typedef struct {
    PanelPos *positions;    /* array of PCB positions */
    int npositions;
    VScoreLine *vscore_lines; /* V-score cut lines */
    int nvscores;
    float panel_w, panel_h; /* total panel dimensions */
} PanelResult;

void panel_config_default(PanelConfig *cfg);
void panel_result_free(PanelResult *r);

/* panelize_pcb: given PCB bounding box + quantity, compute optimal panel layout.
 * Returns number of PCB positions placed, or 0 on failure. */
int panelize_pcb(V2 pcb_min, V2 pcb_max, int qty, PanelConfig *cfg, PanelResult *out, int max_panels);

/* Compute panel bounding box given PCB positions. */
void panel_bbox(PanelPos *positions, int n, float *w, float *h);

/* Generate V-score lines between adjacent board positions. */
int panel_vscore_lines(PanelPos *positions, int n, float pcb_w, float pcb_h, float margin, VScoreLine *lines, int max_lines);
