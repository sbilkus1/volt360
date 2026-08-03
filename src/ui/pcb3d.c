#include "pcb3d.h"
#include <rlgl.h>
#include <stdio.h>
#include <string.h>

void pcb3d_init(Pcb3DView *v, int width, int height) {
    memset(v, 0, sizeof(*v));
    v->camera.position = (Vector3){ 0.0f, 0.0f, 150.0f };
    v->camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    v->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    v->camera.fovy = 45.0f;
    v->camera.projection = CAMERA_PERSPECTIVE;
    v->zoom = 150.0f;
    v->show_copper_front = 1;
    v->show_copper_back = 1;
    v->show_silkscreen = 1;
    v->show_components = 1;
    v->show_vias = 1;
    v->show_board_outline = 1;
    v->board_color = (Vector3){ 0.1f, 0.3f, 0.1f };
    v->copper_color = (Vector3){ 0.8f, 0.7f, 0.2f };
    v->silk_color = (Vector3){ 0.9f, 0.9f, 0.9f };
    v->via_color = (Vector3){ 0.8f, 0.7f, 0.2f };
    v->bg_color = (Vector3){ 0.15f, 0.15f, 0.15f };
    v->wireframe = 0;
    v->vp_x = 0;
    v->vp_y = 0;
    v->vp_w = width;
    v->vp_h = height;
    (void)width;
    (void)height;
}

void pcb3d_update_camera(Pcb3DView *v) {
    UpdateCamera(&v->camera, CAMERA_ORBITAL);
}

void pcb3d_toggle_wireframe(Pcb3DView *v) {
    v->wireframe = !v->wireframe;
}

void pcb3d_reset_view(Pcb3DView *v) {
    v->camera.position = (Vector3){ 0.0f, 0.0f, 150.0f };
    v->camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    v->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    v->zoom = 150.0f;
}

static Color v3_to_color(Vector3 v) {
    return (Color){
        (unsigned char)(v.x * 255.0f),
        (unsigned char)(v.y * 255.0f),
        (unsigned char)(v.z * 255.0f),
        255
    };
}

void pcb3d_render(const Pcb3DView *v, const Pcb *pcb) {
    Pcb *mpcb;
    float bw, bh, cx, cy, half_thick;
    Color brd_col, cu_col, vi_col, sk_col, bg_col;
    int i;

    if (!pcb) return;
    if (!pcb->has_bbox) {
        mpcb = (Pcb *)pcb;
        pcb_bbox(mpcb);
        if (!pcb->has_bbox) return;
    } else {
        mpcb = (Pcb *)pcb;
    }

    bw = pcb->bmax.x - pcb->bmin.x;
    bh = pcb->bmax.y - pcb->bmin.y;
    if (bw <= 0.0f || bh <= 0.0f) return;

    cx = (pcb->bmin.x + pcb->bmax.x) * 0.5f;
    cy = (pcb->bmin.y + pcb->bmax.y) * 0.5f;
    half_thick = 0.8f;

    brd_col = v3_to_color(v->board_color);
    cu_col = v3_to_color(v->copper_color);
    vi_col = v3_to_color(v->via_color);
    sk_col = v3_to_color(v->silk_color);
    bg_col = v3_to_color(v->bg_color);

    DrawRectangle(v->vp_x, v->vp_y, v->vp_w, v->vp_h, bg_col);
    BeginScissorMode(v->vp_x, v->vp_y, v->vp_w, v->vp_h);

    DrawGrid(20, 10.0f);

    BeginMode3D(v->camera);

    /* board substrate */
    if (v->wireframe)
        DrawCubeWires((Vector3){ 0, 0, 0 }, bw, 1.6f, bh, brd_col);
    else
        DrawCube((Vector3){ 0, 0, 0 }, bw, 1.6f, bh, brd_col);

    /* copper tracks - front */
    if (v->show_copper_front) {
        for (i = 0; i < pcb->ntracks; i++) {
            Vector3 a = { pcb->tracks[i].a.x - cx, half_thick, pcb->tracks[i].a.y - cy };
            Vector3 b = { pcb->tracks[i].b.x - cx, half_thick, pcb->tracks[i].b.y - cy };
            DrawLine3D(a, b, cu_col);
        }
    }

    /* copper tracks - back */
    if (v->show_copper_back) {
        for (i = 0; i < pcb->ntracks; i++) {
            Vector3 a = { pcb->tracks[i].a.x - cx, -half_thick, pcb->tracks[i].a.y - cy };
            Vector3 b = { pcb->tracks[i].b.x - cx, -half_thick, pcb->tracks[i].b.y - cy };
            DrawLine3D(a, b, cu_col);
        }
    }

    /* vias */
    if (v->show_vias) {
        for (i = 0; i < pcb->nvias; i++) {
            Vector3 pos = { pcb->vias[i].pos.x - cx, 0.0f, pcb->vias[i].pos.y - cy };
            float radius = pcb->vias[i].outer * 0.5f;
            if (radius <= 0.0f) radius = 0.5f;
            if (v->wireframe)
                DrawCylinderWires(pos, radius, radius, 1.6f, 16, vi_col);
            else
                DrawCylinder(pos, radius, radius, 1.6f, 16, vi_col);
        }
    }

    /* components as colored boxes */
    if (v->show_components) {
        Color comp_colors[6];
        comp_colors[0] = (Color){ 60, 60, 60, 255 };
        comp_colors[1] = (Color){ 40, 40, 55, 255 };
        comp_colors[2] = (Color){ 55, 40, 40, 255 };
        comp_colors[3] = (Color){ 40, 55, 40, 255 };
        comp_colors[4] = (Color){ 55, 55, 40, 255 };
        comp_colors[5] = (Color){ 60, 40, 55, 255 };

        for (i = 0; i < pcb->nfps; i++) {
            FpInst *f = &mpcb->fps[i];
            float dx = f->pos.x - cx;
            float dz = f->pos.y - cy;
            float box_sz = 5.0f;
            float y_pos;
            Color col;

            y_pos = (f->side == 0) ? (half_thick + box_sz * 0.5f) : (-half_thick - box_sz * 0.5f);
            col = comp_colors[i % 6];

            rlPushMatrix();
            rlTranslatef(dx, y_pos, dz);
            if (f->rotation != 0.0f) rlRotatef(f->rotation, 0.0f, 1.0f, 0.0f);
            if (v->wireframe)
                DrawCubeWires((Vector3){ 0, 0, 0 }, box_sz, box_sz, box_sz, col);
            else
                DrawCube((Vector3){ 0, 0, 0 }, box_sz, box_sz, box_sz, col);
            rlPopMatrix();
        }
    }

    /* board outline */
    if (v->show_board_outline) {
        Color outline_col = { 255, 255, 255, 200 };
        if (pcb->noutline > 0) {
            for (i = 0; i < pcb->noutline; i++) {
                int j = (i + 1) % pcb->noutline;
                Vector3 a = { pcb->outline[i].x - cx, 0.0f, pcb->outline[i].y - cy };
                Vector3 b = { pcb->outline[j].x - cx, 0.0f, pcb->outline[j].y - cy };
                DrawLine3D(a, b, outline_col);
            }
        } else {
            Vector3 crn[4] = {
                { -bw * 0.5f, 0.0f, -bh * 0.5f },
                {  bw * 0.5f, 0.0f, -bh * 0.5f },
                {  bw * 0.5f, 0.0f,  bh * 0.5f },
                { -bw * 0.5f, 0.0f,  bh * 0.5f }
            };
            for (i = 0; i < 4; i++)
                DrawLine3D(crn[i], crn[(i + 1) % 4], outline_col);
        }
    }

    EndMode3D();
    EndScissorMode();

    /* text overlay */
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s", pcb->name ? pcb->name : "PCB");
        DrawText(buf, v->vp_x + 8, v->vp_y + 4, 14, YELLOW);

        snprintf(buf, sizeof(buf), "F: %s  B: %s  Silk: %s  Comp: %s  Via: %s  [%s]",
                 v->show_copper_front ? "ON" : "OFF",
                 v->show_copper_back ? "ON" : "OFF",
                 v->show_silkscreen ? "ON" : "OFF",
                 v->show_components ? "ON" : "OFF",
                 v->show_vias ? "ON" : "OFF",
                 v->wireframe ? "WIRE" : "SOLID");
        DrawText(buf, v->vp_x + 8, v->vp_y + 22, 11, LIGHTGRAY);
    }
}
