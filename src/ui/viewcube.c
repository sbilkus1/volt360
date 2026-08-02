#include "viewcube.h"
#include <string.h>
#include <stdio.h>
#include <raylib.h>

#define CUBE_SZ 80

static const char *g_faces[] = { "FRONT", "BACK", "LEFT", "RIGHT", "TOP", "BOTTOM" };

void viewcube_init(ViewCube *vc, int x, int y) {
    memset(vc, 0, sizeof(*vc));
    vc->rect = (Rectangle){ (float)x, (float)y, (float)CUBE_SZ, (float)CUBE_SZ };
    vc->face_highlighted = -1;
    vc->rotation_x = 0;
    vc->rotation_y = 0;
    vc->rotation_z = 0;
}

void viewcube_set_rotation(ViewCube *vc, float rx, float ry, float rz) {
    vc->rotation_x = rx;
    vc->rotation_y = ry;
    vc->rotation_z = rz;
}

int viewcube_render(ViewCube *vc) {
    int vx = (int)vc->rect.x;
    int vy = (int)vc->rect.y;
    int sz = CUBE_SZ;
    int mid = vx + sz / 2;
    int midy = vy + sz / 2;
    int clicked = -1;
    Vector2 mp = GetMousePosition();

    /* background */
    DrawRectangle(vx, vy, sz, sz, (Color){ 28, 30, 34, 200 });
    DrawRectangleLines(vx, vy, sz, sz, (Color){ 80, 80, 80, 200 });

    /* simple 2D isometric cube representation */
    /* Front face */
    int fx = mid - 18;
    int fy = midy - 18;
    Rectangle fr = { (float)fx, (float)fy, 36, 36 };
    int fh = CheckCollisionPointRec(mp, fr);
    DrawRectangle(fx, fy, 36, 36, fh ? (Color){ 90, 110, 210, 200 } : (Color){ 60, 70, 120, 180 });
    DrawRectangleLines(fx, fy, 36, 36, (Color){ 140, 160, 220, 220 });
    DrawText("F", fx + 14, fy + 10, 14, WHITE);

    /* Top face - isometric above front */
    int tx0 = fx;
    int ty0 = fy - 16;
    int tx1 = fx + 20;
    int ty1 = fy - 32;
    /* Draw a skewed top */
    Vector2 top_verts[4] = {
        { (float)tx0, (float)ty0 },
        { (float)(tx0 + 20), (float)ty1 },
        { (float)(tx0 + 56), (float)ty1 },
        { (float)(tx0 + 36), (float)ty0 }
    };
    int th = 1; // rough hover for top
    if (mp.x >= tx0 && mp.x <= tx0 + 36 && mp.y >= ty1 && mp.y <= ty0) th = 1;

    Color tc = th ? (Color){ 80, 180, 100, 200 } : (Color){ 50, 120, 70, 180 };
    DrawTriangle(top_verts[0], top_verts[1], top_verts[2], tc);
    DrawTriangle(top_verts[0], top_verts[2], top_verts[3], tc);
    DrawLineEx(top_verts[0], top_verts[1], 1.0f, (Color){ 140, 220, 160, 220 });
    DrawLineEx(top_verts[1], top_verts[2], 1.0f, (Color){ 140, 220, 160, 220 });
    DrawLineEx(top_verts[2], top_verts[3], 1.0f, (Color){ 140, 220, 160, 220 });
    DrawLineEx(top_verts[3], top_verts[0], 1.0f, (Color){ 140, 220, 160, 220 });
    DrawText("T", mid - 4, ty1 + 4, 10, WHITE);

    /* Right face - isometric right of front */
    int rx0 = fx + 36;
    int ry0 = fy + 18;
    int rx1 = fx + 56;
    int ry1 = fy + 2;
    Vector2 right_verts[4] = {
        { (float)rx0, (float)fy },
        { (float)rx1, (float)ry1 },
        { (float)rx1, (float)(ry1 + 36) },
        { (float)rx0, (float)(fy + 36) }
    };
    int rh = 0;
    if (mp.x >= rx0 && mp.x <= rx1 && mp.y >= ry1 && mp.y <= ry1 + 36) rh = 1;
    Color rtc = rh ? (Color){ 210, 110, 80, 200 } : (Color){ 140, 70, 50, 180 };
    DrawTriangle(right_verts[0], right_verts[1], right_verts[3], rtc);
    DrawTriangle(right_verts[1], right_verts[2], right_verts[3], rtc);
    DrawLineEx(right_verts[0], right_verts[1], 1.0f, (Color){ 240, 160, 120, 220 });
    DrawLineEx(right_verts[1], right_verts[2], 1.0f, (Color){ 240, 160, 120, 220 });
    DrawLineEx(right_verts[2], right_verts[3], 1.0f, (Color){ 240, 160, 120, 220 });
    DrawLineEx(right_verts[3], right_verts[0], 1.0f, (Color){ 240, 160, 120, 220 });
    DrawText("R", rx0 + 2, ry1 + 10, 10, WHITE);

    /* Left face label on front left */
    DrawText("L", fx - 12, fy + 14, 10, (Color){ 200, 200, 200, 150 });

    /* Back label top */
    DrawText("B", mid + 10, ty1 + 2, 10, (Color){ 200, 200, 200, 150 });

    /* Bottom label */
    DrawText("BTM", mid - 10, vy + sz - 14, 8, (Color){ 200, 200, 200, 150 });

    /* Click detection */
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (fh) clicked = 0;      /* Front */
        else if (th) clicked = 4; /* Top */
        else if (rh) clicked = 3; /* Right */
        else if (mp.x >= fx - 14 && mp.x <= fx && mp.y >= fy && mp.y <= fy + 36) clicked = 2; /* Left */
        else if (mp.x >= mid - 18 && mp.x <= mid + 18 && mp.y >= vy + sz - 16 && mp.y <= vy + sz) clicked = 5; /* Bottom */
        else if (mp.x >= mid + 10 && mp.x <= mid + 30 && mp.y >= ty1 && mp.y <= ty1 + 12) clicked = 1; /* Back */
    }

    vc->face_highlighted = clicked >= 0 ? clicked : -1;
    return clicked;
}
