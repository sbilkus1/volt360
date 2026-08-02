#include "pcbcalc.h"
#include <math.h>

// Copper thickness per oz (mm)
static float copper_thickness(float oz) { return oz * 0.035f; }

// IPC-2221 external trace current formula: I = k * dT^0.44 * A^0.725
// k = 0.048 for external, 0.024 for internal
static float ipc_current(float area_mm2, float dT, float k) {
    return k * powf(dT, 0.44f) * powf(area_mm2, 0.725f);
}

float pcbcalc_current_capacity(float width_mm, float copper_oz, float temp_rise) {
    if (width_mm <= 0 || copper_oz <= 0) return 0;
    float area = width_mm * copper_thickness(copper_oz);
    return ipc_current(area, temp_rise > 0 ? temp_rise : 10.0f, 0.048f);
}

float pcbcalc_track_width(float current_a, float copper_oz, float temp_rise) {
    if (current_a <= 0 || copper_oz <= 0) return 0;
    float dT = temp_rise > 0 ? temp_rise : 10.0f;
    // inverse: area = (I / (k * dT^0.44))^(1/0.725)
    float area = powf(current_a / (0.048f * powf(dT, 0.44f)), 1.0f / 0.725f);
    return area / copper_thickness(copper_oz);
}

float pcbcalc_track_resistance(float width_mm, float length_mm, float copper_oz) {
    if (width_mm <= 0 || length_mm <= 0 || copper_oz <= 0) return 0;
    float area = width_mm * copper_thickness(copper_oz); // mm²
    // resistivity of copper = 1.72e-8 ohm*m = 0.0172 ohm*mm²/m
    // R = rho * L / A
    float rho = 0.0172f; // ohm*mm²/mm -> ohm for L in mm
    return rho * length_mm / (area * 1000.0f); // convert to ohms
}

float pcbcalc_voltage_drop(float current_a, float width_mm, float length_mm, float copper_oz) {
    float R = pcbcalc_track_resistance(width_mm, length_mm, copper_oz);
    return current_a * R;
}

float pcbcalc_trace_temp(float current_a, float width_mm, float copper_oz, float ambient_c) {
    float area = width_mm * copper_thickness(copper_oz);
    if (area <= 0) return ambient_c;
    // iterative: solve for dT where I = k * dT^0.44 * A^0.725
    // dT = (I / (k * A^0.725))^(1/0.44)
    float dT = powf(current_a / (0.048f * powf(area, 0.725f)), 1.0f / 0.44f);
    return ambient_c + dT;
}
