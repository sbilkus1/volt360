#include "farmdash.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

char *farm_utilization_report(Farm *f) {
    if (!f || f->n_printers < 1) return str_dup("no printers");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== MACHINE UTILIZATION ===\n");
    float total_util = 0;
    for (int i = 0; i < f->n_printers; i++) {
        float util = f->printers[i].hours_running > 0 ? 100.0f : 0;
        // rough estimate: busy vs idle
        if (f->printers[i].busy) util = 100.0f;
        else if (f->printers[i].hours_running > 0) util = f->printers[i].hours_running / (f->printers[i].hours_running + 100.0f) * 100.0f;
        total_util += util;
        off += snprintf(buf+off, sizeof(buf)-off, "  %-20s %.0f%%\n", f->printers[i].name, util);
    }
    float avg = total_util / (float)f->n_printers;
    off += snprintf(buf+off, sizeof(buf)-off, "  Average: %.0f%%\n", avg);
    return str_dup(buf);
}

char *farm_efficiency_report(Farm *f) {
    if (!f) return str_dup("no farm");
    char buf[1024]; int off = 0;
    int done = 0, total = 0;
    float total_fil = 0, total_hours = 0;
    for (int i = 0; i < f->n_jobs; i++) {
        total++;
        if (strcmp(f->jobs[i].status, "done") == 0) { done++; total_fil += f->jobs[i].filament_mm; total_hours += f->jobs[i].est_time_s / 3600.0f; }
    }
    float rate = total_hours > 0 ? done / total_hours : 0;
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== PRODUCTION EFFICIENCY ===\n"
        "Total jobs: %d  Done: %d (%.0f%%)\n"
        "Filament used: %.0f m\n"
        "Print hours: %.1f h\n"
        "Throughput: %.1f jobs/hour\n"
        "Success rate: %.0f%%\n",
        total, done, total>0?done*100.0f/total:0,
        total_fil/1000.0f, total_hours, rate,
        total>0?done*100.0f/total:0);
    return str_dup(buf);
}

char *farm_downtime_report(Farm *f) {
    if (!f) return str_dup("no farm");
    char buf[1024]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== DOWNTIME ANALYSIS ===\n");
    for (int i = 0; i < f->n_printers; i++) {
        float downtime = f->printers[i].busy ? 0 : (100.0f - f->printers[i].hours_running / (f->printers[i].hours_running + 1.0f) * 100.0f);
        off += snprintf(buf+off, sizeof(buf)-off, "  %-20s %s  downtime ~%.0f%%\n",
            f->printers[i].name, f->printers[i].busy ? "busy" : "idle", f->printers[i].busy ? 0.0f : downtime);
    }
    return str_dup(buf);
}

char *eye_diagram_report(float bitrate, float rise_time, float jitter, float noise) {
    char buf[512];
    float T_bit = 1.0f / bitrate;
    float eye_width = T_bit - jitter * 1e-12f;
    float eye_height = 1.0f - noise * 2e-3f;
    snprintf(buf, sizeof(buf),
        "=== EYE DIAGRAM ===\n"
        "Bitrate: %.1f Gbps  Period: %.1f ps\n"
        "Rise time: %.1f ps  Jitter: %.1f ps  Noise: %.1f mV\n"
        "Eye width: %.1f ps (%.0f%%)\n"
        "Eye height: %.3f V (%.0f%%)\n"
        "Quality: %s\n",
        bitrate, T_bit*1e12f, rise_time, jitter, noise,
        eye_width*1e12f, eye_width/T_bit*100.0f, eye_height, eye_height*100.0f,
        eye_width/T_bit > 0.5f ? "Good" : "Marginal");
    return str_dup(buf);
}
