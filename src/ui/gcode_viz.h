#pragma once
#include "../slicer/slicer.h"
#include "../../third_party/raylib/src/raylib.h"

typedef struct {
    Camera3D camera;
    int current_layer;
    int total_layers;
    int animating;
    float anim_speed;
    float anim_timer;
    int show_travel;
    int show_retract;
    Color extrusion_color;
    Color travel_color;
    Color bed_color;
    Color retract_color;
    float bed_x, bed_y;
    struct {
        V3 *points;
        int *types;
        int npoints;
        int *layer_breaks;
        int nlayers;
    } data;
} GcodeViz;

void gcodeviz_init(GcodeViz *vz);
bool gcodeviz_load(GcodeViz *vz, const char *gcode_path);
void gcodeviz_free(GcodeViz *vz);
void gcodeviz_update(GcodeViz *vz, float dt);
void gcodeviz_render(const GcodeViz *vz, Rectangle viewport);
void gcodeviz_play(GcodeViz *vz);
void gcodeviz_pause(GcodeViz *vz);
void gcodeviz_step(GcodeViz *vz);
void gcodeviz_seek_layer(GcodeViz *vz, int layer);
