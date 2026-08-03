#include "test_points.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

void testpoint_config_default(TestPointConfig *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    cfg->pad_dia = 1.5f;
    cfg->drill_dia = 0.8f;
    cfg->min_spacing = 2.5f;
    cfg->edge_margin = 3.0f;
    cfg->max_per_net = 1;
    cfg->only_unconnected = 1;
}

static int net_in_list(const char **list, int n, const char *net) {
    if (!net || !net[0]) return 0;
    for (int i = 0; i < n; i++) { if (list[i] && strcmp(list[i], net) == 0) return 1; }
    return 0;
}

static int is_tp_via(const Via *via, const TestPointConfig *cfg, const char *net) {
    if (!via->net || strcmp(via->net, net) != 0) return 0;
    if (via->name && str_starts_with(via->name, "TP_")) return 1;
    if (fabsf(via->outer - cfg->pad_dia) < 0.01f && fabsf(via->drill - cfg->drill_dia) < 0.01f) return 1;
    return 0;
}

static int net_has_tp(const Pcb *pcb, const char *net, const TestPointConfig *cfg) {
    for (int i = 0; i < pcb->nvias; i++) {
        if (is_tp_via(&pcb->vias[i], cfg, net)) return 1;
    }
    return 0;
}

static int pos_under_component(const Pcb *pcb, V2 pos) {
    for (int i = 0; i < pcb->nfps; i++) {
        float dx = pos.x - pcb->fps[i].pos.x;
        float dy = pos.y - pcb->fps[i].pos.y;
        if (fabsf(dx) < 5.0f && fabsf(dy) < 5.0f) return 1;
    }
    return 0;
}

static int pos_near_tp(const Pcb *pcb, V2 pos, float min_spacing) {
    for (int i = 0; i < pcb->nvias; i++) {
        if (v2_dist(pos, pcb->vias[i].pos) < min_spacing) return 1;
    }
    return 0;
}

static int pos_on_board(const Pcb *pcb, V2 pos, float margin) {
    if (!pcb->has_bbox) return 1;
    if (pos.x < pcb->bmin.x + margin || pos.x > pcb->bmax.x - margin) return 0;
    if (pos.y < pcb->bmin.y + margin || pos.y > pcb->bmax.y - margin) return 0;
    return 1;
}

TestPointResult testpoint_insert(Pcb *pcb, const TestPointConfig *cfg) {
    TestPointResult r;
    memset(&r, 0, sizeof(r));
    if (!pcb || pcb->noutline < 3) return r;

    if (!pcb->has_bbox) pcb_bbox(pcb);

    /* collect unique nets from tracks */
    const char **nets = NULL;
    int nnets = 0;
    for (int i = 0; i < pcb->ntracks; i++) {
        const char *net = pcb->tracks[i].net;
        if (!net || !net[0]) continue;
        if (net_in_list(nets, nnets, net)) continue;
        const char **n2 = (const char **)realloc(nets, sizeof(char *) * (size_t)(nnets + 1));
        if (!n2) continue;
        nets = n2;
        nets[nnets++] = net;
    }

    /* grid search parameters */
    float bx0 = pcb->bmin.x + cfg->edge_margin;
    float by0 = pcb->bmin.y + cfg->edge_margin;
    float bx1 = pcb->bmax.x - cfg->edge_margin;
    float by1 = pcb->bmax.y - cfg->edge_margin;
    if (bx1 <= bx0 || by1 <= by0) { free(nets); return r; }

    float step = cfg->min_spacing * 1.5f;
    int gx = (int)((bx1 - bx0) / step);
    int gy = (int)((by1 - by0) / step);
    if (gx < 1) gx = 1; if (gy < 1) gy = 1;

    for (int ni = 0; ni < nnets; ni++) {
        if (cfg->only_unconnected && net_has_tp(pcb, nets[ni], cfg)) continue;

        int placed = 0;
        /* preference: edges first, then scan inward */
        for (int pass = 0; pass < 2 && !placed; pass++) {
            int x_range[2], x_rev[2];
            int y_range[2], y_rev[2];
            if (pass == 0) {
                /* pass 0: edges */
                x_range[0] = 0; x_range[1] = gx; x_rev[0] = 0; x_rev[1] = 0;
                y_range[0] = 0; y_range[1] = gy; y_rev[0] = 0; y_rev[1] = 0;
            } else {
                x_range[0] = 1; x_range[1] = gx - 1; x_rev[0] = 0; x_rev[1] = 0;
                y_range[0] = 1; y_range[1] = gy - 1; y_rev[0] = 0; y_rev[1] = 0;
            }

            for (int ix = x_range[0]; ix < x_range[1] && !placed; ix++) {
                for (int iy = y_range[0]; iy < y_range[1] && !placed; iy++) {
                    V2 pos = v2(bx0 + ix * step, by0 + iy * step);
                    if (!pos_on_board(pcb, pos, cfg->edge_margin)) continue;
                    if (pos_near_tp(pcb, pos, cfg->min_spacing)) continue;
                    if (pos_under_component(pcb, pos)) continue;

                    /* add via as test point */
                    int old_nvias = pcb->nvias;
                    Via *v2 = (Via *)realloc(pcb->vias, sizeof(Via) * (size_t)(pcb->nvias + 1));
                    if (!v2) continue;
                    pcb->vias = v2;
                    memset(&pcb->vias[pcb->nvias], 0, sizeof(Via));
                    pcb->vias[pcb->nvias].pos = pos;
                    pcb->vias[pcb->nvias].drill = cfg->drill_dia;
                    pcb->vias[pcb->nvias].outer = cfg->pad_dia;
                    pcb->vias[pcb->nvias].net = str_dup(nets[ni]);
                    char tpname[64];
                    snprintf(tpname, sizeof(tpname), "TP_%s", nets[ni]);
                    pcb->vias[pcb->nvias].name = str_dup(tpname);
                    pcb->nvias = old_nvias + 1;

                    int n = r.npoints;
                    V2 *pp = (V2 *)realloc(r.positions, sizeof(V2) * (size_t)(n + 1));
                    char **nn = (char **)realloc(r.net_names, sizeof(char *) * (size_t)(n + 1));
                    int *vi = (int *)realloc(r.via_indices, sizeof(int) * (size_t)(n + 1));
                    if (pp && nn && vi) {
                        r.positions = pp;
                        r.net_names = nn;
                        r.via_indices = vi;
                        r.positions[n] = pos;
                        r.net_names[n] = str_dup(nets[ni]);
                        r.via_indices[n] = old_nvias;
                        r.npoints = n + 1;
                        r.added++;
                    }
                    placed = 1;
                }
            }
        }
    }

    free(nets);
    return r;
}

void testpoint_result_free(TestPointResult *r) {
    free(r->positions);
    for (int i = 0; i < r->npoints; i++) free(r->net_names[i]);
    free(r->net_names);
    free(r->via_indices);
    memset(r, 0, sizeof(*r));
}

char *testpoint_report(const TestPointResult *r) {
    Buf b;
    buf_init(&b);
    if (r->added == 0) {
        buf_append_str(&b, "No test points added.");
    } else {
        /* count by class */
        int power = 0, gnd = 0, signal = 0, other = 0;
        for (int i = 0; i < r->npoints; i++) {
            if (!r->net_names[i]) continue;
            /* check if net name hints at power/ground */
            const char *n = r->net_names[i];
            if (str_starts_with(n, "GND") || str_starts_with(n, "VSS") || str_starts_with(n, "AGND") || str_starts_with(n, "DGND") || str_starts_with(n, "PGND"))
                gnd++;
            else if (str_starts_with(n, "VCC") || str_starts_with(n, "VDD") || str_starts_with(n, "3V3") || str_starts_with(n, "5V") || str_starts_with(n, "VIN") || str_starts_with(n, "VBAT") || str_starts_with(n, "PWR") || str_starts_with(n, "VREF") || str_starts_with(n, "VCCINT") || str_starts_with(n, "VCORE"))
                power++;
            else if (str_starts_with(n, "CLK") || str_starts_with(n, "SCK") || str_starts_with(n, "RF") || str_starts_with(n, "ADC") || str_starts_with(n, "TX") || str_starts_with(n, "RX"))
                other++;
            else
                signal++;
        }
        int existing = r->npoints - r->added;
        buf_appendf(&b, "Added %d test points: ", r->added);
        if (power > 0) buf_appendf(&b, "%d on power nets, ", power);
        if (gnd > 0) buf_appendf(&b, "%d on ground nets, ", gnd);
        if (signal > 0) buf_appendf(&b, "%d on signal nets, ", signal);
        if (other > 0) buf_appendf(&b, "%d on special nets, ", other);
        if (existing > 0) buf_appendf(&b, "%d nets had existing test points.", existing);
        else buf_append_str(&b, "0 nets had existing test points.");
    }
    return buf_cstr(&b);
}

void testpoint_remove_all(Pcb *pcb) {
    if (!pcb) return;
    int wr = 0;
    for (int i = 0; i < pcb->nvias; i++) {
        if (pcb->vias[i].name && str_starts_with(pcb->vias[i].name, "TP_")) {
            free(pcb->vias[i].net);
            free(pcb->vias[i].name);
            /* skip this via (don't copy to wr) */
        } else {
            if (wr != i) pcb->vias[wr] = pcb->vias[i];
            wr++;
        }
    }
    pcb->nvias = wr;
}
