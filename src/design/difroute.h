#pragma once
// difroute.h - Differential pair routing + net classes.
#include <stdbool.h>
#include "../model/project.h"

// Add a differential pair (two parallel tracks with fixed spacing).
int difroute_add_pair(Pcb *pcb, V2 a1, V2 b1, V2 a2, V2 b2, float width, float spacing);

// Net class: name, track width, clearance, via size.
typedef struct { char name[32]; float track_width; float clearance; float via_drill; float via_outer; } NetClass;

// Apply net class to all tracks/vias matching a net name pattern.
int netclass_apply(Pcb *pcb, const char *net_pattern, NetClass *nc);
