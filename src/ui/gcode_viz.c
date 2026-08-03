#include "gcode_viz.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

void gcodeviz_init(GcodeViz *vz) {
    memset(vz, 0, sizeof(*vz));
    vz->camera.position = (Vector3){ 0.0f, -200.0f, 200.0f };
    vz->camera.target = (Vector3){ 0.0f, 0.0f, 50.0f };
    vz->camera.up = (Vector3){ 0.0f, 0.0f, 1.0f };
    vz->camera.fovy = 45.0f;
    vz->camera.projection = CAMERA_PERSPECTIVE;
    vz->animating = 0;
    vz->anim_speed = 2.0f;
    vz->anim_timer = 0.0f;
    vz->show_travel = 1;
    vz->show_retract = 0;
    vz->extrusion_color = (Color){ 60, 230, 80, 255 };
    vz->travel_color = (Color){ 100, 100, 120, 180 };
    vz->bed_color = (Color){ 60, 60, 60, 255 };
    vz->retract_color = (Color){ 255, 60, 60, 255 };
    vz->bed_x = 220.0f;
    vz->bed_y = 220.0f;
    vz->data.points = NULL;
    vz->data.types = NULL;
    vz->data.npoints = 0;
    vz->data.layer_breaks = NULL;
    vz->data.nlayers = 0;
}

void gcodeviz_free(GcodeViz *vz) {
    free(vz->data.points);
    free(vz->data.types);
    free(vz->data.layer_breaks);
    memset(&vz->data, 0, sizeof(vz->data));
}

static int is_extrude_line(const char *line, float *x, float *y, float *z, float *e) {
    char cmd[8] = { 0 };
    float fx = *x, fy = *y, fz = *z, fe = *e;
    int has_x = 0, has_y = 0, has_z = 0, has_e = 0;
    if (sscanf(line, "%7s", cmd) != 1) return 0;
    if (strcmp(cmd, "G0") != 0 && strcmp(cmd, "G1") != 0) return 0;
    const char *p = line + strlen(cmd);
    while (*p) {
        if (*p == 'X' || *p == 'x') { sscanf(p + 1, "%f", &fx); has_x = 1; }
        if (*p == 'Y' || *p == 'y') { sscanf(p + 1, "%f", &fy); has_y = 1; }
        if (*p == 'Z' || *p == 'z') { sscanf(p + 1, "%f", &fz); has_z = 1; }
        if (*p == 'E' || *p == 'e') { sscanf(p + 1, "%f", &fe); has_e = 1; }
        while (*p && *p != ' ') p++;
        while (*p == ' ') p++;
    }
    *x = fx; *y = fy; *z = fz; *e = fe;
    if (!has_x && !has_y && !has_z) return 0;
    if (strcmp(cmd, "G1") == 0 && has_e && fe > 0) return 2; /* extrude */
    return 1; /* travel */
}

bool gcodeviz_load(GcodeViz *vz, const char *gcode_path) {
    gcodeviz_free(vz);
    size_t flen = 0;
    char *data = file_read(gcode_path, &flen);
    if (!data) return false;

    ARRAY(V3) pts_arr;
    ARRAY(int) types_arr;
    ARRAY(int) layer_arr;
    arr_init(pts_arr);
    arr_init(types_arr);
    arr_init(layer_arr);

    float cx = 0, cy = 0, cz = 0, ce = 0;
    int first = 1;
    float prev_z = -999.0f;
    float max_x = 0, max_y = 0;

    char *saveptr = NULL;
    char *line = strtok_s(data, "\r\n", &saveptr);
    while (line) {
        float x, y, z, e;
        int kind = is_extrude_line(line, &x, &y, &z, &e);
        if (kind > 0) {
            float nx = x, ny = y, nz = z;
            if (x == 0 && y == 0 && z == 0) { nx = cx; ny = cy; nz = cz; }
            else {
                if (x != 0 || y != 0 || z != 0) { nx = (x != 0) ? x : (float)(int)(cx * 1000 + 0.5f) / 1000.0f; }
                if (y != 0 || x != 0 || z != 0) { ny = (y != 0) ? y : (float)(int)(cy * 1000 + 0.5f) / 1000.0f; }
                if (z != 0) { nz = z; } else { nz = cz; }
            }
            V3 pt = v3(nx, ny, nz);
            arr_push(pts_arr, pt);
            arr_push(types_arr, (kind == 2) ? 0 : 1);
            if (first) { first = 0; prev_z = nz; }
            if (fabsf(nz - prev_z) > 0.001f) {
                if (layer_arr.len == 0 || pts_arr.len > 1)
                    arr_push(layer_arr, pts_arr.len - 1);
                prev_z = nz;
            }
            float ax = fabsf(nx), ay = fabsf(ny);
            if (ax > max_x) max_x = ax;
            if (ay > max_y) max_y = ay;
            cx = nx; cy = ny; cz = nz; ce = e;
        }
        line = strtok_s(NULL, "\r\n", &saveptr);
    }
    free(data);

    if (pts_arr.len < 1) return false;
    arr_push(layer_arr, pts_arr.len);

    vz->data.points = pts_arr.v;
    vz->data.types = types_arr.v;
    vz->data.npoints = pts_arr.len;
    vz->data.layer_breaks = layer_arr.v;
    vz->data.nlayers = layer_arr.len;
    vz->total_layers = layer_arr.len;
    vz->current_layer = layer_arr.len;
    if (max_x > 5.0f) vz->bed_x = max_x * 2.2f;
    if (max_y > 5.0f) vz->bed_y = max_y * 2.2f;

    V3 center_pt = pts_arr.v[pts_arr.len / 2];
    vz->camera.target = (Vector3){ center_pt.x, center_pt.y, center_pt.z * 0.5f };
    float dist = fmaxf(vz->bed_x, vz->bed_y) * 1.2f;
    vz->camera.position = (Vector3){ center_pt.x, center_pt.y - dist * 0.8f, dist * 0.6f };

    return true;
}

void gcodeviz_update(GcodeViz *vz, float dt) {
    if (!vz->animating) return;
    vz->anim_timer += dt;
    while (vz->anim_timer >= 1.0f / vz->anim_speed) {
        vz->anim_timer -= 1.0f / vz->anim_speed;
        if (vz->current_layer < vz->total_layers)
            vz->current_layer++;
        else {
            vz->animating = 0;
            vz->anim_timer = 0;
            break;
        }
    }
}

void gcodeviz_play(GcodeViz *vz) { vz->animating = 1; }
void gcodeviz_pause(GcodeViz *vz) { vz->animating = 0; }

void gcodeviz_step(GcodeViz *vz) {
    if (vz->current_layer < vz->total_layers)
        vz->current_layer++;
}

void gcodeviz_seek_layer(GcodeViz *vz, int layer) {
    if (layer < 0) layer = 0;
    if (layer > vz->total_layers) layer = vz->total_layers;
    vz->current_layer = layer;
}

static void draw_line3d(V3 a, V3 b, Color col) {
    DrawLine3D((Vector3){ a.x, a.y, a.z }, (Vector3){ b.x, b.y, b.z }, col);
}

void gcodeviz_render(const GcodeViz *vz, Rectangle viewport) {
    int end_pt = (vz->current_layer < vz->data.nlayers) ? vz->data.layer_breaks[vz->current_layer] : vz->data.npoints;
    if (end_pt > vz->data.npoints) end_pt = vz->data.npoints;

    float cam_x = vz->camera.position.x;
    float cam_y = vz->camera.position.y;
    float cam_z = vz->camera.position.z;

    BeginMode3D(vz->camera);

    /* bed */
    DrawPlane((Vector3){ 0, 0, 0 }, (Vector2){ vz->bed_x, vz->bed_y }, vz->bed_color);
    DrawGrid((int)(vz->bed_x / 10), 10.0f);

    /* gcode paths up to current layer */
    for (int i = 1; i < end_pt; i++) {
        int typ = vz->data.types[i];
        V3 a = vz->data.points[i - 1];
        V3 b = vz->data.points[i];
        if (typ == 0) {
            /* extrude */
            draw_line3d(a, b, vz->extrusion_color);
        } else if (typ == 1 && vz->show_travel) {
            /* travel */
            draw_line3d(a, b, vz->travel_color);
        }
    }

    /* semi-transparent current Z plane */
    if (end_pt > 0) {
        float cur_z = 0;
        for (int i = end_pt - 1; i >= 0; i--) {
            if (vz->data.types[i] == 0) { cur_z = vz->data.points[i].z; break; }
        }
        DrawCubeWiresV((Vector3){ 0, 0, cur_z }, (Vector3){ vz->bed_x, vz->bed_y, 0.05f }, Fade(SKYBLUE, 0.3f));
    }

    EndMode3D();

    /* overlay HUD */
    float cur_z = 0;
    if (end_pt > 0 && vz->current_layer < vz->data.nlayers) {
        int idx = vz->data.layer_breaks[vz->current_layer];
        if (idx < vz->data.npoints) cur_z = vz->data.points[idx].z;
    }
    char buf[128];
    snprintf(buf, sizeof(buf), "Layer %d/%d | Z=%.2f mm", vz->current_layer, vz->total_layers, cur_z);
    DrawText(buf, (int)viewport.x + 10, (int)viewport.y + 10, 20, WHITE);

    /* bottom scrubber */
    float bar_y = viewport.y + viewport.height - 30;
    float bar_w = viewport.width - 40;
    DrawRectangle((int)viewport.x + 10, (int)(bar_y - 2), (int)bar_w, 4, DARKGRAY);
    if (vz->total_layers > 1) {
        float t = (float)vz->current_layer / (float)vz->total_layers;
        DrawCircle((int)(viewport.x + 10 + t * bar_w), (int)bar_y, 8, SKYBLUE);
        DrawCircleLines((int)(viewport.x + 10 + t * bar_w), (int)bar_y, 8, WHITE);
    }
}
