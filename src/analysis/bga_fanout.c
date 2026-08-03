#include "bga_fanout.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* ---------- helpers ---------- */

static float dist_v2(V2 a, V2 b) {
    float dx = a.x - b.x, dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

static int vias_overlap(V2 a, V2 b, float min_dist) {
    return dist_v2(a, b) < min_dist;
}

static int via_near_ball(V2 via, V2 ball, float ball_r, float via_r) {
    return dist_v2(via, ball) < (ball_r + via_r) * 0.9f;
}

/* ---------- lifecycle ---------- */

void bga_fanout_init(BgaFanout *fo) {
    memset(fo, 0, sizeof(*fo));
}

void bga_fanout_free(BgaFanout *fo) {
    free(fo->via_positions); fo->via_positions = NULL;
    free(fo->trace_points);  fo->trace_points  = NULL;
    fo->nvias = 0;
    fo->ntraces = 0;
}

/* ---------- fanout generation ---------- */

static V2 ball_pos(const BgaPackage *pkg, int r, int c, V2 origin) {
    float x = origin.x + (float)c * pkg->pitch;
    float y = origin.y - (float)r * pkg->pitch;
    return v2(x, y);
}

static V2 escape_offset(const BgaPackage *pkg, int r, int c, int tr, int tc) {
    float p = pkg->pitch;
    float s = p * 0.65f;

    int r0 = (r == 0);
    int r1 = (r == tr - 1);
    int c0 = (c == 0);
    int c1 = (c == tc - 1);

    /* outer ring: escape outward */
    if (r0) return v2(0.0f, s);
    if (r1) return v2(0.0f, -s);
    if (c0) return v2(-s, 0.0f);
    if (c1) return v2(s, 0.0f);

    /* second ring: escape inward toward center */
    if (r == 1)      return v2(0.0f, -s);
    if (r == tr - 2) return v2(0.0f, s);
    if (c == 1)      return v2(s, 0.0f);
    if (c == tc - 2) return v2(-s, 0.0f);

    /* third ring: perpendicular escape */
    if (r == 2)      return v2(s, 0.0f);
    if (r == tr - 3) return v2(-s, 0.0f);
    if (c == 2)      return v2(0.0f, -s);
    if (c == tc - 3) return v2(0.0f, s);

    /* inner: diagonal tiered */
    float row_mid = (float)(tr - 1) * 0.5f;
    float col_mid = (float)(tc - 1) * 0.5f;
    float dr = (float)r - row_mid;
    float dc = (float)c - col_mid;
    float angle = atan2f(-dr, dc);
    float dist = p * 0.7f + fabsf(dr) * 0.15f + fabsf(dc) * 0.15f;
    return v2(cosf(angle) * dist, sinf(angle) * dist);
}

static int is_via_collision(const BgaFanout *fo, V2 via, float min_dist) {
    for (int i = 0; i < fo->nvias; i++) {
        if (vias_overlap(via, fo->via_positions[i], min_dist)) return 1;
    }
    return 0;
}

BgaFanout bga_generate_fanout(const BgaPackage *pkg, V2 origin, int start_row, int start_col) {
    BgaFanout fo;
    bga_fanout_init(&fo);

    if (!pkg || pkg->rows < 1 || pkg->cols < 1 || pkg->pitch <= 0.0f) {
        fo.success = 0;
        return fo;
    }

    fo.balls_total = pkg->rows * pkg->cols;
    float via_clearance = pkg->via_pad * 1.5f;
    float ball_radius = pkg->ball_dia * 0.5f;
    float via_radius = pkg->via_pad * 0.5f;

    int cap = fo.balls_total;
    fo.via_positions = (V2 *)malloc((size_t)cap * sizeof(V2));
    fo.trace_points  = (V2 *)malloc((size_t)cap * 2 * sizeof(V2));

    int routed = 0;
    for (int r = start_row; r < pkg->rows; r++) {
        for (int c = start_col; c < pkg->cols; c++) {
            V2 ball = ball_pos(pkg, r, c, origin);
            V2 off = escape_offset(pkg, r, c, pkg->rows, pkg->cols);
            V2 via = v2(ball.x + off.x, ball.y + off.y);

            /* try alternate offsets if collision */
            if (is_via_collision(&fo, via, via_clearance) ||
                via_near_ball(via, ball, ball_radius, via_radius)) {
                int found_alt = 0;
                V2 alts[8] = {
                    v2(ball.x + pkg->pitch * 0.6f, ball.y),
                    v2(ball.x - pkg->pitch * 0.6f, ball.y),
                    v2(ball.x, ball.y + pkg->pitch * 0.6f),
                    v2(ball.x, ball.y - pkg->pitch * 0.6f),
                    v2(ball.x + pkg->pitch * 0.45f, ball.y + pkg->pitch * 0.45f),
                    v2(ball.x - pkg->pitch * 0.45f, ball.y + pkg->pitch * 0.45f),
                    v2(ball.x + pkg->pitch * 0.45f, ball.y - pkg->pitch * 0.45f),
                    v2(ball.x - pkg->pitch * 0.45f, ball.y - pkg->pitch * 0.45f),
                };
                for (int a = 0; a < 8; a++) {
                    if (!is_via_collision(&fo, alts[a], via_clearance) &&
                        !via_near_ball(alts[a], ball, ball_radius, via_radius)) {
                        via = alts[a];
                        found_alt = 1;
                        break;
                    }
                }
                if (!found_alt) continue; /* skip this ball */
            }

            fo.via_positions[fo.nvias++] = via;
            fo.trace_points[fo.ntraces * 2 + 0] = ball;
            fo.trace_points[fo.ntraces * 2 + 1] = via;
            fo.ntraces++;
            routed++;
        }
    }

    fo.balls_routed = routed;
    fo.success = (routed == fo.balls_total) ? 1 : 0;
    return fo;
}

/* ---------- apply fanout to PCB ---------- */

void bga_fanout_to_pcb(const BgaFanout *fo, Pcb *pcb, const char *net_prefix) {
    if (!fo || !pcb || fo->nvias == 0) return;

    const char *prefix = net_prefix ? net_prefix : "BGA";

    for (int i = 0; i < fo->nvias; i++) {
        /* generate unique net name per ball */
        char net_buf[128];
        snprintf(net_buf, sizeof(net_buf), "%s_%d", prefix, i + 1);
        char *net_name = str_dup(net_buf);

        /* add via */
        pcb->vias = (Via *)realloc(pcb->vias, (size_t)(pcb->nvias + 1) * sizeof(Via));
        Via *v = &pcb->vias[pcb->nvias];
        v->pos = fo->via_positions[i];
        v->drill = 0.3f;
        v->outer = 0.6f;
        v->net = net_name;
        pcb->nvias++;

        /* add track from ball to via */
        if (i < fo->ntraces) {
            V2 start = fo->trace_points[i * 2 + 0];
            V2 end   = fo->trace_points[i * 2 + 1];
            float w = 0.2f;
            if (dist_v2(start, end) > 0.01f) {
                pcb->tracks = (Track *)realloc(pcb->tracks,
                    (size_t)(pcb->ntracks + 1) * sizeof(Track));
                Track *t = &pcb->tracks[pcb->ntracks];
                t->a = start;
                t->b = end;
                t->width = w;
                t->net = str_dup(net_buf);
                pcb->ntracks++;
            }
        }
    }
}
