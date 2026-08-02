#pragma once
// fit.h - mechanical / electrical fit analysis
#include "../model/project.h"

typedef enum { FIT_OK = 0, FIT_WARNING, FIT_ERROR } FitLevel;

typedef struct {
    FitLevel level;
    char *message;      // malloc'd human-readable report line
    double metric;      // overlap depth / clearance, mm
} FitIssue;

typedef struct {
    FitIssue *issues; int nissues;
    ARRAY(int) idx;     // stable sort order of issues by severity
} FitReport;

// Populates report with issues. Caller must fit_report_free.
// - CAD box-vs-box overlap within assembly (mesh bboxes)
// - part height vs assembly Z clearance
void fit_analyze_assembly(Project *p, Assembly *a, FitReport *rep);

// Populates report with PCB fit issues:
// - footprint placement inside board outline
// - footprint overlap between parts
void fit_analyze_pcb(Project *p, Pcb *pcb, FitReport *rep);

void fit_report_free(FitReport *rep);

// helper: point-in-polygon (ray casting) for board outline checks
bool point_in_poly(V2 pt, V2 *poly, int n);
// minimum distance from point to polygon boundary (mm)
float dist_to_poly(V2 pt, V2 *poly, int n);
