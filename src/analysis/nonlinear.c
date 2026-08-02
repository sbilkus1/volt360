#include "nonlinear.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

float nonlinear_effective_stiffness(float k0, float P, float L) {
    if (k0 <= 0 || L <= 0) return k0;
    float beta = P / (k0 * L);
    if (beta >= 1.0f) return 0; // buckling
    if (beta <= -1.0f) return k0 * 2.0f; // tension doubling (approximate)
    return k0 * (1.0f - beta);
}

float nonlinear_amplification_factor(float P_over_Pcr, float imperfection, float thickness) {
    if (P_over_Pcr >= 1.0f || thickness <= 0) return 1e9f;
    if (imperfection <= 0) return 1.0f / (1.0f - P_over_Pcr);
    return imperfection / thickness / (1.0f - P_over_Pcr);
}

float nonlinear_beam_deflection(float lin_deflect, float load, float Pcr) {
    if (Pcr <= 0) return lin_deflect;
    float ratio = load / Pcr;
    if (ratio >= 0.99f) return 1e9f;
    return lin_deflect / (1.0f - ratio);
}

char *nonlinear_report(const char *name, float k0, float P, float L, float Pcr) {
    char buf[1024];
    float k_eff = nonlinear_effective_stiffness(k0, P, L);
    float amp = nonlinear_amplification_factor(P/Pcr, 0.5f, 10.0f);
    snprintf(buf, sizeof(buf),
        "=== NONLINEAR ANALYSIS: %s ===\n"
        "Linear stiffness: %.1f N/m\n"
        "Axial load: %.1f N  Length: %.3f m\n"
        "P/Pcr: %.3f  (Pcr=%.1f)\n"
        "Effective stiffness: %.1f N/m\n"
        "Amplification (0.5mm imperfection): %.2fx\n",
        name ? name : "?", k0, P, L, P/Pcr, Pcr, k_eff, amp);
    return str_dup(buf);
}
