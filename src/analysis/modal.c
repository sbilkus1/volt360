#include "modal.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

float modal_beam_natural_freq(float E_GPa, float I_mm4, float L_mm, float mass_kg) {
    if (E_GPa <= 0 || I_mm4 <= 0 || L_mm <= 0 || mass_kg <= 0) return 0;
    float E_Pa = E_GPa * 1e9f;
    float I_m4 = I_mm4 * 1e-12f;
    float L_m = L_mm * 1e-3f;
    return (float)(M_PI * M_PI) * sqrtf(E_Pa * I_m4 / (mass_kg * L_m * L_m * L_m)) / (2.0f * (float)M_PI);
}

float modal_cantilever_freq(float E_GPa, float I_mm4, float L_mm, float mass_kg) {
    if (E_GPa <= 0 || I_mm4 <= 0 || L_mm <= 0 || mass_kg <= 0) return 0;
    float E_Pa = E_GPa * 1e9f;
    float I_m4 = I_mm4 * 1e-12f;
    float L_m = L_mm * 1e-3f;
    return 1.875f * 1.875f * sqrtf(E_Pa * I_m4 / (mass_kg * L_m * L_m * L_m)) / (2.0f * (float)M_PI);
}

int modal_plate_freqs(float a, float b, float t, float E_GPa, float nu, float density, float *freqs, int N) {
    float D = E_GPa * 1e9f * t*t*t * 1e-27f / (12.0f * (1.0f - nu*nu));
    float rho_h = density * 1000.0f * t * 1e-3f;
    if (rho_h <= 0 || D <= 0) return 0;
    int modes = 0;
    for (int m = 1; m <= 5 && modes < N; m++)
        for (int n = 1; n <= 5 && modes < N; n++) {
            float lambda = (float)(m*m) / (a*a) + (float)(n*n) / (b*b);
            freqs[modes++] = (float)(M_PI*M_PI) * lambda * sqrtf(D / rho_h) * 1e6f / (2.0f*(float)M_PI);
        }
    return modes;
}

float modal_spring_mass_freq(float k_N_mm, float mass_kg) {
    if (k_N_mm <= 0 || mass_kg <= 0) return 0;
    return sqrtf(k_N_mm * 1000.0f / mass_kg) / (2.0f * (float)M_PI);
}

char *modal_report(const char *name, float E_GPa, float density, float L, float w, float h) {
    char buf[1024]; int off = 0;
    float I = w * h * h * h / 12.0f;
    float area = w * h;
    float vol = L * area;
    float mass_kg = density * vol * 1e-9f;

    float f1 = modal_beam_natural_freq(E_GPa, I, L, mass_kg);
    float f_cant = modal_cantilever_freq(E_GPa, I, L, mass_kg);

    off += snprintf(buf+off, sizeof(buf)-off,
        "=== MODAL ANALYSIS: %s ===\n"
        "Dimensions: %.0f x %.0f x %.0f mm\n"
        "Material: E=%.0f GPa, density=%.0f kg/m3\n"
        "Mass: %.4f kg | I: %.1f mm4\n"
        "Simply-supported f1: %.1f Hz\n"
        "Cantilever f1: %.1f Hz\n",
        name ? name : "?", L, w, h, E_GPa, density, mass_kg, I, f1, f_cant);

    float plate_f[4]; int np = modal_plate_freqs(L, w, h, E_GPa, 0.33f, density, plate_f, 4);
    off += snprintf(buf+off, sizeof(buf)-off, "Plate modes:");
    for (int i = 0; i < np && i < 4; i++) off += snprintf(buf+off, sizeof(buf)-off, " %.0f", plate_f[i]);
    off += snprintf(buf+off, sizeof(buf)-off, " Hz\n");
    return str_dup(buf);
}
