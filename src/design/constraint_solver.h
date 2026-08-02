#pragma once
/* constraint_solver.h - Geometric constraint solver.
 * Uses iterative relaxation to satisfy 2D geometric constraints.
 */
#include <stdbool.h>
#include "../model/project.h"

#define CS_MAX_ENTITIES 128
#define CS_MAX_CONSTRAINTS 256

typedef enum {
    CS_POINT,
    CS_LINE,
    CS_CIRCLE
} CSEntityType;

typedef enum {
    CS_HORIZONTAL,
    CS_VERTICAL,
    CS_COINCIDENT,
    CS_PARALLEL,
    CS_PERPENDICULAR,
    CS_TANGENT,
    CS_EQUAL_LENGTH,
    CS_FIXED_DISTANCE,
    CS_FIXED_ANGLE,
    CS_FIX_POINT
} CSConstraintType;

typedef struct {
    int type;
    V2 p1, p2;               /* line: endpoints; circle: center=p1; point: p1=p2=pos */
    float radius;             /* circle radius */
    int fixed;                /* 1 if entity is locked */
} CSEntity;

typedef struct {
    int type;
    int e1, e2;              /* entity indices */
    float value;              /* distance or angle value */
} CSConstraint;

typedef struct {
    CSEntity entities[CS_MAX_ENTITIES];
    int nentities;
    CSConstraint constraints[CS_MAX_CONSTRAINTS];
    int nconstraints;
} ConstraintGraph;

void cs_init(ConstraintGraph *g);

int cs_add_point(ConstraintGraph *g, V2 pos, int fix);
int cs_add_line(ConstraintGraph *g, V2 p1, V2 p2);
int cs_add_circle(ConstraintGraph *g, V2 center, float radius);

int cs_add_constraint_horizontal(ConstraintGraph *g, int line_id);
int cs_add_constraint_vertical(ConstraintGraph *g, int line_id);
int cs_add_constraint_coincident(ConstraintGraph *g, int e1, int e2);
int cs_add_constraint_parallel(ConstraintGraph *g, int line1, int line2);
int cs_add_constraint_perpendicular(ConstraintGraph *g, int line1, int line2);
int cs_add_constraint_tangent(ConstraintGraph *g, int line_id, int circle_id);
int cs_add_constraint_equal_length(ConstraintGraph *g, int line1, int line2);
int cs_add_constraint_distance(ConstraintGraph *g, int e1, int e2, float dist);
int cs_add_constraint_angle(ConstraintGraph *g, int line1, int line2, float angle_deg);

/* Solve constraint system iteratively.
 * Returns 1 if converged within tolerance, 0 if max_iter reached. */
bool constraint_solve(ConstraintGraph *g, int max_iter, float tolerance);
