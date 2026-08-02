#pragma once
// v2_features.h - Webcam, RFID, Hierarchy, Vias, Symbol Editor, Paint.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"

// ===== LIVE WEBCAM =====
// Capture a frame from default webcam and return as raw RGB data.
// Returns width*height*3 bytes (caller frees), sets w/h.
unsigned char *webcam_capture_rgb(int *out_w, int *out_h);

// List available webcam devices. Returns count, fills names array.
int v2_webcam_list(char names[8][64]);

// Draw webcam preview status in farm panel.
char *webcam_status_text(void);

// ===== RFID/BARCODE/QR =====
// Simulate a scan event (text input-based).
// Parses GSI barcode format: (01)GTIN(21)SERIAL etc.
typedef struct { char gtin[16]; char serial[32]; char lot[16]; char date[12]; } BarcodeData;
BarcodeData barcode_parse(const char *scan_data);

// RFID tag read simulation.
char *rfid_simulate_scan(void); // returns random spool tag

// QR code data extraction stub.
char *qr_decode_text(const char *image_path);

// ===== HIERARCHICAL SHEET EDITOR =====
// Create a child schematic sheet with ports.
Schematic *sheet_create_child(Project *p, const char *name, const char *parent_name);

// Add a hierarchical port to a sheet.
void sheet_add_port(Schematic *s, const char *port_name, V2 pos, int direction);

// List all sheets in hierarchy with indentation.
char *sheet_list_hierarchy(Project *p);

// ===== BLIND/BURIED VIAS =====
// Create a blind via (from top layer to specified inner layer).
int via_create_blind(Pcb *pcb, V2 pos, float drill, float outer, int end_layer, const char *net);

// Create a buried via (between two inner layers).
int via_create_buried(Pcb *pcb, V2 pos, float drill, float outer, int start_layer, int end_layer, const char *net);

// Via type report.
char *via_type_report(Pcb *pcb);

// ===== SYMBOL EDITOR =====
// Edit a pin's properties (name, number, position, angle, type).
bool symbol_pin_edit(Symbol *sym, int pin_index, const char *name, const char *number, V2 pos, float angle, int ptype);

// Move a pin relative to current position.
bool symbol_pin_move(Symbol *sym, int pin_index, V2 delta);

// Add a new pin to a symbol.
int symbol_pin_add(Symbol *sym, const char *name, const char *number, V2 pos);

// Symbol edit report.
char *symbol_edit_report(Symbol *sym);

// ===== SUPPORT PAINTING =====
// Paint a support region by marking triangles.
// Returns number of triangles painted.
int paint_support_region(CadMesh *mesh, V2 screen_pos, float brush_radius, float zoom, V2 pan, int *painted_tris, int max_tris);

// Paint a seam region.
int paint_seam_region(V2 *perimeter, int n, V2 screen_pos, float zoom, V2 pan, int *painted_indices, int max_indices);

// Paint report text.
char *paint_region_report(int painted_count, const char *type);
