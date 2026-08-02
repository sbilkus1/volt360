#pragma once
// contact.h - Contact/gap analysis for assemblies.
#include <stdbool.h>
#include "../model/project.h"

// Measure gap between two components (bounding box distances).
// Returns minimum gap (negative = penetration).
float contact_gap_mm(CadMesh *a, CadMesh *b);

// Check if two bounding boxes overlap.
bool contact_bbox_overlap(V3 a_min, V3 a_max, V3 b_min, V3 b_max);

// Penetration depth between overlapping bounding boxes.
float contact_penetration(V3 a_min, V3 a_max, V3 b_min, V3 b_max);

// Contact analysis report for an assembly.
char *contact_report(Assembly *as, Project *p);

// Shape optimization: stiffest design by moving material.
// Optimizes a mesh by adding material where strain energy is highest.
// Simple gradient-based approach.
char *shape_optimize_report(CadMesh *m, float target_stiffness, int iterations);
