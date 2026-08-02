#include "printquality.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int quality_score(const SliceResult *sr, const SliceSettings *cfg) {
    if (!sr || !cfg) return 0;
    int score = 70; // baseline
    // layer height - finer = better
    if (cfg->layer_height < 0.1f) score += 10;
    else if (cfg->layer_height > 0.3f) score -= 10;
    // perimeters - more = better surface
    if (cfg->perimeters >= 3) score += 5;
    // speed - slower = better
    if (cfg->print_speed < 40) score += 5;
    else if (cfg->print_speed > 100) score -= 10;
    // infill density
    if (cfg->infill_density >= 30) score += 5;
    // filament usage sanity
    if (sr->filament_mm > 1000) score -= 5;
    if (sr->est_time_s > 3600) score -= 3;
    if (score < 0) score = 0; if (score > 100) score = 100;
    return score;
}

int detect_nozzle_clog(float expected_flow, float actual_flow) {
    if (expected_flow <= 0) return 0;
    float ratio = actual_flow / expected_flow;
    if (ratio < 0.3f) return 2;  // definite clog
    if (ratio < 0.6f) return 1;  // possible clog
    return 0;
}

int detect_layer_shift(V2 prev, V2 curr, float threshold) {
    float dx = curr.x - prev.x, dy = curr.y - prev.y;
    float shift = sqrtf(dx*dx + dy*dy);
    if (shift > threshold * 3.0f) return 2;
    if (shift > threshold) return 1;
    return 0;
}

int detect_warping(int expected, int actual) {
    if (expected <= 0) return 0;
    float ratio = (float)actual / (float)expected;
    if (ratio < 0.5f) return 2;  // severe warping
    if (ratio < 0.8f) return 1;  // minor warping
    return 0;
}

char *failure_action(int clog, int shift, int warp) {
    if (clog == 2 || shift == 2 || warp == 2)
        return str_dup("Action: CANCEL_PRINT (severe failure)");
    if (clog == 1 || shift == 1 || warp == 1)
        return str_dup("Action: PAUSE (check print)");
    return str_dup("Action: continue (no issues)");
}

float optimize_color_swaps(const int *colors, int n, float waste_per_swap) {
    if (n < 2 || !colors) return 0;
    float total = 0; int prev = colors[0];
    for (int i = 1; i < n; i++) {
        if (colors[i] != prev) total += waste_per_swap;
        else total += waste_per_swap * 0.2f;
        prev = colors[i];
    }
    return total;
}

int cooling_for_overhang(float overhang_deg) {
    if (overhang_deg < 20) return 128;
    if (overhang_deg < 45) return 192;
    if (overhang_deg < 60) return 255;
    return 255;
}

float refine_time_estimate(const SliceResult *sr, const SliceSettings *cfg) {
    if (!sr || !cfg) return 0;
    float base = sr->total_dist / (cfg->print_speed > 0 ? cfg->print_speed : 60);
    float travel_time = sr->total_paths * 0.5f;
    float z_time = sr->nlayers * 2.0f;
    float heat_time = 60.0f;
    return base + travel_time + z_time + heat_time;
}

static SlicerProfile g_profiles[5];

static void init_profiles(void) {
    static int done = 0; if (done) return; done = 1;
    slice_settings_default(&g_profiles[0].settings); g_profiles[0].settings.layer_height=0.3f; g_profiles[0].settings.line_width=0.5f; g_profiles[0].settings.perimeters=1; g_profiles[0].settings.infill_density=10; g_profiles[0].settings.print_speed=80; snprintf(g_profiles[0].name,32,"Draft");
    slice_settings_default(&g_profiles[1].settings); g_profiles[1].settings.layer_height=0.2f; g_profiles[1].settings.line_width=0.48f; g_profiles[1].settings.perimeters=2; g_profiles[1].settings.infill_density=15; g_profiles[1].settings.print_speed=60; snprintf(g_profiles[1].name,32,"Standard");
    slice_settings_default(&g_profiles[2].settings); g_profiles[2].settings.layer_height=0.1f; g_profiles[2].settings.line_width=0.4f; g_profiles[2].settings.perimeters=3; g_profiles[2].settings.infill_density=20; g_profiles[2].settings.print_speed=40; snprintf(g_profiles[2].name,32,"Fine");
    slice_settings_default(&g_profiles[3].settings); g_profiles[3].settings.layer_height=0.05f; g_profiles[3].settings.line_width=0.35f; g_profiles[3].settings.perimeters=4; g_profiles[3].settings.infill_density=25; g_profiles[3].settings.print_speed=30; snprintf(g_profiles[3].name,32,"UltraFine");
    slice_settings_default(&g_profiles[4].settings); g_profiles[4].settings.layer_height=0.2f; g_profiles[4].settings.line_width=0.6f; g_profiles[4].settings.perimeters=4; g_profiles[4].settings.infill_density=40; g_profiles[4].settings.print_speed=50; snprintf(g_profiles[4].name,32,"Strength");
}

SlicerProfile *profile_presets(void) { init_profiles(); return g_profiles; }
int profile_count(void) { init_profiles(); return 5; }
SlicerProfile *profile_get(int idx) { init_profiles(); return idx>=0&&idx<5 ? &g_profiles[idx] : NULL; }

char *profile_list_text(void) {
    char buf[1024]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== SLICER PROFILES ===\n");
    for (int i = 0; i < 5; i++) {
        SliceSettings *s = &g_profiles[i].settings;
        off += snprintf(buf+off, sizeof(buf)-off, "  %-12s LH=%.2f LW=%.2f P=%d I=%d%% S=%.0f\n",
            g_profiles[i].name, s->layer_height, s->line_width, s->perimeters, s->infill_density, s->print_speed);
    }
    return str_dup(buf);
}
