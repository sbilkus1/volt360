#include "signalint.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

float si_microstrip_z0(float w, float h, float er) {
    if (w <= 0 || h <= 0 || er <= 0) return 50;
    float wh = w / h;
    float eff = (er + 1.0f) * 0.5f + (er - 1.0f) * 0.5f / sqrtf(1.0f + 12.0f * h / w);
    if (wh < 1.0f) return 60.0f / sqrtf(eff) * logf(8.0f * h / w + w / (4.0f * h));
    else return 120.0f * (float)M_PI / (sqrtf(eff) * (wh + 1.393f + 0.667f * logf(wh + 1.444f)));
}

float si_stripline_z0(float w, float h, float er) {
    if (w <= 0 || h <= 0 || er <= 0) return 50;
    float wh = w / h;
    if (wh < 0.35f) return 60.0f / sqrtf(er) * logf(4.0f * h / w);
    else return 94.0f / (sqrtf(er) * (wh / (1.0f - h * 0.35f / (h * 2.0f)) + 1.9f));
}

float si_diff_pair_z0(float w, float s, float h, float er) {
    float z0_single = si_microstrip_z0(w, h, er);
    return 2.0f * z0_single * (1.0f - 0.48f * expf(-0.96f * s / h));
}

float si_crosstalk_ne(float aggr_len, float vict_len, float spacing) {
    float L = aggr_len < vict_len ? aggr_len : vict_len;
    if (spacing <= 0.1f) spacing = 0.1f;
    return 0.15f * L / (spacing * 10.0f);
}

float si_reflection_coeff(float z1, float z2) {
    return fabsf((z2 - z1) / (z2 + z1));
}

char *si_report(Pcb *pcb) {
    if (!pcb) return str_dup("no PCB");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== SIGNAL INTEGRITY REPORT ===\n"
        "Board: %s\n"
        "Tracks: %d    Vias: %d\n\n",
        pcb->name ? pcb->name : "?", pcb->ntracks, pcb->nvias);

    int high_speed = 0, matched = 0;
    for (int i = 0; i < pcb->ntracks; i++) {
        if (pcb->tracks[i].width < 0.2f) high_speed++;
        if (pcb->tracks[i].net && strstr(pcb->tracks[i].net, "DP")) matched++;
    }
    off += snprintf(buf+off, sizeof(buf)-off,
        "High-speed traces (<0.2mm): %d\n"
        "Differential pairs detected: %d\n\n",
        high_speed, matched);

    float z0 = si_microstrip_z0(0.3f, 0.2f, 4.2f);
    off += snprintf(buf+off, sizeof(buf)-off,
        "Microstrip Z0 (0.3mm/0.2mm H, er=4.2): %.1f ohm\n"
        "Diff pair Z0 (0.3mm, 0.3mm gap): %.1f ohm\n"
        "Reflection (50 vs Z0): %.3f\n",
        z0, si_diff_pair_z0(0.3f, 0.3f, 0.2f, 4.2f),
        si_reflection_coeff(50.0f, z0));
    return str_dup(buf);
}
