#include "energy.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

float energy_estimate_kwh(float hours, float avg_power_w) {
    return hours * avg_power_w / 1000.0f;
}

float energy_cost_total(Farm *f, float cost_per_kwh, float avg_power_w) {
    if (!f) return 0;
    float total_hrs = 0;
    for (int i = 0; i < f->n_printers; i++) total_hrs += f->printers[i].hours_running;
    return energy_estimate_kwh(total_hrs, avg_power_w) * cost_per_kwh;
}

char *farm_failure_report(Farm *f) {
    if (!f) return str_dup("no farm");
    char buf[2048]; int off = 0;
    int total = 0, failed = 0, running = 0, done = 0, queued = 0;
    for (int i = 0; i < f->n_jobs; i++) {
        total++;
        if (strcmp(f->jobs[i].status, "failed") == 0) failed++;
        else if (strcmp(f->jobs[i].status, "running") == 0) running++;
        else if (strcmp(f->jobs[i].status, "done") == 0) done++;
        else queued++;
    }
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== FAILURE ANALYTICS ===\n"
        "Total jobs: %d\n"
        "  Done:    %d (%.0f%%)\n"
        "  Running: %d\n"
        "  Queued:  %d\n"
        "  Failed:  %d (%.0f%%)\n"
        "Success rate: %.0f%%\n",
        total, done, total>0?done*100.0f/total:0, running, queued,
        failed, total>0?failed*100.0f/total:0,
        total>0?(done)*100.0f/total:0);
    return str_dup(buf);
}

char *farm_purchase_forecast(Farm *f, float hours_per_week) {
    if (!f) return str_dup("no farm");
    char buf[1024]; int off = 0;
    float weekly_mm = 0;
    for (int i = 0; i < f->n_jobs; i++)
        if (strcmp(f->jobs[i].status, "done") == 0) weekly_mm += f->jobs[i].filament_mm;
    weekly_mm = weekly_mm > 0 ? weekly_mm * hours_per_week / 168.0f : 100.0f;

    off += snprintf(buf+off, sizeof(buf)-off,
        "=== PURCHASE FORECAST ===\n"
        "Weekly filament usage: %.0f mm (%.0f m)\n"
        "Recommendation: keep 3+ spools per type in stock\n",
        weekly_mm, weekly_mm/1000.0f);
    return str_dup(buf);
}
