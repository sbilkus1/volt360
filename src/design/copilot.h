#pragma once
// copilot.h - Flux-style engineering shortcuts: FMEA, component comparison, pin listing.
#include <stdbool.h>
#include "../model/project.h"

// Generate a basic FMEA (Failure Mode and Effects Analysis) for components in the project.
// Returns malloc'd text (caller frees).
char *copilot_fmea(Project *p);

// Compare two components side-by-side (specs, dimensions, pins).
char *copilot_compare_components(Component *a, Component *b);

// List all pins for a component with their names/types.
char *copilot_pin_list(Component *c);

// Find passive components that can be consolidated (same value, package).
// Returns malloc'd text.
char *copilot_consolidate_passives(Project *p);

// Detect common design issues in a schematic.
char *copilot_design_issues(Schematic *s);
