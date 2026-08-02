#include "creality.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

int creality_status(const char *ip, int port, char *state, int state_len) {
    (void)port;
    // Creality uses MQTT over LAN, HTTP API on port 80/9999
    snprintf(state, state_len, "Creality %s: check LAN mode", ip ? ip : "unknown");
    return 200;
}

bool creality_upload(const char *ip, int port, const char *gcode_path) {
    (void)ip; (void)port; (void)gcode_path;
    return false; // stub: needs MQTT or HTTP implementation
}

bool step_export(CadMesh *m, const char *path) {
    if (!m || !m->valid || !path) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "ISO-10303-21;\nHEADER;\nFILE_DESCRIPTION(('Volt360 export'),'2;1');\n");
    fprintf(f, "FILE_NAME('%s','2026-08-02T',(''),(''),'','','');\n", path);
    fprintf(f, "FILE_SCHEMA(('CONFIG_CONTROL_DESIGN'));\nENDSEC;\nDATA;\n");
    // simplified STEP: just store vertex coordinates and triangle faces
    for (int i = 0; i < m->nverts; i++)
        fprintf(f, "#%d=CARTESIAN_POINT('',(%.6f,%.6f,%.6f));\n",
            i+1, m->pos[i*3], m->pos[i*3+1], m->pos[i*3+2]);
    for (int t = 0; t < m->ntris; t++)
        fprintf(f, "#%d=TRIANGLE_FACE(#%d,#%d,#%d);\n",
            m->nverts+t+1, m->idx[t*3]+1, m->idx[t*3+1]+1, m->idx[t*3+2]+1);
    fprintf(f, "ENDSEC;\nEND-ISO-10303-21;\n");
    fclose(f); return true;
}

char *dashboard_summary(Project *p, Farm *f) {
    char buf[1024]; int off = 0;
    int busy = 0, done = 0;
    if (f) { for (int i=0;i<f->n_printers;i++) if(f->printers[i].busy)busy++; for(int i=0;i<f->n_jobs;i++)if(strcmp(f->jobs[i].status,"done")==0)done++; }
    off += snprintf(buf+off, sizeof(buf)-off,
        "Project: %s | CAD: %d | PCB: %d | Farm: %d/%d printers | Jobs: %d done",
        p ? (p->name ? p->name : "?") : "?",
        p ? p->cad_models.len : 0, p ? p->pcbs.len : 0,
        f ? busy : 0, f ? f->n_printers : 0, done);
    return str_dup(buf);
}

char *render_presets(void) {
    return str_dup(
        "=== RENDER PRESETS ===\n"
        "  1. Studio: soft 3-point lighting, white background\n"
        "  2. Outdoor: HDR sky dome, warm sun angle\n"
        "  3. Technical: flat lighting, wireframe overlay\n"
        "  4. Product: rim lights, dark reflective ground\n"
        "Status: raylib-based rendering available now\n"
        "HDR/Raytracing: needs external renderer integration\n");
}
