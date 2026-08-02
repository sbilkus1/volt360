#include "importer.h"
#include <math.h>

// ================= helpers =================
static bool at_xy(SExprNode *n, float *x, float *y, float *rot) {
    // (at x y [rot])
    if (!n) return false;
    if (sexpr_child_count(n) < 3) return false;
    *x = (float)sexpr_num(n, 1);
    *y = (float)sexpr_num(n, 2);
    *rot = sexpr_child_count(n) >= 4 ? (float)sexpr_num(n, 3) : 0.0f;
    return true;
}

static bool xy_pt(SExprNode *n, int i, V2 *out) {
    // child i is (xy x y)
    SExprNode *c = sexpr_get(n, i);
    if (!c || strcmp(sexpr_name(c), "xy") != 0) return false;
    out->x = (float)sexpr_num(c, 1);
    out->y = (float)sexpr_num(c, 2);
    return true;
}

static int pts_to_arr(SExprNode *pts, V2 **out, int *n) {
    // (pts (xy..) (xy..) ..)
    if (!pts) return 0;
    ARRAY(V2) arr; arr_init(arr);
    for (int i = 1; i < sexpr_child_count(pts); i++) {
        V2 p;
        if (xy_pt(pts, i, &p)) arr_push(arr, p);
    }
    *out = arr.v;
    *n = arr.len;
    return arr.len;
}

static const char *prop_value(SExprNode *node, const char *name) {
    // find (property "Name" "Value" ...)
    for (int i = 0; i < sexpr_child_count(node); i++) {
        SExprNode *c = sexpr_get(node, i);
        if (c->is_list && strcmp(sexpr_name(c), "property") == 0 && sexpr_child_count(c) >= 3) {
            if (strcmp(sexpr_atom(c, 1), name) == 0) return sexpr_atom(c, 2);
        }
    }
    return NULL;
}

// convert 3-point arc to polyline
static void arc_to_poly(V2 p1, V2 p2, V2 p3, Prim *out, float thickness, const char *layer) {
    // center via perpendicular bisectors
    V2 d1 = v2(p2.x - p1.x, p2.y - p1.y);
    V2 m1 = v2((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    V2 d2 = v2(p3.x - p2.x, p3.y - p2.y);
    V2 m2 = v2((p2.x + p3.x) / 2, (p2.y + p3.y) / 2);
    V2 c = v2(m1.x, m1.y);
    float det = d1.x * (-d2.y) - d1.y * (-d2.x);
    if (fabsf(det) > 1e-9f) {
        float dx = m2.x - m1.x, dy = m2.y - m1.y;
        float t = (dx * (-d2.y) - (-d2.x) * dy) / det;
        c = v2(m1.x + t * d1.x, m1.y + t * d1.y);
    }
    float r = v3_len(v3(p1.x - c.x, p1.y - c.y, 0));
    if (r < 1e-6f) return;
    float a1 = atan2f(p1.y - c.y, p1.x - c.x);
    float a2 = atan2f(p3.y - c.y, p3.x - c.x);
    float am = atan2f(p2.y - c.y, p2.x - c.x);
    // ensure ccw from a1 to a2 through am
    float sweep = a2 - a1;
    while (sweep < 0) sweep += 2 * 3.14159265f;
    // check mid is within sweep
    float amrel = am - a1;
    while (amrel < 0) amrel += 2 * 3.14159265f;
    if (amrel > sweep) { a1 = a2; a2 = a1 - sweep; }
    int segs = 32;
    out->type = PRIM_POLY;
    out->thickness = thickness;
    out->layer = str_dup(layer);
    out->poly = (V2 *)malloc(sizeof(V2) * segs);
    out->npoly = segs;
    for (int i = 0; i < segs; i++) {
        float t = (float)i / (segs - 1);
        float a = a1 + (a2 - a1) * t;
        out->poly[i] = v2(c.x + r * cosf(a), c.y + r * sinf(a));
    }
}

static void add_prim(Prim **prims, int *nprims, Prim p) {
    *prims = (Prim *)realloc(*prims, sizeof(Prim) * (size_t)(*nprims + 1));
    (*prims)[*nprims] = p;
    (*nprims)++;
}

// ---------------- symbols (shared by .kicad_sch lib_symbols and .kicad_sym) ----------------
static PinType pin_type_from_str(const char *s) {
    if (strcmp(s, "input") == 0) return PIN_INPUT;
    if (strcmp(s, "output") == 0) return PIN_OUTPUT;
    if (strcmp(s, "bidirectional") == 0) return PIN_BIDIR;
    if (strcmp(s, "passive") == 0) return PIN_PASSIVE;
    if (strcmp(s, "power_in") == 0 || strcmp(s, "power_out") == 0) return PIN_POWER;
    return PIN_PASSIVE;
}

static void parse_symbol_prim(SExprNode *c, Prim *out, const char *layer) {
    memset(out, 0, sizeof(*out));
    const char *name = sexpr_name(c);
    out->layer = str_dup(layer);
    if (strcmp(name, "rectangle") == 0) {
        out->type = PRIM_RECT;
        out->p1 = v2((float)sexpr_num(sexpr_find_child(c, "start"), 1), (float)sexpr_num(sexpr_find_child(c, "start"), 2));
        out->p2 = v2((float)sexpr_num(sexpr_find_child(c, "end"), 1), (float)sexpr_num(sexpr_find_child(c, "end"), 2));
        SExprNode *stroke = sexpr_find_child(c, "stroke");
        if (stroke) out->thickness = (float)sexpr_num(sexpr_find_child(stroke, "width"), 1);
        SExprNode *fill = sexpr_find_child(c, "fill");
        if (fill && sexpr_find_child(fill, "type") && strcmp(sexpr_atom(sexpr_find_child(fill, "type"), 1), "solid") == 0) out->filled = 1;
    } else if (strcmp(name, "circle") == 0) {
        out->type = PRIM_CIRCLE;
        out->p1 = v2((float)sexpr_num(sexpr_find_child(c, "center"), 1), (float)sexpr_num(sexpr_find_child(c, "center"), 2));
        out->radius = (float)sexpr_num(sexpr_find_child(c, "radius"), 1);
        SExprNode *stroke = sexpr_find_child(c, "stroke");
        if (stroke) out->thickness = (float)sexpr_num(sexpr_find_child(stroke, "width"), 1);
    } else if (strcmp(name, "polyline") == 0) {
        V2 *pts; int n;
        if (pts_to_arr(sexpr_find_child(c, "pts"), &pts, &n) > 0) {
            out->type = PRIM_POLY;
            out->poly = pts;
            out->npoly = n;
        }
        SExprNode *stroke = sexpr_find_child(c, "stroke");
        if (stroke) out->thickness = (float)sexpr_num(sexpr_find_child(stroke, "width"), 1);
    } else if (strcmp(name, "arc") == 0) {
        V2 s = v2((float)sexpr_num(sexpr_find_child(c, "start"), 1), (float)sexpr_num(sexpr_find_child(c, "start"), 2));
        V2 m = v2((float)sexpr_num(sexpr_find_child(c, "mid"), 1), (float)sexpr_num(sexpr_find_child(c, "mid"), 2));
        V2 e = v2((float)sexpr_num(sexpr_find_child(c, "end"), 1), (float)sexpr_num(sexpr_find_child(c, "end"), 2));
        float th = 0;
        SExprNode *stroke = sexpr_find_child(c, "stroke");
        if (stroke) th = (float)sexpr_num(sexpr_find_child(stroke, "width"), 1);
        Prim tmp;
        arc_to_poly(s, m, e, &tmp, th, layer);
        *out = tmp;
    }
}

static void parse_kicad_symbol(SExprNode *sym, Symbol *out) {
    memset(out, 0, sizeof(*out));
    // name may be "Library:Name"
    const char *full = sexpr_atom(sym, 1);
    const char *colon = strrchr(full, ':');
    out->name = str_dup(colon ? colon + 1 : full);
    out->reference = str_dup("U");
    const char *ref = prop_value(sym, "Reference");
    if (ref) { free(out->reference); out->reference = str_dup(ref); }
    // sub-symbols
    for (int i = 2; i < sexpr_child_count(sym); i++) {
        SExprNode *sub = sexpr_get(sym, i);
        if (!sub->is_list || strcmp(sexpr_name(sub), "symbol") != 0) continue;
        for (int j = 1; j < sexpr_child_count(sub); j++) {
            SExprNode *c = sexpr_get(sub, j);
            if (!c->is_list) continue;
            const char *cn = sexpr_name(c);
            if (strcmp(cn, "pin") == 0) {
                Pin pin;
                memset(&pin, 0, sizeof(pin));
                pin.ptype = pin_type_from_str(sexpr_atom(c, 1));
                float x, y, ang;
                if (at_xy(sexpr_find_child(c, "at"), &x, &y, &ang)) {
                    pin.pos = v2(x, y);
                    pin.angle = ang;
                }
                SExprNode *nm = sexpr_find_child(c, "name");
                SExprNode *num = sexpr_find_child(c, "number");
                pin.name = str_dup(nm ? sexpr_atom(nm, 1) : "");
                pin.number = str_dup(num ? sexpr_atom(num, 1) : "");
                out->pins = (Pin *)realloc(out->pins, sizeof(Pin) * (size_t)(out->npins + 1));
                out->pins[out->npins++] = pin;
            } else if (strcmp(cn, "rectangle") == 0 || strcmp(cn, "circle") == 0 ||
                       strcmp(cn, "polyline") == 0 || strcmp(cn, "arc") == 0) {
                Prim pr;
                parse_symbol_prim(c, &pr, "Symbol");
                add_prim(&out->prims, &out->nprims, pr);
            }
        }
    }
    symbol_bbox(out);
}

// ---------------- .kicad_sym ----------------
bool import_kicad_sym(const char *path, Project *p, Symbol *out) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    SExprNode *root = sexpr_parse(data, len);
    free(data);
    if (!root) return false;
    bool ok = false;
    for (int i = 0; i < sexpr_child_count(root); i++) {
        SExprNode *sym = sexpr_get(root, i);
        if (sym->is_list && strcmp(sexpr_name(sym), "symbol") == 0) {
            parse_kicad_symbol(sym, out);
            ok = true;
            break;
        }
    }
    sexpr_free(root);
    if (ok && out->name) {
        if (!project_find_symbol(p, out->name)) arr_push(p->symbols, *out);
        return true;
    }
    return false;
}

// ---------------- .kicad_sch ----------------
static void sch_add_inst(Schematic *s, const char *ref, const char *sym, const char *val, V2 pos, float rot) {
    s->insts = (SymInst *)realloc(s->insts, sizeof(SymInst) * (size_t)(s->ninsts + 1));
    SymInst *si = &s->insts[s->ninsts++];
    memset(si, 0, sizeof(*si));
    si->id = make_id();
    si->ref = str_dup(ref);
    si->symbol = str_dup(sym);
    si->value = str_dup(val);
    si->pos = pos;
    si->rotation = rot;
}

bool import_kicad_sch(const char *path, Project *p, Schematic *out) {
    memset(out, 0, sizeof(*out));
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    SExprNode *root = sexpr_parse(data, len);
    free(data);
    if (!root) return false;
    out->id = make_id();
    out->name = path_noext(path);

    // lib_symbols
    SExprNode *libs = sexpr_find_child(root, "lib_symbols");
    if (libs) {
        for (int i = 1; i < sexpr_child_count(libs); i++) {
            SExprNode *sym = sexpr_get(libs, i);
            if (sym->is_list && strcmp(sexpr_name(sym), "symbol") == 0) {
                Symbol s;
                parse_kicad_symbol(sym, &s);
                if (!project_find_symbol(p, s.name)) {
                    arr_push(p->symbols, s);
                } else {
                    symbol_free_contents(&s);
                }
            }
        }
    }
    // instances, wires, labels, texts
    for (int i = 1; i < sexpr_child_count(root); i++) {
        SExprNode *c = sexpr_get(root, i);
        if (!c->is_list) continue;
        const char *cn = sexpr_name(c);
        if (strcmp(cn, "symbol") == 0) {
            const char *libid = sexpr_atom(c, 1);
            const char *colon = strrchr(libid, ':');
            const char *symname = colon ? colon + 1 : libid;
            float x, y, rot = 0;
            if (at_xy(sexpr_find_child(c, "at"), &x, &y, &rot)) {
                const char *ref = prop_value(c, "Reference");
                const char *val = prop_value(c, "Value");
                sch_add_inst(out, ref ? ref : "U?", symname, val ? val : "", v2(x, y), rot);
            }
        } else if (strcmp(cn, "wire") == 0) {
            V2 *pts; int n;
            if (pts_to_arr(sexpr_find_child(c, "pts"), &pts, &n) > 1) {
                for (int k = 0; k + 1 < n; k++) {
                    out->wires = (Wire *)realloc(out->wires, sizeof(Wire) * (size_t)(out->nwires + 1));
                    out->wires[out->nwires].a = pts[k];
                    out->wires[out->nwires].b = pts[k + 1];
                    out->nwires++;
                }
                free(pts);
            }
        } else if (strcmp(cn, "label") == 0) {
            const char *name = sexpr_atom(c, 1);
            float x, y, rot;
            if (at_xy(sexpr_find_child(c, "at"), &x, &y, &rot)) {
                out->labels = (NetLabel *)realloc(out->labels, sizeof(NetLabel) * (size_t)(out->nlabels + 1));
                out->labels[out->nlabels].name = str_dup(name);
                out->labels[out->nlabels].pos = v2(x, y);
                out->nlabels++;
            }
        } else if (strcmp(cn, "text") == 0) {
            const char *txt = sexpr_atom(c, 1);
            float x, y, rot;
            if (at_xy(sexpr_find_child(c, "at"), &x, &y, &rot)) {
                out->texts = (SchText *)realloc(out->texts, sizeof(SchText) * (size_t)(out->ntexts + 1));
                out->texts[out->ntexts].text = str_dup(txt);
                out->texts[out->ntexts].pos = v2(x, y);
                out->texts[out->ntexts].size = 12;
                out->ntexts++;
            }
        }
    }
    sexpr_free(root);
    if (out->ninsts == 0 && out->nwires == 0 && out->nlabels == 0 && out->ntexts == 0) {
        schematic_free_contents(out);
        return false;
    }
    return true;
}

// ---------------- footprints ----------------
static int layer_index(const char *layer) {
    if (!layer) return 0;
    if (strcmp(layer, "F.Cu") == 0) return 0;
    if (strcmp(layer, "B.Cu") == 0) return 1;
    if (strncmp(layer, "*.", 2) == 0 || strcmp(layer, "*") == 0) return -1;
    if (strncmp(layer, "F.", 2) == 0) return 0;
    if (strncmp(layer, "B.", 2) == 0) return 1;
    return 0;
}

static void parse_kicad_footprint(SExprNode *fp, Footprint *out) {
    memset(out, 0, sizeof(*out));
    const char *full = sexpr_atom(fp, 1);
    const char *colon = strrchr(full, ':');
    out->name = str_dup(colon ? colon + 1 : full);
    for (int i = 2; i < sexpr_child_count(fp); i++) {
        SExprNode *c = sexpr_get(fp, i);
        if (!c->is_list) continue;
        const char *cn = sexpr_name(c);
        if (strcmp(cn, "pad") == 0) {
            Pad pad;
            memset(&pad, 0, sizeof(pad));
            pad.name = str_dup(sexpr_atom(c, 1));
            const char *kind = sexpr_atom(c, 2);
            pad.type = (strcmp(kind, "thru_hole") == 0 || strcmp(kind, "np_thru_hole") == 0) ? PAD_THT : PAD_SMD;
            float x, y, ang = 0;
            if (at_xy(sexpr_find_child(c, "at"), &x, &y, &ang)) { pad.pos = v2(x, y); pad.angle = ang; }
            SExprNode *size = sexpr_find_child(c, "size");
            if (size) pad.size = v2((float)sexpr_num(size, 1), (float)sexpr_num(size, 2));
            const char *shape = sexpr_atom(c, 3);
            if (strcmp(shape, "circle") == 0) pad.shape = PAD_CIRCLE;
            else if (strcmp(shape, "oval") == 0) pad.shape = PAD_OVAL;
            else if (strcmp(shape, "roundrect") == 0) pad.shape = PAD_ROUNDRECT;
            else pad.shape = PAD_RECT;
            SExprNode *layers = sexpr_find_child(c, "layers");
            int layer = 0;
            if (layers && sexpr_child_count(layers) >= 2) {
                layer = layer_index(sexpr_atom(layers, 1));
            }
            pad.layer = layer;
            out->pads = (Pad *)realloc(out->pads, sizeof(Pad) * (size_t)(out->npads + 1));
            out->pads[out->npads++] = pad;
        } else if (strcmp(cn, "fp_line") == 0) {
            V2 s = v2((float)sexpr_num(sexpr_find_child(c, "start"), 1), (float)sexpr_num(sexpr_find_child(c, "start"), 2));
            V2 e = v2((float)sexpr_num(sexpr_find_child(c, "end"), 1), (float)sexpr_num(sexpr_find_child(c, "end"), 2));
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_LINE;
            pr.p1 = s; pr.p2 = e;
            pr.layer = str_dup(sexpr_atom(sexpr_find_child(c, "layer"), 1));
            SExprNode *stroke = sexpr_find_child(c, "stroke");
            if (stroke) pr.thickness = (float)sexpr_num(sexpr_find_child(stroke, "width"), 1);
            add_prim(&out->prims, &out->nprims, pr);
        } else if (strcmp(cn, "fp_arc") == 0) {
            V2 s = v2((float)sexpr_num(sexpr_find_child(c, "start"), 1), (float)sexpr_num(sexpr_find_child(c, "start"), 2));
            V2 m = v2((float)sexpr_num(sexpr_find_child(c, "mid"), 1), (float)sexpr_num(sexpr_find_child(c, "mid"), 2));
            V2 e = v2((float)sexpr_num(sexpr_find_child(c, "end"), 1), (float)sexpr_num(sexpr_find_child(c, "end"), 2));
            Prim pr;
            arc_to_poly(s, m, e, &pr, 0.12f, sexpr_atom(sexpr_find_child(c, "layer"), 1));
            if (pr.npoly > 0) add_prim(&out->prims, &out->nprims, pr);
            else prim_free(&pr);
        } else if (strcmp(cn, "fp_circle") == 0) {
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_CIRCLE;
            pr.p1 = v2((float)sexpr_num(sexpr_find_child(c, "center"), 1), (float)sexpr_num(sexpr_find_child(c, "center"), 2));
            pr.radius = (float)sexpr_num(sexpr_find_child(c, "end"), 1);
            pr.layer = str_dup(sexpr_atom(sexpr_find_child(c, "layer"), 1));
            add_prim(&out->prims, &out->nprims, pr);
        } else if (strcmp(cn, "fp_poly") == 0) {
            V2 *pts; int n;
            if (pts_to_arr(sexpr_find_child(c, "pts"), &pts, &n) > 0) {
                Prim pr; memset(&pr, 0, sizeof(pr));
                pr.type = PRIM_POLY;
                pr.poly = pts;
                pr.npoly = n;
                pr.layer = str_dup(sexpr_atom(sexpr_find_child(c, "layer"), 1));
                add_prim(&out->prims, &out->nprims, pr);
            }
        } else if (strcmp(cn, "model") == 0) {
            const char *m3 = sexpr_atom(c, 1);
            free(out->model3d);
            out->model3d = str_dup(m3);
        }
    }
    footprint_bbox(out);
}

bool import_kicad_mod(const char *path, Project *p, Footprint *out) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    SExprNode *root = sexpr_parse(data, len);
    free(data);
    if (!root) return false;
    SExprNode *fp = NULL;
    for (int i = 0; i < sexpr_child_count(root); i++) {
        if (sexpr_get(root, i)->is_list && strcmp(sexpr_name(sexpr_get(root, i)), "footprint") == 0) {
            fp = sexpr_get(root, i);
            break;
        }
    }
    bool ok = false;
    if (fp) {
        parse_kicad_footprint(fp, out);
        ok = true;
    }
    sexpr_free(root);
    if (ok && out->name) {
        if (!project_find_footprint(p, out->name)) arr_push(p->footprints, *out);
        return true;
    }
    return false;
}

// ---------------- .kicad_pcb ----------------
typedef struct { char **v; int len; int cap; } NetList;

static const char *net_name_at(NetList *nets, int idx) {
    if (idx > 0 && idx < nets->len) return nets->v[idx];
    return "";
}

static void pcb_outline_add(Pcb *pcb, V2 a, V2 b) {
    pcb->outline = (V2 *)realloc(pcb->outline, sizeof(V2) * (size_t)(pcb->noutline + 2));
    pcb->outline[pcb->noutline++] = a;
    pcb->outline[pcb->noutline++] = b;
}

bool import_kicad_pcb(const char *path, Project *p, Pcb *out) {
    memset(out, 0, sizeof(*out));
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    SExprNode *root = sexpr_parse(data, len);
    free(data);
    if (!root) return false;
    out->id = make_id();
    out->name = path_noext(path);
    out->thickness = 1.6f;

    // general thickness
    SExprNode *gen = sexpr_find_child(root, "general");
    if (gen) {
        SExprNode *th = sexpr_find_child(gen, "thickness");
        if (th) out->thickness = (float)sexpr_num(th, 1);
    }
    SExprNode *setup = sexpr_find_child(root, "setup");
    if (setup) {
        SExprNode *th = sexpr_find_child(setup, "thickness");
        if (th) out->thickness = (float)sexpr_num(th, 1);
    }

    // nets
    NetList nets; arr_init(nets);
    int nn = sexpr_child_count(root);
    for (int i = 0; i < nn; i++) {
        SExprNode *c = sexpr_get(root, i);
        if (c->is_list && strcmp(sexpr_name(c), "net") == 0) {
            arr_push(nets, str_dup(sexpr_atom(c, 2)));
        }
    }

    // footprints + geometry
    int n = sexpr_child_count(root);
    for (int i = 0; i < n; i++) {
        SExprNode *c = sexpr_get(root, i);
        if (!c->is_list) continue;
        const char *cn = sexpr_name(c);
        if (strcmp(cn, "footprint") == 0) {
            Footprint fp;
            parse_kicad_footprint(c, &fp);
            if (fp.name && !project_find_footprint(p, fp.name)) arr_push(p->footprints, fp);
            else footprint_free_contents(&fp);
            FpInst fi;
            memset(&fi, 0, sizeof(fi));
            fi.id = make_id();
            const char *full = sexpr_atom(c, 1);
            const char *colon = strrchr(full, ':');
            fi.footprint = str_dup(colon ? colon + 1 : full);
            fi.ref = str_dup(prop_value(c, "Reference") ? prop_value(c, "Reference") : "U?");
            fi.value = str_dup(prop_value(c, "Value") ? prop_value(c, "Value") : "");
            float x, y, rot = 0;
            if (at_xy(sexpr_find_child(c, "at"), &x, &y, &rot)) { fi.pos = v2(x, y); fi.rotation = rot; }
            SExprNode *layer = sexpr_find_child(c, "layer");
            fi.side = (layer && strcmp(sexpr_atom(layer, 1), "B.Cu") == 0) ? 1 : 0;
            out->fps = (FpInst *)realloc(out->fps, sizeof(FpInst) * (size_t)(out->nfps + 1));
            out->fps[out->nfps++] = fi;
        } else if (strcmp(cn, "segment") == 0) {
            V2 s = v2((float)sexpr_num(sexpr_find_child(c, "start"), 1), (float)sexpr_num(sexpr_find_child(c, "start"), 2));
            V2 e = v2((float)sexpr_num(sexpr_find_child(c, "end"), 1), (float)sexpr_num(sexpr_find_child(c, "end"), 2));
            Track tr;
            memset(&tr, 0, sizeof(tr));
            tr.a = s; tr.b = e;
            tr.width = (float)sexpr_num(sexpr_find_child(c, "width"), 1);
            SExprNode *netn2 = sexpr_find_child(c, "net");
            tr.net = str_dup(netn2 ? net_name_at(&nets, (int)sexpr_num(netn2, 1)) : "");
            out->tracks = (Track *)realloc(out->tracks, sizeof(Track) * (size_t)(out->ntracks + 1));
            out->tracks[out->ntracks++] = tr;
        } else if (strcmp(cn, "via") == 0) {
            Via v;
            memset(&v, 0, sizeof(v));
            SExprNode *at = sexpr_find_child(c, "at");
            v.pos = v2((float)sexpr_num(at, 1), (float)sexpr_num(at, 2));
            v.outer = (float)sexpr_num(sexpr_find_child(c, "size"), 1);
            v.drill = (float)sexpr_num(sexpr_find_child(c, "drill"), 1);
            SExprNode *netn2 = sexpr_find_child(c, "net");
            v.net = str_dup(netn2 ? net_name_at(&nets, (int)sexpr_num(netn2, 1)) : "");
            out->vias = (Via *)realloc(out->vias, sizeof(Via) * (size_t)(out->nvias + 1));
            out->vias[out->nvias++] = v;
        } else if (strcmp(cn, "gr_line") == 0) {
            V2 s = v2((float)sexpr_num(sexpr_find_child(c, "start"), 1), (float)sexpr_num(sexpr_find_child(c, "start"), 2));
            V2 e = v2((float)sexpr_num(sexpr_find_child(c, "end"), 1), (float)sexpr_num(sexpr_find_child(c, "end"), 2));
            const char *layer = sexpr_atom(sexpr_find_child(c, "layer"), 1);
            if (strcmp(layer, "Edge.Cuts") == 0) {
                pcb_outline_add(out, s, e);
            } else {
                Prim pr; memset(&pr, 0, sizeof(pr));
                pr.type = PRIM_LINE; pr.p1 = s; pr.p2 = e;
                pr.layer = str_dup(layer);
                add_prim(&out->prims, &out->nprims, pr);
            }
        } else if (strcmp(cn, "gr_arc") == 0) {
            V2 s = v2((float)sexpr_num(sexpr_find_child(c, "start"), 1), (float)sexpr_num(sexpr_find_child(c, "start"), 2));
            V2 m = v2((float)sexpr_num(sexpr_find_child(c, "mid"), 1), (float)sexpr_num(sexpr_find_child(c, "mid"), 2));
            V2 e = v2((float)sexpr_num(sexpr_find_child(c, "end"), 1), (float)sexpr_num(sexpr_find_child(c, "end"), 2));
            const char *layer = sexpr_atom(sexpr_find_child(c, "layer"), 1);
            Prim pr;
            arc_to_poly(s, m, e, &pr, 0.1f, layer);
            if (pr.npoly > 0) {
                if (strcmp(layer, "Edge.Cuts") == 0) {
                    for (int k = 0; k + 1 < pr.npoly; k++) pcb_outline_add(out, pr.poly[k], pr.poly[k + 1]);
                    prim_free(&pr);
                } else add_prim(&out->prims, &out->nprims, pr);
            } else prim_free(&pr);
        } else if (strcmp(cn, "gr_circle") == 0) {
            const char *layer = sexpr_atom(sexpr_find_child(c, "layer"), 1);
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_CIRCLE;
            pr.p1 = v2((float)sexpr_num(sexpr_find_child(c, "center"), 1), (float)sexpr_num(sexpr_find_child(c, "center"), 2));
            pr.radius = (float)sexpr_num(sexpr_find_child(c, "end"), 1);
            pr.layer = str_dup(layer);
            add_prim(&out->prims, &out->nprims, pr);
        } else if (strcmp(cn, "gr_rect") == 0) {
            const char *layer = sexpr_atom(sexpr_find_child(c, "layer"), 1);
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_RECT;
            pr.p1 = v2((float)sexpr_num(sexpr_find_child(c, "start"), 1), (float)sexpr_num(sexpr_find_child(c, "start"), 2));
            pr.p2 = v2((float)sexpr_num(sexpr_find_child(c, "end"), 1), (float)sexpr_num(sexpr_find_child(c, "end"), 2));
            pr.layer = str_dup(layer);
            add_prim(&out->prims, &out->nprims, pr);
        } else if (strcmp(cn, "gr_poly") == 0) {
            const char *layer = sexpr_atom(sexpr_find_child(c, "layer"), 1);
            V2 *pts; int np;
            if (pts_to_arr(sexpr_find_child(c, "pts"), &pts, &np) > 0) {
                Prim pr; memset(&pr, 0, sizeof(pr));
                pr.type = PRIM_POLY;
                pr.poly = pts; pr.npoly = np;
                pr.layer = str_dup(layer);
                add_prim(&out->prims, &out->nprims, pr);
            }
        }
    }
    sexpr_free(root);
    for (int i = 0; i < nets.len; i++) free(nets.v[i]);
    arr_free(nets);

    // stitch outline into a loop
    if (out->noutline > 0) {
        ARRAY(V2) loop; arr_init(loop);
        ARRAY(bool) used; arr_init(used);
        for (int i = 0; i < out->noutline; i++) arr_push(used, false);
        // start at first point
        arr_push(loop, out->outline[0]);
        used.v[0] = true;
        V2 cur = out->outline[1];
        bool closed = false;
        for (int guard = 0; guard < out->noutline + 1; guard++) {
            arr_push(loop, cur);
            if (v3_len(v3(cur.x - out->outline[0].x, cur.y - out->outline[0].y, 0)) < 1e-4f) { closed = true; break; }
            bool found = false;
            for (int i = 0; i < out->noutline; i++) {
                if (used.v[i]) continue;
                V2 a = out->outline[i * 2], b = out->outline[i * 2 + 1];
                if (v3_len(v3(a.x - cur.x, a.y - cur.y, 0)) < 1e-4f) {
                    used.v[i] = true; cur = b; found = true; break;
                }
                if (v3_len(v3(b.x - cur.x, b.y - cur.y, 0)) < 1e-4f) {
                    used.v[i] = true; cur = a; found = true; break;
                }
            }
            if (!found) break;
        }
        if (closed && loop.len >= 4) {
            // remove duplicate last point
            loop.len--;
            V2 *cp = (V2 *)malloc(sizeof(V2) * (size_t)loop.len);
            memcpy(cp, loop.v, sizeof(V2) * (size_t)loop.len);
            free(out->outline);
            out->outline = cp;
            out->noutline = loop.len;
        }
        arr_free(loop);
        arr_free(used);
    }
    pcb_bbox(out);
    return out->noutline > 0 || out->nfps > 0 || out->ntracks > 0;
}
