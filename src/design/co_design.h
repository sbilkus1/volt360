#pragma once
// co_design.h - mechanical/electrical co-design links.
// Links the CAD and electronics domains:
//   - PCB -> CAD: generate a printable enclosure for a board and place the PCB
//     + enclosure together in an Assembly (so you can "make a PCB and get the
//     CAD enclosure made").
//   - CAD -> slicer: slice & print any designed/imported CAD mesh directly.
#include "../model/project.h"
#include "../slicer/slicer.h"

typedef struct {
    float wall_thickness;  // mm (default 2.0)
    float wall_height;     // mm internal height above floor top (default 12.0)
    float floor_thickness; // mm (default 2.0)
    float clearance;       // mm gap between pcb edge and inner wall (default 2.0)
    float standoff_h;      // mm height of mounting standoffs (default 4.0)
    float standoff_d;      // mm diameter of mounting standoffs (default 4.0)
    float pcb_thickness;   // mm (default 1.6)
    int lid;               // 1 = include a top lid (default 1)
} EnclosureParams;

void co_enclosure_defaults(EnclosureParams *e);

// Build the enclosure mesh (floor + walls + standoffs + optional lid) around a
// PCB outline. Centered on the PCB bbox center. Returns true on success.
bool pcb_enclosure_mesh(Pcb *pcb, const EnclosureParams *ep, CadMesh *out);

// Create a CadModel "enclosure" for pcb_id, push it into the project, and add
// an Assembly that places the PCB (PART_PCB) sitting on the standoffs inside
// the enclosure (PART_CAD). Returns the assembly index, or -1.
int co_design_enclosure(Project *p, const char *pcb_id, const EnclosureParams *ep);

// Find the generated enclosure CadModel for a PCB (by "<pcb name>-enclosure").
CadModel *co_find_enclosure(Project *p, const char *pcb_id);

// CAD -> slicer: slice a mesh with cfg and write gcode to path.
bool gcode_slice_and_emit(CadMesh *m, const SliceSettings *cfg, const char *path);

// Slice & emit a project CadModel (by id) so you can print what you designed.
bool co_design_print_cad(Project *p, const char *cad_id, const SliceSettings *cfg, const char *path);

// Slice & emit the enclosure of a PCB.
bool co_design_print_enclosure(Project *p, const char *pcb_id, const SliceSettings *cfg, const char *path);
