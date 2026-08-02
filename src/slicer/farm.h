#pragma once
// farm.h - print farm management for Volt360 (AutoFarm3D-style).
// Multi-printer dashboard, smart scheduling, orders, failure detection,
// release-temp auto-eject + custom removal gcode, AMS auto-mapping, central
// file storage with versioning, filament tracking, and stats/analytics.
#include "slicer.h"
#include "../model/project.h"

typedef struct {
    char id[32];
    char name[64];
    char status[16];        // idle / busy / error / offline
    char current_job_id[32];
    int busy;
    char filament_type[32]; // "" = any filament accepted
    int tool_slots;         // AMS/CFS slots this printer exposes
    int total_jobs_done;
    float hours_running;
} FarmPrinter;

typedef struct {
    char id[32];
    char name[128];
    int quantity;
    int priority;           // higher = sooner
    char filament[32];      // requested filament type/color
    int color_index;        // requested AMS/CFS color index (0-based)
    int maps_to_slot;       // resolved physical slot after AMS mapping
    int eject_angle;        // per-surface removal override (0 = farm default)
    char eject_gcode[256];  // custom removal gcode ("" = auto sequence)
    char status[16];        // queued / running / done / failed
    char assigned_printer[32];
    char output_gcode[256];
    float filament_mm;
    float est_time_s;
    int finished;
} FarmJob;

typedef struct {
    char id[32];
    char name[128];
    int job_indices[256];
    int n_jobs;
    int total_units, done_units;
    char status[16];        // open / in-progress / complete
} FarmOrder;

typedef struct {
    char id[32];
    char filament[32];
    char color[16];
    float remaining_m;      // meters of filament left on the spool
    float used_m;
    char printer_id[32];
} FarmSpool;

typedef struct {
    char id[32];
    char name[128];
    char file[256];
    char tags[8][32];
    int n_tags;
    int version;
} FarmFile;

typedef struct {
    char id[32];
    char printer_name[64];
    int interval_hours;
    float last_service_hours;
    char task[128];  // e.g. "lubricate Z rods", "replace nozzle"
} FarmMaintenance;

typedef struct {
    char type[16];       // "rfid", "barcode", "qr"
    char code[64];        // tag identifier
    char spool_id[32];    // linked spool
} FarmTag;

typedef struct {
    char base_dir[512];
    FarmPrinter printers[64]; int n_printers;
    FarmJob jobs[256]; int n_jobs;
    FarmOrder orders[16]; int n_orders;
    FarmSpool spools[64]; int n_spools;
    FarmFile files[256]; int n_files;
    FarmMaintenance maint[64]; int n_maint;
    FarmTag tags[64]; int n_tags;
    int auto_eject;
    float eject_release_temp;
    int eject_angle;
    int conveyor;        // conveyor belt auto-eject
    int robot_arm;       // robot arm part removal
    int auto_bed_clean;  // automatic bed cleaning
    int part_bins;       // number of collection bins
} Farm;

void farm_init(Farm *f, const char *base_dir);
void farm_free(Farm *f);

int farm_add_printer(Farm *f, const char *name, int tool_slots, const char *filament_type);
int farm_add_job(Farm *f, const char *name, int quantity, int priority, const char *filament, int color_index);
int farm_add_order(Farm *f, const char *name, const int *job_indices, int n);
int farm_add_spool(Farm *f, const char *filament, const char *color, float remaining_m);
int farm_add_file(Farm *f, const char *name, const char *path, const char **tags, int ntags);

// Smart scheduling: route a queued job to the best idle printer (filament
// compatibility, tool slots, lowest load). Returns printer index or -1.
int farm_assign_job(Farm *f, int job, int printer_hint);

// AMS auto-mapping: resolve each pending job's requested filament/color to a
// physical slot on its printer, matching spools. Returns number mapped.
int farm_map_ams(Farm *f);

// Failure-detection heuristics (offline "QuinlyVision-style"): returns flag bits.
enum {
    FARM_ANOM_SPAGHETTI   = 1, // layer area collapses mid-print
    FARM_ANOM_FIRST_LAYER = 2, // first layer coverage missing
    FARM_ANOM_STRINGING   = 4, // many tiny retract-heavy moves
};
int farm_detect_anomalies(SliceResult *r, const SliceSettings *cfg);

// Generate one merged gcode file for a printer: all assigned jobs in priority
// order with job markers, AMS mapping, release-temp/custom eject between jobs,
// filament deduction, and completion bookkeeping.
// job_slice(job, &out) must fill out->layers (caller slices per job).
typedef int (*FarmSliceFn)(int job, SliceResult *out);
char *farm_generate_batch(Farm *f, int printer, FarmSliceFn job_slice, const char *out_path);

// Stats & analytics report (malloc'd text).
char *farm_stats_text(Farm *f);

// Scan base_dir for *.gcode / *.stl files into central storage (versioning).
int farm_scan_store(Farm *f);

// Persist farm state to base_dir/farm.json. Returns true on success.
bool farm_save(Farm *f);
bool farm_load(Farm *f, const char *base_dir);

// Maintenance & alerts
int farm_add_maintenance(Farm *f, const char *printer_name, int interval_hours, const char *task);
int farm_check_maintenance(Farm *f, char *report, int report_size); // returns count of due items
float farm_filament_remaining(Farm *f, const char *filament_type); // remaining meters
int farm_low_filament_alerts(Farm *f, float threshold_m, char *report, int report_size); // returns count

// Bulk operations: select all printers
void farm_select_all_printers(Farm *f, bool busy);
void farm_batch_pause(Farm *f);
void farm_batch_resume(Farm *f);

// SmartTags
int farm_tag_add(Farm *f, const char *target_id, const char *tag);
int farm_tag_find(Farm *f, const char *tag, char *results, int results_size); // returns match count
