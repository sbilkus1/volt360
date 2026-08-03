#include "cross_probe.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

void crossprobe_init(CrossProbe *cp) { memset(cp, 0, sizeof(*cp)); }

void crossprobe_select_schematic(CrossProbe *cp, const Schematic *s, const char *ref) {
    if (!cp || !s || !ref) return; cp->active = 1; cp->source_type = 1;
    snprintf(cp->ref, 32, "%s", ref); cp->flash_timer = 0;
    for (int i = 0; i < s->ninsts; i++)
        if (s->insts[i].ref && strcmp(s->insts[i].ref, ref) == 0) { cp->sch_symbol_idx = i; break; }
}

void crossprobe_select_pcb(CrossProbe *cp, const Pcb *p, const char *ref) {
    if (!cp || !p || !ref) return; cp->active = 1; cp->source_type = 2;
    snprintf(cp->ref, 32, "%s", ref); cp->flash_timer = 0; cp->pcb_footprint_idx = -1;
    for (int i = 0; i < p->nfps; i++)
        if (p->fps[i].ref && strcmp(p->fps[i].ref, ref) == 0) { cp->pcb_footprint_idx = i; break; }
    cp->nhigh_tracks = 0;
    for (int i = 0; i < p->ntracks; i++)
        if (p->tracks[i].net && strstr(p->tracks[i].net, ref)) cp->nhigh_tracks++;
}

void crossprobe_clear(CrossProbe *cp) { if (!cp) return; cp->active = 0; }

void crossprobe_render_schematic_highlight(const CrossProbe *cp, const Schematic *s, Rectangle vp, Vector2 pan, float zoom) {
    if (!cp || !cp->active || !s) return; (void)vp; (void)pan; (void)zoom;
}

void crossprobe_render_pcb_highlight(CrossProbe *cp, const Pcb *p, Rectangle vp, Vector2 pan, float zoom) {
    if (!cp || !cp->active || !p) return;
    CrossProbe *mp = (CrossProbe *)cp;
    mp->flash_timer += 0.016f;
    float alpha = 100.0f + 155.0f * (0.5f + 0.5f * sinf(mp->flash_timer * 10.0f));
    Color glow = { 255, 200, 0, (unsigned char)alpha };
    if (cp->pcb_footprint_idx >= 0 && cp->pcb_footprint_idx < p->nfps) {
        FpInst *fp = &p->fps[cp->pcb_footprint_idx];
        float sx = (fp->pos.x - pan.x) * zoom + vp.x;
        float sy = (fp->pos.y - pan.y) * zoom + vp.y;
        float sz = 3.0f * zoom;
        DrawRectangleLinesEx((Rectangle){sx - sz/2, sy - sz/2, sz, sz}, 2.0f, glow);
        DrawText(cp->ref, (int)sx + (int)sz, (int)sy - 10, 10, glow);
    }
}

int crossprobe_find_at_pos(const Pcb *p, V2 pos, float tolerance) {
    if (!p) return -1;
    for (int i = 0; i < p->nfps; i++) {
        float dx = p->fps[i].pos.x - pos.x; float dy = p->fps[i].pos.y - pos.y;
        if (sqrtf(dx*dx+dy*dy) < tolerance) return i;
    }
    return -1;
}
