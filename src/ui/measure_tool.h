#pragma once
#include <raylib.h>
#include "../core/util.h"
#include "../model/project.h"

typedef enum { MEASURE_IDLE, MEASURE_FIRST, MEASURE_SECOND } MeasureState;

typedef struct {
    MeasureState state;
    V2 point_a;
    V2 point_b;
    V2 mouse_pos;
    int snap_idx;
    int snap_type;
    int active;
    float snap_radius;
} MeasureTool;

void measure_init(MeasureTool *mt);
void measure_activate(MeasureTool *mt);
void measure_deactivate(MeasureTool *mt);
void measure_update(MeasureTool *mt, const Pcb *pcb, Rectangle viewport, V2 pan, float zoom);
void measure_render(const MeasureTool *mt, Rectangle viewport, V2 pan, float zoom);
float mt_measure_distance(const MeasureTool *mt);
float mt_measure_angle_deg(const MeasureTool *mt);
char *mt_measure_report(const MeasureTool *mt);
