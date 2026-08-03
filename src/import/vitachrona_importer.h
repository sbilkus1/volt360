#pragma once
// vitachrona_importer.h - Import Vitachrona Flux AI prompt into Volt360
// Parses the structured spec, auto-completes missing components,
// generates KiCad schematics, netlists, BOM CSV, enclosures, and connector footprints.
#include "../model/project.h"
#include "../slicer/slicer.h"
#include "../analysis/spice_solver.h"

typedef struct {
    char designator[16];
    char mfg[32];
    char mpn[64];
    char function[128];
    char package[32];
    char value[32];
    int confirmed;
    int qty;
    float x, y;
    char pcb[32];
} VitComponent;

typedef struct {
    char net_name[32];
    char from_comp[16];
    char from_pin[16];
    char to_comp[16];
    char to_pin[16];
    char pcb[32];
} VitNet;

typedef struct {
    VitComponent *components;
    int ncomponents;
    VitNet *nets;
    int nnets;
    char *pcb_names[8];
    int npcbs;
    int version;
} VitProject;

// ===== PARSING =====
bool vit_parse_flux_prompt(const char *filepath, VitProject *out);
void vit_free(VitProject *vp);

// ===== AI AUTO-COMPLETE =====
int vit_autocomplete_components(VitProject *vp);
char *vit_missing_components_report(VitProject *vp);

// ===== SCHEMATIC GENERATION =====
bool vit_generate_kicad_sch(VitProject *vp, const char *pcb_name, const char *output_path);
bool vit_generate_all_schematics(VitProject *vp, const char *output_dir);

// ===== NETLIST / BOM =====
bool vit_generate_netlist(VitProject *vp, const char *output_path);
bool vit_generate_bom_csv(VitProject *vp, const char *output_path);

// ===== ENCLOSURE =====
bool vit_generate_watch_enclosure(VitProject *vp, CadMesh *out);
bool vit_generate_dock_enclosure(VitProject *vp, CadMesh *out);
bool vit_generate_pcb_main_enclosure(VitProject *vp, CadMesh *out);

// ===== CONNECTOR FOOTPRINTS =====
bool vit_generate_dock_pogo_footprint(VitProject *vp, Footprint *out);
bool vit_generate_strap_connector_footprint(VitProject *vp, Footprint *out);
bool vit_generate_sensor_link_footprint(VitProject *vp, Footprint *out);
bool vit_generate_display_connector_footprint(VitProject *vp, Footprint *out);

// ===== VRTL =====
bool vit_validate_power_tree(VitProject *vp);
bool vit_validate_pin_mux(VitProject *vp);
char *vit_validation_report(VitProject *vp);

// ===== FULL WORKFLOW =====
bool vit_full_import_workflow(const char *flux_prompt_path, Project *proj);
