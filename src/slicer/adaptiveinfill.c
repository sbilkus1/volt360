#include "adaptiveinfill.h"
#include <math.h>
#include <stdlib.h>

float adaptive_spacing(float base_spacing, float wall_spacing, float dist_from_wall) {
    if (dist_from_wall < 3.0f) return wall_spacing;
    if (dist_from_wall < 10.0f)
        return wall_spacing + (base_spacing - wall_spacing) * (dist_from_wall - 3.0f) / 7.0f;
    return base_spacing;
}

int gap_fill_segments(V2 *outer, int no, V2 *inner, int ni,
                      float line_w, float gap_max, V2 *out, int maxseg) {
    if (!outer || !inner || !out || no < 3 || ni < 3 || maxseg < 1) return 0;
    int cnt = 0;
    // for each point on outer, find closest inner point
    for (int i = 0; i < no && cnt < maxseg; i++) {
        float best_d = 1e9f; int best_j = 0;
        for (int j = 0; j < ni; j++) {
            float dx = outer[i].x - inner[j].x, dy = outer[i].y - inner[j].y;
            float d = sqrtf(dx*dx + dy*dy);
            if (d < best_d) { best_d = d; best_j = j; }
        }
        if (best_d > line_w * 1.2f && best_d < gap_max) {
            out[cnt++] = outer[i]; if (cnt < maxseg) out[cnt++] = inner[best_j];
        }
    }
    return cnt;
}

float variable_wall_thickness(float base_thickness, float factor) {
    if (factor < 0.5f) factor = 0.5f; if (factor > 2.0f) factor = 2.0f;
    return base_thickness * factor;
}

float ironing_flow_rate(float base_flow, float slope_deg) {
    if (slope_deg < 5.0f) return base_flow * 0.3f; // flat surface = light ironing
    if (slope_deg < 20.0f) return base_flow * 0.6f;
    return base_flow * 1.0f; // steep = no ironing benefit, use normal flow
}

int fan_speed_region(int base_fan, int region_type) {
    switch (region_type) {
        case 0: return base_fan;           // perimeter: normal
        case 1: return base_fan * 3 / 2;   // infill: higher (bridging)
        case 2: return 255;                // bridge: max
        case 3: return 255;                // overhang: max
        default: return base_fan;
    }
}
