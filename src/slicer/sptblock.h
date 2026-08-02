#pragma once
// sptblock.h - Support blockers, modifier meshes, print history, quick toolbar.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"
#include "../slicer/farm.h"

// Support blocker: region where supports are disabled.
typedef struct {
    V3 bmin, bmax;    // bounding box
    int enabled;       // 0=block supports, 1=force supports
} SupportBlocker;

// Modifier mesh: override settings in a region.
typedef struct {
    V3 bmin, bmax;
    float infill_density_override; // -1 = no override
    int perimeters_override;       // -1 = no override
    float speed_override;          // -1 = no override
} ModifierMesh;

// Add support blocker to slice settings.
void sptblock_add(SliceSettings *cfg, SupportBlocker blk);
SupportBlocker *sptblock_get(SliceSettings *cfg, int idx);
int sptblock_count(SliceSettings *cfg);

// Add modifier mesh.
void modifier_add(SliceSettings *cfg, ModifierMesh mod);
ModifierMesh *modifier_get(SliceSettings *cfg, int idx);
int modifier_count(SliceSettings *cfg);

// Print history: log completed jobs.
typedef struct {
    char job_name[128]; char printer[64]; char filament[32];
    float filament_mm; float est_time_s; int success; // 1=success, 0=fail
} PrintLog;

void printlog_add(Farm *f, PrintLog entry);
char *printlog_report(Farm *f);

// Network printer status widget text.
char *netprinter_status_widget(Farm *f);

// Quick access toolbar: context-sensitive shortcuts.
char *quick_toolbar_text(int mode);
void quick_toolbar_draw(int x, int y);

// Localization stub.
char *localize(const char *english_text);

// Render settings panel text.
char *render_settings_text(void);
