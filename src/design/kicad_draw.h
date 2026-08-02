#pragma once
// kicad_draw.h - KiCad-style drawing: grid, junctions, layers, ratsnest, pads.
#include <stdbool.h>
#include "../model/project.h"

// === GRID SYSTEM ===
// Grid snap: round coordinate to nearest grid point.
V2 grid_snap(V2 pos, float grid_size);
float grid_size_current(void);
void grid_set_size(float mm);
void grid_toggle(void); // cycle through common grid sizes

// === JUNCTION DOTS ===
// Check if a point is a wire junction (3+ wire endpoints within tolerance).
bool is_junction(Schematic *s, V2 pos, float tolerance);
// Generate all junction points in the schematic.
int junctions_find(Schematic *s, V2 *out, int max_out, float tolerance);

// === PCB LAYER COLORS ===
// KiCad-style layer colors.
typedef struct { unsigned char r,g,b,a; const char *name; } LayerColor;
LayerColor layer_color_get(int layer_id);
int layer_id_from_name(const char *name);

// === RATSNEST ===
// Compute unrouted connections (pads that are on the same net but not connected by tracks).
// Returns number of ratsnest lines, fills from/to arrays.
int ratsnest_compute(Pcb *pcb, V2 *from, V2 *to, int max_lines);

// === PAD RENDERING ===
// Draw a pad of specified shape at position.
void pad_draw_outline(V2 pos, V2 size, float angle, int shape, int layer, float zoom, int x_off, int y_off);

// === VIA DRILL RENDERING ===
// Draw a via with drill hole visualization.
void via_draw(V2 pos, float outer_dia, float drill_dia, int x_off, int y_off, float zoom);

// === SYMBOL DRAWING ===
// Draw a component symbol (rectangle with pins).
void symbol_draw_outline(V2 pos, float w, float h, const char *ref, const char *value, int npins, int x_off, int y_off, float zoom);

// === WIRE ROUTING ===
// Convert a direct line to 90° corner routing (manhattan).
int wire_manhattan(V2 start, V2 end, V2 *waypoints, int max_points);

// === SELECTION RECTANGLE ===
typedef struct { V2 p1, p2; int active; } SelectionRect;
int items_in_rect(V2 *positions, int n, V2 r1, V2 r2, int *selected, int max_sel);

// === GRID AND COORDINATE RENDERING ===
void grid_draw_kiCad_style(int x, int y, int w, int h, float zoom, V2 pan, float grid_size);
char *coord_status_text(V2 world_pos, float grid_size);
