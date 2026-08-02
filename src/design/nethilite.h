#pragma once
// nethilite.h - Live net highlighting in PCB view.
#include <stdbool.h>
#include "../model/project.h"

// Highlight a specific net: returns indices of tracks on that net.
// Fills track_indices array (caller allocates).
int nethilite_find_tracks(Pcb *pcb, const char *net_name, int *track_indices, int max_tracks);

// Highlight all tracks connected to a given footprint.
int nethilite_connected(Pcb *pcb, int fp_index, int *track_indices, int max_tracks);

// Get the net name at a given position (click-to-query).
const char *nethilite_query(Pcb *pcb, V2 pos, float tolerance);

// Net report: list all unique nets in the PCB.
char *nethilite_nets_report(Pcb *pcb);
