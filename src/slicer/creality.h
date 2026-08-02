#pragma once
// creality.h - Creality printer connectivity + STEP export + dashboard.
#include <stdbool.h>
#include "../model/project.h"
#include "farm.h"

// Check Creality printer status over LAN.
int creality_status(const char *ip, int port, char *state, int state_len);

// Upload G-code to Creality printer.
bool creality_upload(const char *ip, int port, const char *gcode_path);

// STEP file export from CAD mesh (ASCII STEP AP203).
// Converts triangle mesh to STEP format with shell + advanced_face entities.
bool step_export(CadMesh *m, const char *path);

// Full dashboard status summary for the status bar.
char *dashboard_summary(Project *p, Farm *f);

// HDR/rendering presets text.
char *render_presets(void);
