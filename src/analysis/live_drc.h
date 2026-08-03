#pragma once
#include "../model/project.h"
#include <raylib.h>

typedef struct {
    float min_track_width;
    float min_clearance;
    float min_track_pad;
    float min_via_drill;
    float min_via_annular;
    float min_hole_hole;
    float min_silkscreen;
    int enabled;
} DrcRules;

typedef enum { DRC_CLEAR, DRC_WARNING, DRC_ERROR } DrcSeverity;

typedef struct {
    V2 pos;
    V2 size;
    DrcSeverity severity;
    char message[128];
    int track_idx;
    int via_idx;
} DrcViolation;

typedef struct {
    DrcRules rules;
    DrcViolation *violations;
    int nviolations;
    int last_check_frame;
    float glow_timer;
    int highlighted_violation;
} DrcEngine;

void drc_init(DrcEngine *drc);
void drc_set_rules_default(DrcEngine *drc);
void live_drc_check(DrcEngine *drc, const Pcb *pcb);
void drc_render_overlay(const DrcEngine *drc, const Pcb *pcb, Rectangle viewport, Vector2 pan, float zoom);
int drc_violation_count(const DrcEngine *drc, DrcSeverity min_sev);
char *drc_report(const DrcEngine *drc);
