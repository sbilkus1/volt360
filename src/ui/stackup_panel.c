#include "stackup_panel.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

static bool ui_button(const char *label, int x, int y, int w, int h) {
    Vector2 m = GetMousePosition();
    bool hover = m.x >= (float)x && m.x <= (float)(x + w) && m.y >= (float)y && m.y <= (float)(y + h);
    DrawRectangle(x, y, w, h, hover ? (Color){ 70, 90, 130, 255 } : (Color){ 55, 60, 75, 255 });
    DrawRectangleLines(x, y, w, h, (Color){ 120, 130, 150, 255 });
    DrawText(label, x + 6, y + (h - 14) / 2, 14, WHITE);
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void stackup_init(StackupPanel *sp) {
    memset(sp, 0, sizeof(*sp));
    sp->trace_width = 0.20f;
    sp->trace_spacing = 0.20f;
    sp->target_z0 = 50.0f;
    sp->target_zdiff = 100.0f;
    sp->viewport = (Rectangle){ 0, 0, 0, 0 };
}

void stackup_add_layer(StackupPanel *sp, const char *name, int type, float thickness, float er) {
    if (sp->nlayers >= 16) return;
    StackupLayer *l = &sp->layers[sp->nlayers++];
    memset(l, 0, sizeof(*l));
    snprintf(l->name, sizeof(l->name), "%s", name);
    l->type = type;
    l->thickness = thickness;
    l->er = er;

    switch (type) {
        case 0: l->color = (Color){ 220, 140, 60, 255 }; l->copper_weight = 1.0f; l->signal_layer = 0; break;
        case 1: l->color = (Color){ 180, 180, 100, 255 }; l->signal_layer = 0; break;
        case 2: l->color = (Color){ 0, 150, 0, 255 }; l->signal_layer = 0; break;
        case 3: l->color = (Color){ 200, 200, 200, 255 }; l->signal_layer = 0; break;
        default: l->color = (Color){ 128, 128, 128, 255 }; break;
    }

    sp->total_thickness += thickness;
}

void stackup_set_preset(StackupPanel *sp, int preset) {
    sp->nlayers = 0;
    sp->total_thickness = 0;

    switch (preset) {
        case 0:
            stackup_add_layer(sp, "F.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 1.530f, 4.2f);
            stackup_add_layer(sp, "B.Cu", 0, 0.035f, 0);
            sp->layers[0].signal_layer = 1;
            sp->layers[2].signal_layer = 1;
            break;

        case 1:
            stackup_add_layer(sp, "F.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "Prepreg", 1, 0.200f, 4.0f);
            stackup_add_layer(sp, "GND", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 1.060f, 4.2f);
            stackup_add_layer(sp, "PWR", 0, 0.035f, 0);
            stackup_add_layer(sp, "Prepreg", 1, 0.200f, 4.0f);
            stackup_add_layer(sp, "B.Cu", 0, 0.035f, 0);
            sp->layers[0].signal_layer = 1;
            sp->layers[6].signal_layer = 1;
            break;

        case 2:
            stackup_add_layer(sp, "F.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "Prepreg", 1, 0.150f, 4.0f);
            stackup_add_layer(sp, "In1.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 0.500f, 4.2f);
            stackup_add_layer(sp, "GND", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 0.500f, 4.2f);
            stackup_add_layer(sp, "PWR", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 0.500f, 4.2f);
            stackup_add_layer(sp, "In2.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "Prepreg", 1, 0.150f, 4.0f);
            stackup_add_layer(sp, "B.Cu", 0, 0.035f, 0);
            sp->layers[0].signal_layer = 1;
            sp->layers[2].signal_layer = 1;
            sp->layers[8].signal_layer = 1;
            sp->layers[10].signal_layer = 1;
            break;

        case 3:
            stackup_add_layer(sp, "F.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "Prepreg", 1, 0.120f, 4.0f);
            stackup_add_layer(sp, "In1.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 0.300f, 4.2f);
            stackup_add_layer(sp, "In2.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 0.400f, 4.2f);
            stackup_add_layer(sp, "GND", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 0.400f, 4.2f);
            stackup_add_layer(sp, "PWR", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 0.300f, 4.2f);
            stackup_add_layer(sp, "In3.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "FR4", 1, 0.300f, 4.2f);
            stackup_add_layer(sp, "In4.Cu", 0, 0.035f, 0);
            stackup_add_layer(sp, "Prepreg", 1, 0.120f, 4.0f);
            stackup_add_layer(sp, "B.Cu", 0, 0.035f, 0);
            sp->layers[0].signal_layer = 1;
            sp->layers[2].signal_layer = 1;
            sp->layers[4].signal_layer = 1;
            sp->layers[8].signal_layer = 1;
            sp->layers[10].signal_layer = 1;
            sp->layers[12].signal_layer = 1;
            sp->layers[14].signal_layer = 1;
            break;

        default:
            break;
    }
}

float stackup_calc_z0(const StackupPanel *sp, int signal_layer) {
    if (signal_layer < 0 || signal_layer >= sp->nlayers) return -1.0f;
    if (sp->layers[signal_layer].type != 0) return -1.0f;

    float h = 0;
    float er = 4.2f;
    int is_top = 1;
    int is_bottom = 1;

    for (int i = 0; i < signal_layer; i++) is_top = 0;
    for (int i = signal_layer + 1; i < sp->nlayers; i++) is_bottom = 0;

    if (!is_top && !is_bottom) {
        float h_above = 0, er_above = 4.2f;
        float h_below = 0, er_below = 4.2f;

        for (int i = signal_layer - 1; i >= 0; i--) {
            if (sp->layers[i].type == 1) {
                h_above += sp->layers[i].thickness;
                er_above = sp->layers[i].er;
                if (sp->layers[i].thickness > 1e-6f) break;
            }
        }
        for (int i = signal_layer + 1; i < sp->nlayers; i++) {
            if (sp->layers[i].type == 1) {
                h_below += sp->layers[i].thickness;
                er_below = sp->layers[i].er;
                if (sp->layers[i].thickness > 1e-6f) break;
            }
        }
        if (h_above < 1e-6f || h_below < 1e-6f) return -1.0f;

        h = h_above + h_below;
        float er_eff = (er_above * h_above + er_below * h_below) / h;
        er = er_eff;

        float w = sp->trace_width;
        float t = sp->layers[signal_layer].thickness;
        float term = (w > 0 ? (w / h) : 2.0f);
        float z0 = 60.0f / sqrtf(er) * logf(4.0f * h / (0.67f * M_PI * w * (0.8f + term)));
        return z0;
    }

    if (is_top) {
        for (int i = signal_layer + 1; i < sp->nlayers; i++) {
            if (sp->layers[i].type == 1) {
                h += sp->layers[i].thickness;
                er = sp->layers[i].er;
                if (sp->layers[i].thickness > 1e-6f) break;
            }
        }
    } else {
        for (int i = signal_layer - 1; i >= 0; i--) {
            if (sp->layers[i].type == 1) {
                h += sp->layers[i].thickness;
                er = sp->layers[i].er;
                if (sp->layers[i].thickness > 1e-6f) break;
            }
        }
    }

    if (h < 1e-6f) return -1.0f;

    float w = sp->trace_width;
    float t = sp->layers[signal_layer].thickness;
    float z0_micro = 87.0f / sqrtf(er + 1.41f) * logf(5.98f * h / (0.8f * w + t));
    return z0_micro;
}

float stackup_calc_zdiff(const StackupPanel *sp, int signal_layer) {
    float z0 = stackup_calc_z0(sp, signal_layer);
    if (z0 <= 0) return -1.0f;

    float s = sp->trace_spacing;
    float w = sp->trace_width;
    float h = 0;

    int is_top = 1;
    for (int i = 0; i < signal_layer; i++) is_top = 0;

    if (is_top) {
        for (int i = signal_layer + 1; i < sp->nlayers; i++) {
            if (sp->layers[i].type == 1) {
                h += sp->layers[i].thickness;
                if (sp->layers[i].thickness > 1e-6f) break;
            }
        }
    } else {
        for (int i = signal_layer - 1; i >= 0; i--) {
            if (sp->layers[i].type == 1) {
                h += sp->layers[i].thickness;
                if (sp->layers[i].thickness > 1e-6f) break;
            }
        }
    }

    if (h < 1e-6f) return z0 * 1.8f;

    float ratio = s / h;
    float zdiff = 2.0f * z0 * (1.0f - 0.48f * expf(-0.96f * ratio));
    return zdiff;
}

void stackup_render(StackupPanel *sp) {
    Rectangle vp = sp->viewport;
    if (vp.width <= 0 || vp.height <= 0) return;

    int x = (int)vp.x;
    int y = (int)vp.y;
    int w = (int)vp.width;
    int h = (int)vp.height;

    DrawRectangle(x, y, w, h, (Color){ 20, 22, 26, 255 });
    DrawRectangleLines(x, y, w, h, (Color){ 60, 60, 60, 255 });

    DrawText("Layer Stackup", x + 8, y + 4, 14, WHITE);

    if (sp->nlayers == 0) {
        DrawText("No layers defined", x + 20, y + 40, 12, (Color){ 120, 120, 120, 255 });
        DrawText("Use preset buttons below", x + 20, y + 56, 12, (Color){ 120, 120, 120, 255 });
        int py = y + h - 80;
        if (ui_button("2-layer", x + 8, py, 88, 26)) stackup_set_preset(sp, 0);
        if (ui_button("4-layer", x + 100, py, 88, 26)) stackup_set_preset(sp, 1);
        if (ui_button("6-layer", x + 192, py, 88, 26)) stackup_set_preset(sp, 2);
        if (ui_button("8-layer", x + 284, py, 88, 26)) stackup_set_preset(sp, 3);
        return;
    }

    int bar_x = x + 100;
    int bar_y = y + 24;
    int bar_w = w - 220;
    int bar_h_total = h - 140;

    if (bar_h_total < 20) bar_h_total = 20;

    float max_thick = sp->total_thickness > 0 ? sp->total_thickness : 1.6f;
    float scale_y = (float)bar_h_total / max_thick;

    float cy = (float)bar_y;
    for (int i = 0; i < sp->nlayers; i++) {
        StackupLayer *l = &sp->layers[i];
        int lh = l->thickness > 0.0035f ? (int)(l->thickness * scale_y) : 2;
        if (lh < 1) lh = 1;

        char label[128];
        const char *tname = l->type == 0 ? "Cu" : l->type == 1 ? "DI" : l->type == 2 ? "SM" : "SS";
        snprintf(label, sizeof(label), "%s (%s) %.0fum", l->name, tname, l->thickness * 1000.0f);
        DrawText(label, x + 4, (int)cy + lh / 2 - 6, 10, (Color){ 180, 180, 200, 255 });

        DrawRectangle(bar_x, (int)cy, bar_w, lh, l->color);
        DrawRectangleLines(bar_x, (int)cy, bar_w, lh, (Color){ 80, 80, 80, 255 });

        if (l->signal_layer) {
            char sl[16];
            snprintf(sl, sizeof(sl), "SIG");
            DrawText(sl, bar_x + 4, (int)cy + lh / 2 - 6, 10, WHITE);
        }

        cy += (float)lh;
    }

    int imp_x = x + 100 + bar_w + 8;
    int imp_y = bar_y;
    for (int i = 0; i < sp->nlayers; i++) {
        if (sp->layers[i].signal_layer) {
            float z0 = stackup_calc_z0(sp, i);
            char zbuf[32];
            if (z0 > 0) snprintf(zbuf, sizeof(zbuf), "Z0=%.0fR", z0);
            else snprintf(zbuf, sizeof(zbuf), "Z0=--");
            DrawText(zbuf, imp_x, imp_y, 10, (Color){ 100, 180, 255, 255 });

            float zd = stackup_calc_zdiff(sp, i);
            if (zd > 0) {
                snprintf(zbuf, sizeof(zbuf), "Zdiff=%.0fR", zd);
                DrawText(zbuf, imp_x, imp_y + 12, 10, (Color){ 100, 200, 180, 255 });
            }
        }
        imp_y += sp->layers[i].thickness > 0.0035f ? (int)(sp->layers[i].thickness * scale_y) : 2;
        if (imp_y < imp_y + 1) imp_y = imp_y + 1;
    }

    float total_y = (float)bar_y + bar_h_total;
    int slider_y = (int)total_y + 8;

    DrawText("Trace W:", x + 4, slider_y + 2, 12, (Color){ 160, 160, 180, 255 });
    DrawText(TextFormat("%.2f mm", sp->trace_width), x + 60, slider_y + 2, 12, WHITE);
    if (ui_button("-", x + 140, slider_y, 24, 18)) sp->trace_width = sp->trace_width > 0.05f ? sp->trace_width - 0.05f : sp->trace_width;
    if (ui_button("+", x + 166, slider_y, 24, 18)) sp->trace_width = sp->trace_width < 2.0f ? sp->trace_width + 0.05f : sp->trace_width;

    DrawText("Spacing:", x + 4, slider_y + 22, 12, (Color){ 160, 160, 180, 255 });
    DrawText(TextFormat("%.2f mm", sp->trace_spacing), x + 60, slider_y + 22, 12, WHITE);
    if (ui_button("-", x + 140, slider_y + 22, 24, 18)) sp->trace_spacing = sp->trace_spacing > 0.05f ? sp->trace_spacing - 0.05f : sp->trace_spacing;
    if (ui_button("+", x + 166, slider_y + 22, 24, 18)) sp->trace_spacing = sp->trace_spacing < 2.0f ? sp->trace_spacing + 0.05f : sp->trace_spacing;

    DrawText("Target Z0:", x + 4, slider_y + 44, 12, (Color){ 160, 160, 180, 255 });
    DrawText(TextFormat("%.0f R", sp->target_z0), x + 60, slider_y + 44, 12, WHITE);
    if (ui_button("-", x + 140, slider_y + 44, 24, 18)) sp->target_z0 = sp->target_z0 > 10.0f ? sp->target_z0 - 5.0f : sp->target_z0;
    if (ui_button("+", x + 166, slider_y + 44, 24, 18)) sp->target_z0 = sp->target_z0 < 200.0f ? sp->target_z0 + 5.0f : sp->target_z0;

    int py = y + h - 36;
    if (ui_button("2-layer", x + 8, py, 68, 24)) stackup_set_preset(sp, 0);
    if (ui_button("4-layer", x + 80, py, 68, 24)) stackup_set_preset(sp, 1);
    if (ui_button("6-layer", x + 152, py, 68, 24)) stackup_set_preset(sp, 2);
    if (ui_button("8-layer", x + 224, py, 68, 24)) stackup_set_preset(sp, 3);
}
