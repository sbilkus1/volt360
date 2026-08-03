#pragma once
/* thermal_sim.h - 2D finite-difference heat distribution simulation */
#include "../model/project.h"
#include "raylib.h"

typedef struct {
    float power;
    float max_temp;
    float theta_ja;
} ThermalComponent;

typedef struct {
    float *grid;
    float *copper_factor;
    float *heat_source;
    float *source_temp;
    int w, h;
    float cell_size;
    float ambient;
    float convection;
    float copper_weight;
    float max_temp;
    float min_temp;
    V2 hotspot;
    float *render_colors;
} ThermalGrid;

typedef struct {
    ThermalGrid grid;
    int iterations;
    float convergence;
    int success;
} ThermalResult;

void thermal_grid_init(ThermalGrid *tg, const Pcb *pcb, float cell_size);
void thermal_grid_free(ThermalGrid *tg);
void thermal_set_component(ThermalGrid *tg, V2 pos, float power, float theta_ja);
void thermal_apply_copper(ThermalGrid *tg, const Pcb *pcb);
ThermalResult thermal_solve(ThermalGrid *tg, int max_iter, float tolerance);
void thermal_compute_colors(ThermalGrid *tg);
Color thermal_get_color(const ThermalGrid *tg, float temp);
void thermal_render(const ThermalGrid *tg, Rectangle viewport, Vector2 pan, float zoom);
void thermal_render_legend(Rectangle area);
char *thermal_sim_report(const ThermalGrid *tg);
