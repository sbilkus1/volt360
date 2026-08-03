#include "sch_canvas.h"
#include <stdio.h>

#define BG_COLOR    (Color){30,30,30,255}
#define GRID_COLOR  (Color){50,50,50,255}
#define WIRE_COLOR  (Color){200,200,200,255}
#define JUNC_COLOR  (Color){100,180,255,255}
#define SEL_COLOR   (Color){255,200,0,255}
#define PAL_BG      (Color){40,42,48,255}
#define PAL_ITEM    (Color){55,57,63,255}
#define PAL_HOVER   (Color){70,72,78,255}
#define TOOL_BG     (Color){25,28,35,255}

static V2 snap_to_grid(V2 p, float gs) {
    V2 r;
    r.x = roundf(p.x / gs) * gs;
    r.y = roundf(p.y / gs) * gs;
    return r;
}

static V2 world_to_screen(SchCanvas *c, V2 world) {
    V2 r;
    r.x = c->viewport.x + c->viewport.width * 0.5f + (world.x - c->pan_offset.x) * c->zoom;
    r.y = c->viewport.y + c->viewport.height * 0.5f + (world.y - c->pan_offset.y) * c->zoom;
    return r;
}

static V2 screen_to_world(SchCanvas *c, V2 screen) {
    V2 r;
    r.x = c->pan_offset.x + (screen.x - c->viewport.x - c->viewport.width * 0.5f) / c->zoom;
    r.y = c->pan_offset.y + (screen.y - c->viewport.y - c->viewport.height * 0.5f) / c->zoom;
    return r;
}

static int lines_intersect(V2 a1, V2 a2, V2 b1, V2 b2, V2 *out) {
    float dax = a2.x - a1.x, day = a2.y - a1.y;
    float dbx = b2.x - b1.x, dby = b2.y - b1.y;
    float cross = dax * dby - day * dbx;
    if (fabsf(cross) < 1e-6f) return 0;
    float t = ((b1.x - a1.x) * dby - (b1.y - a1.y) * dbx) / cross;
    float u = ((b1.x - a1.x) * day - (b1.y - a1.y) * dax) / cross;
    if (t > 0.001f && t < 0.999f && u > 0.001f && u < 0.999f) {
        if (out) { out->x = a1.x + t * dax; out->y = a1.y + t * day; }
        return 1;
    }
    return 0;
}

static void recalc_junctions(SchCanvas *c) {
    int max_junctions = 512;
    if (c->junctions) free(c->junctions);
    c->junctions = NULL;
    c->njunctions = 0;
    if (c->nwires < 2) return;
    c->junctions = (V2 *)malloc((size_t)max_junctions * sizeof(V2));
    for (int i = 0; i < c->nwires && c->njunctions < max_junctions; i++) {
        for (int j = i + 1; j < c->nwires && c->njunctions < max_junctions; j++) {
            V2 pt;
            if (lines_intersect(c->wires[i].start, c->wires[i].end,
                                c->wires[j].start, c->wires[j].end, &pt)) {
                int dup = 0;
                for (int k = 0; k < c->njunctions; k++) {
                    if (fabsf(c->junctions[k].x - pt.x) < 0.5f &&
                        fabsf(c->junctions[k].y - pt.y) < 0.5f) { dup = 1; break; }
                }
                if (!dup) c->junctions[c->njunctions++] = pt;
            }
        }
    }
}

void sch_canvas_init(SchCanvas *c, int x, int y, int w, int h) {
    memset(c, 0, sizeof(*c));
    c->viewport = (Rectangle){ (float)x, (float)y, (float)w, (float)h };
    c->zoom = 1.0f;
    c->pan_offset = (Vector2){ 0, 0 };
    c->grid_on = 1;
    c->grid_size = 1.0f;
    c->tool = TOOL_SELECT;
    c->dragging = -1;
    c->wire_started = 0;
    c->placing = 0;
    c->place_index = -1;
    c->symbols = NULL;
    c->nsymbols = 0;
    c->wires = NULL;
    c->nwires = 0;
    c->junctions = NULL;
    c->njunctions = 0;

    c->npalette = 16;
    const char *names[] = {
        "Resistor", "Capacitor", "Inductor", "Diode",
        "NPN", "PNP", "GND", "VCC",
        "IC", "Connector", "Crystal", "Switch",
        "LED", "Fuse", "OpAmp", "Header"
    };
    const char *values[] = {
        "10k", "100nF", "10uH", "1N4148",
        "2N3904", "2N3906", "GND", "5V",
        "LM358", "CONN", "8MHz", "SW_SPST",
        "LED", "FUSE", "TL072", "HDR-8"
    };
    const char *mfgs[] = {
        "Generic", "Generic", "Generic", "Diodes Inc",
        "OnSemi", "OnSemi", "Net", "Net",
        "TI", "Molex", "ECS", "C&K",
        "Osram", "Littelfuse", "TI", "Samtec"
    };
    for (int i = 0; i < c->npalette; i++) {
        c->palette[i].name = names[i];
        c->palette[i].value = values[i];
        c->palette[i].mfg = mfgs[i];
    }
}

void sch_canvas_set_tool(SchCanvas *c, SchTool t) {
    c->tool = t;
    c->dragging = -1;
    c->wire_started = 0;
    c->placing = 0;
    c->place_index = -1;
}

void sch_canvas_zoom(SchCanvas *c, float delta) {
    c->zoom += delta;
    if (c->zoom < 0.1f) c->zoom = 0.1f;
    if (c->zoom > 10.0f) c->zoom = 10.0f;
}

void sch_canvas_add_symbol(SchCanvas *c, const SchematicSymbol *s) {
    SchematicSymbol *newarr = (SchematicSymbol *)realloc(c->symbols,
        (size_t)(c->nsymbols + 1) * sizeof(SchematicSymbol));
    if (!newarr) return;
    c->symbols = newarr;
    memset(&c->symbols[c->nsymbols], 0, sizeof(SchematicSymbol));
    c->symbols[c->nsymbols].ref = s->ref ? str_dup(s->ref) : NULL;
    c->symbols[c->nsymbols].value = s->value ? str_dup(s->value) : NULL;
    c->symbols[c->nsymbols].symbol = s->symbol ? str_dup(s->symbol) : NULL;
    c->symbols[c->nsymbols].pos = s->pos;
    c->symbols[c->nsymbols].rotation = s->rotation;
    c->symbols[c->nsymbols].mirrored = s->mirrored;
    c->nsymbols++;
}

void sch_canvas_clear(SchCanvas *c) {
    for (int i = 0; i < c->nsymbols; i++) {
        free(c->symbols[i].ref);
        free(c->symbols[i].value);
        free(c->symbols[i].symbol);
    }
    free(c->symbols); c->symbols = NULL; c->nsymbols = 0;
    free(c->wires); c->wires = NULL; c->nwires = 0;
    free(c->junctions); c->junctions = NULL; c->njunctions = 0;
    c->dragging = -1;
    c->wire_started = 0;
    c->placing = 0;
    c->place_index = -1;
}

void sch_canvas_update(SchCanvas *c, Project *p) {
    (void)p;
    Vector2 mp = GetMousePosition();
    float wheel = GetMouseWheelMove();

    if (wheel != 0) {
        if (CheckCollisionPointRec(mp, c->viewport)) {
            c->zoom *= (1.0f + wheel * 0.1f);
            if (c->zoom < 0.1f) c->zoom = 0.1f;
            if (c->zoom > 10.0f) c->zoom = 10.0f;
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) &&
        CheckCollisionPointRec(mp, c->viewport)) {
        c->dragging = -2;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) && c->dragging == -2) {
        Vector2 delta = GetMouseDelta();
        c->pan_offset.x -= delta.x / c->zoom;
        c->pan_offset.y -= delta.y / c->zoom;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {
        c->dragging = -1;
    }

    if (c->tool == TOOL_PLACE) {
        if (c->placing) {
            V2 world = screen_to_world(c, (V2){mp.x, mp.y});
            c->place_pos = c->grid_on ? snap_to_grid(world, c->grid_size) : world;
        }
    }

    if (c->tool == TOOL_WIRE && c->wire_started) {
        V2 world = screen_to_world(c, (V2){mp.x, mp.y});
        c->wire_end_pos = c->grid_on ? snap_to_grid(world, c->grid_size) : world;
    }

    if (!CheckCollisionPointRec(mp, c->viewport)) {
        /* check palette clicks */
        float pal_w = 110.0f;
        Rectangle pal_area = { c->viewport.x, c->viewport.y, pal_w, c->viewport.height };
        if (CheckCollisionPointRec(mp, pal_area)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                float item_h = 28.0f;
                for (int i = 0; i < c->npalette; i++) {
                    Rectangle r = { c->viewport.x + 2, c->viewport.y + 2 + (float)i * item_h,
                                    pal_w - 4, item_h - 2 };
                    if (CheckCollisionPointRec(mp, r)) {
                        c->tool = TOOL_PLACE;
                        c->placing = 1;
                        c->place_index = i;
                        V2 world = screen_to_world(c, (V2){c->viewport.x + pal_w + 30, c->viewport.y + 50});
                        c->place_pos = c->grid_on ? snap_to_grid(world, c->grid_size) : world;
                        break;
                    }
                }
            }
        }
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        if (c->wire_started) {
            c->wire_started = 0;
        }
        if (c->placing) {
            c->placing = 0;
            c->place_index = -1;
        }
        c->dragging = -1;
        c->tool = TOOL_SELECT;
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (c->tool == TOOL_PLACE && c->placing && c->place_index >= 0) {
            SchematicSymbol sym;
            memset(&sym, 0, sizeof(sym));
            sym.pos = c->place_pos;
            sym.rotation = 0;
            sym.mirrored = 0;
            char refbuf[16];
            snprintf(refbuf, sizeof(refbuf), "%c%d",
                c->palette[c->place_index].name[0],
                c->nsymbols + 1);
            sym.ref = refbuf;
            sym.value = (char *)c->palette[c->place_index].value;
            sym.symbol = (char *)c->palette[c->place_index].name;
            sch_canvas_add_symbol(c, &sym);
            c->placing = 0;
            c->place_index = -1;
            return;
        }

        if (c->tool == TOOL_WIRE) {
            if (!c->wire_started) {
                c->wire_started = 1;
                V2 world = screen_to_world(c, (V2){mp.x, mp.y});
                c->wire_start_pos = c->grid_on ? snap_to_grid(world, c->grid_size) : world;
                c->wire_end_pos = c->wire_start_pos;
            } else {
                V2 world = screen_to_world(c, (V2){mp.x, mp.y});
                V2 end_pos = c->grid_on ? snap_to_grid(world, c->grid_size) : world;
                float dx = end_pos.x - c->wire_start_pos.x;
                float dy = end_pos.y - c->wire_start_pos.y;
                if (fabsf(dx) > 0.1f || fabsf(dy) > 0.1f) {
                    /* orthogonal routing: first horizontal, then vertical */
                    V2 mid = { end_pos.x, c->wire_start_pos.y };
                    if (fabsf(dx) > 0.1f && fabsf(dy) > 0.1f) {
                        /* add H then V */
                    void *tmp = realloc(c->wires, (size_t)(c->nwires + 2) * sizeof(c->wires[0]));
                        if (tmp) {
                            c->wires = tmp;
                            c->wires[c->nwires].start = c->wire_start_pos;
                            c->wires[c->nwires].end = mid;
                            c->wires[c->nwires].net_index = 0;
                            c->nwires++;
                            c->wires[c->nwires].start = mid;
                            c->wires[c->nwires].end = end_pos;
                            c->wires[c->nwires].net_index = 0;
                            c->nwires++;
                        }
                    } else {
                        void *tmp = realloc(c->wires, (size_t)(c->nwires + 1) * sizeof(c->wires[0]));
                        if (tmp) {
                            c->wires = tmp;
                            c->wires[c->nwires].start = c->wire_start_pos;
                            c->wires[c->nwires].end = end_pos;
                            c->wires[c->nwires].net_index = 0;
                            c->nwires++;
                        }
                    }
                    recalc_junctions(c);
                    c->wire_start_pos = end_pos;
                }
                c->wire_started = 0;
            }
            return;
        }

        if (c->tool == TOOL_SELECT) {
            float best_dist = 20.0f * 20.0f;
            int best_idx = -1;
            V2 world = screen_to_world(c, (V2){mp.x, mp.y});
            for (int i = 0; i < c->nsymbols; i++) {
                float dx = c->symbols[i].pos.x - world.x;
                float dy = c->symbols[i].pos.y - world.y;
                float d = dx * dx + dy * dy;
                if (d < best_dist) { best_dist = d; best_idx = i; }
            }
            if (best_idx >= 0) {
                c->dragging = best_idx;
                c->drag_offset = v2_sub(c->symbols[best_idx].pos, world);
            } else {
                c->dragging = -1;
            }
            return;
        }

        if (c->tool == TOOL_DELETE) {
            V2 world = screen_to_world(c, (V2){mp.x, mp.y});
            for (int i = 0; i < c->nsymbols; i++) {
                float dx = c->symbols[i].pos.x - world.x;
                float dy = c->symbols[i].pos.y - world.y;
                if (dx * dx + dy * dy < 100.0f) {
                    free(c->symbols[i].ref);
                    free(c->symbols[i].value);
                    free(c->symbols[i].symbol);
                    memmove(&c->symbols[i], &c->symbols[i + 1],
                        (size_t)(c->nsymbols - i - 1) * sizeof(SchematicSymbol));
                    c->nsymbols--;
                    break;
                }
            }
            return;
        }
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && c->dragging >= 0) {
        V2 world = screen_to_world(c, (V2){mp.x, mp.y});
        c->symbols[c->dragging].pos = v2_add(world, c->drag_offset);
        if (c->grid_on) {
            c->symbols[c->dragging].pos = snap_to_grid(c->symbols[c->dragging].pos, c->grid_size);
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && c->dragging >= 0) {
        c->dragging = -1;
    }
}

void sch_canvas_render(SchCanvas *c) {
    float pal_w = 110.0f;
    float vx = c->viewport.x + pal_w;
    float vy = c->viewport.y;
    float vw = c->viewport.width - pal_w;
    float vh = c->viewport.height;
    Rectangle draw_area = { vx, vy, vw, vh };

    /* palette background */
    DrawRectangle((int)c->viewport.x, (int)c->viewport.y, (int)pal_w, (int)c->viewport.height, PAL_BG);
    DrawRectangleLines((int)c->viewport.x, (int)c->viewport.y, (int)pal_w, (int)c->viewport.height, (Color){60,60,66,255});

    /* palette items */
    float item_h = 28.0f;
    for (int i = 0; i < c->npalette; i++) {
        Rectangle r = { c->viewport.x + 2, c->viewport.y + 2 + (float)i * item_h,
                        pal_w - 4, item_h - 2 };
        c->palette[i].rect = r;
        Vector2 mp = GetMousePosition();
        int hover = CheckCollisionPointRec(mp, r);
        Color bg = hover ? PAL_HOVER : PAL_ITEM;
        DrawRectangleRec(r, bg);
        DrawRectangleLinesEx(r, 1, (Color){80,80,86,255});
        DrawText(c->palette[i].name, (int)r.x + 4, (int)r.y + 2, 11, WHITE);
        DrawText(c->palette[i].value, (int)r.x + 4, (int)r.y + 14, 9, GRAY);
    }

    /* canvas background */
    DrawRectangle((int)vx, (int)vy, (int)vw, (int)vh, BG_COLOR);
    DrawRectangleLines((int)vx, (int)vy, (int)vw, (int)vh, (Color){60,60,66,255});

    /* tool indicator */
    const char *tool_names[] = {"SELECT","WIRE","PLACE","LABEL","DELETE"};
    char tool_text[64];
    snprintf(tool_text, sizeof(tool_text), "Tool: %s | Grid: %.0fmm",
        tool_names[c->tool], c->grid_size);
    DrawRectangle((int)vx, (int)vy, (int)vw, 22, TOOL_BG);
    DrawText(tool_text, (int)vx + 8, (int)vy + 4, 11, (Color){180,180,190,255});

    /* grid */
    if (c->grid_on) {
        float gs = c->grid_size * c->zoom;
        if (gs >= 4.0f) {
            float ox = fmodf(c->pan_offset.x, c->grid_size) * c->zoom;
            float oy = fmodf(c->pan_offset.y, c->grid_size) * c->zoom;
            for (float gx = -ox; gx < vw; gx += gs) {
                if (gx >= 0) DrawLine((int)(vx + gx), (int)vy, (int)(vx + gx), (int)(vy + vh), GRID_COLOR);
            }
            for (float gy = -oy; gy < vh; gy += gs) {
                if (gy >= 0) DrawLine((int)vx, (int)(vy + gy), (int)(vx + vw), (int)(vy + gy), GRID_COLOR);
            }
        }
    }

    /* wires */
    for (int i = 0; i < c->nwires; i++) {
        V2 a = world_to_screen(c, c->wires[i].start);
        V2 b = world_to_screen(c, c->wires[i].end);
        if (a.x >= vx || b.x >= vx) {
            DrawLineEx((Vector2){a.x, a.y}, (Vector2){b.x, b.y}, 2.0f, WIRE_COLOR);
        }
    }

    /* junction dots */
    for (int i = 0; i < c->njunctions; i++) {
        V2 jp = world_to_screen(c, c->junctions[i]);
        DrawCircleV((Vector2){jp.x, jp.y}, 5.0f, JUNC_COLOR);
    }

    /* symbols */
    for (int i = 0; i < c->nsymbols; i++) {
        SchematicSymbol *si = &c->symbols[i];
        V2 sc = world_to_screen(c, si->pos);
        float rw = 30.0f * c->zoom, rh = 20.0f * c->zoom;
        int selected = (c->dragging == i);
        Color border = selected ? SEL_COLOR : (Color){100,180,255,255};
        Color fill = selected ? (Color){50,45,15,255} : (Color){35,35,40,255};
        Rectangle body = { sc.x - rw, sc.y - rh, rw * 2, rh * 2 };
        DrawRectangleRec(body, fill);
        DrawRectangleLinesEx(body, 1.5f, border);
        if (si->ref) {
            int fs = (int)(11.0f * c->zoom);
            if (fs < 8) fs = 8;
            DrawText(si->ref, (int)(sc.x - rw + 3), (int)(sc.y - rh + 1), fs, WHITE);
        }
        if (si->value && si->value[0]) {
            int fs = (int)(9.0f * c->zoom);
            if (fs < 6) fs = 6;
            DrawText(si->value, (int)(sc.x - rw + 3), (int)(sc.y + rh - (float)fs - 2), fs, GRAY);
        }
        /* pin dots left + right */
        for (int p = 0; p < 4; p++) {
            float py_off = sc.y - rh + ((float)(p * 2 + 1) / 8.0f) * (rh * 2);
            float r = 3.0f * c->zoom;
            if (r < 1.5f) r = 1.5f;
            DrawCircle((int)(sc.x - rw), (int)py_off, r, GREEN);
            DrawCircle((int)(sc.x + rw), (int)py_off, r, RED);
        }
    }

    /* placing preview */
    if (c->placing && c->place_index >= 0) {
        V2 sc = world_to_screen(c, c->place_pos);
        float rw = 30.0f * c->zoom, rh = 20.0f * c->zoom;
        DrawRectangleLinesEx((Rectangle){sc.x - rw, sc.y - rh, rw * 2, rh * 2}, 1.5f,
            (Color){255,255,255,120});
        DrawText(c->palette[c->place_index].name,
            (int)(sc.x - rw + 3), (int)(sc.y - 6), 12, (Color){255,255,255,150});
    }

    /* wire preview */
    if (c->tool == TOOL_WIRE && c->wire_started) {
        V2 a = world_to_screen(c, c->wire_start_pos);
        V2 b = world_to_screen(c, c->wire_end_pos);
        DrawLineEx((Vector2){a.x, a.y}, (Vector2){b.x, b.y}, 2.0f, (Color){255,200,100,180});
        DrawCircleV((Vector2){a.x, a.y}, 4.0f, (Color){255,200,100,255});
    }
}
