#pragma once
// autorouter.h - Lee's algorithm grid-based maze router with ripup-and-retry
#include "../model/project.h"

typedef struct {
    int success;
    int total_nets;
    int routed_nets;
    int failed_nets;
    int iterations;
    float total_length;
    int vias_placed;
} RouteResult;

typedef enum { CELL_FREE, CELL_OBSTACLE, CELL_VIA } CellState;

typedef struct {
    CellState *cells;
    int w, h;
    float cell_size;
    float via_cost;
    float clearance;
    int max_iterations;
    V2 origin; /* world pos of grid cell (0,0) */
    int *dist; /* BFS distance grid, NULL when idle */
} RouteGrid;

void route_grid_init(RouteGrid *g, const Pcb *pcb, float cell_size);
void route_grid_free(RouteGrid *g);
void route_grid_mark_obstacles(RouteGrid *g, const Pcb *pcb);
void route_grid_mark_track(RouteGrid *g, const Track *t);
void route_grid_clear_path(RouteGrid *g);

bool route_lee(RouteGrid *g, int sx, int sy, int tx, int ty,
               int *out_x, int *out_y, int *out_count, int max_segments);

RouteResult autoroute_all(Pcb *pcb, float cell_size, int max_ripup_iterations);
void autoroute_progress_report(const RouteResult *r);
