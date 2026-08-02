#pragma once
// kicadfeat.h - KiCad-specific features: CvPcb, page layout, netlist export, lib table.
#include <stdbool.h>
#include "../model/project.h"

// === CvPcb: Auto-assign footprints to schematic components ===
// Matches schematic component names to available footprint names.
// Returns number of components with assigned footprints.
int cvpcb_auto_assign(Project *p);

// Suggest best footprint match for a component name.
char *cvpcb_suggest_footprint(const char *comp_name, Footprint *footprints, int n_footprints);

// === PAGE LAYOUT EDITOR ===
// Generate a title block and border for drawings.
char *pagelayout_title_block(const char *title, const char *author, const char *date, const char *revision, float page_w, float page_h);

// Generate a full page layout frame with zones.
char *pagelayout_full_frame(const char *project_name, float w, float h);

// === MULTI-FORMAT NETLIST EXPORT ===
// PADS netlist format.
bool netlist_export_pads(Schematic *s, const char *path);

// Allegro/OrCAD netlist format.
bool netlist_export_allegro(Schematic *s, const char *path);

// KiCad native netlist format.
bool netlist_export_kicad_net(Schematic *s, const char *path);

// Export all netlist formats.
bool netlist_export_all(Schematic *s, const char *dir);

// === INTERACTIVE DIFFERENTIAL PAIR ROUTER ===
// Route a differential pair with phase matching.
// Returns number of segments added.
int diffpair_route_interactive(Pcb *pcb, V2 start_p, V2 start_n, V2 end_p, V2 end_n, float width, float spacing);

// Add phase-matching meanders to match lengths of a diff pair.
int diffpair_phase_match(Pcb *pcb, int track_a_idx, int track_b_idx, float tolerance);

// === COMPONENT-FOOTPRINT VALIDATOR ===
// Check if footprints match their component pin count.
char *validator_check(Project *p);

// === LIBRARY TABLE ===
// Manage footprint library table (fp-lib-table format).
typedef struct { char nickname[32]; char uri[256]; char type[16]; } LibTableEntry;

void libtable_add(LibTableEntry *table, int *count, const char *nick, const char *uri);
char *libtable_export(LibTableEntry *table, int count);
char *libtable_get_default(void);

// === IDF EXPORT ===
bool idf_export_board(Pcb *pcb, const char *path);

// === STEP MODEL ===
bool step_export_assembly(Project *p, const char *path);
