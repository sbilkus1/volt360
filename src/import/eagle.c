#include "importer.h"
#include "../core/xml.h"

#define E_PUSH(ptr, count, val) do { \
    (ptr) = (__typeof__(ptr))realloc((ptr), sizeof(*(ptr)) * ((size_t)(count) + 1)); \
    (ptr)[(count)] = (val); (count)++; \
} while (0)

// ================= Eagle helpers =================
static double eagle_rot(const char *rot) {
    // "R90", "MR180", "0", "M0" -> degrees
    if (!rot) return 0.0;
    const char *p = rot;
    if (*p == 'M') p++;
    if (*p == 'R') p++;
    return atof(p);
}

static int eagle_mirrored(const char *rot) {
    return rot && rot[0] == 'M';
}

static double eagle_grid_scale(XmlNode *drawing) {
    XmlNode *g = xml_child(drawing, "grid");
    if (g) {
        const char *unit = xml_attr(g, "unit");
        if (unit && str_icmp(unit, "inch") == 0) return 25.4;
    }
    return 1.0;
}

static int eagle_layer_copper(const char *layer) {
    if (!layer) return 0;
    int l = atoi(layer);
    return l == 1 ? 0 : (l == 16 ? 1 : 0);
}

static void eagle_scale_symbol(Symbol *s, double k) {
    if (k == 1.0) return;
    for (int i = 0; i < s->npins; i++) {
        s->pins[i].pos.x = (float)(s->pins[i].pos.x * k);
        s->pins[i].pos.y = (float)(s->pins[i].pos.y * k);
    }
    for (int i = 0; i < s->nprims; i++) {
        s->prims[i].p1.x = (float)(s->prims[i].p1.x * k);
        s->prims[i].p1.y = (float)(s->prims[i].p1.y * k);
        s->prims[i].p2.x = (float)(s->prims[i].p2.x * k);
        s->prims[i].p2.y = (float)(s->prims[i].p2.y * k);
        if (s->prims[i].type == PRIM_CIRCLE) s->prims[i].radius = (float)(s->prims[i].radius * k);
        s->prims[i].thickness = (float)(s->prims[i].thickness * k);
    }
    symbol_bbox(s);
}

static void eagle_scale_footprint(Footprint *f, double k) {
    if (k == 1.0) return;
    for (int i = 0; i < f->npads; i++) {
        f->pads[i].pos.x = (float)(f->pads[i].pos.x * k);
        f->pads[i].pos.y = (float)(f->pads[i].pos.y * k);
        f->pads[i].size.x = (float)(f->pads[i].size.x * k);
        f->pads[i].size.y = (float)(f->pads[i].size.y * k);
    }
    for (int i = 0; i < f->nprims; i++) {
        f->prims[i].p1.x = (float)(f->prims[i].p1.x * k);
        f->prims[i].p1.y = (float)(f->prims[i].p1.y * k);
        f->prims[i].p2.x = (float)(f->prims[i].p2.x * k);
        f->prims[i].p2.y = (float)(f->prims[i].p2.y * k);
        if (f->prims[i].type == PRIM_CIRCLE) f->prims[i].radius = (float)(f->prims[i].radius * k);
        f->prims[i].thickness = (float)(f->prims[i].thickness * k);
    }
    footprint_bbox(f);
}

// ---- symbol / footprint from eagle nodes ----
static void eagle_symbol_from(XmlNode *sym, Symbol *out) {
    symbol_free_contents(out);
    memset(out, 0, sizeof(*out));
    out->name = str_dup(xml_attr(sym, "name") ? xml_attr(sym, "name") : "symbol");
    for (int i = 0; i < sym->children.len; i++) {
        XmlNode *c = sym->children.v[i];
        if (xml_name_is(c, "pin")) {
            Pin pn; memset(&pn, 0, sizeof(pn));
            pn.name = str_dup(xml_attr(c, "name") ? xml_attr(c, "name") : "");
            pn.number = str_dup(xml_attr(c, "name") ? xml_attr(c, "name") : "");
            pn.pos = v2((float)atof(xml_attr(c, "x") ? xml_attr(c, "x") : "0"),
                        (float)atof(xml_attr(c, "y") ? xml_attr(c, "y") : "0"));
            pn.angle = 0;
            const char *dir = xml_attr(c, "direction");
            if (dir) {
                if (str_icmp(dir, "in") == 0) pn.ptype = PIN_INPUT;
                else if (str_icmp(dir, "out") == 0) pn.ptype = PIN_OUTPUT;
                else if (str_icmp(dir, "io") == 0) pn.ptype = PIN_BIDIR;
                else if (str_icmp(dir, "bidir") == 0) pn.ptype = PIN_BIDIR;
                else if (str_icmp(dir, "nc") == 0) pn.ptype = PIN_NC;
                else if (str_icmp(dir, "power") == 0 || str_icmp(dir, "sup") == 0) pn.ptype = PIN_POWER;
                else pn.ptype = PIN_PASSIVE;
            }
            E_PUSH(out->pins, out->npins, pn);
        } else if (xml_name_is(c, "wire")) {
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_LINE;
            pr.p1 = v2((float)atof(xml_attr(c, "x1") ? xml_attr(c, "x1") : "0"),
                       (float)atof(xml_attr(c, "y1") ? xml_attr(c, "y1") : "0"));
            pr.p2 = v2((float)atof(xml_attr(c, "x2") ? xml_attr(c, "x2") : "0"),
                       (float)atof(xml_attr(c, "y2") ? xml_attr(c, "y2") : "0"));
            pr.thickness = (float)atof(xml_attr(c, "width") ? xml_attr(c, "width") : "0.2");
            pr.layer = str_dup("Symbols");
            E_PUSH(out->prims, out->nprims, pr);
        } else if (xml_name_is(c, "rectangle")) {
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_RECT;
            pr.p1 = v2((float)atof(xml_attr(c, "x1") ? xml_attr(c, "x1") : "0"),
                       (float)atof(xml_attr(c, "y1") ? xml_attr(c, "y1") : "0"));
            pr.p2 = v2((float)atof(xml_attr(c, "x2") ? xml_attr(c, "x2") : "0"),
                       (float)atof(xml_attr(c, "y2") ? xml_attr(c, "y2") : "0"));
            pr.layer = str_dup("Symbols");
            const char *fill = xml_attr(c, "fill");
            pr.filled = fill && (str_icmp(fill, "solid") == 0);
            E_PUSH(out->prims, out->nprims, pr);
        } else if (xml_name_is(c, "circle")) {
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_CIRCLE;
            pr.p1 = v2((float)atof(xml_attr(c, "x") ? xml_attr(c, "x") : "0"),
                       (float)atof(xml_attr(c, "y") ? xml_attr(c, "y") : "0"));
            pr.radius = (float)atof(xml_attr(c, "radius") ? xml_attr(c, "radius") : "0");
            pr.layer = str_dup("Symbols");
            E_PUSH(out->prims, out->nprims, pr);
        } else if (xml_name_is(c, "text")) {
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_TEXT;
            pr.p1 = v2((float)atof(xml_attr(c, "x") ? xml_attr(c, "x") : "0"),
                       (float)atof(xml_attr(c, "y") ? xml_attr(c, "y") : "0"));
            pr.thickness = (float)atof(xml_attr(c, "size") ? xml_attr(c, "size") : "1.5");
            const char *t = xml_attr(c, "name");
            pr.text = str_dup(t ? t : "");
            pr.layer = str_dup("Symbols");
            E_PUSH(out->prims, out->nprims, pr);
        }
    }
    symbol_bbox(out);
}

static void eagle_footprint_from(XmlNode *pkg, Footprint *out) {
    footprint_free_contents(out);
    memset(out, 0, sizeof(*out));
    out->name = str_dup(xml_attr(pkg, "name") ? xml_attr(pkg, "name") : "package");
    for (int i = 0; i < pkg->children.len; i++) {
        XmlNode *c = pkg->children.v[i];
        if (xml_name_is(c, "smd") || xml_name_is(c, "pad")) {
            Pad pd; memset(&pd, 0, sizeof(pd));
            pd.name = str_dup(xml_attr(c, "name") ? xml_attr(c, "name") : "");
            pd.pos = v2((float)atof(xml_attr(c, "x") ? xml_attr(c, "x") : "0"),
                        (float)atof(xml_attr(c, "y") ? xml_attr(c, "y") : "0"));
            const char *dx = xml_attr(c, "dx"), *dy = xml_attr(c, "dy");
            float sx = dx ? (float)atof(dx) : 0.5f;
            float sy = dy ? (float)atof(dy) : 0.5f;
            pd.size = v2(sx, sy);
            if (xml_name_is(c, "smd")) {
                pd.type = PAD_SMD;
                const char *shape = xml_attr(c, "shape");
                if (shape && str_icmp(shape, "round") == 0) pd.shape = PAD_CIRCLE;
                else if (shape && (str_icmp(shape, "long") == 0 || str_icmp(shape, "octagon") == 0)) pd.shape = PAD_OVAL;
                else pd.shape = sx == sy ? PAD_CIRCLE : PAD_RECT;
                pd.layer = eagle_layer_copper(xml_attr(c, "layer"));
            } else {
                pd.type = PAD_THT;
                pd.shape = PAD_CIRCLE;
                pd.layer = -1;
            }
            E_PUSH(out->pads, out->npads, pd);
        } else if (xml_name_is(c, "wire")) {
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_LINE;
            pr.p1 = v2((float)atof(xml_attr(c, "x1") ? xml_attr(c, "x1") : "0"),
                       (float)atof(xml_attr(c, "y1") ? xml_attr(c, "y1") : "0"));
            pr.p2 = v2((float)atof(xml_attr(c, "x2") ? xml_attr(c, "x2") : "0"),
                       (float)atof(xml_attr(c, "y2") ? xml_attr(c, "y2") : "0"));
            pr.thickness = (float)atof(xml_attr(c, "width") ? xml_attr(c, "width") : "0.2");
            const char *layer = xml_attr(c, "layer");
            pr.layer = str_dup(layer ? layer : "F.SilkS");
            E_PUSH(out->prims, out->nprims, pr);
        } else if (xml_name_is(c, "rectangle")) {
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_RECT;
            pr.p1 = v2((float)atof(xml_attr(c, "x1") ? xml_attr(c, "x1") : "0"),
                       (float)atof(xml_attr(c, "y1") ? xml_attr(c, "y1") : "0"));
            pr.p2 = v2((float)atof(xml_attr(c, "x2") ? xml_attr(c, "x2") : "0"),
                       (float)atof(xml_attr(c, "y2") ? xml_attr(c, "y2") : "0"));
            const char *layer = xml_attr(c, "layer");
            pr.layer = str_dup(layer ? layer : "F.SilkS");
            E_PUSH(out->prims, out->nprims, pr);
        } else if (xml_name_is(c, "text")) {
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_TEXT;
            pr.p1 = v2((float)atof(xml_attr(c, "x") ? xml_attr(c, "x") : "0"),
                       (float)atof(xml_attr(c, "y") ? xml_attr(c, "y") : "0"));
            pr.thickness = (float)atof(xml_attr(c, "size") ? xml_attr(c, "size") : "1");
            const char *t = xml_attr(c, "name");
            pr.text = str_dup(t ? t : "");
            const char *layer = xml_attr(c, "layer");
            pr.layer = str_dup(layer ? layer : "F.SilkS");
            E_PUSH(out->prims, out->nprims, pr);
        }
    }
    footprint_bbox(out);
}

// ================= schematic (.sch) =================
bool import_eagle_sch(const char *path, Project *p, Schematic *out) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    XmlNode *root = xml_parse(data, len);
    free(data);
    if (!root) return false;
    double scale = eagle_grid_scale(root);
    XmlNode *sch = xml_child(root, "schematic");
    if (!sch) { xml_free(root); return false; }
    schematic_free_contents(out);
    memset(out, 0, sizeof(*out));
    out->id = make_id();
    char *base = path_noext(path);
    out->name = path_basename(base);
    free(base);

    // library -> symbols + footprints
    XmlNode *lib = xml_child(sch, "library");
    if (lib) {
        XmlNode *syms = xml_child(lib, "symbols");
        if (syms) {
            for (int i = 0; i < syms->children.len; i++) {
                XmlNode *s = syms->children.v[i];
                if (!xml_name_is(s, "symbol")) continue;
                const char *name = xml_attr(s, "name");
                if (!name) continue;
                if (project_find_symbol(p, name)) continue;
                Symbol sym; memset(&sym, 0, sizeof(sym));
                eagle_symbol_from(s, &sym);
                eagle_scale_symbol(&sym, scale);
                arr_push(p->symbols, sym);
            }
        }
        XmlNode *pkgs = xml_child(lib, "packages");
        if (pkgs) {
            for (int i = 0; i < pkgs->children.len; i++) {
                XmlNode *pk = pkgs->children.v[i];
                if (!xml_name_is(pk, "package")) continue;
                const char *name = xml_attr(pk, "name");
                if (!name) continue;
                if (project_find_footprint(p, name)) continue;
                Footprint fp; memset(&fp, 0, sizeof(fp));
                eagle_footprint_from(pk, &fp);
                eagle_scale_footprint(&fp, scale);
                arr_push(p->footprints, fp);
            }
        }
    }

    // parts list
    ARRAY(char *) part_names; arr_init(part_names);
    ARRAY(char *) part_symbols; arr_init(part_symbols);
    XmlNode *parts = xml_child(sch, "parts");
    if (parts) {
        for (int i = 0; i < parts->children.len; i++) {
            XmlNode *pa = parts->children.v[i];
            if (!xml_name_is(pa, "part")) continue;
            const char *nm = xml_attr(pa, "name");
            const char *ds = xml_attr(pa, "deviceset");
            if (!nm) continue;
            arr_push(part_names, str_dup(nm));
            arr_push(part_symbols, str_dup(ds ? ds : nm));
        }
    }

    XmlNode *sheets = xml_child(sch, "sheets");
    if (sheets) {
        for (int i = 0; i < sheets->children.len; i++) {
            XmlNode *sheet = sheets->children.v[i];
            if (!xml_name_is(sheet, "sheet")) continue;
            XmlNode *instances = xml_child(sheet, "instances");
            if (instances) {
                for (int j = 0; j < instances->children.len; j++) {
                    XmlNode *in = instances->children.v[j];
                    if (!xml_name_is(in, "instance")) continue;
                    const char *part = xml_attr(in, "part");
                    if (!part) continue;
                    SymInst si; memset(&si, 0, sizeof(si));
                    si.id = make_id();
                    si.ref = str_dup(part);
                    const char *symname = part;
                    for (int k = 0; k < part_names.len; k++) {
                        if (strcmp(part_names.v[k], part) == 0) { symname = part_symbols.v[k]; break; }
                    }
                    si.symbol = str_dup(symname);
                    si.pos = v2((float)(atof(xml_attr(in, "x") ? xml_attr(in, "x") : "0") * scale),
                                (float)(atof(xml_attr(in, "y") ? xml_attr(in, "y") : "0") * scale));
                    si.rotation = (float)eagle_rot(xml_attr(in, "rot"));
                    si.mirrored = eagle_mirrored(xml_attr(in, "rot"));
                    E_PUSH(out->insts, out->ninsts, si);
                }
            }
            XmlNode *nets = xml_child(sheet, "nets");
            if (nets) {
                for (int j = 0; j < nets->children.len; j++) {
                    XmlNode *net = nets->children.v[j];
                    if (!xml_name_is(net, "net")) continue;
                    const char *nm = xml_attr(net, "name");
                    V2 first_wire = v2(0, 0);
                    bool have_wire = false;
                    for (int k = 0; k < net->children.len; k++) {
                        XmlNode *seg = net->children.v[k];
                        if (!xml_name_is(seg, "segment")) continue;
                        for (int m = 0; m < seg->children.len; m++) {
                            XmlNode *w = seg->children.v[m];
                            if (xml_name_is(w, "wire")) {
                                Wire wr;
                                wr.a = v2((float)(atof(xml_attr(w, "x1") ? xml_attr(w, "x1") : "0") * scale),
                                          (float)(atof(xml_attr(w, "y1") ? xml_attr(w, "y1") : "0") * scale));
                                wr.b = v2((float)(atof(xml_attr(w, "x2") ? xml_attr(w, "x2") : "0") * scale),
                                          (float)(atof(xml_attr(w, "y2") ? xml_attr(w, "y2") : "0") * scale));
                                E_PUSH(out->wires, out->nwires, wr);
                                if (!have_wire) { first_wire = wr.a; have_wire = true; }
                            }
                        }
                    }
                    if (have_wire) {
                        NetLabel nl; memset(&nl, 0, sizeof(nl));
                        nl.name = str_dup(nm ? nm : "");
                        nl.pos = first_wire;
                        E_PUSH(out->labels, out->nlabels, nl);
                    }
                }
            }
        }
    }
    str_free_split(part_names.v, part_names.len);
    str_free_split(part_symbols.v, part_symbols.len);
    xml_free(root);
    return true;
}

// ================= board (.brd) =================
bool import_eagle_brd(const char *path, Project *p, Pcb *out) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    XmlNode *root = xml_parse(data, len);
    free(data);
    if (!root) return false;
    double scale = eagle_grid_scale(root);
    XmlNode *brd = xml_child(root, "board");
    if (!brd) { xml_free(root); return false; }
    pcb_free_contents(out);
    memset(out, 0, sizeof(*out));
    out->id = make_id();
    char *base = path_noext(path);
    out->name = path_basename(base);
    free(base);
    out->thickness = 1.6f;

    // libraries -> footprints
    XmlNode *libs = xml_child(brd, "libraries");
    if (libs) {
        for (int i = 0; i < libs->children.len; i++) {
            XmlNode *lib = libs->children.v[i];
            if (!xml_name_is(lib, "library")) continue;
            XmlNode *pkgs = xml_child(lib, "packages");
            if (!pkgs) continue;
            for (int j = 0; j < pkgs->children.len; j++) {
                XmlNode *pk = pkgs->children.v[j];
                if (!xml_name_is(pk, "package")) continue;
                const char *name = xml_attr(pk, "name");
                if (!name) continue;
                if (project_find_footprint(p, name)) continue;
                Footprint fp; memset(&fp, 0, sizeof(fp));
                eagle_footprint_from(pk, &fp);
                eagle_scale_footprint(&fp, scale);
                arr_push(p->footprints, fp);
            }
        }
    }

    // plain -> outline (layer 20) + graphics
    XmlNode *plain = xml_child(brd, "plain");
    if (plain) {
        for (int i = 0; i < plain->children.len; i++) {
            XmlNode *w = plain->children.v[i];
            if (!xml_name_is(w, "wire")) continue;
            const char *layer = xml_attr(w, "layer");
            if (layer && atoi(layer) == 20) {
                V2 a = v2((float)(atof(xml_attr(w, "x1") ? xml_attr(w, "x1") : "0") * scale),
                          (float)(atof(xml_attr(w, "y1") ? xml_attr(w, "y1") : "0") * scale));
                V2 b = v2((float)(atof(xml_attr(w, "x2") ? xml_attr(w, "x2") : "0") * scale),
                          (float)(atof(xml_attr(w, "y2") ? xml_attr(w, "y2") : "0") * scale));
                if (out->noutline == 0 ||
                    (v2_len(v2_sub(a, out->outline[out->noutline - 1])) > 0.001f &&
                     v2_len(v2_sub(a, out->outline[0])) > 0.001f)) {
                    E_PUSH(out->outline, out->noutline, a);
                }
                E_PUSH(out->outline, out->noutline, b);
            } else {
                Prim pr; memset(&pr, 0, sizeof(pr));
                pr.type = PRIM_LINE;
                pr.p1 = v2((float)(atof(xml_attr(w, "x1") ? xml_attr(w, "x1") : "0") * scale),
                           (float)(atof(xml_attr(w, "y1") ? xml_attr(w, "y1") : "0") * scale));
                pr.p2 = v2((float)(atof(xml_attr(w, "x2") ? xml_attr(w, "x2") : "0") * scale),
                           (float)(atof(xml_attr(w, "y2") ? xml_attr(w, "y2") : "0") * scale));
                pr.thickness = (float)atof(xml_attr(w, "width") ? xml_attr(w, "width") : "0.2");
                pr.layer = str_dup(layer ? layer : "");
                E_PUSH(out->prims, out->nprims, pr);
            }
        }
    }

    // elements -> FpInst
    XmlNode *elements = xml_child(brd, "elements");
    if (elements) {
        for (int i = 0; i < elements->children.len; i++) {
            XmlNode *el = elements->children.v[i];
            if (!xml_name_is(el, "element")) continue;
            const char *nm = xml_attr(el, "name");
            if (!nm) continue;
            FpInst fi; memset(&fi, 0, sizeof(fi));
            fi.id = make_id();
            fi.ref = str_dup(nm);
            const char *pkg = xml_attr(el, "package");
            fi.footprint = str_dup(pkg ? pkg : "");
            const char *val = xml_attr(el, "value");
            fi.value = str_dup(val ? val : "");
            fi.pos = v2((float)(atof(xml_attr(el, "x") ? xml_attr(el, "x") : "0") * scale),
                        (float)(atof(xml_attr(el, "y") ? xml_attr(el, "y") : "0") * scale));
            fi.rotation = (float)eagle_rot(xml_attr(el, "rot"));
            fi.side = eagle_mirrored(xml_attr(el, "rot")) ? 1 : 0;
            E_PUSH(out->fps, out->nfps, fi);
        }
    }

    // signals -> tracks/vias
    XmlNode *signals = xml_child(brd, "signals");
    if (signals) {
        for (int i = 0; i < signals->children.len; i++) {
            XmlNode *sig = signals->children.v[i];
            if (!xml_name_is(sig, "signal")) continue;
            const char *nm = xml_attr(sig, "name");
            for (int j = 0; j < sig->children.len; j++) {
                XmlNode *c = sig->children.v[j];
                if (xml_name_is(c, "wire")) {
                    Track t;
                    t.a = v2((float)(atof(xml_attr(c, "x1") ? xml_attr(c, "x1") : "0") * scale),
                             (float)(atof(xml_attr(c, "y1") ? xml_attr(c, "y1") : "0") * scale));
                    t.b = v2((float)(atof(xml_attr(c, "x2") ? xml_attr(c, "x2") : "0") * scale),
                             (float)(atof(xml_attr(c, "y2") ? xml_attr(c, "y2") : "0") * scale));
                    t.width = (float)(atof(xml_attr(c, "width") ? xml_attr(c, "width") : "0.2") * scale);
                    t.net = str_dup(nm ? nm : "");
                    E_PUSH(out->tracks, out->ntracks, t);
                } else if (xml_name_is(c, "via")) {
                    Via v; memset(&v, 0, sizeof(v));
                    v.pos = v2((float)(atof(xml_attr(c, "x") ? xml_attr(c, "x") : "0") * scale),
                               (float)(atof(xml_attr(c, "y") ? xml_attr(c, "y") : "0") * scale));
                    v.drill = (float)(atof(xml_attr(c, "drill") ? xml_attr(c, "drill") : "0.4") * scale);
                    v.outer = (float)(atof(xml_attr(c, "diameter") ? xml_attr(c, "diameter") : "0.8") * scale);
                    v.net = str_dup(nm ? nm : "");
                    E_PUSH(out->vias, out->nvias, v);
                }
            }
        }
    }

    pcb_bbox(out);
    xml_free(root);
    return true;
}
