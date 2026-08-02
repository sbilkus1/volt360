#pragma once
// erc.h - Electrical Rules Check for schematics.
#include <stdbool.h>
#include "../model/project.h"

typedef struct {
    int level;        // 0=OK, 1=WARN, 2=ERR
    char message[256];
} ErcIssue;

typedef struct {
    ErcIssue *issues;
    int nissues;
} ErcReport;

void erc_report_free(ErcReport *r);

// Run ERC on a schematic.
ErcReport erc_check(Schematic *s);
