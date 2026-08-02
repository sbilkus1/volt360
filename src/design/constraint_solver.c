#include "constraint_solver.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

static float cs_min(float a, float b) { return a < b ? a : b; }
static float cs_max(float a, float b) { return a > b ? a : b; }
static float cs_abs(float a) { return a < 0 ? -a : a; }

void cs_init(ConstraintGraph *g) {
    memset(g, 0, sizeof(*g));
}

int cs_add_point(ConstraintGraph *g, V2 pos, int fix) {
    if (g->nentities >= CS_MAX_ENTITIES) return -1;
    g->entities[g->nentities].type = CS_POINT;
    g->entities[g->nentities].p1 = pos;
    g->entities[g->nentities].p2 = pos;
    g->entities[g->nentities].radius = 0;
    g->entities[g->nentities].fixed = fix;
    return g->nentities++;
}

int cs_add_line(ConstraintGraph *g, V2 p1, V2 p2) {
    if (g->nentities >= CS_MAX_ENTITIES) return -1;
    g->entities[g->nentities].type = CS_LINE;
    g->entities[g->nentities].p1 = p1;
    g->entities[g->nentities].p2 = p2;
    g->entities[g->nentities].radius = 0;
    g->entities[g->nentities].fixed = 0;
    return g->nentities++;
}

int cs_add_circle(ConstraintGraph *g, V2 center, float radius) {
    if (g->nentities >= CS_MAX_ENTITIES) return -1;
    g->entities[g->nentities].type = CS_CIRCLE;
    g->entities[g->nentities].p1 = center;
    g->entities[g->nentities].p2 = center;
    g->entities[g->nentities].radius = radius;
    g->entities[g->nentities].fixed = 0;
    return g->nentities++;
}

static int cs_add_constraint(ConstraintGraph *g, int type, int e1, int e2, float value) {
    if (g->nconstraints >= CS_MAX_CONSTRAINTS) return -1;
    if (e1 < 0 || e1 >= g->nentities) return -1;
    if (e2 < -1 || e2 >= g->nentities) return -1;
    g->constraints[g->nconstraints].type = type;
    g->constraints[g->nconstraints].e1 = e1;
    g->constraints[g->nconstraints].e2 = e2;
    g->constraints[g->nconstraints].value = value;
    return g->nconstraints++;
}

int cs_add_constraint_horizontal(ConstraintGraph *g, int line_id) {
    return cs_add_constraint(g, CS_HORIZONTAL, line_id, -1, 0);
}
int cs_add_constraint_vertical(ConstraintGraph *g, int line_id) {
    return cs_add_constraint(g, CS_VERTICAL, line_id, -1, 0);
}
int cs_add_constraint_coincident(ConstraintGraph *g, int e1, int e2) {
    return cs_add_constraint(g, CS_COINCIDENT, e1, e2, 0);
}
int cs_add_constraint_parallel(ConstraintGraph *g, int line1, int line2) {
    return cs_add_constraint(g, CS_PARALLEL, line1, line2, 0);
}
int cs_add_constraint_perpendicular(ConstraintGraph *g, int line1, int line2) {
    return cs_add_constraint(g, CS_PERPENDICULAR, line1, line2, 0);
}
int cs_add_constraint_tangent(ConstraintGraph *g, int line_id, int circle_id) {
    return cs_add_constraint(g, CS_TANGENT, line_id, circle_id, 0);
}
int cs_add_constraint_equal_length(ConstraintGraph *g, int line1, int line2) {
    return cs_add_constraint(g, CS_EQUAL_LENGTH, line1, line2, 0);
}
int cs_add_constraint_distance(ConstraintGraph *g, int e1, int e2, float dist) {
    return cs_add_constraint(g, CS_FIXED_DISTANCE, e1, e2, dist);
}
int cs_add_constraint_angle(ConstraintGraph *g, int line1, int line2, float angle_deg) {
    return cs_add_constraint(g, CS_FIXED_ANGLE, line1, line2, angle_deg);
}

static V2 cs_line_dir(CSEntity *e) {
    float dx = e->p2.x - e->p1.x;
    float dy = e->p2.y - e->p1.y;
    float len = sqrtf(dx * dx + dy * dy);
    if (len < 1e-6f) return v2(1, 0);
    return v2(dx / len, dy / len);
}

static float cs_line_len(CSEntity *e) {
    float dx = e->p2.x - e->p1.x;
    float dy = e->p2.y - e->p1.y;
    return sqrtf(dx * dx + dy * dy);
}

static void cs_set_line_dir_len(CSEntity *e, V2 dir, float len) {
    V2 mid = v2((e->p1.x + e->p2.x) * 0.5f, (e->p1.y + e->p2.y) * 0.5f);
    float half = len * 0.5f;
    e->p1 = v2(mid.x - dir.x * half, mid.y - dir.y * half);
    e->p2 = v2(mid.x + dir.x * half, mid.y + dir.y * half);
}

static void cs_move_point(CSEntity *e, V2 delta) {
    if (e->type == CS_POINT) {
        e->p1 = v2(e->p1.x + delta.x, e->p1.y + delta.y);
        e->p2 = e->p1;
    } else if (e->type == CS_CIRCLE) {
        e->p1 = v2(e->p1.x + delta.x, e->p1.y + delta.y);
        e->p2 = e->p1;
    } else if (e->type == CS_LINE) {
        e->p1 = v2(e->p1.x + delta.x, e->p1.y + delta.y);
        e->p2 = v2(e->p2.x + delta.x, e->p2.y + delta.y);
    }
}

static V2 cs_center(CSEntity *e) {
    if (e->type == CS_LINE) return v2((e->p1.x + e->p2.x) * 0.5f, (e->p1.y + e->p2.y) * 0.5f);
    return e->p1;
}

static int apply_constraint(ConstraintGraph *g, CSConstraint *c) {
    CSEntity *e1, *e2;
    float eps = 0.5f;

    e1 = (c->e1 >= 0) ? &g->entities[c->e1] : NULL;
    e2 = (c->e2 >= 0) ? &g->entities[c->e2] : NULL;

    if (!e1) return 0;
    if (e1->fixed) return 0;

    switch (c->type) {
    case CS_HORIZONTAL:
        if (e1->type == CS_LINE) {
            float mid_y = (e1->p1.y + e1->p2.y) * 0.5f;
            e1->p1.y = mid_y;
            e1->p2.y = mid_y;
            return 1;
        }
        return 0;

    case CS_VERTICAL:
        if (e1->type == CS_LINE) {
            float mid_x = (e1->p1.x + e1->p2.x) * 0.5f;
            e1->p1.x = mid_x;
            e1->p2.x = mid_x;
            return 1;
        }
        return 0;

    case CS_COINCIDENT:
        if (!e2) return 0;
        {
            V2 c1 = cs_center(e1);
            V2 c2 = cs_center(e2);
            V2 delta = v2(c2.x - c1.x, c2.y - c1.y);
            float d = sqrtf(delta.x * delta.x + delta.y * delta.y);
            if (d > 1e-6f && d < 100.0f) {
                if (!e1->fixed) cs_move_point(e1, delta);
                else if (!e2->fixed) cs_move_point(e2, v2(-delta.x, -delta.y));
            }
            return 1;
        }

    case CS_PARALLEL:
        if (!e2 || e1->type != CS_LINE || e2->type != CS_LINE) return 0;
        {
            V2 d2 = cs_line_dir(e2);
            float l1 = cs_line_len(e1);
            if (!e1->fixed) cs_set_line_dir_len(e1, d2, l1);
            return 1;
        }

    case CS_PERPENDICULAR:
        if (!e2 || e1->type != CS_LINE || e2->type != CS_LINE) return 0;
        {
            V2 d2 = cs_line_dir(e2);
            V2 perp = v2(-d2.y, d2.x);
            float l1 = cs_line_len(e1);
            if (!e1->fixed) cs_set_line_dir_len(e1, perp, l1);
            return 1;
        }

    case CS_TANGENT:
        if (!e2 || e1->type != CS_LINE || e2->type != CS_CIRCLE) return 0;
        {
            V2 dir = cs_line_dir(e1);
            V2 perp = v2(-dir.y, dir.x);
            float r = e2->radius;
            V2 mid = cs_center(e1);
            V2 cc = v2(e2->p1.x + perp.x * r, e2->p1.y + perp.y * r);
            V2 delta = v2(cc.x - mid.x, cc.y - mid.y);
            if (!e1->fixed) cs_move_point(e1, delta);
            else if (!e2->fixed) cs_move_point(e2, v2(-delta.x, -delta.y));
            return 1;
        }

    case CS_EQUAL_LENGTH:
        if (!e2 || e1->type != CS_LINE || e2->type != CS_LINE) return 0;
        {
            float l2 = cs_line_len(e2);
            V2 dir = cs_line_dir(e1);
            if (!e1->fixed) cs_set_line_dir_len(e1, dir, l2);
            else if (!e2->fixed) { V2 d2 = cs_line_dir(e2); cs_set_line_dir_len(e2, d2, cs_line_len(e1)); }
            return 1;
        }

    case CS_FIXED_DISTANCE:
        if (!e2) return 0;
        {
            V2 c1 = cs_center(e1);
            V2 c2 = cs_center(e2);
            V2 delta = v2(c2.x - c1.x, c2.y - c1.y);
            float d = sqrtf(delta.x * delta.x + delta.y * delta.y);
            float target = c->value;
            if (d < 1e-6f) d = 1.0f;
            if (target < 1e-6f) target = 1.0f;
            {
                float scale = target / d;
                V2 adj = v2(delta.x * (scale - 1.0f) * 0.5f, delta.y * (scale - 1.0f) * 0.5f);
                if (!e1->fixed) cs_move_point(e1, adj);
                if (!e2->fixed) cs_move_point(e2, v2(-adj.x, -adj.y));
            }
            return 1;
        }

    case CS_FIXED_ANGLE:
        if (!e2 || e1->type != CS_LINE || e2->type != CS_LINE) return 0;
        {
            V2 d2 = cs_line_dir(e2);
            float target_rad = c->value * (float)M_PI / 180.0f;
            V2 rot_dir = v2(d2.x * cosf(target_rad) - d2.y * sinf(target_rad),
                           d2.x * sinf(target_rad) + d2.y * cosf(target_rad));
            float l1 = cs_line_len(e1);
            if (!e1->fixed) cs_set_line_dir_len(e1, rot_dir, l1);
            else if (!e2->fixed) {
                V2 inv = v2(d2.x * cosf(-target_rad) - d2.y * sinf(-target_rad),
                           d2.x * sinf(-target_rad) + d2.y * cosf(-target_rad));
                cs_set_line_dir_len(e2, inv, cs_line_len(e2));
            }
            return 1;
        }

    case CS_FIX_POINT:
        e1->fixed = 1;
        return 1;

    default:
        return 0;
    }
}

bool constraint_solve(ConstraintGraph *g, int max_iter, float tolerance) {
    int iter, ci, any_change;
    int max_it;
    float tol;

    (void)tolerance;
    max_it = (max_iter > 0) ? max_iter : 100;
    tol = tolerance > 0 ? tolerance : 0.001f;

    for (iter = 0; iter < max_it; iter++) {
        any_change = 0;
        for (ci = 0; ci < g->nconstraints; ci++) {
            if (apply_constraint(g, &g->constraints[ci])) any_change = 1;
        }
        if (!any_change) break;
    }
    return (iter < max_it);
}
