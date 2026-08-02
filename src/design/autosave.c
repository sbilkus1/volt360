#include "autosave.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static int g_interval = 300;
static time_t g_last_save = 0;

bool autosave_tick(Project *p) {
    if (!p || !p->path || !p->path[0]) return false;
    time_t now = time(NULL);
    if (now - g_last_save < g_interval) return false;
    g_last_save = now;
    return project_save(p);
}

void autosave_set_interval(int seconds) { g_interval = seconds > 0 ? seconds : 300; }
int autosave_seconds_since_save(void) { return (int)(time(NULL) - g_last_save); }

Project *template_create(const char *name, const char *path, Template t) {
    Project *p = (Project *)calloc(1, sizeof(Project));
    project_init(p, name ? name : "Untitled", path ? path : ".");
    (void)t;
    return p;
}

char *farm_capacity_report(Farm *f) {
    if (!f) return str_dup("no farm");
    char buf[1024]; int off = 0;
    int idle = 0, busy = 0;
    for (int i = 0; i < f->n_printers; i++)
        if (f->printers[i].busy) busy++; else idle++;
    int queued = 0;
    for (int i = 0; i < f->n_jobs; i++)
        if (strcmp(f->jobs[i].status, "queued") == 0) queued++;
    float est_hours = 0;
    for (int i = 0; i < f->n_jobs; i++)
        if (strcmp(f->jobs[i].status, "queued") == 0) est_hours += f->jobs[i].est_time_s / 3600.0f;
    float throughput = idle > 0 ? est_hours / (float)idle : est_hours;

    off += snprintf(buf+off, sizeof(buf)-off,
        "=== CAPACITY PLANNING ===\n"
        "Printers: %d total (%d busy, %d idle)\n"
        "Queued jobs: %d (est %.1f hours)\n"
        "With %d idle printers: ~%.1f hours to clear queue\n"
        "Daily capacity: ~%d jobs/day (at 2hr avg)\n",
        f->n_printers, busy, idle, queued, est_hours, idle, throughput, idle * 12);
    return str_dup(buf);
}

int farm_reserve_slot(Farm *f, int printer_index, float hours_from_now, float duration) {
    (void)f; (void)printer_index; (void)hours_from_now; (void)duration;
    return -1; // stub: would need reservation data structure
}

char *farm_reservations_report(Farm *f) {
    (void)f;
    return str_dup("=== RESERVATIONS ===\nNo reservations scheduled\n(Print reservations stub)");
}
