#pragma once
#include <raylib.h>

typedef struct {
    char name[16];
    int type;
    float thickness;
    float er;
    Color color;
    int signal_layer;
    float copper_weight;
} StackupLayer;

typedef struct {
    StackupLayer layers[16];
    int nlayers;
    float total_thickness;
    Rectangle viewport;
    float trace_width;
    float trace_spacing;
    float target_z0;
    float target_zdiff;
} StackupPanel;

void stackup_init(StackupPanel *sp);
void stackup_set_preset(StackupPanel *sp, int preset);
void stackup_add_layer(StackupPanel *sp, const char *name, int type, float thickness, float er);
float stackup_calc_z0(const StackupPanel *sp, int signal_layer);
float stackup_calc_zdiff(const StackupPanel *sp, int signal_layer);
void stackup_render(StackupPanel *sp);
