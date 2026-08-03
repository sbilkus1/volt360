#pragma once
#include "../model/project.h"
#include <raylib.h>

typedef struct {
    Camera3D camera;
    Vector3 rotation;
    float zoom;
    int show_copper_front;
    int show_copper_back;
    int show_silkscreen;
    int show_components;
    int show_vias;
    int show_board_outline;
    Vector3 board_color;
    Vector3 copper_color;
    Vector3 silk_color;
    Vector3 via_color;
    Vector3 bg_color;
    int wireframe;
    int vp_x, vp_y, vp_w, vp_h;
} Pcb3DView;

void pcb3d_init(Pcb3DView *v, int width, int height);
void pcb3d_update_camera(Pcb3DView *v);
void pcb3d_render(const Pcb3DView *v, const Pcb *pcb);
void pcb3d_toggle_wireframe(Pcb3DView *v);
void pcb3d_reset_view(Pcb3DView *v);
