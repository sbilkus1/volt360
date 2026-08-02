#pragma once
// sketcher.h - 2D constraint-based sketcher (FreeCAD-style Sketcher workbench).
#include <stdbool.h>
#include "../model/project.h"

// Sketch entity types.
typedef enum { SK_POINT, SK_LINE, SK_CIRCLE, SK_ARC } SkEntityType;

typedef struct {
    SkEntityType type;
    int id;
    // line: p1, p2; circle: center, radius; point: pos; arc: center, radius, start/end angle
    V2 p1, p2;
    float radius;
    float arc_start, arc_end;
    int constrained; // bit flags for constraints applied
} SkEntity;

// Constraint types.
typedef enum { CT_HORIZONTAL, CT_VERTICAL, CT_COINCIDENT, CT_PARALLEL, CT_PERPENDICULAR,
               CT_TANGENT, CT_EQUAL, CT_DISTANCE, CT_ANGLE, CT_FIX } ConstraintType;

typedef struct {
    ConstraintType type;
    int entity_a, entity_b; // entity IDs
    float value;            // distance/angle value
} SkConstraint;

// A sketch is a collection of entities and constraints.
typedef struct {
    char name[64];
    SkEntity *entities; int n_entities;
    SkConstraint *constraints; int n_constraints;
    V2 origin;
    int solved; // 1 if the constraint system is solved
} Sketch;

Sketch *sketch_create(const char *name);
void sketch_free(Sketch *sk);

// Add entities.
int sketch_add_point(Sketch *sk, V2 pos);
int sketch_add_line(Sketch *sk, V2 p1, V2 p2);
int sketch_add_circle(Sketch *sk, V2 center, float radius);

// Add constraints.
void sketch_add_horizontal(Sketch *sk, int line_id);
void sketch_add_vertical(Sketch *sk, int line_id);
void sketch_add_coincident(Sketch *sk, int entity_a, int entity_b);
void sketch_add_distance(Sketch *sk, int entity_a, int entity_b, float distance);
void sketch_add_fix(Sketch *sk, int entity_id);

// Solve the constraint system (simple iterative relaxation).
int sketch_solve(Sketch *sk);

// Convert sketch to PCB tracks (for board outline generation).
int sketch_to_pcb_outline(Sketch *sk, Pcb *pcb);

// Sketch report text.
char *sketch_report(Sketch *sk);
