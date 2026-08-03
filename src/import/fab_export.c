#include "fab_export.h"
#include "gerber_out.h"
#include "mfg_out.h"
#include "../core/util.h"
#include <miniz.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef _MSC_VER
#include <windows.h>
#else
#include <sys/stat.h>
#endif

void fab_export_opts_default(FabExportOpts *opts) {
    memset(opts, 0, sizeof(*opts));
    opts->gerber_copper_top = 1;
    opts->gerber_copper_bottom = 1;
    opts->gerber_soldermask_top = 1;
    opts->gerber_soldermask_bottom = 1;
    opts->gerber_silkscreen_top = 1;
    opts->gerber_silkscreen_bottom = 1;
    opts->gerber_outline = 1;
    opts->gerber_paste_top = 1;
    opts->gerber_paste_bottom = 1;
    opts->excellon_drill = 1;
    opts->excellon_slot = 0;
    opts->bom_csv = 1;
    opts->pick_place_csv = 1;
    opts->assembly_drawing = 1;
    opts->ipc356 = 1;
    opts->readme = 1;
}

static void add_file(FabExportResult *r, const char *name) {
    int n = r->nfiles;
    char **f = (char **)realloc(r->files_included, sizeof(char *) * (size_t)(n + 1));
    if (f) {
        r->files_included = f;
        r->files_included[n] = str_dup(name);
        r->nfiles = n + 1;
    }
}

static int fab_write_file(const char *dir, const char *fname, const char *data) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", dir, fname);
    FILE *f = fopen(path, "w");
    if (!f) return 0;
    fprintf(f, "%s", data);
    fclose(f);
    return 1;
}

static int fab_write_copper(Pcb *pcb, const char *dir, const char *fname, const char *layer) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", dir, fname);
    return gerber_write_copper(pcb, path, layer) ? 1 : 0;
}

static int fab_write_outline(Pcb *pcb, const char *dir, const char *fname) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", dir, fname);
    return gerber_write_outline(pcb, path) ? 1 : 0;
}

static int fab_write_drill(Pcb *pcb, const char *dir, const char *fname) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", dir, fname);
    return gerber_write_drill(pcb, path) ? 1 : 0;
}

static int fab_write_pnp(Pcb *pcb, const char *dir, const char *fname) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", dir, fname);
    return mfg_pickplace(pcb, path) ? 1 : 0;
}

static int fab_write_ipc356(Pcb *pcb, const char *dir, const char *fname) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", dir, fname);
    return mfg_ipc356(pcb, path) ? 1 : 0;
}

static char *fab_bom_text(const Pcb *pcb) {
    Buf b;
    buf_init(&b);
    buf_appendf(&b, "BOM for: %s\n", pcb->name ? pcb->name : "PCB");
    buf_append_str(&b, "Ref,Value,Footprint,Side\n");
    for (int i = 0; i < pcb->nfps; i++) {
        buf_appendf(&b, "%s,%s,%s,%s\n",
            pcb->fps[i].ref ? pcb->fps[i].ref : "?",
            pcb->fps[i].value ? pcb->fps[i].value : "",
            pcb->fps[i].footprint ? pcb->fps[i].footprint : "?",
            pcb->fps[i].side == 0 ? "Top" : "Bottom");
    }
    char *r = buf_cstr(&b);
    char *dup = str_dup(r);
    buf_free(&b);
    return dup;
}

static char *fab_assembly_text(const Pcb *pcb) {
    Buf b;
    buf_init(&b);
    buf_appendf(&b, "Assembly Drawing: %s\n", pcb->name ? pcb->name : "PCB");
    buf_append_str(&b, "========================================\n");
    if (pcb->noutline >= 3) {
        float min_x = pcb->outline[0].x, max_x = min_x;
        float min_y = pcb->outline[0].y, max_y = min_y;
        for (int i = 1; i < pcb->noutline; i++) {
            if (pcb->outline[i].x < min_x) min_x = pcb->outline[i].x;
            if (pcb->outline[i].x > max_x) max_x = pcb->outline[i].x;
            if (pcb->outline[i].y < min_y) min_y = pcb->outline[i].y;
            if (pcb->outline[i].y > max_y) max_y = pcb->outline[i].y;
        }
        buf_appendf(&b, "Board Dimensions: %.1f x %.1f mm\n", max_x - min_x, max_y - min_y);
    }
    if (pcb->thickness > 0) buf_appendf(&b, "Board Thickness: %.2f mm\n", pcb->thickness);
    buf_appendf(&b, "Layer count derived from fab export\n");
    buf_appendf(&b, "Component Count: %d\n", pcb->nfps);
    buf_append_str(&b, "\nComponent Placement:\n");

    for (int i = 0; i < pcb->nfps; i++) {
        buf_appendf(&b, "%-12s %-12s %-8s (%.1f, %.1f) R%.0f\n",
            pcb->fps[i].ref ? pcb->fps[i].ref : "?",
            pcb->fps[i].value ? pcb->fps[i].value : "",
            pcb->fps[i].side == 0 ? "Top" : "Bottom",
            pcb->fps[i].pos.x, pcb->fps[i].pos.y,
            pcb->fps[i].rotation);
    }

    buf_append_str(&b, "\nOrientation Notes:\n");
    buf_append_str(&b, "  - All rotations are in degrees CCW from reference orientation.\n");
    buf_append_str(&b, "  - Verify pin 1 orientation for polarized components during assembly.\n");
    buf_append_str(&b, "  - For SMD parts on bottom side, rotation is viewed from the mirror plane.\n");

    char *r = buf_cstr(&b);
    char *dup = str_dup(r);
    buf_free(&b);
    return dup;
}

static char *fab_readme_text(const Pcb *pcb) {
    Buf b;
    buf_init(&b);
    time_t now = time(NULL);
    char tbuf[64];
#ifdef _MSC_VER
    struct tm tinfo;
    localtime_s(&tinfo, &now);
    snprintf(tbuf, sizeof(tbuf), "%d-%02d-%02d", tinfo.tm_year + 1900, tinfo.tm_mon + 1, tinfo.tm_mday);
#else
    struct tm *tinfo = localtime(&now);
    snprintf(tbuf, sizeof(tbuf), "%d-%02d-%02d", tinfo->tm_year + 1900, tinfo->tm_mon + 1, tinfo->tm_mday);
#endif

    buf_appendf(&b, "PCB Fabrication Package\n");
    buf_appendf(&b, "========================\n");
    buf_appendf(&b, "Board: %s\n", pcb->name ? pcb->name : "Unnamed");
    buf_appendf(&b, "Date: %s\n", tbuf);
    buf_appendf(&b, "Generated by: Volt360\n");
    buf_appendf(&b, "\nBoard Properties:\n");
    if (pcb->thickness > 0) buf_appendf(&b, "  Thickness: %.2f mm\n", pcb->thickness);
    if (pcb->noutline >= 3) {
        float min_x = pcb->outline[0].x, max_x = min_x;
        float min_y = pcb->outline[0].y, max_y = min_y;
        for (int i = 1; i < pcb->noutline; i++) {
            if (pcb->outline[i].x < min_x) min_x = pcb->outline[i].x;
            if (pcb->outline[i].x > max_x) max_x = pcb->outline[i].x;
            if (pcb->outline[i].y < min_y) min_y = pcb->outline[i].y;
            if (pcb->outline[i].y > max_y) max_y = pcb->outline[i].y;
        }
        buf_appendf(&b, "  Dimensions: %.1f x %.1f mm\n", max_x - min_x, max_y - min_y);
    }
    buf_appendf(&b, "  Copper weight: 1 oz (standard)\n");
    buf_appendf(&b, "  Surface finish: ENIG (recommended)\n");
    buf_appendf(&b, "\nFiles Included:\n");

    char *r = buf_cstr(&b);
    char *dup = str_dup(r);
    buf_free(&b);
    return dup;
}

static int fab_add_to_zip(mz_zip_archive *zip, const char *dir, const char *fname, const char *zipname) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", dir, fname);
    if (!file_exists(path)) return 0;
    if (!mz_zip_writer_add_file(zip, zipname, path, NULL, 0, MZ_DEFAULT_LEVEL)) return 0;
    return 1;
}

FabExportResult fab_export_generate(const Pcb *pcb, const FabExportOpts *opts, const char *output_dir) {
    FabExportResult r;
    memset(&r, 0, sizeof(r));
    r.success = 0;

    if (!pcb || !opts || !output_dir) {
        snprintf(r.error, sizeof(r.error), "Invalid arguments");
        return r;
    }

    char tmpdir[512];
    snprintf(tmpdir, sizeof(tmpdir), "%s/fabtmp", output_dir);

#ifdef _MSC_VER
    CreateDirectoryA(tmpdir, NULL);
#else
    mkdir(tmpdir, 0755);
#endif

    const char *bname = pcb->name ? pcb->name : "board";

    if (opts->gerber_copper_top) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-F_Cu.gbr", bname);
        if (fab_write_copper((Pcb *)pcb, tmpdir, fn, "F_Cu")) add_file(&r, fn);
    }
    if (opts->gerber_copper_bottom) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-B_Cu.gbr", bname);
        if (fab_write_copper((Pcb *)pcb, tmpdir, fn, "B_Cu")) add_file(&r, fn);
    }
    if (opts->gerber_outline) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-Edge_Cuts.gbr", bname);
        if (fab_write_outline((Pcb *)pcb, tmpdir, fn)) add_file(&r, fn);
    }
    if (opts->gerber_soldermask_top) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-F_Mask.gbr", bname);
        if (fab_write_copper((Pcb *)pcb, tmpdir, fn, "F_Mask")) add_file(&r, fn);
    }
    if (opts->gerber_soldermask_bottom) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-B_Mask.gbr", bname);
        if (fab_write_copper((Pcb *)pcb, tmpdir, fn, "B_Mask")) add_file(&r, fn);
    }
    if (opts->gerber_silkscreen_top) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-F_SilkS.gbr", bname);
        if (fab_write_copper((Pcb *)pcb, tmpdir, fn, "F_SilkS")) add_file(&r, fn);
    }
    if (opts->gerber_silkscreen_bottom) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-B_SilkS.gbr", bname);
        if (fab_write_copper((Pcb *)pcb, tmpdir, fn, "B_SilkS")) add_file(&r, fn);
    }
    if (opts->gerber_paste_top) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-F_Paste.gbr", bname);
        if (fab_write_copper((Pcb *)pcb, tmpdir, fn, "F_Paste")) add_file(&r, fn);
    }
    if (opts->gerber_paste_bottom) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-B_Paste.gbr", bname);
        if (fab_write_copper((Pcb *)pcb, tmpdir, fn, "B_Paste")) add_file(&r, fn);
    }
    if (opts->excellon_drill) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s.drl", bname);
        if (fab_write_drill((Pcb *)pcb, tmpdir, fn)) add_file(&r, fn);
    }
    if (opts->bom_csv) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-BOM.csv", bname);
        char *txt = fab_bom_text(pcb);
        if (txt && fab_write_file(tmpdir, fn, txt)) add_file(&r, fn);
        free(txt);
    }
    if (opts->pick_place_csv) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-pnp.csv", bname);
        if (fab_write_pnp((Pcb *)pcb, tmpdir, fn)) add_file(&r, fn);
    }
    if (opts->assembly_drawing) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-assembly.txt", bname);
        char *txt = fab_assembly_text(pcb);
        if (txt && fab_write_file(tmpdir, fn, txt)) add_file(&r, fn);
        free(txt);
    }
    if (opts->ipc356) {
        char fn[128]; snprintf(fn, sizeof(fn), "%s-ipc356.net", bname);
        if (fab_write_ipc356((Pcb *)pcb, tmpdir, fn)) add_file(&r, fn);
    }
    if (opts->readme) {
        char fn[128]; snprintf(fn, sizeof(fn), "README.txt");
        char *txt = fab_readme_text(pcb);
        if (txt && fab_write_file(tmpdir, fn, txt)) add_file(&r, fn);
        free(txt);
    }

    char zippath[512];
    snprintf(zippath, sizeof(zippath), "%s/%s-fab.zip", output_dir, bname);
    r.zip_path = str_dup(zippath);

    if (r.nfiles == 0) {
        snprintf(r.error, sizeof(r.error), "No files generated");
        return r;
    }

    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));

    if (!mz_zip_writer_init_file(&zip, zippath, 0)) {
        snprintf(r.error, sizeof(r.error), "Failed to create ZIP file");
        return r;
    }

    for (int i = 0; i < r.nfiles; i++) {
        char fullpath[512];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", tmpdir, r.files_included[i]);
        if (!mz_zip_writer_add_file(&zip, r.files_included[i], fullpath, NULL, 0, MZ_DEFAULT_LEVEL)) {
            snprintf(r.error, sizeof(r.error), "Failed to add file: %s", r.files_included[i]);
            mz_zip_end(&zip);
            return r;
        }
    }

    if (!mz_zip_writer_finalize_archive(&zip)) {
        snprintf(r.error, sizeof(r.error), "Failed to finalize ZIP");
        mz_zip_end(&zip);
        return r;
    }

    mz_zip_end(&zip);
    r.success = 1;
    return r;
}

void fab_export_result_free(FabExportResult *r) {
    free(r->zip_path);
    r->zip_path = NULL;
    for (int i = 0; i < r->nfiles; i++) free(r->files_included[i]);
    free(r->files_included);
    r->files_included = NULL;
    r->nfiles = 0;
}
