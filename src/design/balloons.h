#pragma once
// balloons.h - Assembly balloons/callouts for drawings.
#include <stdbool.h>
#include "../model/project.h"

// Generate balloon callout text for assembly items.
// Returns malloc'd text.
char *balloons_text(Assembly *as);

// Generate assembly instruction text with numbered steps.
char *balloons_assembly_instructions(Assembly *as);
