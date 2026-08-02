#pragma once
// lib3d.h - 3D model / symbol library organization.
#include <stdbool.h>
#include "../model/project.h"

// Organize CAD models into named libraries.
typedef struct {
    char name[64];
    char *model_ids[64]; // references to cad_models by id
    int n_models;
} ModelLibrary;

// Create a new library.
int lib3d_create(Project *p, const char *name);

// Add a model to a library.
bool lib3d_add_model(Project *p, const char *lib_name, const char *model_id);

// List all libraries.
char *lib3d_list_report(Project *p);

// Symbol library: add to project symbols collection.
int libsym_add_from_template(Project *p, const char *name, const char *ref_prefix, int n_pins);

// Community preset sharing stub: export project config as a shareable JSON.
char *community_export_presets(Project *p);

// Surface finish: ironing visualization data.
char *surface_finish_report(CadMesh *m, float layer_height);
