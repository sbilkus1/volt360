#pragma once
// interact.h - Interactive SPICE simulation + workflow tutorial.
#include <stdbool.h>
#include "../model/project.h"

// Run interactive SPICE: modify component value and re-solve.
// Returns updated node voltages as text.
char *interact_spice_set_value(Schematic *s, const char *ref, const char *new_value);

// List all modifiable components in the schematic.
char *interact_list_components(Schematic *s);

// Generate circuit network list (nodes + components).
char *interact_netlist_text(Schematic *s);

// Full workflow tutorial text.
char *tutorial_text(void);

// Get tutorial for a specific topic.
char *tutorial_topic(const char *topic);
