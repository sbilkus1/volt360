#pragma once
// pushroute.h - Push-and-shove PCB track routing.
#include <stdbool.h>
#include "../model/project.h"

// Add a track between two points, pushing existing tracks out of the way.
// Returns 0 on success, -1 if impossible.
int pushroute_add(Pcb *pcb, V2 a, V2 b, float width, float min_clearance);
