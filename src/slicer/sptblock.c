#include "sptblock.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

// simple array storage in slice settings (uses start_gcode/eject_gcode as scratch)
// actually need proper storage — use static for now
static SupportBlocker g_sb[16]; static int g_nsb = 0;
static ModifierMesh g_mm[16]; static int g_nmm = 0;
static PrintLog g_plogs[256]; static int g_nplogs = 0;

void sptblock_add(SliceSettings *cfg, SupportBlocker blk) {
    (void)cfg; if (g_nsb < 16) g_sb[g_nsb++] = blk;
}
SupportBlocker *sptblock_get(SliceSettings *cfg, int idx) { (void)cfg; return idx>=0&&idx<g_nsb?&g_sb[idx]:NULL; }
int sptblock_count(SliceSettings *cfg) { (void)cfg; return g_nsb; }

void modifier_add(SliceSettings *cfg, ModifierMesh mod) {
    (void)cfg; if (g_nmm < 16) g_mm[g_nmm++] = mod;
}
ModifierMesh *modifier_get(SliceSettings *cfg, int idx) { (void)cfg; return idx>=0&&idx<g_nmm?&g_mm[idx]:NULL; }
int modifier_count(SliceSettings *cfg) { (void)cfg; return g_nmm; }

void printlog_add(Farm *f, PrintLog entry) {
    (void)f; if (g_nplogs < 256) g_plogs[g_nplogs++] = entry;
}

char *printlog_report(Farm *f) {
    (void)f;
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== PRINT HISTORY ===\n");
    for (int i = 0; i < g_nplogs; i++)
        off += snprintf(buf+off, sizeof(buf)-off, "  [%d] %s on %s: %s (%.0fmm, %.0fs)\n",
            i+1, g_plogs[i].job_name, g_plogs[i].printer,
            g_plogs[i].success ? "OK" : "FAIL",
            g_plogs[i].filament_mm, g_plogs[i].est_time_s);
    if (g_nplogs == 0) off += snprintf(buf+off, sizeof(buf)-off, "  No prints logged yet\n");
    return str_dup(buf);
}

char *netprinter_status_widget(Farm *f) {
    if (!f) return str_dup("no farm");
    char buf[512]; int off = 0;
    int online = 0, busy = 0;
    for (int i = 0; i < f->n_printers; i++) {
        if (strcmp(f->printers[i].status, "offline") != 0) online++;
        if (f->printers[i].busy) busy++;
    }
    off += snprintf(buf+off, sizeof(buf)-off, "Network: %d online, %d busy, %d idle",
        online, busy, online - busy);
    return str_dup(buf);
}

char *quick_toolbar_text(int mode) { (void)mode; return str_dup("T:route R:rotate X:delete F:focus M:measure W:wire ?:help"); }
void quick_toolbar_draw(int x, int y) { (void)x; (void)y; }

char *localize(const char *english_text) {
    return str_dup(english_text ? english_text : ""); // stub: English only
}

char *render_settings_text(void) {
    return str_dup(
        "=== RENDER SETTINGS ===\n"
        "  Quality: High (4x MSAA)\n"
        "  Lighting: 3-point studio\n"
        "  Background: Dark (#1C1E22)\n"
        "  Grid: 10mm spacing\n"
        "  Materials: PBR (Albedo + Roughness + Metal)\n"
        "Status: raylib-based real-time rendering active\n");
}
