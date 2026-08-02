#pragma once
// autosave.h - Autosave, project templates, capacity planning, reservations.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/farm.h"

// Autosave: save project periodically.
// Returns true if saved successfully.
bool autosave_tick(Project *p); // call each frame, auto-saves every N seconds

// Set autosave interval (seconds). Default 300 (5 min).
void autosave_set_interval(int seconds);

// Get time since last save (seconds).
int autosave_seconds_since_save(void);

// Project template: create a new project from a built-in template.
typedef enum { TEMPLATE_BLANK, TEMPLATE_ARDUINO, TEMPLATE_PSU, TEMPLATE_ENCLOSURE } Template;
Project *template_create(const char *name, const char *path, Template t);

// Farm capacity planning: estimate how many more jobs the farm can handle.
char *farm_capacity_report(Farm *f);

// Print reservation: reserve printer time for a future job.
int farm_reserve_slot(Farm *f, int printer_index, float hours_from_now, float duration_hours);

// List reservations.
char *farm_reservations_report(Farm *f);
