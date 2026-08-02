#include "importer.h"
#include "../core/json.h"

#define L_PUSH(ptr, count, val) do { \
    (ptr) = (__typeof__(ptr))realloc((ptr), sizeof(*(ptr)) * ((size_t)(count) + 1)); \
    (ptr)[(count)] = (val); (count)++; \
} while (0)

// ---- helpers ----
static JsonValue *jp(const char *data, size_t len) { return json_parse(data, len); }

static V2 jv2(JsonValue *arr, double scale) {
    V2 r = v2(0, 0);
    if (arr && arr->type == J_ARR) {
        r.x = (float)(json_num_value(json_arr_get(arr, 0)) * scale);
        r.y = (float)(json_num_value(json_arr_get(arr, 1)) * scale);
    }
    return r;
}

static V2 jv2raw(JsonValue *arr) { return jv2(arr, 1.0); }

static const char *net_name_of(JsonValue *net) {
    if (!net) return "";
    if (net->type == J_STR) return net->u.str;
    return json_get_str(net, "name");
}

// ================= library items =================
static bool import_lp_sym(const char *path, Project *p, JsonValue *root) {
    Symbol s; memset(&s, 0, sizeof(s));
    s.name = str_dup(json_get_str(root, "name") ? json_get_str(root, "name") : "symbol");
    JsonValue *pins = json_get_arr(root, "pins");
    if (pins) {
        for (int i = 0; i < json_arr_len(pins); i++) {
            JsonValue *pn = json_arr_get(pins, i);
            if (!pn || pn->type != J_OBJ) continue;
            Pin pin; memset(&pin, 0, sizeof(pin));
            pin.name = str_dup(json_get_str(pn, "name") ? json_get_str(pn, "name") : "");
            pin.number = str_dup(json_get_str(pn, "name") ? json_get_str(pn, "name") : "");
            pin.pos = jv2raw(json_get_arr(pn, "position"));
            pin.angle = (float)json_get_num(pn, "rotation");
            const char *shape = json_get_str(pn, "shape");
            if (shape && strcmp(shape, "dot") == 0) pin.ptype = PIN_BIDIR;
            else if (shape && strcmp(shape, "clock") == 0) pin.ptype = PIN_INPUT;
            else pin.ptype = PIN_PASSIVE;
            L_PUSH(s.pins, s.npins, pin);
        }
    }
    JsonValue *polys = json_get_arr(root, "polygons");
    if (polys) {
        for (int i = 0; i < json_arr_len(polys); i++) {
            JsonValue *pg = json_arr_get(polys, i);
            if (!pg || pg->type != J_OBJ) continue;
            JsonValue *path_ = json_get_arr(pg, "path");
            if (!path_) continue;
            int n = json_arr_len(path_);
            if (n < 2) continue;
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_POLY;
            pr.poly = (V2 *)malloc(sizeof(V2) * (size_t)n);
            for (int k = 0; k < n; k++) pr.poly[k] = jv2raw(json_arr_get(path_, k));
            pr.npoly = n;
            pr.thickness = (float)json_get_num(pg, "line_width");
            if (pr.thickness == 0) pr.thickness = 0.2f;
            pr.layer = str_dup("Symbols");
            pr.filled = json_get_bool(pg, "is_filled");
            L_PUSH(s.prims, s.nprims, pr);
        }
    }
    symbol_bbox(&s);
    if (project_find_symbol(p, s.name)) symbol_free_contents(&s);
    else arr_push(p->symbols, s);
    return true;
}

static bool import_lp_fp(const char *path, Project *p, JsonValue *root) {
    Footprint f; memset(&f, 0, sizeof(f));
    f.name = str_dup(json_get_str(root, "name") ? json_get_str(root, "name") : "package");
    JsonValue *pads = json_get_arr(root, "pads");
    if (pads) {
        for (int i = 0; i < json_arr_len(pads); i++) {
            JsonValue *pd = json_arr_get(pads, i);
            if (!pd || pd->type != J_OBJ) continue;
            Pad pad; memset(&pad, 0, sizeof(pad));
            pad.name = str_dup(json_get_str(pd, "name") ? json_get_str(pd, "name") : "");
            pad.pos = jv2raw(json_get_arr(pd, "position"));
            pad.size = jv2raw(json_get_arr(pd, "size"));
            pad.angle = (float)json_get_num(pd, "rotation");
            if (pad.size.x == 0) pad.size = v2(1.5f, 1.5f);
            const char *shape = json_get_str(pd, "shape");
            if (shape && (strcmp(shape, "round") == 0 || strcmp(shape, "circle") == 0)) pad.shape = PAD_CIRCLE;
            else if (shape && strcmp(shape, "oblong") == 0) pad.shape = PAD_OVAL;
            else pad.shape = pad.size.x == pad.size.y ? PAD_CIRCLE : PAD_RECT;
            const char *drill = json_get_str(pd, "drill_shape");
            const char *layer = json_get_str(pd, "layer_id");
            if (drill && strcmp(drill, "none") != 0) {
                pad.type = PAD_THT;
                pad.layer = -1;
            } else {
                pad.type = PAD_SMD;
                pad.layer = (layer && strstr(layer, "bottom")) ? 1 : 0;
            }
            L_PUSH(f.pads, f.npads, pad);
        }
    }
    JsonValue *polys = json_get_arr(root, "polygons");
    if (polys) {
        for (int i = 0; i < json_arr_len(polys); i++) {
            JsonValue *pg = json_arr_get(polys, i);
            if (!pg || pg->type != J_OBJ) continue;
            JsonValue *path_ = json_get_arr(pg, "path");
            if (!path_) continue;
            int n = json_arr_len(path_);
            if (n < 2) continue;
            Prim pr; memset(&pr, 0, sizeof(pr));
            pr.type = PRIM_POLY;
            pr.poly = (V2 *)malloc(sizeof(V2) * (size_t)n);
            for (int k = 0; k < n; k++) pr.poly[k] = jv2raw(json_arr_get(path_, k));
            pr.npoly = n;
            pr.thickness = (float)json_get_num(pg, "line_width");
            if (pr.thickness == 0) pr.thickness = 0.2f;
            const char *layer = json_get_str(pg, "layer_id");
            if (layer && strstr(layer, "silkscreen")) pr.layer = str_dup(strstr(layer, "bottom") ? "B.SilkS" : "F.SilkS");
            else if (layer && strstr(layer, "solder")) pr.layer = str_dup(strstr(layer, "bottom") ? "B.Mask" : "F.Mask");
            else pr.layer = str_dup(layer ? layer : "F.SilkS");
            pr.filled = json_get_bool(pg, "is_filled");
            L_PUSH(f.prims, f.nprims, pr);
        }
    }
    footprint_bbox(&f);
    if (project_find_footprint(p, f.name)) footprint_free_contents(&f);
    else arr_push(p->footprints, f);
    return true;
}

static bool import_lp_cmp(const char *path, Project *p, JsonValue *root) {
    Component c; memset(&c, 0, sizeof(c));
    c.id = make_id();
    c.name = str_dup(json_get_str(root, "name") ? json_get_str(root, "name") : "component");
    c.source_file = str_dup(path);
    c.doc_text = NULL;
    if (project_find_component(p, c.id)) component_free_contents(&c);
    else arr_push(p->components, c);
    return true;
}

static bool import_lp_dev(const char *path, Project *p, JsonValue *root) {
    Component c; memset(&c, 0, sizeof(c));
    c.id = make_id();
    c.name = str_dup(json_get_str(root, "name") ? json_get_str(root, "name") : "device");
    c.source_file = str_dup(path);
    c.doc_text = NULL;
    JsonValue *pkg = json_get_obj(root, "package");
    if (pkg) {
        const char *uuid = json_get_str(pkg, "uuid");
        if (uuid) c.footprint = str_dup(uuid);
    }
    if (project_find_component(p, c.id)) component_free_contents(&c);
    else arr_push(p->components, c);
    return true;
}

// ================= board / schematic files (.lp) =================
static bool import_lp_board(const char *path, Project *p, Pcb *out) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    JsonValue *root = json_parse(data, len);
    if (!root) { free(data); return false; }
    JsonValue *board = json_get_obj(root, "board");
    pcb_free_contents(out);
    memset(out, 0, sizeof(*out));
    out->id = make_id();
    char *base = path_noext(path);
    out->name = path_basename(base);
    free(base);
    out->thickness = 1.6f;
    if (board) {
        JsonValue *polys = json_get_arr(board, "polygons");
        if (polys) {
            for (int i = 0; i < json_arr_len(polys); i++) {
                JsonValue *pg = json_arr_get(polys, i);
                if (!pg || pg->type != J_OBJ) continue;
                const char *layer = json_get_str(pg, "layer_id");
                bool is_outline = layer && (strstr(layer, "board_outline") || strstr(layer, "board_outlines"));
                JsonValue *path_ = json_get_arr(pg, "path");
                if (!path_) continue;
                int n = json_arr_len(path_);
                if (is_outline) {
                    for (int k = 0; k < n; k++) {
                        V2 pt = jv2raw(json_arr_get(path_, k));
                        L_PUSH(out->outline, out->noutline, pt);
                    }
                } else {
                    if (n < 2) continue;
                    Prim pr; memset(&pr, 0, sizeof(pr));
                    pr.type = PRIM_POLY;
                    pr.poly = (V2 *)malloc(sizeof(V2) * (size_t)n);
                    for (int k = 0; k < n; k++) pr.poly[k] = jv2raw(json_arr_get(path_, k));
                    pr.npoly = n;
                    pr.thickness = (float)json_get_num(pg, "line_width");
                    if (pr.thickness == 0) pr.thickness = 0.2f;
                    pr.layer = str_dup(layer ? layer : "");
                    L_PUSH(out->prims, out->nprims, pr);
                }
            }
        }
        JsonValue *components = json_get_arr(board, "components");
        if (components) {
            for (int i = 0; i < json_arr_len(components); i++) {
                JsonValue *co = json_arr_get(components, i);
                if (!co || co->type != J_OBJ) continue;
                FpInst fi; memset(&fi, 0, sizeof(fi));
                fi.id = make_id();
                const char *nm = json_get_str(co, "name");
                fi.ref = str_dup(nm ? nm : "?");
                const char *val = json_get_str(co, "value");
                fi.value = str_dup(val ? val : "");
                fi.pos = jv2raw(json_get_arr(co, "position"));
                fi.rotation = (float)json_get_num(co, "rotation");
                const char *side = json_get_str(co, "side");
                fi.side = side && strcmp(side, "bottom") == 0 ? 1 : 0;
                JsonValue *fp = json_get_obj(co, "footprint");
                if (fp) {
                    const char *fpuuid = json_get_str(fp, "uuid");
                    fi.footprint = str_dup(fpuuid ? fpuuid : "");
                }
                L_PUSH(out->fps, out->nfps, fi);
            }
        }
        JsonValue *traces = json_get_arr(board, "traces");
        if (traces) {
            for (int i = 0; i < json_arr_len(traces); i++) {
                JsonValue *tr = json_arr_get(traces, i);
                if (!tr || tr->type != J_OBJ) continue;
                JsonValue *path_ = json_get_arr(tr, "path");
                if (!path_) continue;
                float width = (float)json_get_num(tr, "width");
                if (width == 0) width = 0.2f;
                const char *net = net_name_of(json_get(tr, "net"));
                int n = json_arr_len(path_);
                for (int k = 0; k + 1 < n; k++) {
                    Track t;
                    t.a = jv2raw(json_arr_get(path_, k));
                    t.b = jv2raw(json_arr_get(path_, k + 1));
                    t.width = width;
                    t.net = str_dup(net ? net : "");
                    L_PUSH(out->tracks, out->ntracks, t);
                }
            }
        }
        JsonValue *vias = json_get_arr(board, "vias");
        if (vias) {
            for (int i = 0; i < json_arr_len(vias); i++) {
                JsonValue *vi = json_arr_get(vias, i);
                if (!vi || vi->type != J_OBJ) continue;
                Via v; memset(&v, 0, sizeof(v));
                v.pos = jv2raw(json_get_arr(vi, "position"));
                v.drill = (float)json_get_num(vi, "drill_diameter");
                v.outer = (float)json_get_num(vi, "size");
                if (v.outer == 0) v.outer = 1.5f;
                if (v.drill == 0) v.drill = v.outer * 0.5f;
                const char *net = net_name_of(json_get(vi, "net"));
                v.net = str_dup(net ? net : "");
                L_PUSH(out->vias, out->nvias, v);
            }
        }
    }
    pcb_bbox(out);
    json_free(root);
    free(data);
    return true;
}

static bool import_lp_schematic(const char *path, Project *p, Schematic *out) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    JsonValue *root = json_parse(data, len);
    if (!root) { free(data); return false; }
    JsonValue *sch = json_get_obj(root, "schematic");
    schematic_free_contents(out);
    memset(out, 0, sizeof(*out));
    out->id = make_id();
    char *base = path_noext(path);
    out->name = path_basename(base);
    free(base);
    if (sch) {
        JsonValue *components = json_get_arr(sch, "components");
        if (components) {
            for (int i = 0; i < json_arr_len(components); i++) {
                JsonValue *co = json_arr_get(components, i);
                if (!co || co->type != J_OBJ) continue;
                SymInst si; memset(&si, 0, sizeof(si));
                si.id = make_id();
                const char *nm = json_get_str(co, "name");
                si.ref = str_dup(nm ? nm : "?");
                const char *val = json_get_str(co, "value");
                si.value = str_dup(val ? val : "");
                si.pos = jv2raw(json_get_arr(co, "position"));
                si.rotation = (float)json_get_num(co, "rotation");
                si.symbol = str_dup("?");
                L_PUSH(out->insts, out->ninsts, si);
            }
        }
        JsonValue *segs = json_get_arr(sch, "net_segments");
        if (segs) {
            for (int i = 0; i < json_arr_len(segs); i++) {
                JsonValue *seg = json_arr_get(segs, i);
                if (!seg || seg->type != J_OBJ) continue;
                const char *net = net_name_of(json_get(seg, "net"));
                JsonValue *wires = json_get_arr(seg, "wires");
                if (wires) {
                    for (int k = 0; k < json_arr_len(wires); k++) {
                        JsonValue *w = json_arr_get(wires, k);
                        if (!w || w->type != J_OBJ) continue;
                        JsonValue *path_ = json_get_arr(w, "path");
                        if (!path_ || json_arr_len(path_) < 2) continue;
                        Wire wr;
                        wr.a = jv2raw(json_arr_get(path_, 0));
                        wr.b = jv2raw(json_arr_get(path_, 1));
                        L_PUSH(out->wires, out->nwires, wr);
                    }
                }
                JsonValue *labels = json_get_arr(seg, "labels");
                if (labels) {
                    for (int k = 0; k < json_arr_len(labels); k++) {
                        JsonValue *lb = json_arr_get(labels, k);
                        if (!lb || lb->type != J_OBJ) continue;
                        NetLabel nl; memset(&nl, 0, sizeof(nl));
                        nl.name = str_dup(net ? net : "");
                        nl.pos = jv2raw(json_get_arr(lb, "position"));
                        L_PUSH(out->labels, out->nlabels, nl);
                    }
                }
                if (out->nlabels == 0 && net && *net && wires) {
                    JsonValue *w = json_arr_get(wires, 0);
                    if (w && w->type == J_OBJ) {
                        JsonValue *path_ = json_get_arr(w, "path");
                        if (path_ && json_arr_len(path_) > 0) {
                            NetLabel nl; memset(&nl, 0, sizeof(nl));
                            nl.name = str_dup(net);
                            nl.pos = jv2raw(json_arr_get(path_, 0));
                            L_PUSH(out->labels, out->nlabels, nl);
                        }
                    }
                }
            }
        }
    }
    json_free(root);
    free(data);
    return true;
}

// ================= project (.lpp) =================
bool import_librepcb_project(const char *path, Project *p) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    JsonValue *root = json_parse(data, len);
    if (!root) { free(data); return false; }
    bool ok = false;
    // project name
    const char *pname = json_get_str(root, "name");
    if (pname && pname[0] && !p->name) p->name = str_dup(pname);
    char *dir = path_dirname(path);
    // boards
    JsonValue *boards = json_get_arr(root, "boards");
    if (boards) {
        for (int i = 0; i < json_arr_len(boards); i++) {
            JsonValue *b = json_arr_get(boards, i);
            const char *file = json_get_str(b, "file");
            if (!file) continue;
            char *full = path_join(dir, file);
            Pcb pc; memset(&pc, 0, sizeof(pc));
            if (import_lp_board(full, p, &pc)) { arr_push(p->pcbs, pc); ok = true; }
            else pcb_free_contents(&pc);
            free(full);
        }
    }
    // schematics
    JsonValue *schs = json_get_arr(root, "schematics");
    if (schs) {
        for (int i = 0; i < json_arr_len(schs); i++) {
            JsonValue *s = json_arr_get(schs, i);
            const char *file = json_get_str(s, "file");
            if (!file) continue;
            char *full = path_join(dir, file);
            Schematic sc; memset(&sc, 0, sizeof(sc));
            if (import_lp_schematic(full, p, &sc)) { arr_push(p->schematics, sc); ok = true; }
            else schematic_free_contents(&sc);
            free(full);
        }
    }
    free(dir);
    json_free(root);
    free(data);
    return ok;
}

// ================= library dispatch =================
bool import_librepcb_lib(const char *path, Project *p) {
    const char *ext = path_ext(path);
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    JsonValue *root = json_parse(data, len);
    if (!root) { free(data); return false; }
    bool ok = false;
    if (strcmp(ext, "lp_sym") == 0) ok = import_lp_sym(path, p, root);
    else if (strcmp(ext, "lp_fp") == 0 || strcmp(ext, "lp_pkg") == 0) ok = import_lp_fp(path, p, root);
    else if (strcmp(ext, "lp_cmp") == 0) ok = import_lp_cmp(path, p, root);
    else if (strcmp(ext, "lp_dev") == 0) ok = import_lp_dev(path, p, root);
    else if (strcmp(ext, "lp") == 0) {
        // could be board or schematic file
        const char *type = json_get_str(root, "type");
        if (type && strcmp(type, "board") == 0) {
            Pcb pc; memset(&pc, 0, sizeof(pc));
            ok = import_lp_board(path, p, &pc);
            if (ok) arr_push(p->pcbs, pc);
            else pcb_free_contents(&pc);
        } else if (type && strcmp(type, "schematic") == 0) {
            Schematic sc; memset(&sc, 0, sizeof(sc));
            ok = import_lp_schematic(path, p, &sc);
            if (ok) arr_push(p->schematics, sc);
            else schematic_free_contents(&sc);
        }
    }
    json_free(root);
    free(data);
    return ok;
}
