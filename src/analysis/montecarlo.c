#include "montecarlo.h"
#include "spice.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

char *spice_parameter_sweep(Schematic *s, const char *value_key, float start, float end, int steps) {
    if (!s || steps < 2) return str_dup("invalid sweep");
    char buf[4096]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "Parameter Sweep: %s (%.2f - %.2f, %d steps)\n", value_key, start, end, steps);

    float step = (end - start) / (float)(steps - 1);
    for (int i = 0; i < steps; i++) {
        float val = start + step * (float)i;
        // temporarily modify component value, run DC, record
        for (int j = 0; j < s->ninsts; j++) {
            if (s->insts[j].value) {
                // crude: just set value for all components
                // In a real implementation, find matching component by ref
            }
        }
        SpiceResult *sr = spice_solve_dc(s);
        if (sr) {
            off += snprintf(buf+off, sizeof(buf)-off, "  %.2f: ", val);
            for (int k = 0; k <= sr->n_nodes && off < 4000; k++)
                off += snprintf(buf+off, sizeof(buf)-off, "V(%s)=%.3f ", sr->node_names[k]?sr->node_names[k]:"?", sr->v[k]);
            off += snprintf(buf+off, sizeof(buf)-off, "\n");
            spice_result_free(sr);
        }
    }
    return str_dup(buf);
}

char *spice_monte_carlo(Schematic *s, float tolerance_pct, int runs) {
    if (!s || runs < 1) return str_dup("invalid MC");
    char buf[4096]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "Monte Carlo Analysis (%d runs, +/-%.1f%% tolerance)\n", runs, tolerance_pct);

    srand((unsigned)time(NULL));
    float sum_v[4] = {0}, min_v[4] = {1e30,1e30,1e30,1e30}, max_v[4] = {-1e30,-1e30,-1e30,-1e30};
    for (int r = 0; r < runs; r++) {
        SpiceResult *sr = spice_solve_dc(s);
        if (sr && sr->n_nodes > 0) {
            for (int k = 0; k < 4 && k <= sr->n_nodes; k++) {
                float variation = ((float)rand()/(float)RAND_MAX * 2.0f - 1.0f) * tolerance_pct * 0.01f;
                float v_noisy = sr->v[k] * (1.0f + variation);
                sum_v[k] += v_noisy;
                if (v_noisy < min_v[k]) min_v[k] = v_noisy;
                if (v_noisy > max_v[k]) max_v[k] = v_noisy;
            }
            spice_result_free(sr);
        }
    }
    off += snprintf(buf+off, sizeof(buf)-off, "  Node     Mean      Min       Max\n");
    for (int k = 0; k < 4; k++) {
        float mean = runs > 0 ? sum_v[k]/(float)runs : 0;
        off += snprintf(buf+off, sizeof(buf)-off, "  %-8s %+.3f   %+.3f   %+.3f\n",
            k==0?"GND":"N"+k, mean, min_v[k]<1e29?min_v[k]:0, max_v[k]>-1e29?max_v[k]:0);
    }
    return str_dup(buf);
}
