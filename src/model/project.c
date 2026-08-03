#include "project.h"
#include "../core/json.h"
#include <math.h>

// ============ lifecycle ============
void project_init(Project *p, const char *name, const char *path) {
    memset(p, 0, sizeof(*p));
    p->name = str_dup(name ? name : "Untitled");
    p->path = str_dup(path ? path : "");
    arr_init(p->symbols);
    arr_init(p->footprints);
    arr_init(p->components);
    arr_init(p->cad_models);
    arr_init(p->schematics);
    arr_init(p->pcbs);
    arr_init(p->assemblies);
    arr_init(p->docs);
    arr_init(p->notes);
    p->llm_model = str_dup("llama3");
    p->llm_url = str_dup("http://127.0.0.1:11434");
}

void prim_free(Prim *p) {
    free(p->layer);
    free(p->text);
    free(p->poly);
    memset(p, 0, sizeof(*p));
}

void prim_copy(Prim *dst, Prim *src) {
    *dst = *src;
    dst->layer = str_dup(src->layer);
    dst->text = str_dup(src->text);
    if (src->poly && src->npoly > 0) {
        dst->poly = (V2 *)malloc(sizeof(V2) * (size_t)src->npoly);
        memcpy(dst->poly, src->poly, sizeof(V2) * (size_t)src->npoly);
    } else {
        dst->poly = NULL;
        dst->npoly = 0;
    }
}

void symbol_free_contents(Symbol *s) {
    for (int i = 0; i < s->npins; i++) { free(s->pins[i].name); free(s->pins[i].number); }
    free(s->pins);
    for (int i = 0; i < s->nprims; i++) prim_free(&s->prims[i]);
    free(s->prims);
    free(s->name);
    free(s->reference);
    memset(s, 0, sizeof(*s));
}

void footprint_free_contents(Footprint *f) {
    for (int i = 0; i < f->npads; i++) free(f->pads[i].name);
    free(f->pads);
    for (int i = 0; i < f->nprims; i++) prim_free(&f->prims[i]);
    free(f->prims);
    free(f->name);
    free(f->model3d);
    memset(f, 0, sizeof(*f));
}

void component_free_contents(Component *c) {
    free(c->id); free(c->name); free(c->description); free(c->source_file);
    free(c->manufacturer); free(c->package); free(c->footprint);
    for (int i = 0; i < c->nspecs; i++) { free(c->specs[i].key); free(c->specs[i].value); }
    free(c->specs);
    for (int i = 0; i < c->npins; i++) { free(c->pins[i].name); free(c->pins[i].number); free(c->pins[i].function); }
    free(c->pins);
    free(c->doc_text);
    free(c->doc_id);
    memset(c, 0, sizeof(*c));
}

void schematic_free_contents(Schematic *s) {
    free(s->id); free(s->name);
    for (int i = 0; i < s->ninsts; i++) {
        SymInst *si = &s->insts[i];
        free(si->id); free(si->ref); free(si->symbol); free(si->value);
    }
    free(s->insts);
    free(s->wires);
    for (int i = 0; i < s->nlabels; i++) free(s->labels[i].name);
    free(s->labels);
    for (int i = 0; i < s->ntexts; i++) free(s->texts[i].text);
    free(s->texts);
    memset(s, 0, sizeof(*s));
}

void pcb_free_contents(Pcb *p) {
    free(p->id); free(p->name); free(p->outline);
    for (int i = 0; i < p->nfps; i++) {
        FpInst *f = &p->fps[i];
        free(f->id); free(f->ref); free(f->footprint); free(f->value); free(f->component_id);
    }
    free(p->fps);
    for (int i = 0; i < p->ntracks; i++) free(p->tracks[i].net);
    free(p->tracks);
    for (int i = 0; i < p->nvias; i++) { free(p->vias[i].net); free(p->vias[i].name); }
    free(p->vias);
    for (int i = 0; i < p->nzones; i++) { free(p->zones[i].net); free(p->zones[i].layer); free(p->zones[i].pts); }
    free(p->zones);
    for (int i = 0; i < p->nprims; i++) prim_free(&p->prims[i]);
    free(p->prims);
    memset(p, 0, sizeof(*p));
}

void cad_free_contents(CadModel *c) {
    free(c->id); free(c->name); free(c->source_file);
    mesh_free(&c->mesh);
    memset(c, 0, sizeof(*c));
}

void assembly_free_contents(Assembly *a) {
    free(a->id); free(a->name);
    for (int i = 0; i < a->nitems; i++) { free(a->items[i].part_id); free(a->items[i].label); }
    free(a->items);
    free(a->joints);
    memset(a, 0, sizeof(*a));
}

void doc_free_contents(Doc *d) {
    free(d->id); free(d->name); free(d->source_file); free(d->text);
    memset(d, 0, sizeof(*d));
}

void project_free(Project *p) {
    for (int i = 0; i < p->symbols.len; i++) symbol_free_contents(&p->symbols.v[i]);
    arr_free(p->symbols);
    for (int i = 0; i < p->footprints.len; i++) footprint_free_contents(&p->footprints.v[i]);
    arr_free(p->footprints);
    for (int i = 0; i < p->components.len; i++) component_free_contents(&p->components.v[i]);
    arr_free(p->components);
    for (int i = 0; i < p->cad_models.len; i++) cad_free_contents(&p->cad_models.v[i]);
    arr_free(p->cad_models);
    for (int i = 0; i < p->schematics.len; i++) schematic_free_contents(&p->schematics.v[i]);
    arr_free(p->schematics);
    for (int i = 0; i < p->pcbs.len; i++) pcb_free_contents(&p->pcbs.v[i]);
    arr_free(p->pcbs);
    for (int i = 0; i < p->assemblies.len; i++) assembly_free_contents(&p->assemblies.v[i]);
    arr_free(p->assemblies);
    for (int i = 0; i < p->docs.len; i++) doc_free_contents(&p->docs.v[i]);
    arr_free(p->docs);
    for (int i = 0; i < p->notes.len; i++) free(p->notes.v[i]);
    arr_free(p->notes);
    free(p->name);
    free(p->path);
    free(p->llm_model);
    free(p->llm_url);
    memset(p, 0, sizeof(*p));
}

// ============ accessors ============
#define DEFINE_FIND(fn, CType, field, idfield) \
    CType *project_find_##fn(Project *p, const char *id) { \
        for (int i = 0; i < p->field.len; i++) if (p->field.v[i].idfield && id && strcmp(p->field.v[i].idfield, id) == 0) return &p->field.v[i]; \
        return NULL; \
    }

DEFINE_FIND(component, Component, components, id)
DEFINE_FIND(cad, CadModel, cad_models, id)
DEFINE_FIND(pcb, Pcb, pcbs, id)
DEFINE_FIND(sch, Schematic, schematics, id)
DEFINE_FIND(assy, Assembly, assemblies, id)
DEFINE_FIND(doc, Doc, docs, id)

Symbol *project_find_symbol(Project *p, const char *name) {
    if (!name) return NULL;
    for (int i = 0; i < p->symbols.len; i++) if (p->symbols.v[i].name && strcmp(p->symbols.v[i].name, name) == 0) return &p->symbols.v[i];
    return NULL;
}

Footprint *project_find_footprint(Project *p, const char *name) {
    if (!name) return NULL;
    for (int i = 0; i < p->footprints.len; i++) if (p->footprints.v[i].name && strcmp(p->footprints.v[i].name, name) == 0) return &p->footprints.v[i];
    return NULL;
}

// ============ geometry helpers ============
static void bbox_acc(V2 pt, V2 *lo, V2 *hi) {
    if (pt.x < lo->x) lo->x = pt.x;
    if (pt.y < lo->y) lo->y = pt.y;
    if (pt.x > hi->x) hi->x = pt.x;
    if (pt.y > hi->y) hi->y = pt.y;
}

void prim_bbox(Prim *pr, V2 *bmin, V2 *bmax) {
    V2 lo = { INFINITY, INFINITY }, hi = { -INFINITY, -INFINITY };
    switch (pr->type) {
        case PRIM_RECT: bbox_acc(pr->p1, &lo, &hi); bbox_acc(pr->p2, &lo, &hi); break;
        case PRIM_CIRCLE: {
            float r = pr->radius > 0 ? pr->radius : pr->p2.x;
            bbox_acc(v2(pr->p1.x - r, pr->p1.y - r), &lo, &hi);
            bbox_acc(v2(pr->p1.x + r, pr->p1.y + r), &lo, &hi);
            break;
        }
        case PRIM_ARC:
        case PRIM_LINE: bbox_acc(pr->p1, &lo, &hi); bbox_acc(pr->p2, &lo, &hi); break;
        case PRIM_POLY: for (int i = 0; i < pr->npoly; i++) bbox_acc(pr->poly[i], &lo, &hi); break;
        case PRIM_TEXT: bbox_acc(pr->p1, &lo, &hi); bbox_acc(v2(pr->p1.x + pr->p2.x, pr->p1.y + pr->p2.y), &lo, &hi); break;
    }
    if (lo.x > hi.x) { lo = v2(0, 0); hi = v2(0, 0); }
    *bmin = lo; *bmax = hi;
}

void symbol_bbox(Symbol *s) {
    V2 lo = { INFINITY, INFINITY }, hi = { -INFINITY, -INFINITY };
    for (int i = 0; i < s->nprims; i++) {
        V2 a, b;
        prim_bbox(&s->prims[i], &a, &b);
        lo = v2(fminf(lo.x, a.x), fminf(lo.y, a.y));
        hi = v2(fmaxf(hi.x, b.x), fmaxf(hi.y, b.y));
    }
    for (int i = 0; i < s->npins; i++) {
        lo = v2(fminf(lo.x, s->pins[i].pos.x), fminf(lo.y, s->pins[i].pos.y));
        hi = v2(fmaxf(hi.x, s->pins[i].pos.x), fmaxf(hi.y, s->pins[i].pos.y));
    }
    if (lo.x > hi.x) { lo = v2(-5, -5); hi = v2(5, 5); }
    s->bmin = lo; s->bmax = hi;
    s->has_bbox = 1;
}

void footprint_bbox(Footprint *f) {
    V2 lo = { INFINITY, INFINITY }, hi = { -INFINITY, -INFINITY };
    for (int i = 0; i < f->npads; i++) {
        V2 p = f->pads[i].pos, s = f->pads[i].size;
        V2 a = v2(p.x - s.x / 2, p.y - s.y / 2), b = v2(p.x + s.x / 2, p.y + s.y / 2);
        lo = v2(fminf(lo.x, a.x), fminf(lo.y, a.y));
        hi = v2(fmaxf(hi.x, b.x), fmaxf(hi.y, b.y));
    }
    for (int i = 0; i < f->nprims; i++) {
        V2 a, b;
        prim_bbox(&f->prims[i], &a, &b);
        lo = v2(fminf(lo.x, a.x), fminf(lo.y, a.y));
        hi = v2(fmaxf(hi.x, b.x), fmaxf(hi.y, b.y));
    }
    if (lo.x > hi.x) { lo = v2(-5, -5); hi = v2(5, 5); }
    f->bmin = lo; f->bmax = hi;
    f->has_bbox = 1;
}

void pcb_bbox(Pcb *p) {
    V2 lo = { INFINITY, INFINITY }, hi = { -INFINITY, -INFINITY };
    for (int i = 0; i < p->noutline; i++) {
        lo = v2(fminf(lo.x, p->outline[i].x), fminf(lo.y, p->outline[i].y));
        hi = v2(fmaxf(hi.x, p->outline[i].x), fmaxf(hi.y, p->outline[i].y));
    }
    for (int i = 0; i < p->nfps; i++) {
        Footprint *fp = NULL;
        // bbox from footprint pad extents approx
        V2 a = v2(p->fps[i].pos.x - 5, p->fps[i].pos.y - 5), b = v2(p->fps[i].pos.x + 5, p->fps[i].pos.y + 5);
        lo = v2(fminf(lo.x, a.x), fminf(lo.y, a.y));
        hi = v2(fmaxf(hi.x, b.x), fmaxf(hi.y, b.y));
        (void)fp;
    }
    if (lo.x > hi.x) { lo = v2(-25, -25); hi = v2(25, 25); }
    p->bmin = lo; p->bmax = hi;
    p->has_bbox = 1;
}

void mesh_bbox(CadMesh *m) {
    if (!m || m->nverts <= 0) { m->valid = 0; return; }
    V3 lo = { INFINITY, INFINITY, INFINITY }, hi = { -INFINITY, -INFINITY, -INFINITY };
    for (int i = 0; i < m->nverts; i++) {
        V3 v = { m->pos[i * 3], m->pos[i * 3 + 1], m->pos[i * 3 + 2] };
        lo = v3_min(lo, v); hi = v3_max(hi, v);
    }
    m->bmin = lo; m->bmax = hi;
    m->valid = 1;
}

V3 mesh_center(CadMesh *m) { return v3((m->bmin.x + m->bmax.x) * 0.5f, (m->bmin.y + m->bmax.y) * 0.5f, (m->bmin.z + m->bmax.z) * 0.5f); }

float mesh_radius(CadMesh *m) {
    V3 c = mesh_center(m);
    float r = 0;
    for (int i = 0; i < m->nverts; i++) {
        V3 v = { m->pos[i * 3] - c.x, m->pos[i * 3 + 1] - c.y, m->pos[i * 3 + 2] - c.z };
        float d = v3_len(v);
        if (d > r) r = d;
    }
    return r;
}

void mesh_free(CadMesh *m) {
    free(m->pos);
    free(m->nrm);
    free(m->idx);
    memset(m, 0, sizeof(*m));
}

void mesh_transform(CadMesh *src, CadMesh *dst, V3 pos, V3 rotdeg) {
    float rx = rotdeg.x * 3.14159265f / 180.0f;
    float ry = rotdeg.y * 3.14159265f / 180.0f;
    float rz = rotdeg.z * 3.14159265f / 180.0f;
    dst->nverts = src->nverts;
    dst->ntris = src->ntris;
    dst->pos = (float *)malloc(sizeof(float) * 3 * (size_t)src->nverts);
    if (src->nrm) {
        dst->nrm = (float *)malloc(sizeof(float) * 3 * (size_t)src->nverts);
    } else dst->nrm = NULL;
    if (src->idx) {
        dst->idx = (int *)malloc(sizeof(int) * 3 * (size_t)src->ntris);
        memcpy(dst->idx, src->idx, sizeof(int) * 3 * (size_t)src->ntris);
    } else dst->idx = NULL;
    for (int i = 0; i < src->nverts; i++) {
        V3 v = { src->pos[i * 3], src->pos[i * 3 + 1], src->pos[i * 3 + 2] };
        v = v3_rot_x(v, rx);
        v = v3_rot_y(v, ry);
        v = v3_rot_z(v, rz);
        v = v3_add(v, pos);
        dst->pos[i * 3] = v.x; dst->pos[i * 3 + 1] = v.y; dst->pos[i * 3 + 2] = v.z;
        if (src->nrm && dst->nrm) {
            V3 n = { src->nrm[i * 3], src->nrm[i * 3 + 1], src->nrm[i * 3 + 2] };
            n = v3_rot_x(n, rx); n = v3_rot_y(n, ry); n = v3_rot_z(n, rz);
            dst->nrm[i * 3] = n.x; dst->nrm[i * 3 + 1] = n.y; dst->nrm[i * 3 + 2] = n.z;
        }
    }
    mesh_bbox(dst);
    dst->valid = src->valid;
}

// ============ JSON serialization ============
static void prim_write(JsonW *w, Prim *pr) {
    jw_arr_item_obj(w);
    jw_key(w, "type"); jw_int(w, pr->type);
    jw_key(w, "p1x"); jw_num(w, pr->p1.x); jw_key(w, "p1y"); jw_num(w, pr->p1.y);
    jw_key(w, "p2x"); jw_num(w, pr->p2.x); jw_key(w, "p2y"); jw_num(w, pr->p2.y);
    jw_key(w, "radius"); jw_num(w, pr->radius);
    jw_key(w, "a1"); jw_num(w, pr->a1); jw_key(w, "a2"); jw_num(w, pr->a2);
    jw_key(w, "thickness"); jw_num(w, pr->thickness);
    jw_key(w, "layer"); jw_str(w, pr->layer);
    jw_key(w, "text"); jw_str(w, pr->text);
    jw_key(w, "filled"); jw_bool(w, pr->filled != 0);
    if (pr->type == PRIM_POLY && pr->npoly > 0) {
        jw_key(w, "poly");
        jw_arr(w);
        for (int i = 0; i < pr->npoly; i++) { jw_arr_item_obj(w); jw_key(w, "x"); jw_num(w, pr->poly[i].x); jw_key(w, "y"); jw_num(w, pr->poly[i].y); jw_end(w); }
        jw_end(w);
    }
    jw_end(w);
}

static Prim prim_read(JsonValue *obj) {
    Prim pr; memset(&pr, 0, sizeof(pr));
    pr.type = (int)json_get_num(obj, "type");
    pr.p1 = v2((float)json_get_num(obj, "p1x"), (float)json_get_num(obj, "p1y"));
    pr.p2 = v2((float)json_get_num(obj, "p2x"), (float)json_get_num(obj, "p2y"));
    pr.radius = (float)json_get_num(obj, "radius");
    pr.a1 = (float)json_get_num(obj, "a1");
    pr.a2 = (float)json_get_num(obj, "a2");
    pr.thickness = (float)json_get_num(obj, "thickness");
    const char *layer = json_get_str(obj, "layer");
    const char *text = json_get_str(obj, "text");
    pr.layer = str_dup(layer ? layer : "");
    pr.text = str_dup(text ? text : "");
    pr.filled = json_get_bool(obj, "filled") ? 1 : 0;
    JsonValue *poly = json_get_arr(obj, "poly");
    if (poly && pr.type == PRIM_POLY) {
        int n = json_arr_len(poly);
        pr.poly = (V2 *)malloc(sizeof(V2) * (size_t)(n ? n : 1));
        pr.npoly = n;
        for (int i = 0; i < n; i++) {
            JsonValue *it = json_arr_get(poly, i);
            pr.poly[i] = v2((float)json_get_num(it, "x"), (float)json_get_num(it, "y"));
        }
    }
    return pr;
}

static void symbol_write(JsonW *w, Symbol *s) {
    jw_arr_item_obj(w);
    jw_key(w, "name"); jw_str(w, s->name);
    jw_key(w, "reference"); jw_str(w, s->reference);
    jw_key(w, "pins");
    jw_arr(w);
    for (int i = 0; i < s->npins; i++) {
        jw_arr_item_obj(w);
        jw_key(w, "name"); jw_str(w, s->pins[i].name);
        jw_key(w, "number"); jw_str(w, s->pins[i].number);
        jw_key(w, "x"); jw_num(w, s->pins[i].pos.x);
        jw_key(w, "y"); jw_num(w, s->pins[i].pos.y);
        jw_key(w, "angle"); jw_num(w, s->pins[i].angle);
        jw_key(w, "ptype"); jw_int(w, s->pins[i].ptype);
        jw_end(w);
    }
    jw_end(w);
    jw_key(w, "prims");
    jw_arr(w);
    for (int i = 0; i < s->nprims; i++) prim_write(w, &s->prims[i]);
    jw_end(w);
    jw_end(w);
}

static void symbol_read(Symbol *s, JsonValue *obj) {
    memset(s, 0, sizeof(*s));
    const char *name = json_get_str(obj, "name");
    const char *ref = json_get_str(obj, "reference");
    s->name = str_dup(name ? name : "sym");
    s->reference = str_dup(ref ? ref : "U");
    JsonValue *pins = json_get_arr(obj, "pins");
    if (pins) {
        s->npins = json_arr_len(pins);
        s->pins = (Pin *)calloc((size_t)(s->npins ? s->npins : 1), sizeof(Pin));
        for (int i = 0; i < s->npins; i++) {
            JsonValue *it = json_arr_get(pins, i);
            const char *pn = json_get_str(it, "name");
            const char *nn = json_get_str(it, "number");
            s->pins[i].name = str_dup(pn ? pn : "");
            s->pins[i].number = str_dup(nn ? nn : "");
            s->pins[i].pos = v2((float)json_get_num(it, "x"), (float)json_get_num(it, "y"));
            s->pins[i].angle = (float)json_get_num(it, "angle");
            s->pins[i].ptype = (int)json_get_num(it, "ptype");
        }
    }
    JsonValue *prims = json_get_arr(obj, "prims");
    if (prims) {
        s->nprims = json_arr_len(prims);
        s->prims = (Prim *)calloc((size_t)(s->nprims ? s->nprims : 1), sizeof(Prim));
        for (int i = 0; i < s->nprims; i++) s->prims[i] = prim_read(json_arr_get(prims, i));
    }
}

static void footprint_write(JsonW *w, Footprint *f) {
    jw_arr_item_obj(w);
    jw_key(w, "name"); jw_str(w, f->name);
    jw_key(w, "model3d"); jw_str(w, f->model3d);
    jw_key(w, "pads");
    jw_arr(w);
    for (int i = 0; i < f->npads; i++) {
        jw_arr_item_obj(w);
        jw_key(w, "name"); jw_str(w, f->pads[i].name);
        jw_key(w, "x"); jw_num(w, f->pads[i].pos.x);
        jw_key(w, "y"); jw_num(w, f->pads[i].pos.y);
        jw_key(w, "sx"); jw_num(w, f->pads[i].size.x);
        jw_key(w, "sy"); jw_num(w, f->pads[i].size.y);
        jw_key(w, "angle"); jw_num(w, f->pads[i].angle);
        jw_key(w, "shape"); jw_int(w, f->pads[i].shape);
        jw_key(w, "layer"); jw_int(w, f->pads[i].layer);
        jw_key(w, "type"); jw_int(w, f->pads[i].type);
        jw_end(w);
    }
    jw_end(w);
    jw_key(w, "prims");
    jw_arr(w);
    for (int i = 0; i < f->nprims; i++) prim_write(w, &f->prims[i]);
    jw_end(w);
    jw_end(w);
}

static void footprint_read(Footprint *f, JsonValue *obj) {
    memset(f, 0, sizeof(*f));
    const char *name = json_get_str(obj, "name");
    const char *m3 = json_get_str(obj, "model3d");
    f->name = str_dup(name ? name : "fp");
    f->model3d = str_dup(m3 ? m3 : "");
    JsonValue *pads = json_get_arr(obj, "pads");
    if (pads) {
        f->npads = json_arr_len(pads);
        f->pads = (Pad *)calloc((size_t)(f->npads ? f->npads : 1), sizeof(Pad));
        for (int i = 0; i < f->npads; i++) {
            JsonValue *it = json_arr_get(pads, i);
            const char *pn = json_get_str(it, "name");
            f->pads[i].name = str_dup(pn ? pn : "");
            f->pads[i].pos = v2((float)json_get_num(it, "x"), (float)json_get_num(it, "y"));
            f->pads[i].size = v2((float)json_get_num(it, "sx"), (float)json_get_num(it, "sy"));
            f->pads[i].angle = (float)json_get_num(it, "angle");
            f->pads[i].shape = (int)json_get_num(it, "shape");
            f->pads[i].layer = (int)json_get_num(it, "layer");
            f->pads[i].type = (int)json_get_num(it, "type");
        }
    }
    JsonValue *prims = json_get_arr(obj, "prims");
    if (prims) {
        f->nprims = json_arr_len(prims);
        f->prims = (Prim *)calloc((size_t)(f->nprims ? f->nprims : 1), sizeof(Prim));
        for (int i = 0; i < f->nprims; i++) f->prims[i] = prim_read(json_arr_get(prims, i));
    }
}

static void component_write(JsonW *w, Component *c) {
    jw_arr_item_obj(w);
    jw_key(w, "id"); jw_str(w, c->id);
    jw_key(w, "name"); jw_str(w, c->name);
    jw_key(w, "description"); jw_str(w, c->description);
    jw_key(w, "source_file"); jw_str(w, c->source_file);
    jw_key(w, "manufacturer"); jw_str(w, c->manufacturer);
    jw_key(w, "package"); jw_str(w, c->package);
    jw_key(w, "footprint"); jw_str(w, c->footprint);
    jw_key(w, "sx"); jw_num(w, c->size_mm.x);
    jw_key(w, "sy"); jw_num(w, c->size_mm.y);
    jw_key(w, "sz"); jw_num(w, c->size_mm.z);
    jw_key(w, "height"); jw_num(w, c->height_mm);
    jw_key(w, "specs");
    jw_arr(w);
    for (int i = 0; i < c->nspecs; i++) { jw_arr_item_obj(w); jw_key(w, "k"); jw_str(w, c->specs[i].key); jw_key(w, "v"); jw_str(w, c->specs[i].value); jw_end(w); }
    jw_end(w);
    jw_key(w, "pins");
    jw_arr(w);
    for (int i = 0; i < c->npins; i++) { jw_arr_item_obj(w); jw_key(w, "n"); jw_str(w, c->pins[i].name); jw_key(w, "num"); jw_str(w, c->pins[i].number); jw_key(w, "fn"); jw_str(w, c->pins[i].function); jw_end(w); }
    jw_end(w);
    jw_key(w, "doc_text"); jw_str(w, c->doc_text);
    jw_key(w, "doc_id"); jw_str(w, c->doc_id);
    jw_end(w);
}

static void component_read(Component *c, JsonValue *obj) {
    memset(c, 0, sizeof(*c));
    c->id = str_dup(json_get_str(obj, "id") ? json_get_str(obj, "id") : make_id());
    c->name = str_dup(json_get_str(obj, "name") ? json_get_str(obj, "name") : "part");
    c->description = str_dup(json_get_str(obj, "description"));
    c->source_file = str_dup(json_get_str(obj, "source_file"));
    c->manufacturer = str_dup(json_get_str(obj, "manufacturer"));
    c->package = str_dup(json_get_str(obj, "package"));
    c->footprint = str_dup(json_get_str(obj, "footprint"));
    c->size_mm = v3((float)json_get_num(obj, "sx"), (float)json_get_num(obj, "sy"), (float)json_get_num(obj, "sz"));
    c->height_mm = (float)json_get_num(obj, "height");
    JsonValue *specs = json_get_arr(obj, "specs");
    if (specs) {
        c->nspecs = json_arr_len(specs);
        c->specs = (Spec *)calloc((size_t)(c->nspecs ? c->nspecs : 1), sizeof(Spec));
        for (int i = 0; i < c->nspecs; i++) {
            JsonValue *it = json_arr_get(specs, i);
            c->specs[i].key = str_dup(json_get_str(it, "k") ? json_get_str(it, "k") : "");
            c->specs[i].value = str_dup(json_get_str(it, "v") ? json_get_str(it, "v") : "");
        }
    }
    JsonValue *pins = json_get_arr(obj, "pins");
    if (pins) {
        c->npins = json_arr_len(pins);
        c->pins = (PinMap *)calloc((size_t)(c->npins ? c->npins : 1), sizeof(PinMap));
        for (int i = 0; i < c->npins; i++) {
            JsonValue *it = json_arr_get(pins, i);
            c->pins[i].name = str_dup(json_get_str(it, "n") ? json_get_str(it, "n") : "");
            c->pins[i].number = str_dup(json_get_str(it, "num") ? json_get_str(it, "num") : "");
            c->pins[i].function = str_dup(json_get_str(it, "fn") ? json_get_str(it, "fn") : "");
        }
    }
    c->doc_text = str_dup(json_get_str(obj, "doc_text"));
    c->doc_id = str_dup(json_get_str(obj, "doc_id"));
}

static void schematic_write(JsonW *w, Schematic *s) {
    jw_arr_item_obj(w);
    jw_key(w, "id"); jw_str(w, s->id);
    jw_key(w, "name"); jw_str(w, s->name);
    jw_key(w, "insts");
    jw_arr(w);
    for (int i = 0; i < s->ninsts; i++) {
        SymInst *si = &s->insts[i];
        jw_arr_item_obj(w);
        jw_key(w, "id"); jw_str(w, si->id);
        jw_key(w, "ref"); jw_str(w, si->ref);
        jw_key(w, "symbol"); jw_str(w, si->symbol);
        jw_key(w, "value"); jw_str(w, si->value);
        jw_key(w, "x"); jw_num(w, si->pos.x);
        jw_key(w, "y"); jw_num(w, si->pos.y);
        jw_key(w, "rotation"); jw_num(w, si->rotation);
        jw_key(w, "mirrored"); jw_bool(w, si->mirrored != 0);
        jw_end(w);
    }
    jw_end(w);
    jw_key(w, "wires");
    jw_arr(w);
    for (int i = 0; i < s->nwires; i++) { jw_arr_item_obj(w); jw_key(w, "ax"); jw_num(w, s->wires[i].a.x); jw_key(w, "ay"); jw_num(w, s->wires[i].a.y); jw_key(w, "bx"); jw_num(w, s->wires[i].b.x); jw_key(w, "by"); jw_num(w, s->wires[i].b.y); jw_end(w); }
    jw_end(w);
    jw_key(w, "labels");
    jw_arr(w);
    for (int i = 0; i < s->nlabels; i++) { jw_arr_item_obj(w); jw_key(w, "name"); jw_str(w, s->labels[i].name); jw_key(w, "x"); jw_num(w, s->labels[i].pos.x); jw_key(w, "y"); jw_num(w, s->labels[i].pos.y); jw_end(w); }
    jw_end(w);
    jw_key(w, "texts");
    jw_arr(w);
    for (int i = 0; i < s->ntexts; i++) { jw_arr_item_obj(w); jw_key(w, "t"); jw_str(w, s->texts[i].text); jw_key(w, "x"); jw_num(w, s->texts[i].pos.x); jw_key(w, "y"); jw_num(w, s->texts[i].pos.y); jw_key(w, "size"); jw_int(w, s->texts[i].size); jw_end(w); }
    jw_end(w);
    jw_end(w);
}

static void schematic_read(Schematic *s, JsonValue *obj) {
    memset(s, 0, sizeof(*s));
    s->id = str_dup(json_get_str(obj, "id") ? json_get_str(obj, "id") : make_id());
    s->name = str_dup(json_get_str(obj, "name") ? json_get_str(obj, "name") : "schematic");
    JsonValue *insts = json_get_arr(obj, "insts");
    if (insts) {
        s->ninsts = json_arr_len(insts);
        s->insts = (SymInst *)calloc((size_t)(s->ninsts ? s->ninsts : 1), sizeof(SymInst));
        for (int i = 0; i < s->ninsts; i++) {
            JsonValue *it = json_arr_get(insts, i);
            s->insts[i].id = str_dup(json_get_str(it, "id") ? json_get_str(it, "id") : make_id());
            s->insts[i].ref = str_dup(json_get_str(it, "ref") ? json_get_str(it, "ref") : "U?");
            s->insts[i].symbol = str_dup(json_get_str(it, "symbol") ? json_get_str(it, "symbol") : "");
            s->insts[i].value = str_dup(json_get_str(it, "value") ? json_get_str(it, "value") : "");
            s->insts[i].pos = v2((float)json_get_num(it, "x"), (float)json_get_num(it, "y"));
            s->insts[i].rotation = (float)json_get_num(it, "rotation");
            s->insts[i].mirrored = json_get_bool(it, "mirrored") ? 1 : 0;
        }
    }
    JsonValue *wires = json_get_arr(obj, "wires");
    if (wires) {
        s->nwires = json_arr_len(wires);
        s->wires = (Wire *)calloc((size_t)(s->nwires ? s->nwires : 1), sizeof(Wire));
        for (int i = 0; i < s->nwires; i++) {
            JsonValue *it = json_arr_get(wires, i);
            s->wires[i].a = v2((float)json_get_num(it, "ax"), (float)json_get_num(it, "ay"));
            s->wires[i].b = v2((float)json_get_num(it, "bx"), (float)json_get_num(it, "by"));
        }
    }
    JsonValue *labels = json_get_arr(obj, "labels");
    if (labels) {
        s->nlabels = json_arr_len(labels);
        s->labels = (NetLabel *)calloc((size_t)(s->nlabels ? s->nlabels : 1), sizeof(NetLabel));
        for (int i = 0; i < s->nlabels; i++) {
            JsonValue *it = json_arr_get(labels, i);
            s->labels[i].name = str_dup(json_get_str(it, "name") ? json_get_str(it, "name") : "");
            s->labels[i].pos = v2((float)json_get_num(it, "x"), (float)json_get_num(it, "y"));
        }
    }
    JsonValue *texts = json_get_arr(obj, "texts");
    if (texts) {
        s->ntexts = json_arr_len(texts);
        s->texts = (SchText *)calloc((size_t)(s->ntexts ? s->ntexts : 1), sizeof(SchText));
        for (int i = 0; i < s->ntexts; i++) {
            JsonValue *it = json_arr_get(texts, i);
            s->texts[i].text = str_dup(json_get_str(it, "t") ? json_get_str(it, "t") : "");
            s->texts[i].pos = v2((float)json_get_num(it, "x"), (float)json_get_num(it, "y"));
            s->texts[i].size = (int)json_get_num(it, "size");
        }
    }
}

static void pcb_write(JsonW *w, Pcb *p) {
    jw_arr_item_obj(w);
    jw_key(w, "id"); jw_str(w, p->id);
    jw_key(w, "name"); jw_str(w, p->name);
    jw_key(w, "thickness"); jw_num(w, p->thickness);
    jw_key(w, "outline");
    jw_arr(w);
    for (int i = 0; i < p->noutline; i++) { jw_arr_item_obj(w); jw_key(w, "x"); jw_num(w, p->outline[i].x); jw_key(w, "y"); jw_num(w, p->outline[i].y); jw_end(w); }
    jw_end(w);
    jw_key(w, "fps");
    jw_arr(w);
    for (int i = 0; i < p->nfps; i++) {
        FpInst *f = &p->fps[i];
        jw_arr_item_obj(w);
        jw_key(w, "id"); jw_str(w, f->id);
        jw_key(w, "ref"); jw_str(w, f->ref);
        jw_key(w, "footprint"); jw_str(w, f->footprint);
        jw_key(w, "value"); jw_str(w, f->value);
        jw_key(w, "component_id"); jw_str(w, f->component_id);
        jw_key(w, "x"); jw_num(w, f->pos.x);
        jw_key(w, "y"); jw_num(w, f->pos.y);
        jw_key(w, "rotation"); jw_num(w, f->rotation);
        jw_key(w, "side"); jw_int(w, f->side);
        jw_end(w);
    }
    jw_end(w);
    jw_key(w, "tracks");
    jw_arr(w);
    for (int i = 0; i < p->ntracks; i++) { jw_arr_item_obj(w); jw_key(w, "ax"); jw_num(w, p->tracks[i].a.x); jw_key(w, "ay"); jw_num(w, p->tracks[i].a.y); jw_key(w, "bx"); jw_num(w, p->tracks[i].b.x); jw_key(w, "by"); jw_num(w, p->tracks[i].b.y); jw_key(w, "width"); jw_num(w, p->tracks[i].width); jw_key(w, "net"); jw_str(w, p->tracks[i].net); jw_end(w); }
    jw_end(w);
    jw_key(w, "vias");
    jw_arr(w);
    for (int i = 0; i < p->nvias; i++) { jw_arr_item_obj(w); jw_key(w, "x"); jw_num(w, p->vias[i].pos.x); jw_key(w, "y"); jw_num(w, p->vias[i].pos.y); jw_key(w, "drill"); jw_num(w, p->vias[i].drill); jw_key(w, "outer"); jw_num(w, p->vias[i].outer); jw_key(w, "net"); jw_str(w, p->vias[i].net); jw_key(w, "name"); jw_str(w, p->vias[i].name); jw_end(w); }
    jw_end(w);
    jw_key(w, "prims");
    jw_arr(w);
    for (int i = 0; i < p->nprims; i++) prim_write(w, &p->prims[i]);
    jw_end(w);
    jw_end(w);
}

static void pcb_read(Pcb *p, JsonValue *obj) {
    memset(p, 0, sizeof(*p));
    p->id = str_dup(json_get_str(obj, "id") ? json_get_str(obj, "id") : make_id());
    p->name = str_dup(json_get_str(obj, "name") ? json_get_str(obj, "name") : "pcb");
    p->thickness = (float)json_get_num(obj, "thickness");
    JsonValue *outline = json_get_arr(obj, "outline");
    if (outline) {
        p->noutline = json_arr_len(outline);
        p->outline = (V2 *)calloc((size_t)(p->noutline ? p->noutline : 1), sizeof(V2));
        for (int i = 0; i < p->noutline; i++) {
            JsonValue *it = json_arr_get(outline, i);
            p->outline[i] = v2((float)json_get_num(it, "x"), (float)json_get_num(it, "y"));
        }
    }
    JsonValue *fps = json_get_arr(obj, "fps");
    if (fps) {
        p->nfps = json_arr_len(fps);
        p->fps = (FpInst *)calloc((size_t)(p->nfps ? p->nfps : 1), sizeof(FpInst));
        for (int i = 0; i < p->nfps; i++) {
            JsonValue *it = json_arr_get(fps, i);
            p->fps[i].id = str_dup(json_get_str(it, "id") ? json_get_str(it, "id") : make_id());
            p->fps[i].ref = str_dup(json_get_str(it, "ref") ? json_get_str(it, "ref") : "U?");
            p->fps[i].footprint = str_dup(json_get_str(it, "footprint") ? json_get_str(it, "footprint") : "");
            p->fps[i].value = str_dup(json_get_str(it, "value") ? json_get_str(it, "value") : "");
            p->fps[i].component_id = str_dup(json_get_str(it, "component_id") ? json_get_str(it, "component_id") : "");
            p->fps[i].pos = v2((float)json_get_num(it, "x"), (float)json_get_num(it, "y"));
            p->fps[i].rotation = (float)json_get_num(it, "rotation");
            p->fps[i].side = (int)json_get_num(it, "side");
        }
    }
    JsonValue *tracks = json_get_arr(obj, "tracks");
    if (tracks) {
        p->ntracks = json_arr_len(tracks);
        p->tracks = (Track *)calloc((size_t)(p->ntracks ? p->ntracks : 1), sizeof(Track));
        for (int i = 0; i < p->ntracks; i++) {
            JsonValue *it = json_arr_get(tracks, i);
            p->tracks[i].a = v2((float)json_get_num(it, "ax"), (float)json_get_num(it, "ay"));
            p->tracks[i].b = v2((float)json_get_num(it, "bx"), (float)json_get_num(it, "by"));
            p->tracks[i].width = (float)json_get_num(it, "width");
            p->tracks[i].net = str_dup(json_get_str(it, "net") ? json_get_str(it, "net") : "");
        }
    }
    JsonValue *vias = json_get_arr(obj, "vias");
    if (vias) {
        p->nvias = json_arr_len(vias);
        p->vias = (Via *)calloc((size_t)(p->nvias ? p->nvias : 1), sizeof(Via));
        for (int i = 0; i < p->nvias; i++) {
            JsonValue *it = json_arr_get(vias, i);
            p->vias[i].pos = v2((float)json_get_num(it, "x"), (float)json_get_num(it, "y"));
            p->vias[i].drill = (float)json_get_num(it, "drill");
            p->vias[i].outer = (float)json_get_num(it, "outer");
            p->vias[i].net = str_dup(json_get_str(it, "net") ? json_get_str(it, "net") : "");
            p->vias[i].name = str_dup(json_get_str(it, "name") ? json_get_str(it, "name") : "");
        }
    }
    JsonValue *prims = json_get_arr(obj, "prims");
    if (prims) {
        p->nprims = json_arr_len(prims);
        p->prims = (Prim *)calloc((size_t)(p->nprims ? p->nprims : 1), sizeof(Prim));
        for (int i = 0; i < p->nprims; i++) p->prims[i] = prim_read(json_arr_get(prims, i));
    }
}

static void cad_write(JsonW *w, CadModel *c) {
    jw_arr_item_obj(w);
    jw_key(w, "id"); jw_str(w, c->id);
    jw_key(w, "name"); jw_str(w, c->name);
    jw_key(w, "source_file"); jw_str(w, c->source_file);
    jw_key(w, "nverts"); jw_int(w, c->mesh.nverts);
    jw_key(w, "ntris"); jw_int(w, c->mesh.ntris);
    if (c->mesh.pos && c->mesh.nverts > 0) {
        jw_key(w, "pos"); jw_arr(w);
        for (int i = 0; i < c->mesh.nverts * 3; i++) jw_arr_item_num(w, c->mesh.pos[i]);
        jw_end(w);
    }
    if (c->mesh.nrm && c->mesh.nverts > 0) {
        jw_key(w, "nrm"); jw_arr(w);
        for (int i = 0; i < c->mesh.nverts * 3; i++) jw_arr_item_num(w, c->mesh.nrm[i]);
        jw_end(w);
    }
    if (c->mesh.idx && c->mesh.ntris > 0) {
        jw_key(w, "idx"); jw_arr(w);
        for (int i = 0; i < c->mesh.ntris * 3; i++) jw_arr_item_num(w, (double)c->mesh.idx[i]);
        jw_end(w);
    }
    jw_end(w);
}

static void cad_read(CadModel *c, JsonValue *obj) {
    memset(c, 0, sizeof(*c));
    c->id = str_dup(json_get_str(obj, "id") ? json_get_str(obj, "id") : make_id());
    c->name = str_dup(json_get_str(obj, "name") ? json_get_str(obj, "name") : "model");
    c->source_file = str_dup(json_get_str(obj, "source_file") ? json_get_str(obj, "source_file") : "");
    c->mesh.nverts = (int)json_get_num(obj, "nverts");
    c->mesh.ntris = (int)json_get_num(obj, "ntris");
    JsonValue *pos = json_get_arr(obj, "pos");
    if (pos && c->mesh.nverts > 0) {
        c->mesh.pos = (float *)malloc(sizeof(float) * 3 * (size_t)c->mesh.nverts);
        int n = json_arr_len(pos);
        for (int i = 0; i < c->mesh.nverts * 3 && i < n; i++) c->mesh.pos[i] = (float)json_num_value(json_arr_get(pos, i));
    }
    JsonValue *nrm = json_get_arr(obj, "nrm");
    if (nrm && c->mesh.nverts > 0) {
        c->mesh.nrm = (float *)malloc(sizeof(float) * 3 * (size_t)c->mesh.nverts);
        int n = json_arr_len(nrm);
        for (int i = 0; i < c->mesh.nverts * 3 && i < n; i++) c->mesh.nrm[i] = (float)json_num_value(json_arr_get(nrm, i));
    }
    JsonValue *idx = json_get_arr(obj, "idx");
    if (idx && c->mesh.ntris > 0) {
        c->mesh.idx = (int *)malloc(sizeof(int) * 3 * (size_t)c->mesh.ntris);
        int n = json_arr_len(idx);
        for (int i = 0; i < c->mesh.ntris * 3 && i < n; i++) c->mesh.idx[i] = (int)json_num_value(json_arr_get(idx, i));
    }
    mesh_bbox(&c->mesh);
}

static void assembly_write(JsonW *w, Assembly *a) {
    jw_arr_item_obj(w);
    jw_key(w, "id"); jw_str(w, a->id);
    jw_key(w, "name"); jw_str(w, a->name);
    jw_key(w, "items");
    jw_arr(w);
    for (int i = 0; i < a->nitems; i++) {
        PlaceItem *it = &a->items[i];
        jw_arr_item_obj(w);
        jw_key(w, "kind"); jw_int(w, it->kind);
        jw_key(w, "part_id"); jw_str(w, it->part_id);
        jw_key(w, "label"); jw_str(w, it->label);
        jw_key(w, "x"); jw_num(w, it->pos.x);
        jw_key(w, "y"); jw_num(w, it->pos.y);
        jw_key(w, "z"); jw_num(w, it->pos.z);
        jw_key(w, "rx"); jw_num(w, it->rot.x);
        jw_key(w, "ry"); jw_num(w, it->rot.y);
        jw_key(w, "rz"); jw_num(w, it->rot.z);
        jw_key(w, "flipped"); jw_bool(w, it->flipped != 0);
        jw_end(w);
    }
    jw_end(w);
    jw_key(w, "joints");
    jw_arr(w);
    for (int i = 0; i < a->njoints; i++) {
        Joint *j = &a->joints[i];
        jw_arr_item_obj(w);
        jw_key(w, "type"); jw_int(w, j->type);
        jw_key(w, "a"); jw_int(w, j->a);
        jw_key(w, "b"); jw_int(w, j->b);
        jw_key(w, "ax"); jw_num(w, j->axis.x);
        jw_key(w, "ay"); jw_num(w, j->axis.y);
        jw_key(w, "az"); jw_num(w, j->axis.z);
        jw_key(w, "px"); jw_num(w, j->pivot.x);
        jw_key(w, "py"); jw_num(w, j->pivot.y);
        jw_key(w, "pz"); jw_num(w, j->pivot.z);
        jw_key(w, "angle"); jw_num(w, j->angle);
        jw_key(w, "speed"); jw_num(w, j->speed);
        jw_key(w, "amp"); jw_num(w, j->amp);
        jw_key(w, "anim"); jw_bool(w, j->anim != 0);
        jw_end(w);
    }
    jw_end(w);
    jw_key(w, "animated"); jw_bool(w, a->animated != 0);
    jw_end(w);
}

static void assembly_read(Assembly *a, JsonValue *obj) {
    memset(a, 0, sizeof(*a));
    a->id = str_dup(json_get_str(obj, "id") ? json_get_str(obj, "id") : make_id());
    a->name = str_dup(json_get_str(obj, "name") ? json_get_str(obj, "name") : "assembly");
    JsonValue *items = json_get_arr(obj, "items");
    if (items) {
        a->nitems = json_arr_len(items);
        a->items = (PlaceItem *)calloc((size_t)(a->nitems ? a->nitems : 1), sizeof(PlaceItem));
        for (int i = 0; i < a->nitems; i++) {
            JsonValue *it = json_arr_get(items, i);
            a->items[i].kind = (int)json_get_num(it, "kind");
            a->items[i].part_id = str_dup(json_get_str(it, "part_id") ? json_get_str(it, "part_id") : "");
            a->items[i].label = str_dup(json_get_str(it, "label") ? json_get_str(it, "label") : "");
            a->items[i].pos = v3((float)json_get_num(it, "x"), (float)json_get_num(it, "y"), (float)json_get_num(it, "z"));
            a->items[i].rot = v3((float)json_get_num(it, "rx"), (float)json_get_num(it, "ry"), (float)json_get_num(it, "rz"));
            a->items[i].flipped = json_get_bool(it, "flipped") ? 1 : 0;
        }
    }
    JsonValue *joints = json_get_arr(obj, "joints");
    if (joints) {
        a->njoints = json_arr_len(joints);
        a->joints = (Joint *)calloc((size_t)(a->njoints ? a->njoints : 1), sizeof(Joint));
        for (int i = 0; i < a->njoints; i++) {
            JsonValue *j = json_arr_get(joints, i);
            a->joints[i].type = (int)json_get_num(j, "type");
            a->joints[i].a = (int)json_get_num(j, "a");
            a->joints[i].b = (int)json_get_num(j, "b");
            a->joints[i].axis = v3((float)json_get_num(j, "ax"), (float)json_get_num(j, "ay"), (float)json_get_num(j, "az"));
            a->joints[i].pivot = v3((float)json_get_num(j, "px"), (float)json_get_num(j, "py"), (float)json_get_num(j, "pz"));
            a->joints[i].angle = (float)json_get_num(j, "angle");
            a->joints[i].speed = (float)json_get_num(j, "speed");
            a->joints[i].amp = (float)json_get_num(j, "amp");
            a->joints[i].anim = json_get_bool(j, "anim") ? 1 : 0;
        }
    }
    a->animated = json_get_bool(obj, "animated") ? 1 : 0;
}

static void doc_write(JsonW *w, Doc *d) {
    jw_arr_item_obj(w);
    jw_key(w, "id"); jw_str(w, d->id);
    jw_key(w, "name"); jw_str(w, d->name);
    jw_key(w, "source_file"); jw_str(w, d->source_file);
    jw_key(w, "text"); jw_str(w, d->text);
    jw_key(w, "parsed"); jw_bool(w, d->parsed != 0);
    jw_end(w);
}

static void doc_read(Doc *d, JsonValue *obj) {
    memset(d, 0, sizeof(*d));
    d->id = str_dup(json_get_str(obj, "id") ? json_get_str(obj, "id") : make_id());
    d->name = str_dup(json_get_str(obj, "name") ? json_get_str(obj, "name") : "doc");
    d->source_file = str_dup(json_get_str(obj, "source_file") ? json_get_str(obj, "source_file") : "");
    d->text = str_dup(json_get_str(obj, "text") ? json_get_str(obj, "text") : "");
    d->parsed = json_get_bool(obj, "parsed") ? 1 : 0;
}

bool project_save(Project *p) {
    JsonW w; jw_init(&w);
    jw_begin(&w);
    jw_key(&w, "app"); jw_str(&w, "Volt360");
    jw_key(&w, "name"); jw_str(&w, p->name);
    jw_key(&w, "llm_model"); jw_str(&w, p->llm_model);
    jw_key(&w, "llm_url"); jw_str(&w, p->llm_url);

    jw_key(&w, "symbols"); jw_arr(&w);
    for (int i = 0; i < p->symbols.len; i++) symbol_write(&w, &p->symbols.v[i]);
    jw_end(&w);

    jw_key(&w, "footprints"); jw_arr(&w);
    for (int i = 0; i < p->footprints.len; i++) footprint_write(&w, &p->footprints.v[i]);
    jw_end(&w);

    jw_key(&w, "components"); jw_arr(&w);
    for (int i = 0; i < p->components.len; i++) component_write(&w, &p->components.v[i]);
    jw_end(&w);

    jw_key(&w, "cad_models"); jw_arr(&w);
    for (int i = 0; i < p->cad_models.len; i++) cad_write(&w, &p->cad_models.v[i]);
    jw_end(&w);

    jw_key(&w, "schematics"); jw_arr(&w);
    for (int i = 0; i < p->schematics.len; i++) schematic_write(&w, &p->schematics.v[i]);
    jw_end(&w);

    jw_key(&w, "pcbs"); jw_arr(&w);
    for (int i = 0; i < p->pcbs.len; i++) pcb_write(&w, &p->pcbs.v[i]);
    jw_end(&w);

    jw_key(&w, "assemblies"); jw_arr(&w);
    for (int i = 0; i < p->assemblies.len; i++) assembly_write(&w, &p->assemblies.v[i]);
    jw_end(&w);

    jw_key(&w, "docs"); jw_arr(&w);
    for (int i = 0; i < p->docs.len; i++) doc_write(&w, &p->docs.v[i]);
    jw_end(&w);

    jw_end(&w);
    char *dir = path_dirname(p->path && *p->path ? p->path : ".");
    char *save = path_join(p->path && *p->path ? p->path : ".", "project.json");
    bool ok = file_write(save, jw_cstr(&w), strlen(jw_cstr(&w)));
    if (!ok) {
        // try to create dir
        // (mkdir via system for MVP)
    }
    (void)dir;
    free(save);
    free(dir);
    jw_free(&w);
    return ok;
}

static void load_array(Project *p, JsonValue *root, const char *name, int elem_size, void (*reader)(void *out, JsonValue *it)) {
    (void)elem_size;
    JsonValue *arr = json_get_arr(root, name);
    if (!arr) return;
    for (int i = 0; i < json_arr_len(arr); i++) {
        JsonValue *it = json_arr_get(arr, i);
        if (!it) continue;
        if (strcmp(name, "symbols") == 0) { Symbol s; symbol_read(&s, it); arr_push(p->symbols, s); }
        else if (strcmp(name, "footprints") == 0) { Footprint f; footprint_read(&f, it); arr_push(p->footprints, f); }
        else if (strcmp(name, "components") == 0) { Component c; component_read(&c, it); arr_push(p->components, c); }
        else if (strcmp(name, "cad_models") == 0) { CadModel c; cad_read(&c, it); arr_push(p->cad_models, c); }
        else if (strcmp(name, "schematics") == 0) { Schematic s; schematic_read(&s, it); arr_push(p->schematics, s); }
        else if (strcmp(name, "pcbs") == 0) { Pcb pcb; pcb_read(&pcb, it); arr_push(p->pcbs, pcb); }
        else if (strcmp(name, "assemblies") == 0) { Assembly a; assembly_read(&a, it); arr_push(p->assemblies, a); }
        else if (strcmp(name, "docs") == 0) { Doc d; doc_read(&d, it); arr_push(p->docs, d); }
    }
    (void)reader;
}

bool project_load(Project *p, const char *dir) {
    char *path = path_join(dir, "project.json");
    size_t len = 0;
    char *data = file_read(path, &len);
    free(path);
    if (!data) return false;
    JsonValue *root = json_parse(data, len);
    free(data);
    if (!root || root->type != J_OBJ) { json_free(root); return false; }
    project_init(p, json_get_str(root, "name") ? json_get_str(root, "name") : "Untitled", dir);
    const char *m = json_get_str(root, "llm_model");
    const char *u = json_get_str(root, "llm_url");
    free(p->llm_model); free(p->llm_url);
    p->llm_model = str_dup(m ? m : "llama3");
    p->llm_url = str_dup(u ? u : "http://127.0.0.1:11434");
    load_array(p, root, "symbols", 0, NULL);
    load_array(p, root, "footprints", 0, NULL);
    load_array(p, root, "components", 0, NULL);
    load_array(p, root, "cad_models", 0, NULL);
    load_array(p, root, "schematics", 0, NULL);
    load_array(p, root, "pcbs", 0, NULL);
    load_array(p, root, "assemblies", 0, NULL);
    load_array(p, root, "docs", 0, NULL);
    json_free(root);
    return true;
}

char *project_save_path(Project *p) { return path_join(p->path, "project.json"); }
