#pragma once
// bus.h - Schematic bus management + global/local net labels.
#include <stdbool.h>
#include "../model/project.h"

// Bus definition: a group of related signals.
typedef struct {
    char name[64];
    char **signals; int n_signals; // signal names in the bus
} Bus;

// Create a bus from a list of signal names.
Bus *bus_create(const char *name, const char **signals, int n);

// Free a bus.
void bus_free(Bus *b);

// Add a bus tap: connect a signal from the bus to a component pin.
void bus_tap(Schematic *s, const char *bus_name, const char *signal_name, V2 pos);

// Global label: add a named net label at a position.
void label_add_global(Schematic *s, const char *name, V2 pos);

// Local label (sheet-local, not global).
void label_add_local(Schematic *s, const char *name, V2 pos);

// Auto-generate bus labels from existing net labels with matching prefixes.
// e.g. "DATA0", "DATA1", "DATA2" -> bus "DATA[0:2]"
char *bus_auto_detect(Schematic *s);
