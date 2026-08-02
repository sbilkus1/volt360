#include "libmgr.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

bool lib_export_footprint(Footprint *fp, const char *path) {
    if (!fp || !path) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "(module %s (layer F.Cu) (tedit 0)\n", fp->name ? fp->name : "footprint");
    for (int i = 0; i < fp->npads; i++) {
        Pad *p = &fp->pads[i];
        fprintf(f, "  (pad %s smd rect (at %.3f %.3f) (size 1.5 1.5) (layers F.Cu F.Paste F.Mask))\n",
            p->name ? p->name : "?", p->pos.x, p->pos.y);
    }
    fprintf(f, ")\n");
    fclose(f); return true;
}

int lib_import_footprints(Project *p, const char *dir) {
    (void)p; (void)dir;
    // would scan directory for .kicad_mod files and parse them
    return 0; // stub
}

int lib_export_all_footprints(Project *p, const char *dir) {
    if (!p || !dir) return 0;
    int count = 0;
    for (int i = 0; i < p->footprints.len; i++) {
        char path[512]; snprintf(path, sizeof(path), "%s/%s.kicad_mod", dir, p->footprints.v[i].name ? p->footprints.v[i].name : "fp");
        if (lib_export_footprint(&p->footprints.v[i], path)) count++;
    }
    return count;
}

char *lib_list_report(Project *p) {
    if (!p) return str_dup("no project");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off,
        "Library Report\n--------------\n"
        "Symbols: %d\n", p->symbols.len);
    for (int i = 0; i < p->symbols.len; i++)
        off += snprintf(buf+off, sizeof(buf)-off, "  [S] %s (%d pins)\n", p->symbols.v[i].name ? p->symbols.v[i].name : "?", p->symbols.v[i].npins);
    off += snprintf(buf+off, sizeof(buf)-off, "Footprints: %d\n", p->footprints.len);
    for (int i = 0; i < p->footprints.len; i++)
        off += snprintf(buf+off, sizeof(buf)-off, "  [F] %s (%d pads)\n", p->footprints.v[i].name ? p->footprints.v[i].name : "?", p->footprints.v[i].npads);
    return str_dup(buf);
}
