#pragma once
// scaletool.h - Scale, mesh-to-solid, corner reliefs, ejection profiles, favorites.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/farm.h"

// Scale a mesh uniformly or per-axis.
void mesh_scale(CadMesh *m, float sx, float sy, float sz);

// Scale a mesh about a reference point.
void mesh_scale_about(CadMesh *m, V3 ref, float sx, float sy, float sz);

// Mesh-to-solid: create a simple solid from mesh by extruding thickness.
bool mesh_to_solid(CadMesh *in, CadMesh *out, float thickness);

// Corner relief: compute circular relief at sheet metal bend corners.
// Returns relief diameter to add.
float corner_relief_diameter(float thickness, float bend_radius);

// Bend table entry for a specific material + thickness.
typedef struct {
    float thickness; float radius; float k_factor; float deduction_90;
} BendTableEntry;

BendTableEntry bend_table_lookup(float thickness, float radius);

// Surface ejection profile: per-material release temperature + angle.
typedef struct {
    char material[32];
    float release_temp;
    int eject_angle;
    char gcode_suffix[128]; // custom G-code appended
} EjectProfile;

void eject_profile_default(EjectProfile *ep, const char *material);
char *eject_profile_report(EjectProfile *ep);

// Predictive maintenance: check all printers and flag upcoming needs.
char *predictive_maintenance_report(Farm *f);

// Favorites/bookmarks for sidebar items.
void favorites_add(Project *p, const char *item_type, const char *item_id);
char *favorites_list(Project *p);

// Export all formats for a project.
char *export_all_formats(Project *p, const char *output_dir);
