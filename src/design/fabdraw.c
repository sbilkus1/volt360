#include "fabdraw.h"
#include "../import/gerber_out.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

char *fabdraw_report(Pcb *pcb) {
    if (!pcb) return str_dup("no PCB");
    char buf[4096]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off,
        "========================================\n"
        "  FABRICATION DRAWING: %s\n"
        "========================================\n"
        "  Board size: %.1f x %.1f mm\n"
        "  Copper layers: 2\n"
        "  Board thickness: %.1f mm\n\n",
        pcb->name ? pcb->name : "Untitled",
        pcb->bmax.x - pcb->bmin.x, pcb->bmax.y - pcb->bmin.y,
        pcb->thickness > 0 ? pcb->thickness : 1.6f);

    off += snprintf(buf+off, sizeof(buf)-off,
        "  TRACKS: %d\n", pcb->ntracks);
    if (pcb->ntracks > 0) {
        float tw = 0; for (int i=0; i<pcb->ntracks; i++) tw += pcb->tracks[i].width;
        tw /= (float)pcb->ntracks;
        off += snprintf(buf+off, sizeof(buf)-off, "    avg width: %.2f mm (min: 0.15 mm)\n", tw);
    }
    off += snprintf(buf+off, sizeof(buf)-off, "  FOOTPRINTS: %d\n", pcb->nfps);
    off += snprintf(buf+off, sizeof(buf)-off, "  VIAS: %d\n", pcb->nvias);
    if (pcb->nvias > 0) {
        float d = 0; for (int i=0; i<pcb->nvias; i++) d += pcb->vias[i].drill;
        d /= (float)pcb->nvias;
        off += snprintf(buf+off, sizeof(buf)-off, "    avg drill: %.2f mm\n", d);
    }
    off += snprintf(buf+off, sizeof(buf)-off, "\n  GD&T CALLOUTS:\n");
    off += snprintf(buf+off, sizeof(buf)-off, "    Board outline: %.1f +/- 0.2 mm\n", pcb->bmax.x-pcb->bmin.x);
    off += snprintf(buf+off, sizeof(buf)-off, "    Track clearance: 0.20 +/- 0.05 mm\n");
    off += snprintf(buf+off, sizeof(buf)-off, "    Drill tolerance: +/- 0.05 mm\n");
    off += snprintf(buf+off, sizeof(buf)-off, "    Surface finish: HASL / ENIG\n");
    off += snprintf(buf+off, sizeof(buf)-off, "\n  MANUFACTURING NOTES:\n");
    off += snprintf(buf+off, sizeof(buf)-off, "    - FR4, Tg 130-140, 1.6mm\n");
    off += snprintf(buf+off, sizeof(buf)-off, "    - Solder mask: green LPI\n");
    off += snprintf(buf+off, sizeof(buf)-off, "    - Silkscreen: white\n");
    off += snprintf(buf+off, sizeof(buf)-off, "    - IPC Class 2\n");
    off += snprintf(buf+off, sizeof(buf)-off, "========================================\n");
    return str_dup(buf);
}

char *fabdraw_gdt_feature(const char *name, float nominal, float tol_p, float tol_m) {
    char buf[128];
    snprintf(buf, sizeof(buf), "  %s: %.2f +%.2f/-%.2f mm", name, nominal, tol_p, tol_m);
    return str_dup(buf);
}

bool fabdraw_export_manufacturing(Pcb *pcb, const char *output_dir) {
    if (!pcb || !output_dir) return false;
    gerber_write_all(pcb, output_dir);
    char path[512];
    snprintf(path, sizeof(path), "%s/%s-fab.txt", output_dir, pcb->name ? pcb->name : "board");
    char *report = fabdraw_report(pcb);
    FILE *f = fopen(path, "w");
    if (f) { fprintf(f, "%s", report); fclose(f); }
    free(report);
    return true;
}
