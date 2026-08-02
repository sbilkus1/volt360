#pragma once
// exploded.h - Assembly exploded view generation.
#include <stdbool.h>
#include "../model/project.h"

// Generate an exploded view by offsetting assembly items outward from centroid.
// explode_factor: multiplier on item position relative to centroid (0=compact, 1=normal, 2=exploded).
void assembly_explode(Assembly *as, float explode_factor);

// Generate exploded view text description.
char *assembly_explode_text(Assembly *as, float explode_factor);
