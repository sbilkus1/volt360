#include "sketcher.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

Sketch *sketch_create(const char *name) {
    Sketch *sk = (Sketch *)calloc(1, sizeof(Sketch));
    snprintf(sk->name, sizeof(sk->name), "%s", name ? name : "sketch");
    return sk;
}

void sketch_free(Sketch *sk) { if (sk) { free(sk->entities); free(sk->constraints); free(sk); } }

int sketch_add_point(Sketch *sk, V2 pos) {
    if (!sk) return -1;
    sk->entities = (SkEntity *)realloc(sk->entities, sizeof(SkEntity)*(size_t)(sk->n_entities+1));
    SkEntity *e = &sk->entities[sk->n_entities];
    memset(e, 0, sizeof(*e)); e->type = SK_POINT; e->id = sk->n_entities; e->p1 = pos;
    return sk->n_entities++;
}

int sketch_add_line(Sketch *sk, V2 p1, V2 p2) {
    if (!sk) return -1;
    sk->entities = (SkEntity *)realloc(sk->entities, sizeof(SkEntity)*(size_t)(sk->n_entities+1));
    SkEntity *e = &sk->entities[sk->n_entities];
    memset(e, 0, sizeof(*e)); e->type = SK_LINE; e->id = sk->n_entities; e->p1 = p1; e->p2 = p2;
    return sk->n_entities++;
}

int sketch_add_circle(Sketch *sk, V2 center, float radius) {
    if (!sk) return -1;
    sk->entities = (SkEntity *)realloc(sk->entities, sizeof(SkEntity)*(size_t)(sk->n_entities+1));
    SkEntity *e = &sk->entities[sk->n_entities];
    memset(e, 0, sizeof(*e)); e->type = SK_CIRCLE; e->id = sk->n_entities; e->p1 = center; e->radius = radius;
    return sk->n_entities++;
}

static void sk_add_constraint(Sketch *sk, ConstraintType type, int a, int b, float val) {
    sk->constraints = (SkConstraint *)realloc(sk->constraints, sizeof(SkConstraint)*(size_t)(sk->n_constraints+1));
    SkConstraint *c = &sk->constraints[sk->n_constraints++];
    c->type = type; c->entity_a = a; c->entity_b = b; c->value = val;
}

void sketch_add_horizontal(Sketch *sk, int id) { sk_add_constraint(sk, CT_HORIZONTAL, id, -1, 0); }
void sketch_add_vertical(Sketch *sk, int id) { sk_add_constraint(sk, CT_VERTICAL, id, -1, 0); }
void sketch_add_coincident(Sketch *sk, int a, int b) { sk_add_constraint(sk, CT_COINCIDENT, a, b, 0); }
void sketch_add_distance(Sketch *sk, int a, int b, float d) { sk_add_constraint(sk, CT_DISTANCE, a, b, d); }
void sketch_add_fix(Sketch *sk, int id) { sk_add_constraint(sk, CT_FIX, id, -1, 0); }

int sketch_solve(Sketch *sk) {
    if (!sk) return 0;
    // simple: apply constraints iteratively
    for (int iter = 0; iter < 10; iter++)
        for (int ci = 0; ci < sk->n_constraints; ci++) {
            SkConstraint *c = &sk->constraints[ci];
            SkEntity *a = (c->entity_a >= 0 && c->entity_a < sk->n_entities) ? &sk->entities[c->entity_a] : NULL;
            SkEntity *b = (c->entity_b >= 0 && c->entity_b < sk->n_entities) ? &sk->entities[c->entity_b] : NULL;
            if (!a) continue;
            switch (c->type) {
                case CT_HORIZONTAL: if (a->type == SK_LINE) a->p2.y = a->p1.y; break;
                case CT_VERTICAL: if (a->type == SK_LINE) a->p2.x = a->p1.x; break;
                case CT_COINCIDENT: if (a && b) b->p1 = a->p1; break;
                case CT_DISTANCE: if (a && b) {
                    V2 d = v2(b->p1.x-a->p1.x, b->p1.y-a->p1.y);
                    float len = sqrtf(d.x*d.x+d.y*d.y);
                    if (len > 0.001f) { b->p1.x = a->p1.x + d.x/len*c->value; b->p1.y = a->p1.y + d.y/len*c->value; }
                } break;
                case CT_FIX: break; // fixed entities don't move
                default: break;
            }
        }
    sk->solved = 1;
    return 1;
}

int sketch_to_pcb_outline(Sketch *sk, Pcb *pcb) {
    if (!sk || !pcb) return 0;
    int count = 0;
    for (int i = 0; i < sk->n_entities; i++) {
        SkEntity *e = &sk->entities[i];
        if (e->type == SK_LINE) {
            pcb->outline = (V2 *)realloc(pcb->outline, sizeof(V2)*(size_t)(pcb->noutline+2));
            pcb->outline[pcb->noutline++] = e->p1;
            pcb->outline[pcb->noutline++] = e->p2;
            count += 2;
        }
    }
    if (count > 0) pcb_bbox(pcb);
    return count;
}

char *sketch_report(Sketch *sk) {
    if (!sk) return str_dup("no sketch");
    char buf[1024]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== SKETCH: %s ===\n", sk->name);
    off += snprintf(buf+off, sizeof(buf)-off, "Entities: %d  Constraints: %d  Solved: %s\n",
        sk->n_entities, sk->n_constraints, sk->solved ? "yes" : "no");
    for (int i = 0; i < sk->n_entities; i++) {
        SkEntity *e = &sk->entities[i];
        if (e->type == SK_LINE) off += snprintf(buf+off, sizeof(buf)-off, "  LINE[%d]: (%.0f,%.0f)-(%.0f,%.0f)\n", i, e->p1.x, e->p1.y, e->p2.x, e->p2.y);
        else if (e->type == SK_POINT) off += snprintf(buf+off, sizeof(buf)-off, "  POINT[%d]: (%.0f,%.0f)\n", i, e->p1.x, e->p1.y);
        else if (e->type == SK_CIRCLE) off += snprintf(buf+off, sizeof(buf)-off, "  CIRCLE[%d]: (%.0f,%.0f) r=%.1f\n", i, e->p1.x, e->p1.y, e->radius);
    }
    return str_dup(buf);
}
