#pragma once
#include "../model/project.h"

typedef struct {
    int gerber_copper_top;
    int gerber_copper_bottom;
    int gerber_copper_inner[8];
    int gerber_soldermask_top;
    int gerber_soldermask_bottom;
    int gerber_silkscreen_top;
    int gerber_silkscreen_bottom;
    int gerber_outline;
    int gerber_paste_top;
    int gerber_paste_bottom;
    int excellon_drill;
    int excellon_slot;
    int bom_csv;
    int pick_place_csv;
    int assembly_drawing;
    int ipc356;
    int readme;
} FabExportOpts;

typedef struct {
    char *zip_path;
    char **files_included;
    int nfiles;
    int success;
    char error[256];
} FabExportResult;

void fab_export_opts_default(FabExportOpts *opts);
FabExportResult fab_export_generate(const Pcb *pcb, const FabExportOpts *opts, const char *output_dir);
void fab_export_result_free(FabExportResult *r);
