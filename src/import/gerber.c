// gerber.c - RS-274X Gerber file parser. Reads tracks and pads into a Pcb.
#include "importer.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

typedef struct {
    float x, y, w, h; int type; // 0=circle, 1=rect
} GerberAperture;

typedef struct { float x, y; } GBPt;

// ================= unit conversion =================
// RS-274X: format = %FSLAXnnYnn*  e.g. L=incremental (absolute default), AXnn = X with nn fractional digits
// Units: %MOIN* = inches, %MOMM* = mm
// We use mm internally; convert inches -> mm

static float g2mm = 1.0f; // adjusts on %MO detection

// ================= aperture parsing =================
static GerberAperture apts[128]; static int napts = 0;

static void add_apt_circle(float d) { if (napts < 128) { apts[napts].type=0; apts[napts].w=d; napts++; } }
static void add_apt_rect(float w, float h) { if (napts < 128) { apts[napts].type=1; apts[napts].w=w; apts[napts].h=h; napts++; } }

// ================= current tool / mode =================
static GBPt pos = {0,0};   // current position
static int tool = -1;      // current D code (aperture index)
static int last_op = 2;    // D02 (move)
static int interp = 1;     // G01 (linear)
static Pcb *out_pcb = NULL;

static void gb_add_track(float x1, float y1, float x2, float y2, float width) {
    if (!out_pcb) return;
    Track t; t.a = v2(x1, y1); t.b = v2(x2, y2); t.width = width; t.net = NULL;
    out_pcb->tracks = (Track *)realloc(out_pcb->tracks, sizeof(Track) * (size_t)(out_pcb->ntracks + 1));
    out_pcb->tracks[out_pcb->ntracks++] = t;
}

static void gb_add_flash(float x, float y, GerberAperture *ap) {
    if (!out_pcb || !ap) return;
    FpInst fp; memset(&fp, 0, sizeof(fp));
    char buf[32]; snprintf(buf, sizeof(buf), "pad-%d", out_pcb->nfps);
    fp.ref = str_dup(buf); fp.id = str_dup(buf); fp.pos = v2(x, y);
    fp.footprint = str_dup("gerber-pad"); fp.value = NULL; fp.component_id = NULL;
    fp.rotation = 0; fp.side = 0;
    out_pcb->fps = (FpInst *)realloc(out_pcb->fps, sizeof(FpInst) * (size_t)(out_pcb->nfps + 1));
    out_pcb->fps[out_pcb->nfps++] = fp;
}

// ================= line parser =================
static void gb_parse_x_y(const char *s, float *x, float *y) {
    *x = pos.x; *y = pos.y;
    const char *p = s;
    while (*p) {
        if (*p == 'X') { *x = (float)atof(p + 1) * g2mm; while (*p && *p != 'Y' && *p != 'D' && *p != 'G' && *p != '*') p++; }
        else if (*p == 'Y') { *y = (float)atof(p + 1) * g2mm; while (*p && *p != 'X' && *p != 'D' && *p != 'G' && *p != '*') p++; }
        else p++;
    }
}

static void gb_parse_line(const char *line) {
    // skip comments, empty lines
    if (!line || !line[0] || line[0] == ';' || line[0] == '\r' || line[0] == '\n') return;

    // % commands
    if (line[0] == '%') {
        const char *s = line + 1;
        // %MOMM* or %MOIN*
        if (strncmp(s, "MOMM", 4) == 0) g2mm = 1.0f;
        else if (strncmp(s, "MOIN", 4) == 0) g2mm = 25.4f;
        // %ADDnnC, d* or %ADDnnR, wxh*
        else if (strncmp(s, "ADD", 3) == 0) {
            s += 3;
            while (*s >= '0' && *s <= '9') s++; // skip D code number
            if (*s == 'C' || *s == 'c') add_apt_circle((float)atof(s + 1) * g2mm);
            else if (*s == 'R' || *s == 'r') { float w = (float)atof(s + 1) * g2mm; const char *q = s+1; while (*q && *q != 'X' && *q != 'x') q++; float h = (float)atof(q + 1) * g2mm; add_apt_rect(w, h); }
        }
        return;
    }

    // operation line: Gxx, Dxx, X, Y
    const char *s = line;
    int g_code = interp;
    int d_code = last_op;
    float nx = pos.x, ny = pos.y;

    while (*s && *s != '*') {
        if (*s == 'G') { g_code = atoi(s + 1); while (*s && *s != 'D' && *s != 'X' && *s != 'Y' && *s != '*') s++; }
        else if (*s == 'D') { d_code = atoi(s + 1); while (*s && *s != 'G' && *s != 'X' && *s != 'Y' && *s != '*') s++; }
        else if (*s == 'X') { gb_parse_x_y(s, &nx, &ny); break; }
        else s++;
    }

    // if tool is set, use it
    if (d_code >= 10 && d_code - 10 < napts) tool = d_code - 10;

    // draw or flash
    if (d_code == 1) { // D01: draw
        float w = (tool >= 0 && tool < napts) ? apts[tool].w * 0.5f : 0.1f;
        gb_add_track(pos.x, pos.y, nx, ny, w);
    } else if (d_code == 3) { // D03: flash
        if (tool >= 0 && tool < napts) gb_add_flash(nx, ny, &apts[tool]);
    }
    // update position
    pos.x = nx; pos.y = ny;
    last_op = d_code;
    interp = g_code;
}

// ================= public API =================
bool gerber_import(const char *path, Project *p) {
    if (!path || !p) return false;
    FILE *f = fopen(path, "r");
    if (!f) return false;

    // init state
    g2mm = 1.0f; napts = 0; pos.x = pos.y = 0; tool = -1; last_op = 2; interp = 1;

    Pcb pcb; memset(&pcb, 0, sizeof(pcb));
    pcb.id = str_dup(make_id());
    // extract filename from path
    const char *slash = strrchr(path, '/');
    const char *bs = strrchr(path, '\\');
    if (bs && bs > slash) slash = bs;
    const char *fname = slash ? slash + 1 : path;
    pcb.name = str_dup(fname && fname[0] ? fname : "gerber");
    pcb.outline = NULL; pcb.noutline = 0;
    pcb.tracks = NULL; pcb.ntracks = 0;
    pcb.fps = NULL; pcb.nfps = 0;
    out_pcb = &pcb;

    char line[4096];
    while (fgets(line, sizeof(line), f)) {
        // strip trailing \r\n
        char *nl = line; while (*nl) { if (*nl == '\r' || *nl == '\n') { *nl = '\0'; break; } nl++; }
        gb_parse_line(line);
    }
    fclose(f);

    // compute bbox from tracks or add a default
    pcb_bbox(&pcb);
    if (!pcb.has_bbox) { pcb.bmin = v2(-50,-50); pcb.bmax = v2(50,50); pcb.has_bbox = true; }

    arr_push(p->pcbs, pcb);
    out_pcb = NULL;
    return true;
}
