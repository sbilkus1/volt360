#pragma once
// lengthtune.h - Length matching and time-domain tuning for PCB tracks.
#include <stdbool.h>
#include "../model/project.h"

// Measure total length of all tracks on a given net (mm).
float lengthtune_net_length(Pcb *pcb, const char *net_name);

// Add meanders (serpentine sections) to a track to reach target length.
// Returns number of meander segments added.
int lengthtune_add_meanders(Pcb *pcb, const char *net_name, float target_length_mm, float amplitude, float spacing);

// Time-domain delay for a net (ns), assuming FR4 er=4.2.
float lengthtune_delay_ns(float length_mm);

// Match two nets to within tolerance by adding meanders to the shorter one.
int lengthtune_match_nets(Pcb *pcb, const char *net_a, const char *net_b, float tolerance_mm);
