#pragma once
// multisheet.h - Multi-sheet hierarchical schematic support.
#include <stdbool.h>
#include "../model/project.h"

// Sheet reference: a hierarchical sheet pin/port.
typedef struct {
    char *name;       // port name
    int direction;    // 0=input, 1=output, 2=bidir
    V2 pos;           // position on the sheet
} SheetPort;

// A schematic sheet (extends the existing Schematic).
// Sheets are stored as separate Schematic entries in the project.
// This module provides navigation and hierarchy management.

// Add a hierarchical sheet reference to a parent schematic.
// Points to another schematic (child) by name.
void sheet_add_hierarchical(Schematic *parent, const char *child_name);

// Find child schematics of a given parent.
int sheet_children(Project *p, const char *parent_name, char ***out_names);

// Navigate hierarchy: find the root schematic (one with no parent).
Schematic *sheet_find_root(Project *p);

// List all sheets in hierarchy.
char *sheet_hierarchy_report(Project *p);
