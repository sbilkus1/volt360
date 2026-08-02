#include "kicad_lib.h"
#include "importer.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ===== SYMBOL LIBRARY PARSER =====
Symbol *kicad_parse_symbol_def(const char *s_expr, const char *def_name) {
    if (!s_expr || !def_name) return NULL;
    // simplified KiCad symbol S-expression parser
    Symbol *sym = (Symbol *)calloc(1, sizeof(Symbol));
    sym->name = str_dup(def_name);
    sym->reference = str_dup("U"); // default prefix

    // parse pins from (pin ...) entries
    const char *p = s_expr;
    int pin_count = 0;
    V2 pin_positions[64]; char *pin_names[64]; char *pin_numbers[64];

    while ((p = strstr(p, "(pin"))) {
        if (pin_count >= 64) break;
        // extract pin name
        const char *name_start = strstr(p, "\"");
        if (!name_start) break;
        name_start++;
        const char *name_end = strchr(name_start, '"');
        if (!name_end) break;
        char pname[64]; int nlen = (int)(name_end - name_start);
        if (nlen > 63) nlen = 63;
        memcpy(pname, name_start, (size_t)nlen); pname[nlen] = 0;

        // extract pin number
        const char *num_start = strstr(name_end, "\"");
        if (!num_start) break;
        num_start++;
        const char *num_end = strchr(num_start, '"');
        if (!num_end) break;
        char pnum[16]; int nnlen = (int)(num_end - num_start);
        if (nnlen > 15) nnlen = 15;
        memcpy(pnum, num_start, (size_t)nnlen); pnum[nnlen] = 0;

        // approximate position
        V2 pos = v2(0, (float)pin_count * 5.0f);

        pin_names[pin_count] = str_dup(pname);
        pin_numbers[pin_count] = str_dup(pnum);
        pin_positions[pin_count] = pos;
        pin_count++;
        p = name_end + 1;
    }

    // create pins
    sym->npins = pin_count;
    sym->pins = (Pin *)calloc((size_t)pin_count, sizeof(Pin));
    for (int i = 0; i < pin_count; i++) {
        sym->pins[i].name = pin_names[i];
        sym->pins[i].number = pin_numbers[i];
        sym->pins[i].pos = pin_positions[i];
        sym->pins[i].angle = 0;
    }

    sym->bmin = v2(-10, -pin_count * 2.5f); sym->bmax = v2(10, pin_count * 2.5f);
    sym->has_bbox = 1;
    return sym;
}

int kicad_import_sym_lib(Project *p, const char *lib_path) {
    if (!p || !lib_path) return 0;
    FILE *f = fopen(lib_path, "r"); if (!f) return 0;
    // read entire file
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    char *data = (char *)malloc((size_t)(sz + 1));
    fread(data, 1, (size_t)sz, f); data[sz] = '\0'; fclose(f);

    int imported = 0;
    // find all (symbol "NAME" ... ) blocks
    const char *cur = data;
    while ((cur = strstr(cur, "(symbol \""))) {
        cur += 9; // skip "(symbol \""
        const char *name_end = strchr(cur, '"');
        if (!name_end) break;
        char sym_name[128]; int nlen = (int)(name_end - cur);
        if (nlen > 127) nlen = 127;
        memcpy(sym_name, cur, (size_t)nlen); sym_name[nlen] = 0;

        const char *block_end = name_end;
        int depth = 0;
        while (*block_end) {
            if (*block_end == '(') depth++;
            if (*block_end == ')') depth--;
            if (depth == 0) break;
            block_end++;
        }

        Symbol *sym = kicad_parse_symbol_def(name_end, sym_name);
        if (sym) {
            arr_push(p->symbols, *sym);
            free(sym);
            imported++;
        }
        cur = block_end + 1;
    }
    free(data);
    return imported;
}

// ===== FOOTPRINT LIBRARY PARSER =====
Footprint *kicad_parse_footprint(const char *s_expr) {
    if (!s_expr) return NULL;
    Footprint *fp = (Footprint *)calloc(1, sizeof(Footprint));

    // extract module name
    const char *name_start = strstr(s_expr, "(module ");
    if (!name_start) { fp->name = str_dup("unknown"); return fp; }
    name_start += 8;
    const char *name_end = name_start;
    while (*name_end && *name_end != ' ' && *name_end != '\n' && *name_end != '(') name_end++;
    char fname[128]; int nlen = (int)(name_end - name_start);
    if (nlen > 127) nlen = 127;
    memcpy(fname, name_start, (size_t)nlen); fname[nlen] = 0;
    fp->name = str_dup(fname);

    // count pads
    int npads = 0;
    const char *p = s_expr;
    while ((p = strstr(p, "(pad "))) { npads++; p += 5; }

    fp->npads = npads;
    fp->pads = (Pad *)calloc((size_t)(npads > 0 ? npads : 1), sizeof(Pad));

    p = s_expr; int pi = 0;
    while ((p = strstr(p, "(pad ")) && pi < npads) {
        p += 5;
        // pad number
        const char *pn_start = p;
        while (*pn_start == ' ') pn_start++;
        const char *pn_end = pn_start;
        while (*pn_end && *pn_end != ' ' && *pn_end != ')') pn_end++;
        char pnum[16]; int plen = (int)(pn_end - pn_start);
        if (plen > 15) plen = 15;
        memcpy(pnum, pn_start, (size_t)plen); pnum[plen] = 0;
        fp->pads[pi].name = str_dup(pnum);
        fp->pads[pi].pos = v2((float)pi * 2.54f, 0); // approximate
        fp->pads[pi].size = v2(1.5f, 1.5f);
        fp->pads[pi].shape = PAD_RECT;
        fp->pads[pi].type = PAD_SMD;
        pi++;
    }

    fp->bmin = v2(-npads*1.27f, -2); fp->bmax = v2(npads*1.27f, 2);
    fp->has_bbox = 1;
    return fp;
}

int kicad_import_fp_lib(Project *p, const char *lib_path) {
    if (!p || !lib_path) return 0;
    // for .pretty directories, scan for .kicad_mod files
    // simplified: treat as single directory
    int imported = 0;
    // try as file first
    FILE *f = fopen(lib_path, "r");
    if (f) {
        fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
        char *data = (char *)malloc((size_t)(sz + 1));
        fread(data, 1, (size_t)sz, f); data[sz] = '\0'; fclose(f);
        Footprint *fp = kicad_parse_footprint(data);
        if (fp) { arr_push(p->footprints, *fp); free(fp); imported++; }
        free(data);
    }
    return imported;
}

// ===== 3D MODEL ASSOCIATION =====
int kicad_link_3d_models(Project *p, const char *models_dir) {
    if (!p || !models_dir) return 0;
    int linked = 0;
    for (int i = 0; i < p->footprints.len; i++) {
        char model_path[512];
        snprintf(model_path, sizeof(model_path), "%s/%s.step", models_dir, p->footprints.v[i].name ? p->footprints.v[i].name : "");
        // check if file exists; if so, link
        FILE *test = fopen(model_path, "r");
        if (test) { fclose(test); p->footprints.v[i].model3d = str_dup(model_path); linked++; }
        else {
            snprintf(model_path, sizeof(model_path), "%s/%s.wrl", models_dir, p->footprints.v[i].name ? p->footprints.v[i].name : "");
            test = fopen(model_path, "r");
            if (test) { fclose(test); p->footprints.v[i].model3d = str_dup(model_path); linked++; }
        }
    }
    return linked;
}

// ===== UNIFIED COMPONENT SYSTEM =====
LibComponent *libcomponent_create(Project *p, const char *sym, const char *fp, const char *m3d, const char *ds) {
    LibComponent *lc = (LibComponent *)calloc(1, sizeof(LibComponent));
    lc->name = str_dup(sym ? sym : "?");
    lc->symbol_name = str_dup(sym);
    lc->footprint_name = str_dup(fp);
    lc->model3d_path = m3d ? str_dup(m3d) : NULL;
    lc->datasheet_url = ds ? str_dup(ds) : NULL;
    (void)p;
    return lc;
}

void libcomponent_free(LibComponent *lc) {
    if (!lc) return;
    free(lc->name); free(lc->description); free(lc->keywords);
    free(lc->symbol_name); free(lc->footprint_name);
    free(lc->model3d_path); free(lc->datasheet_url);
    for (int i = 0; i < lc->n_aliases; i++) free(lc->aliases[i]);
    free(lc->aliases); free(lc);
}

void libcomponent_merge(LibComponent *keep, LibComponent *other) {
    if (!keep || !other) return;
    keep->aliases = (char **)realloc(keep->aliases, sizeof(char *)*(size_t)(keep->n_aliases + 1));
    keep->aliases[keep->n_aliases++] = str_dup(other->name);
    keep->merged_from++;
    // merge fields if keep's are empty
    if (!keep->footprint_name && other->footprint_name) keep->footprint_name = str_dup(other->footprint_name);
    if (!keep->model3d_path && other->model3d_path) keep->model3d_path = str_dup(other->model3d_path);
    if (!keep->datasheet_url && other->datasheet_url) keep->datasheet_url = str_dup(other->datasheet_url);
}

LibComponent *libcomponent_find(LibComponent *lib, int n, const char *name) {
    if (!lib || !name) return NULL;
    for (int i = 0; i < n; i++) {
        if (lib[i].name && strcmp(lib[i].name, name) == 0) return &lib[i];
        for (int j = 0; j < lib[i].n_aliases; j++)
            if (strcmp(lib[i].aliases[j], name) == 0) return &lib[i];
    }
    return NULL;
}

bool libcomponent_export_to_project(Project *p, LibComponent *lc) {
    if (!p || !lc || !lc->symbol_name) return false;
    // find/create symbol
    Symbol *sym = project_find_symbol(p, lc->symbol_name);
    if (!sym) {
        Symbol ns; memset(&ns, 0, sizeof(ns));
        ns.name = str_dup(lc->symbol_name); ns.reference = str_dup("U");
        ns.bmin = v2(-10,-10); ns.bmax = v2(10,10); ns.has_bbox = 1;
        ns.npins = 2; ns.pins = (Pin *)calloc(2, sizeof(Pin));
        ns.pins[0].name = str_dup("1"); ns.pins[0].number = str_dup("1"); ns.pins[0].pos = v2(-8,0);
        ns.pins[1].name = str_dup("2"); ns.pins[1].number = str_dup("2"); ns.pins[1].pos = v2(8,0);
        arr_push(p->symbols, ns);
    }
    // find/create footprint
    if (lc->footprint_name) {
        Footprint *fp = project_find_footprint(p, lc->footprint_name);
        if (!fp) {
            Footprint nf; memset(&nf, 0, sizeof(nf));
            nf.name = str_dup(lc->footprint_name);
            nf.pads = (Pad *)calloc(2, sizeof(Pad));
            nf.pads[0].name = str_dup("1"); nf.pads[0].pos = v2(-1.27f,0); nf.pads[0].size = v2(1.5f,1.0f); nf.pads[0].shape=PAD_RECT; nf.pads[0].type=PAD_SMD;
            nf.pads[1].name = str_dup("2"); nf.pads[1].pos = v2(1.27f,0); nf.pads[1].size = v2(1.5f,1.0f); nf.pads[1].shape=PAD_RECT; nf.pads[1].type=PAD_SMD;
            nf.npads = 2; nf.model3d = lc->model3d_path ? str_dup(lc->model3d_path) : NULL;
            nf.bmin = v2(-2,0); nf.bmax = v2(2,0); nf.has_bbox = 1;
            arr_push(p->footprints, nf);
        }
    }
    // create Component with datasheet link
    Component c; memset(&c, 0, sizeof(c));
    c.id = str_dup(make_id()); c.name = str_dup(lc->name);
    c.package = str_dup(lc->footprint_name);
    c.footprint = str_dup(lc->footprint_name);
    if (lc->datasheet_url) {
        c.doc_id = str_dup(lc->datasheet_url);
        c.doc_text = str_dup("Datasheet available at manufacturer site");
    }
    arr_push(p->components, c);
    return true;
}

// ===== TEMPLATE SYSTEM =====
bool template_apply(Project *p, TemplateID tid) {
    if (!p) return false;
    switch (tid) {
        case TMPL_ARDUINO_UNO: {
            // Add common Arduino components
            Component c; memset(&c, 0, sizeof(c));
            c.id = str_dup(make_id()); c.name = str_dup("ATmega328P"); c.package = str_dup("TQFP-32"); c.footprint = str_dup("TQFP-32");
            arr_push(p->components, c);
            c.id = str_dup(make_id()); c.name = str_dup("16MHz Crystal"); c.package = str_dup("HC-49"); c.footprint = str_dup("Crystal_HC49");
            arr_push(p->components, c);
            c.id = str_dup(make_id()); c.name = str_dup("AMS1117-5.0"); c.package = str_dup("SOT-223"); c.footprint = str_dup("SOT-223");
            arr_push(p->components, c);
            // create schematic
            Schematic s; memset(&s, 0, sizeof(s)); s.name = str_dup("Arduino Uno"); s.ninsts = 0;
            arr_push(p->schematics, s);
            // create PCB outline
            Pcb pcb; memset(&pcb, 0, sizeof(pcb)); pcb.id = str_dup(make_id()); pcb.name = str_dup("Arduino"); pcb.thickness = 1.6f;
            pcb.outline = (V2 *)malloc(sizeof(V2)*4);
            pcb.outline[0]=v2(0,0); pcb.outline[1]=v2(68.6f,0); pcb.outline[2]=v2(68.6f,53.3f); pcb.outline[3]=v2(0,53.3f);
            pcb.noutline=4; pcb_bbox(&pcb);
            arr_push(p->pcbs, pcb);
            return true;
        }
        case TMPL_PSU_LINEAR: {
            Component c; memset(&c, 0, sizeof(c));
            c.id = str_dup(make_id()); c.name = str_dup("LM7805"); c.package = str_dup("TO-220"); arr_push(p->components, c);
            c.id = str_dup(make_id()); c.name = str_dup("Bridge Rectifier"); c.package = str_dup("DIP-4"); arr_push(p->components, c);
            c.id = str_dup(make_id()); c.name = str_dup("1000uF Cap"); c.package = str_dup("Radial_D8.0mm"); arr_push(p->components, c);
            Schematic s; memset(&s, 0, sizeof(s)); s.name = str_dup("Linear PSU"); arr_push(p->schematics, s);
            return true;
        }
        default: return false;
    }
}

char *template_info(TemplateID tid) {
    switch (tid) {
        case TMPL_ARDUINO_UNO: return str_dup("Arduino Uno - ATmega328P, 16MHz crystal, AMS1117-5.0 regulator, 68.6x53.3mm PCB");
        case TMPL_PSU_LINEAR: return str_dup("Linear PSU - LM7805 regulator, bridge rectifier, 1000uF filter capacitor");
        case TMPL_H_BRIDGE: return str_dup("H-Bridge - L298N dual full-bridge driver, diodes, capacitors");
        case TMPL_OPAMP: return str_dup("OpAmp circuit - LM358 dual op-amp, gain resistors, feedback network");
        case TMPL_555_TIMER: return str_dup("555 Timer - NE555P, timing resistors, capacitor, astable/monostable");
        default: return str_dup("Unknown template");
    }
}

// ===== LIBRARY BROWSER =====
char *lib_browser_search(LibComponent *lib, int n, const char *query) {
    if (!lib || !query) return str_dup("no results");
    char buf[1024]; int off = 0, found = 0;
    for (int i = 0; i < n; i++) {
        int match = 0;
        if (lib[i].name && strstr(lib[i].name, query)) match = 1;
        if (lib[i].keywords && strstr(lib[i].keywords, query)) match = 1;
        for (int j = 0; j < lib[i].n_aliases && !match; j++)
            if (lib[i].aliases[j] && strstr(lib[i].aliases[j], query)) match = 1;
        if (match) {
            off += snprintf(buf+off, sizeof(buf)-off, "%s (fp:%s 3D:%s)\n",
                lib[i].name, lib[i].footprint_name?lib[i].footprint_name:"none", lib[i].model3d_path?lib[i].model3d_path:"none");
            found++;
        }
    }
    if (found == 0) off += snprintf(buf+off, sizeof(buf)-off, "No matches for '%s'\n", query);
    return str_dup(buf);
}

char *lib_browser_report(LibComponent *lib, int n) {
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== UNIFIED LIBRARY (%d items) ===\n", n);
    for (int i = 0; i < n; i++) {
        off += snprintf(buf+off, sizeof(buf)-off, "  %s | sym:%s fp:%s 3D:%s ds:%s merged:%d\n",
            lib[i].name, lib[i].symbol_name?lib[i].symbol_name:"-", lib[i].footprint_name?lib[i].footprint_name:"-",
            lib[i].model3d_path?lib[i].model3d_path:"-", lib[i].datasheet_url?lib[i].datasheet_url:"-", lib[i].merged_from);
    }
    return str_dup(buf);
}
