#pragma once
// symedit.h - Symbol editor: create/edit schematic symbols.
#include <stdbool.h>
#include "../model/project.h"

// Create a new symbol with given name and dimensions.
// pins: array of pin positions (x,y) relative to symbol center
// pin_names: array of pin names
Symbol *symedit_create(Project *p, const char *name, float w, float h,
                       V2 *pin_pos, const char **pin_names, int n_pins);

// Delete a symbol from the project.
bool symedit_delete(Project *p, const char *symbol_id);
