#include "co_design.h"
#include "../slicer/gcode.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

void co_enclosure_defaults(EnclosureParams *e) {
    if (!e) return;
    e->wall_thickness = 2.0f;
    e->wall_height = 12.0f;
    e->floor_thickness = 2.0f;
    e->clearance = 2.0f;
    e->standoff_h = 4.0f;
    e->standoff_d = 4.0f;
    e->pcb_thickness = 1.6f;
    e->lid = 1;
}

// ---------------- mesh builder ----------------
static void m_make(CadMesh *m) { memset(m, 0, sizeof(*m)); m->valid = 1; }

static void m_grow(CadMesh *m, int nv, int nt) {
    m->pos = (float *)realloc(m->pos, (size_t)(m->nverts + nv) * 3 * sizeof(float));
    m->nrm = (float *)realloc(m->nrm, (size_t)(m->nverts + nv) * 3 * sizeof(float));
    m->idx = (int *)realloc(m->idx, (size_t)(m->ntris + nt) * 3 * sizeof(int));
}

static void m_vert(CadMesh *m, V3 p, V3 n) {
    int i = m->nverts * 3;
    m->pos[i] = p.x; m->pos[i + 1] = p.y; m->pos[i + 2] = p.z;
    m->nrm[i] = n.x; m->nrm[i + 1] = n.y; m->nrm[i + 2] = n.z;
    m->nverts++;
}

static void m_tri(CadMesh *m, int a, int b, int c) {
    int i = m->ntris * 3;
    m->idx[i] = a; m->idx[i + 1] = b; m->idx[i + 2] = c;
    m->ntris++;
}

// Append a quad (4 verts with normal n) as 2 triangles. Vert order should be
// CCW seen from the normal's side.
static void m_quad(CadMesh *m, V3 p0, V3 p1, V3 p2, V3 p3, V3 n) {
    m_grow(m, 4, 2);
    int base = m->nverts;
    m_vert(m, p0, n);
    m_vert(m, p1, n);
    m_vert(m, p2, n);
    m_vert(m, p3, n);
    m_tri(m, base, base + 1, base + 2);
    m_tri(m, base, base + 2, base + 3);
}

static void m_box(CadMesh *m, V3 c, V3 s) {
    m_grow(m, 24, 12);
    float hx = s.x * 0.5f, hy = s.y * 0.5f, hz = s.z * 0.5f;
    V3 v[8] = {
        v3(c.x - hx, c.y - hy, c.z - hz), v3(c.x + hx, c.y - hy, c.z - hz),
        v3(c.x + hx, c.y + hy, c.z - hz), v3(c.x - hx, c.y + hy, c.z - hz),
        v3(c.x - hx, c.y - hy, c.z + hz), v3(c.x + hx, c.y - hy, c.z + hz),
        v3(c.x + hx, c.y + hy, c.z + hz), v3(c.x - hx, c.y + hy, c.z + hz),
    };
    V3 n[6] = { v3(0,0,-1), v3(0,0,1), v3(0,-1,0), v3(0,1,0), v3(-1,0,0), v3(1,0,0) };
    int faces[6][4] = {
        { 0, 1, 2, 3 }, { 5, 4, 7, 6 }, { 0, 4, 5, 1 },
        { 2, 6, 7, 3 }, { 0, 3, 7, 4 }, { 1, 5, 6, 2 },
    };
    for (int fi = 0; fi < 6; fi++) {
        int base = m->nverts;
        for (int k = 0; k < 4; k++) m_vert(m, v[faces[fi][k]], n[fi]);
        m_tri(m, base, base + 1, base + 2);
        m_tri(m, base, base + 2, base + 3);
    }
}

static void m_cyl(CadMesh *m, V3 c, float r, float h, int segs) {
    m_grow(m, 2 * segs + 2, 4 * segs);
    int base = m->nverts;
    for (int i = 0; i < segs; i++) {
        float a = (float)i / (float)segs * 2.0f * (float)M_PI;
        m_vert(m, v3(c.x + r * cosf(a), c.y + r * sinf(a), c.z - h * 0.5f), v3(cosf(a), sinf(a), 0));
    }
    for (int i = 0; i < segs; i++) {
        float a = (float)i / (float)segs * 2.0f * (float)M_PI;
        m_vert(m, v3(c.x + r * cosf(a), c.y + r * sinf(a), c.z + h * 0.5f), v3(cosf(a), sinf(a), 0));
    }
    for (int i = 0; i < segs; i++) {
        int i0 = base + i, i1 = base + ((i + 1) % segs);
        m_tri(m, i0, i1, base + segs + i0);
        m_tri(m, i1, base + segs + i1, base + segs + i0);
    }
    int tc = m->nverts;
    m_vert(m, v3(c.x, c.y, c.z + h * 0.5f), v3(0, 0, 1));
    for (int i = 0; i < segs; i++) {
        int i0 = base + segs + i, i1 = base + segs + ((i + 1) % segs);
        m_tri(m, tc, i1, i0);
    }
    int bc = m->nverts;
    m_vert(m, v3(c.x, c.y, c.z - h * 0.5f), v3(0, 0, -1));
    for (int i = 0; i < segs; i++) {
        int i0 = base + i, i1 = base + ((i + 1) % segs);
        m_tri(m, bc, i0, i1);
    }
}

// ---------------- enclosure generation ----------------
// Builds a closed, watertight solid: a pocket shell (floor + 4 walls as one
// surface, open at the top), 4 standoff cylinders sitting inside the cavity,
// and an optional lid plate. No overlapping volumes / coincident faces, so the
// slicer handles it cleanly.
bool pcb_enclosure_mesh(Pcb *pcb, const EnclosureParams *ep, CadMesh *out) {
    if (!pcb || pcb->noutline < 3 || !out) return false;
    EnclosureParams e;
    if (ep) e = *ep; else co_enclosure_defaults(&e);
    float mnx = pcb->outline[0].x, mny = pcb->outline[0].y;
    float mxx = mnx, mxy = mny;
    for (int i = 1; i < pcb->noutline; i++) {
        if (pcb->outline[i].x < mnx) mnx = pcb->outline[i].x;
        if (pcb->outline[i].y < mny) mny = pcb->outline[i].y;
        if (pcb->outline[i].x > mxx) mxx = pcb->outline[i].x;
        if (pcb->outline[i].y > mxy) mxy = pcb->outline[i].y;
    }
    float cx = (mnx + mxx) * 0.5f, cy = (mny + mxy) * 0.5f;
    float pw = mxx - mnx, pd = mxy - mny;
    float cavx = pw + 2.0f * e.clearance;
    float cvy = pd + 2.0f * e.clearance;
    float ox = cavx + 2.0f * e.wall_thickness;
    float oy = cvy + 2.0f * e.wall_thickness;
    float ft = e.floor_thickness, wh = e.wall_height;
    float H = ft + wh;
    float hx = ox * 0.5f, hy = oy * 0.5f;
    float chx = cavx * 0.5f, chy = cvy * 0.5f;
    float cxm = cx - hx, cxp = cx + hx, cym = cy - hy, cyp = cy + hy;
    float ccmx = cx - chx, ccpx = cx + chx, ccmz = cy - chy, ccpz = cy + chy;

    m_make(out);
    // outer bottom face (z=0)
    m_quad(out, v3(cxm, cym, 0), v3(cxp, cym, 0), v3(cxp, cyp, 0), v3(cxm, cyp, 0), v3(0, 0, -1));
    // outer side faces (z=0..H)
    m_quad(out, v3(cxm, cym, 0), v3(cxp, cym, 0), v3(cxp, cym, H), v3(cxm, cym, H), v3(0, -1, 0));
    m_quad(out, v3(cxp, cyp, 0), v3(cxm, cyp, 0), v3(cxm, cyp, H), v3(cxp, cyp, H), v3(0, 1, 0));
    m_quad(out, v3(cxm, cyp, 0), v3(cxm, cym, 0), v3(cxm, cym, H), v3(cxm, cyp, H), v3(-1, 0, 0));
    m_quad(out, v3(cxp, cym, 0), v3(cxp, cyp, 0), v3(cxp, cyp, H), v3(cxp, cym, H), v3(1, 0, 0));
    // cavity floor face (z=ft)
    m_quad(out, v3(ccmx, ccmz, ft), v3(ccpx, ccmz, ft), v3(ccpx, ccpz, ft), v3(ccmx, ccpz, ft), v3(0, 0, 1));
    // cavity side faces (z=ft..H), normals point into the cavity
    m_quad(out, v3(ccmx, ccmz, H), v3(ccpx, ccmz, H), v3(ccpx, ccmz, ft), v3(ccmx, ccmz, ft), v3(0, 1, 0));
    m_quad(out, v3(ccpx, ccpz, H), v3(ccmx, ccpz, H), v3(ccmx, ccpz, ft), v3(ccpx, ccpz, ft), v3(0, -1, 0));
    m_quad(out, v3(ccmx, ccpz, H), v3(ccmx, ccmz, H), v3(ccmx, ccmz, ft), v3(ccmx, ccpz, ft), v3(1, 0, 0));
    m_quad(out, v3(ccpx, ccmz, H), v3(ccpx, ccpz, H), v3(ccpx, ccpz, ft), v3(ccpx, ccmz, ft), v3(-1, 0, 0));
    // standoffs near the pcb bbox corners, sitting on the cavity floor
    V2 co[4] = { v2(mnx + 1, mny + 1), v2(mxx - 1, mny + 1), v2(mxx - 1, mxy - 1), v2(mnx + 1, mxy - 1) };
    float so_z = ft + 0.05f; // tiny lift so standoff bases don't coincide with cavity floor
    for (int i = 0; i < 4; i++)
        m_cyl(out, v3(co[i].x, co[i].y, so_z + e.standoff_h * 0.5f), e.standoff_d * 0.5f, e.standoff_h, 12);
    if (e.lid) {
        float lt = 1.5f;
        m_box(out, v3(cx, cy, H + lt * 0.5f), v3(ox, oy, lt));
    }
    mesh_bbox(out);
    return out->valid;
}

static void pcb_center(Pcb *pcb, V2 *c) {
    float mnx = pcb->outline[0].x, mny = pcb->outline[0].y;
    float mxx = mnx, mxy = mny;
    for (int i = 1; i < pcb->noutline; i++) {
        if (pcb->outline[i].x < mnx) mnx = pcb->outline[i].x;
        if (pcb->outline[i].y < mny) mny = pcb->outline[i].y;
        if (pcb->outline[i].x > mxx) mxx = pcb->outline[i].x;
        if (pcb->outline[i].y > mxy) mxy = pcb->outline[i].y;
    }
    c->x = (mnx + mxx) * 0.5f;
    c->y = (mny + mxy) * 0.5f;
}

int co_design_enclosure(Project *p, const char *pcb_id, const EnclosureParams *ep) {
    if (!p || !pcb_id) return -1;
    Pcb *pcb = project_find_pcb(p, pcb_id);
    if (!pcb) return -1;
    EnclosureParams e;
    if (ep) e = *ep; else co_enclosure_defaults(&e);
    CadMesh em;
    if (!pcb_enclosure_mesh(pcb, &e, &em)) return -1;

    CadModel cm; memset(&cm, 0, sizeof(cm));
    cm.id = str_dup(make_id());
    char nm[160];
    snprintf(nm, sizeof(nm), "%s-enclosure", pcb->name && pcb->name[0] ? pcb->name : "pcb");
    cm.name = str_dup(nm);
    cm.source_file = NULL;
    cm.mesh = em;
    arr_push(p->cad_models, cm);

    V2 cc;
    pcb_center(pcb, &cc);
    Assembly a; memset(&a, 0, sizeof(a));
    a.id = str_dup(make_id());
    char an[160];
    snprintf(an, sizeof(an), "%s-assembly", nm);
    a.name = str_dup(an);
    a.nitems = 2;
    a.items = (PlaceItem *)calloc(2, sizeof(PlaceItem));
    PlaceItem *ip = &a.items[0];
    ip->kind = PART_PCB;
    ip->part_id = str_dup(pcb->id);
    snprintf(nm, sizeof(nm), "%s", pcb->name && pcb->name[0] ? pcb->name : pcb_id);
    ip->label = str_dup(nm);
    ip->pos = v3(-cc.x, -cc.y, e.floor_thickness + e.standoff_h + e.pcb_thickness * 0.5f);
    PlaceItem *ic = &a.items[1];
    ic->kind = PART_CAD;
    ic->part_id = str_dup(cm.id);
    snprintf(nm, sizeof(nm), "%s-enclosure", pcb->name && pcb->name[0] ? pcb->name : "pcb");
    ic->label = str_dup(nm);
    ic->pos = v3(0, 0, 0);
    arr_push(p->assemblies, a);

    char note[320];
    snprintf(note, sizeof(note), "co-design: CAD enclosure '%s' linked to PCB '%s' via assembly '%s'",
             nm, pcb->name ? pcb->name : pcb_id, an);
    arr_push(p->notes, str_dup(note));
    return p->assemblies.len - 1;
}

CadModel *co_find_enclosure(Project *p, const char *pcb_id) {
    if (!p || !pcb_id) return NULL;
    Pcb *pcb = project_find_pcb(p, pcb_id);
    if (!pcb || !pcb->name) return NULL;
    char nm[160];
    snprintf(nm, sizeof(nm), "%s-enclosure", pcb->name);
    for (int i = 0; i < p->cad_models.len; i++)
        if (p->cad_models.v[i].name && strcmp(p->cad_models.v[i].name, nm) == 0) return &p->cad_models.v[i];
    return NULL;
}

// ---------------- CAD -> slicer ----------------
bool gcode_slice_and_emit(CadMesh *m, const SliceSettings *cfg, const char *path) {
    if (!m || !m->valid || !cfg || !path) return false;
    SliceResult sr;
    memset(&sr, 0, sizeof(sr));
    if (!slice_mesh(m, cfg, &sr)) return false;
    bool ok = gcode_emit(&sr, cfg, path);
    slice_result_free(&sr);
    return ok;
}

bool co_design_print_cad(Project *p, const char *cad_id, const SliceSettings *cfg, const char *path) {
    if (!p || !cad_id || !cfg || !path) return false;
    CadModel *cm = project_find_cad(p, cad_id);
    if (!cm) return false;
    return gcode_slice_and_emit(&cm->mesh, cfg, path);
}

bool co_design_print_enclosure(Project *p, const char *pcb_id, const SliceSettings *cfg, const char *path) {
    if (!p || !pcb_id || !cfg || !path) return false;
    CadModel *cm = co_find_enclosure(p, pcb_id);
    if (!cm) return false;
    return gcode_slice_and_emit(&cm->mesh, cfg, path);
}
