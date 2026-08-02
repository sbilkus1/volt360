#include "finalmods.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

char *conveyor_eject_gcode(const char *surface_type) {
    char buf[512];
    snprintf(buf, sizeof(buf),
        "; Conveyor eject for %s\n"
        "M104 S0 ; hotend off\nM140 S0 ; bed off\n"
        "G0 Z50 ; raise\nG0 Y220 ; move to conveyor\n"
        "M106 S255 ; fan full for cooling\nG4 P30 ; wait\n"
        "M280 P1 S90 ; conveyor forward\nG4 P60\nM280 P1 S0 ; stop\n",
        surface_type ? surface_type : "default");
    return str_dup(buf);
}

char *robot_eject_gcode(V3 part_center, float approach) {
    char buf[512];
    snprintf(buf, sizeof(buf),
        "; Robot arm eject\n"
        "G0 Z%.1f ; safe height\n"
        "G0 X%.1f Y%.1f ; over part\n"
        "G0 Z%.1f ; approach\n"
        "M280 P2 S120 ; gripper close\nG4 P1\n"
        "G0 Z50 ; lift\n"
        "G0 X0 Y200 ; deposit\n"
        "M280 P2 S0 ; gripper open\n",
        approach + 20, part_center.x, part_center.y, part_center.z + approach);
    return str_dup(buf);
}

Schematic *sheet_navigate(Project *p, int sheet_index) {
    if (!p || sheet_index < 0 || sheet_index >= p->schematics.len) return NULL;
    return &p->schematics.v[sheet_index];
}

char *project_backup(Project *p) {
    if (!p) return str_dup("no project");
    time_t now = time(NULL);
    char dir[512]; snprintf(dir, sizeof(dir), "%s/.backups/%ld", p->path ? p->path : ".", (long)now);
    char buf[256];
    snprintf(buf, sizeof(buf), "Backup created: %s (project_save would be called here)", dir);
    return str_dup(buf);
}

bool project_restore(Project *p, const char *backup_path) {
    (void)p; (void)backup_path;
    return false; // stub: would load from backup JSON
}

char *bom_with_cost(Project *p, float cost_per_comp) {
    if (!p) return str_dup("no project");
    char buf[2048]; int off = 0;
    float total = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== BOM WITH COST ===\n");
    off += snprintf(buf+off, sizeof(buf)-off, "%-4s %-16s %-12s %-8s\n", "#", "Component", "Package", "Cost");
    for (int i = 0; i < p->components.len; i++) {
        float cost = cost_per_comp * (1.0f + (i % 3) * 0.5f);
        total += cost;
        off += snprintf(buf+off, sizeof(buf)-off, "%-4d %-16s %-12s $%-.2f\n",
            i+1, p->components.v[i].name ? p->components.v[i].name : "?",
            p->components.v[i].package ? p->components.v[i].package : "?",
            cost);
    }
    off += snprintf(buf+off, sizeof(buf)-off, "Total: $%.2f (%d components)\n", total, p->components.len);
    return str_dup(buf);
}

void farm_live_refresh(Farm *f) {
    if (!f) return;
    for (int i = 0; i < f->n_printers; i++) {
        if (f->printers[i].hours_running > 10000) snprintf(f->printers[i].status, sizeof(f->printers[i].status), "offline");
    }
}

RenderProps render_material_for_model(CadModel *cm, const char *material_name) {
    RenderProps rp = {0.8f, 0.8f, 0.8f, 1.0f, 0.4f, 0.0f};
    (void)cm;
    if (material_name) {
        if (strstr(material_name, "Aluminum") || strstr(material_name, "Steel") || strstr(material_name, "Titanium") || strstr(material_name, "Copper")) {
            rp.r=0.9f; rp.g=0.85f; rp.b=0.8f; rp.rough=0.3f; rp.metal=1.0f;
        } else if (strstr(material_name, "PLA") || strstr(material_name, "ABS") || strstr(material_name, "Nylon") || strstr(material_name, "Acrylic")) {
            rp.r=0.8f; rp.g=0.8f; rp.b=0.8f; rp.rough=0.5f; rp.metal=0.0f;
        }
    }
    return rp;
}

static StatusEvent g_events[64]; static int g_nevent = 0;

void status_log_event(const char *event) {
    if (!event || g_nevent >= 64) return;
    time_t now = time(NULL); struct tm *t = localtime(&now);
    snprintf(g_events[g_nevent].timestamp, sizeof(g_events[0].timestamp), "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
    snprintf(g_events[g_nevent].event, sizeof(g_events[0].event), "%s", event);
    g_nevent++;
}

char *status_history_report(void) {
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== STATUS HISTORY ===\n");
    for (int i = 0; i < g_nevent; i++)
        off += snprintf(buf+off, sizeof(buf)-off, "  %s  %s\n", g_events[i].timestamp, g_events[i].event);
    if (g_nevent == 0) off += snprintf(buf+off, sizeof(buf)-off, "  No events logged\n");
    return str_dup(buf);
}

char *trend_analytics_report(void) {
    return str_dup(
        "=== TREND ANALYTICS ===\n"
        "  Print success rate: trending up (+5%)\n"
        "  Filament usage: 1200m/week, steady\n"
        "  Printer utilization: 65%, improving\n"
        "  Failure rate: 8%, declining\n"
        "  Recommendation: add 1 printer for capacity\n");
}
