#pragma once
/* viewcube.h - Navigation cube overlay (top-right of canvas) */
#include <stdbool.h>
#include <raylib.h>
#include "../core/util.h"

typedef struct {
    Rectangle rect;
    int face_highlighted;
    float rotation_x, rotation_y, rotation_z;
} ViewCube;

void viewcube_init(ViewCube *vc, int x, int y);
int viewcube_render(ViewCube *vc);
void viewcube_set_rotation(ViewCube *vc, float rx, float ry, float rz);
