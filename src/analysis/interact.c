#include "interact.h"
#include "spice.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

char *interact_spice_set_value(Schematic *s, const char *ref, const char *new_value) {
    if (!s || !ref || !new_value) return str_dup("missing arguments");
    for (int i = 0; i < s->ninsts; i++)
        if (s->insts[i].ref && strcmp(s->insts[i].ref, ref) == 0) {
            free(s->insts[i].value);
            s->insts[i].value = str_dup(new_value);
            SpiceResult *sr = spice_solve_dc(s);
            if (sr) { char *r = spice_result_text(sr); spice_result_free(sr); return r; }
            return str_dup("SPICE solve failed");
        }
    return str_dup("component not found");
}

char *interact_list_components(Schematic *s) {
    if (!s) return str_dup("no schematic");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== COMPONENTS ===\n");
    for (int i = 0; i < s->ninsts; i++)
        off += snprintf(buf+off, sizeof(buf)-off, "  [%d] %s: %s @(%.0f,%.0f)\n",
            i+1, s->insts[i].ref ? s->insts[i].ref : "?",
            s->insts[i].value ? s->insts[i].value : "?",
            s->insts[i].pos.x, s->insts[i].pos.y);
    return str_dup(buf);
}

char *interact_netlist_text(Schematic *s) {
    if (!s) return str_dup("no schematic");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== NETLIST ===\n");
    off += snprintf(buf+off, sizeof(buf)-off, "Components: %d  Wires: %d  Labels: %d\n",
        s->ninsts, s->nwires, s->nlabels);
    for (int i = 0; i < s->ninsts; i++)
        off += snprintf(buf+off, sizeof(buf)-off, "%s %s (%.0f,%.0f)\n",
            s->insts[i].ref ? s->insts[i].ref : "R?",
            s->insts[i].value ? s->insts[i].value : "?",
            s->insts[i].pos.x, s->insts[i].pos.y);
    return str_dup(buf);
}

char *tutorial_text(void) {
    return str_dup(
        "=== Volt360 WORKFLOW ===\n\n"
        "1. IMPORT: Drag & drop a folder with .kicad_sch/.kicad_pcb/.stl files\n"
        "2. SCHEMATIC: View circuits, run SPICE (DC), check ERC\n"
        "3. PCB: View board, drag footprints, route tracks (T key), run DRC\n"
        "4. 3D: Orbit/zoom, click models to select, measure with M, wireframe W\n"
        "5. FIT: Check for clearance/collision issues\n"
        "6. DESIGN: Create parametric features, run topology optimization\n"
        "7. PRINT: Generate enclosures, slice, calibrate, manage print farm\n"
        "8. EXPORT: Gerber, DXF, PDF, STL, G-code, P&P, IPC-356\n\n"
        "For help: press ? or F1 anytime.\n");
}

char *tutorial_topic(const char *topic) {
    if (!topic) return tutorial_text();
    if (strstr(topic, "slice")) return str_dup("SLICER: Layer height 0.2mm default. Adjust in Print Center (7). Use calibration tests to tune.");
    if (strstr(topic, "enclosure")) return str_dup("ENCLOSURE: Select PCB in sidebar, go to Print Center (7), click Generate Enclosure.");
    if (strstr(topic, "spice")) return str_dup("SPICE: In Schematic (1), click Simulate (DC). Circuit must have R and V components.");
    if (strstr(topic, "farm")) return str_dup("PRINT FARM: In Print Center (7), add printers/jobs/spools. Use Assign to route jobs.");
    if (strstr(topic, "cam")) return str_dup("CAM: In Print Center (7), CAM section generates CNC G-code for mills, lasers, waterjets.");
    if (strstr(topic, "pcb")) return str_dup("PCB EDITING: In PCB mode (2), T=routing, click+drag footprint, R=rotate, X=delete, DRC=check.");
    return tutorial_text();
}
