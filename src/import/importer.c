#include "importer.h"
#include <windows.h>

void import_result_init(ImportResult *r) {
    memset(r, 0, sizeof(*r));
    r->ok = 1;
    r->message[0] = '\0';
}

static ImportResult importer_result(const char *msg) {
    ImportResult r;
    import_result_init(&r);
    if (msg) snprintf(r.message, sizeof(r.message), "%s", msg);
    return r;
}

// ---------------- dispatcher ----------------
ImportResult importer_import_file(Project *p, const char *path) {
    const char *ext = path_ext(path);
    ImportResult r;
    import_result_init(&r);
    char *base = path_noext(path);

    // ---- documents / datasheets ----
    if (strcmp(ext, "md") == 0 || strcmp(ext, "markdown") == 0 ||
        strcmp(ext, "csv") == 0 || strcmp(ext, "docx") == 0 ||
        strcmp(ext, "txt") == 0) {
        Doc d; memset(&d, 0, sizeof(d));
        d.id = make_id();
        d.name = path_basename(base);
        d.source_file = str_dup(path);
        d.text = importer_text_from_file(path);
        if (!d.text) { d.text = str_dup(""); }
        d.parsed = d.text[0] != '\0';
        arr_push(p->docs, d);
        r.ndocs = 1;
        snprintf(r.message, sizeof(r.message), "Imported document %s", d.name);
        free(base);
        return r;
    }

    // ---- CAD meshes ----
    if (strcmp(ext, "step") == 0 || strcmp(ext, "stp") == 0 ||
        strcmp(ext, "stl") == 0 || strcmp(ext, "obj") == 0) {
        CadMesh m; memset(&m, 0, sizeof(m));
        bool ok = mesh_load_any(path, &m);
        if (!ok) {
            snprintf(r.message, sizeof(r.message), "Failed to load CadMesh: %s", path);
            r.ok = 0;
            free(base);
            return r;
        }
        CadModel cm; memset(&cm, 0, sizeof(cm));
        cm.id = make_id();
        cm.name = path_basename(base);
        cm.source_file = str_dup(path);
        cm.mesh = m;
        arr_push(p->cad_models, cm);
        r.ncad = 1;
        snprintf(r.message, sizeof(r.message), "Imported CAD model %s (%d tris)", cm.name, m.ntris);
        free(base);
        return r;
    }

    // ---- KiCad ----
    if (strcmp(ext, "kicad_sch") == 0) {
        Schematic sc; memset(&sc, 0, sizeof(sc));
        if (import_kicad_sch(path, p, &sc)) { arr_push(p->schematics, sc); r.nsch = 1; }
        else { schematic_free_contents(&sc); r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import KiCad schematic %s", path); }
        free(base);
        return r;
    }
    if (strcmp(ext, "kicad_pcb") == 0) {
        Pcb pc; memset(&pc, 0, sizeof(pc));
        if (import_kicad_pcb(path, p, &pc)) { arr_push(p->pcbs, pc); r.npcb = 1; }
        else { pcb_free_contents(&pc); r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import KiCad PCB %s", path); }
        free(base);
        return r;
    }
    // Gerber files: .gbr, .gtl, .gbl, .gto, .gts, .gbo, .gbs, .gko, .gm1
    if (strncmp(ext, "gbr", 3) == 0 || strncmp(ext, "gtl", 3) == 0 || strncmp(ext, "gbl", 3) == 0 ||
        strncmp(ext, "gto", 3) == 0 || strncmp(ext, "gts", 3) == 0 || strncmp(ext, "gbo", 3) == 0 ||
        strncmp(ext, "gbs", 3) == 0 || strncmp(ext, "gko", 3) == 0 || strncmp(ext, "gm1", 3) == 0 ||
        strncmp(ext, "gml", 3) == 0) {
        if (gerber_import(path, p)) { r.npcb = 1; }
        else { r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import gerber %s", path); }
        free(base);
        return r;
    }
    // DXF import
    if (strcmp(ext, "dxf") == 0) {
        Pcb pc; memset(&pc, 0, sizeof(pc));
        if (dxf_import(path, &pc)) { arr_push(p->pcbs, pc); r.npcb = 1; }
        else { pcb_free_contents(&pc); r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import DXF %s", path); }
        free(base); return r;
    }
    // SVG import
    if (strcmp(ext, "svg") == 0) {
        Pcb pc; memset(&pc, 0, sizeof(pc));
        if (svg_import_pcb(path, &pc)) { arr_push(p->pcbs, pc); r.npcb = 1; }
        else { pcb_free_contents(&pc); r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import SVG %s", path); }
        free(base); return r;
    }
    // PLY mesh
    if (strcmp(ext, "ply") == 0) {
        CadMesh m; memset(&m, 0, sizeof(m));
        if (mesh_import_ply(path, &m)) {
            CadModel cm; memset(&cm, 0, sizeof(cm)); cm.id = str_dup(make_id()); cm.name = str_dup(base); cm.mesh = m;
            arr_push(p->cad_models, cm); r.ncad = 1;
        } else { r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import PLY %s", path); }
        free(base); return r;
    }
    if (strcmp(ext, "kicad_sym") == 0) {
        Symbol s; memset(&s, 0, sizeof(s));
        if (import_kicad_sym(path, p, &s)) { arr_push(p->symbols, s); r.nsymbols = 1; }
        else { symbol_free_contents(&s); r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import symbol %s", path); }
        free(base);
        return r;
    }
    if (strcmp(ext, "kicad_mod") == 0) {
        Footprint f; memset(&f, 0, sizeof(f));
        if (import_kicad_mod(path, p, &f)) { arr_push(p->footprints, f); r.nfootprints = 1; }
        else { footprint_free_contents(&f); r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import footprint %s", path); }
        free(base);
        return r;
    }
    if (strcmp(ext, "kicad_pro") == 0) {
        // project file: mostly settings; accept silently
        free(base);
        return importer_result("KiCad project file accepted (settings only)");
    }

    // ---- Eagle ----
    if (strcmp(ext, "sch") == 0) {
        Schematic sc; memset(&sc, 0, sizeof(sc));
        if (import_eagle_sch(path, p, &sc)) { arr_push(p->schematics, sc); r.nsch = 1; }
        else { schematic_free_contents(&sc); r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import Eagle schematic %s", path); }
        free(base);
        return r;
    }
    if (strcmp(ext, "brd") == 0) {
        Pcb pc; memset(&pc, 0, sizeof(pc));
        if (import_eagle_brd(path, p, &pc)) { arr_push(p->pcbs, pc); r.npcb = 1; }
        else { pcb_free_contents(&pc); r.ok = 0; snprintf(r.message, sizeof(r.message), "Failed to import Eagle board %s", path); }
        free(base);
        return r;
    }

    // ---- LibrePCB ----
    if (strcmp(ext, "lpp") == 0) {
        bool ok = import_librepcb_project(path, p);
        free(base);
        if (ok) return importer_result("LibrePCB project imported");
        r.ok = 0;
        snprintf(r.message, sizeof(r.message), "Failed to import LibrePCB project %s", path);
        return r;
    }
    if (strcmp(ext, "lp_sym") == 0 || strcmp(ext, "lp_fp") == 0 ||
        strcmp(ext, "lp_cmp") == 0 || strcmp(ext, "lp_dev") == 0 ||
        strcmp(ext, "lp_pkg") == 0 || strcmp(ext, "lp") == 0) {
        bool ok = import_librepcb_lib(path, p);
        free(base);
        if (ok) return importer_result("LibrePCB library item imported");
        r.ok = 0;
        snprintf(r.message, sizeof(r.message), "Failed to import LibrePCB file %s", path);
        return r;
    }

    // ---- netlists ----
    if (strcmp(ext, "net") == 0 || strcmp(ext, "cir") == 0 || strcmp(ext, "asc") == 0) {
        bool ok = import_netlist(path, p);
        free(base);
        if (ok) return importer_result("Netlist imported as schematic");
        r.ok = 0;
        snprintf(r.message, sizeof(r.message), "Failed to import netlist %s", path);
        return r;
    }

    // ---- zip ----
    if (strcmp(ext, "zip") == 0) {
        ImportResult zr = importer_import_zip(p, path);
        free(base);
        return zr;
    }

    free(base);
    r.ok = 0;
    snprintf(r.message, sizeof(r.message), "Unsupported file type: .%s", ext);
    return r;
}

// ---------------- directory walk ----------------
static void import_dir_rec(Project *p, const char *dir, ImportResult *acc) {
    char search[MAX_PATH];
    snprintf(search, sizeof(search), "%s\\*", dir);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0) continue;
        char full[MAX_PATH];
        snprintf(full, sizeof(full), "%s\\%s", dir, fd.cFileName);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            import_dir_rec(p, full, acc);
            continue;
        }
        ImportResult sub = importer_import_file(p, full);
        acc->ncomponents += sub.ncomponents;
        acc->nsymbols += sub.nsymbols;
        acc->nfootprints += sub.nfootprints;
        acc->ncad += sub.ncad;
        acc->nsch += sub.nsch;
        acc->npcb += sub.npcb;
        acc->ndocs += sub.ndocs;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

ImportResult importer_import_dir(Project *p, const char *dir) {
    ImportResult r;
    import_result_init(&r);
    if (!dir_exists(dir)) {
        r.ok = 0;
        snprintf(r.message, sizeof(r.message), "Directory not found: %s", dir);
        return r;
    }
    import_dir_rec(p, dir, &r);
    snprintf(r.message, sizeof(r.message), "Scanned %s", dir);
    return r;
}

ImportResult importer_import_any(Project *p, const char *path) {
    if (path_is_dir(path)) return importer_import_dir(p, path);
    return importer_import_file(p, path);
}
