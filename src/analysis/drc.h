#pragma once
// drc.h - PCB Design Rule Checks: clearance, width, outline containment.
#include <stdbool.h>
#include "../model/project.h"

typedef struct {
    int level;           // 0=OK, 1=WARNING, 2=ERROR
    char message[256];
    V2 pos;              // location of violation
} DrcIssue;

typedef struct {
    DrcIssue *issues;
    int nissues;
} DrcReport;

void drc_report_free(DrcReport *r);

// Run all checks on a PCB with given rules.
DrcReport drc_check(Pcb *pcb, float min_track_width, float min_clearance, float min_annular_ring);
