#pragma once
#include "../model/project.h"
#include "../../raylib/src/raylib.h"

typedef struct {
    char ref[32];
    int source_type;          // 0=none, 1=from_schematic, 2=from_pcb
    float flash_timer;
    int active;
    int sch_symbol_idx;
    int pcb_footprint_idx;
    int *highlighted_tracks;
    int nhigh_tracks;
} CrossProbe;

void crossprobe_init(CrossProbe *cp);
void crossprobe_select_schematic(CrossProbe *cp, const Schematic *s, const char *ref);
void crossprobe_select_pcb(CrossProbe *cp, const Pcb *p, const char *ref);
void crossprobe_clear(CrossProbe *cp);
void crossprobe_render_schematic_highlight(const CrossProbe *cp, const Schematic *s, Rectangle vp, Vector2 pan, float zoom);
void crossprobe_render_pcb_highlight(const CrossProbe *cp, const Pcb *p, Rectangle vp, Vector2 pan, float zoom);
int crossprobe_find_at_pos(const Pcb *p, V2 pos, float tolerance);
