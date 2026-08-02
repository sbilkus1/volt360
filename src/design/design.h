#pragma once
// design.h - parametric design, materials, mass properties, mesh export
#include "../model/project.h"

// ---------------- materials ----------------
typedef struct { unsigned char r, g, b; } MatRGB;

typedef struct {
    char *name;
    float density;      // g/cm3
    float young_gpa;    // GPa
    MatRGB color;
} CadMaterial;

// returns a static list of built-in materials (Aluminum, Steel, ABS, FR4, Copper, Titanium...)
int material_lib_count(void);
CadMaterial *material_lib_get(int i);
// finds by name (case-insensitive), returns NULL if not found
CadMaterial *material_find(const char *name);
// returns a freshly allocated copy of the default (first) material
CadMaterial *material_default(void);
void material_free(CadMaterial *m);

// ---------------- parametric features (design timeline) ----------------
typedef enum { FEAT_BOX, FEAT_CYLINDER, FEAT_SPHERE } FeatureType;

typedef struct {
    int type;
    float x, y, z;      // center / origin offset (mm)
    float rx, ry, rz;   // rotation (deg)
    float w, h, d;      // box dims (mm); cylinder/sphere use w = radius, h = height/diameter
    int segments;       // tessellation
    int material_id;    // index into material lib
} DesignFeature;

// Generate a mesh for a single feature (local space, origin-centered). Returns true on success.
bool feature_make_mesh(DesignFeature *f, int material_index, CadMesh *out);
// Simple CSG-like union by placing feature meshes side by side along X.
// Builds a "blocky" fused part from the feature list (used by the demo).
void design_build_assembly(Project *p, DesignFeature *feats, int nfeats);
void feature_free(DesignFeature *f);

// ---------------- mass properties ----------------
// Fills volume (mm3), mass (g), centroid (mm). Uses divergence theorem over triangle soup.
void mesh_mass_props(CadMesh *m, float density_g_cm3, float *out_volume_mm3, float *out_mass_g, V3 *out_centroid);

// ---------------- export ----------------
// Writes binary STL. Returns true on success.
bool mesh_write_stl(const char *path, CadMesh *m);
// Writes a BOM (bill of materials) text from project components. Returns malloc'd string.
char *project_bom_text(Project *p);
