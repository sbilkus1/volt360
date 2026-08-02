#pragma once
// pdfout.h - Simple text-based PDF export from reports/drawings.
#include <stdbool.h>
#include "../model/project.h"

// Write a minimal PDF document from text content.
// title: document title, text: body content
bool pdf_write_simple(const char *path, const char *title, const char *text);

// Export a PCB drawing as PDF with dimensions.
bool pdf_export_drawing(Pcb *pcb, const char *path);

// Export a schematic as PDF.
bool pdf_export_schematic(Schematic *s, const char *path);

// Export DXF from PCB data.
bool dxf_export_pcb(Pcb *pcb, const char *path);
