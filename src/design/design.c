#include "design.h"
#include "../core/util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================= materials =================
static const struct { const char *name; float den; float e; unsigned char r, g, b; } BUILTINS[] = {
    { "Aluminum 6061",  2.70f, 68.9f,  200, 200, 210 },
    { "Steel 1018",     7.87f, 205.0f, 160, 165, 170 },
    { "Titanium Ti-6Al-4V", 4.43f, 113.8f, 180, 180, 200 },
    { "Copper C110",    8.96f, 110.0f, 210, 130, 90 },
    { "ABS plastic",    1.04f, 2.3f,   240, 230, 200 },
    { "PLA",            1.24f, 3.5f,   200, 220, 200 },
    { "Nylon PA12",     1.14f, 2.6f,   230, 230, 240 },
    { "Acrylic",        1.19f, 3.2f,   160, 200, 220 },
    { "FR4 (PCB)",      1.85f, 22.0f,  120, 200, 90 },
    { "Alumina",        3.90f, 300.0f, 220, 220, 235 },
};

int material_lib_count(void) { return (int)(sizeof(BUILTINS) / sizeof(BUILTINS[0])); }

CadMaterial *material_lib_get(int i) {
    if (i < 0 || i >= material_lib_count()) i = 0;
    CadMaterial *m = (CadMaterial *)calloc(1, sizeof(CadMaterial));
    m->name = str_dup(BUILTINS[i].name);
    m->density = BUILTINS[i].den;
    m->young_gpa = BUILTINS[i].e;
    m->color.r = BUILTINS[i].r; m->color.g = BUILTINS[i].g; m->color.b = BUILTINS[i].b;
    return m;
}

CadMaterial *material_find(const char *name) {
    if (!name) return NULL;
    for (int i = 0; i < material_lib_count(); i++)
        if (str_icmp(name, BUILTINS[i].name) == 0) return material_lib_get(i);
    return NULL;
}

CadMaterial *material_default(void) { return material_lib_get(0); }

void material_free(CadMaterial *m) { if (m) { free(m->name); free(m); } }

// ================= parametric primitives =================
static void mesh_push_quad(CadMesh *m, V3 a, V3 b, V3 c, V3 d, V3 n) {
    int base = m->nverts;
    float pa[3] = { a.x, a.y, a.z }, pb[3] = { b.x, b.y, b.z }, pc[3] = { c.x, c.y, c.z }, pd[3] = { d.x, d.y, d.z };
    m->pos = (float *)realloc(m->pos, sizeof(float) * 3 * (size_t)(m->nverts + 4));
    m->nrm = (float *)realloc(m->nrm, sizeof(float) * 3 * (size_t)(m->nverts + 4));
    memcpy(m->pos + base * 3, pa, sizeof(pa));
    memcpy(m->pos + (base + 1) * 3, pb, sizeof(pb));
    memcpy(m->pos + (base + 2) * 3, pc, sizeof(pc));
    memcpy(m->pos + (base + 3) * 3, pd, sizeof(pd));
    for (int i = 0; i < 4; i++) { m->nrm[(base + i) * 3] = n.x; m->nrm[(base + i) * 3 + 1] = n.y; m->nrm[(base + i) * 3 + 2] = n.z; }
    int *tr = (int *)realloc(m->idx, sizeof(int) * 3 * (size_t)(m->ntris + 2));
    m->idx = tr;
    m->idx[m->ntris * 3] = base; m->idx[m->ntris * 3 + 1] = base + 1; m->idx[m->ntris * 3 + 2] = base + 2;
    m->idx[m->ntris * 3 + 3] = base; m->idx[m->ntris * 3 + 4] = base + 2; m->idx[m->ntris * 3 + 5] = base + 3;
    m->nverts += 4; m->ntris += 2;
}

static V3 feattrans(DesignFeature *f, V3 v) {
    // rotate about Z then X then translate
    float rz = f->rz * 3.14159265f / 180.0f, rx = f->rx * 3.14159265f / 180.0f, ry = f->ry * 3.14159265f / 180.0f;
    v = v3_rot_z(v, rz);
    v = v3_rot_x(v, rx);
    v = v3_rot_y(v, ry);
    return v3_add(v, v3(f->x, f->y, f->z));
}

static void push_box(DesignFeature *f, CadMesh *m) {
    float w = f->w * 0.5f, h = f->h * 0.5f, d = f->d * 0.5f;
    V3 c[8] = {
        v3(-w, -h, -d), v3( w, -h, -d), v3( w,  h, -d), v3(-w,  h, -d),
        v3(-w, -h,  d), v3( w, -h,  d), v3( w,  h,  d), v3(-w,  h,  d),
    };
    for (int i = 0; i < 8; i++) c[i] = feattrans(f, c[i]);
    V3 n[6] = { v3(0,0,-1), v3(0,0,1), v3(0,-1,0), v3(0,1,0), v3(-1,0,0), v3(1,0,0) };
    mesh_push_quad(m, c[0], c[1], c[2], c[3], n[0]);
    mesh_push_quad(m, c[5], c[4], c[7], c[6], n[1]);
    mesh_push_quad(m, c[0], c[4], c[5], c[1], n[2]);
    mesh_push_quad(m, c[3], c[2], c[6], c[7], n[3]);
    mesh_push_quad(m, c[0], c[3], c[7], c[4], n[4]);
    mesh_push_quad(m, c[1], c[5], c[6], c[2], n[5]);
}

static void push_cylinder(DesignFeature *f, CadMesh *m) {
    int seg = f->segments > 4 ? f->segments : 24;
    float r = f->w, hh = f->h * 0.5f;
    // side
    V3 *ring_bot = (V3 *)malloc(sizeof(V3) * (size_t)seg);
    V3 *ring_top = (V3 *)malloc(sizeof(V3) * (size_t)seg);
    for (int i = 0; i < seg; i++) {
        float a = 6.2831853f * (float)i / (float)seg;
        V3 pb = v3(cosf(a) * r, -hh, sinf(a) * r);
        V3 pt = v3(cosf(a) * r, hh, sinf(a) * r);
        ring_bot[i] = feattrans(f, pb);
        ring_top[i] = feattrans(f, pt);
    }
    for (int i = 0; i < seg; i++) {
        int j = (i + 1) % seg;
        V3 n = v3_rot_y(v3(cosf(6.2831853f * ((float)i + 0.5f) / (float)seg), 0, sinf(6.2831853f * ((float)i + 0.5f) / (float)seg)), 0);
        n = v3_rot_x(n, 0); n = v3_rot_z(n, 0);
        mesh_push_quad(m, ring_bot[i], ring_bot[j], ring_top[j], ring_top[i], n);
    }
    // caps (fan)
    V3 cbot = feattrans(f, v3(0, -hh, 0)), ctop = feattrans(f, v3(0, hh, 0));
    int base = m->nverts;
    m->pos = (float *)realloc(m->pos, sizeof(float) * 3 * (size_t)(m->nverts + seg + 1));
    m->nrm = (float *)realloc(m->nrm, sizeof(float) * 3 * (size_t)(m->nverts + seg + 1));
    m->pos[base * 3] = cbot.x; m->pos[base * 3 + 1] = cbot.y; m->pos[base * 3 + 2] = cbot.z;
    m->nrm[base * 3] = 0; m->nrm[base * 3 + 1] = -1; m->nrm[base * 3 + 2] = 0;
    for (int i = 0; i < seg; i++) {
        m->pos[(base + 1 + i) * 3] = ring_bot[i].x;
        m->pos[(base + 1 + i) * 3 + 1] = ring_bot[i].y;
        m->pos[(base + 1 + i) * 3 + 2] = ring_bot[i].z;
        m->nrm[(base + 1 + i) * 3] = 0; m->nrm[(base + 1 + i) * 3 + 1] = -1; m->nrm[(base + 1 + i) * 3 + 2] = 0;
    }
    for (int i = 0; i < seg; i++) {
        int j = (i + 1) % seg;
        m->idx = (int *)realloc(m->idx, sizeof(int) * 3 * (size_t)(m->ntris + 1));
        m->idx[m->ntris * 3] = base; m->idx[m->ntris * 3 + 1] = base + 1 + j; m->idx[m->ntris * 3 + 2] = base + 1 + i;
        m->ntris++;
    }
    m->nverts += seg + 1;
    // top cap
    base = m->nverts;
    m->pos = (float *)realloc(m->pos, sizeof(float) * 3 * (size_t)(m->nverts + seg + 1));
    m->nrm = (float *)realloc(m->nrm, sizeof(float) * 3 * (size_t)(m->nverts + seg + 1));
    m->pos[base * 3] = ctop.x; m->pos[base * 3 + 1] = ctop.y; m->pos[base * 3 + 2] = ctop.z;
    m->nrm[base * 3] = 0; m->nrm[base * 3 + 1] = 1; m->nrm[base * 3 + 2] = 0;
    for (int i = 0; i < seg; i++) {
        m->pos[(base + 1 + i) * 3] = ring_top[i].x;
        m->pos[(base + 1 + i) * 3 + 1] = ring_top[i].y;
        m->pos[(base + 1 + i) * 3 + 2] = ring_top[i].z;
        m->nrm[(base + 1 + i) * 3] = 0; m->nrm[(base + 1 + i) * 3 + 1] = 1; m->nrm[(base + 1 + i) * 3 + 2] = 0;
    }
    for (int i = 0; i < seg; i++) {
        int j = (i + 1) % seg;
        m->idx = (int *)realloc(m->idx, sizeof(int) * 3 * (size_t)(m->ntris + 1));
        m->idx[m->ntris * 3] = base; m->idx[m->ntris * 3 + 1] = base + 1 + i; m->idx[m->ntris * 3 + 2] = base + 1 + j;
        m->ntris++;
    }
    m->nverts += seg + 1;
    free(ring_bot); free(ring_top);
}

static void push_sphere(DesignFeature *f, CadMesh *m) {
    int seg = f->segments > 4 ? f->segments : 20;
    float r = f->w;
    V3 **rows = (V3 **)malloc(sizeof(V3 *) * (size_t)(seg + 1));
    for (int i = 0; i <= seg; i++) rows[i] = (V3 *)malloc(sizeof(V3) * (size_t)seg);
    for (int i = 0; i <= seg; i++) {
        float phi = 3.14159265f * (float)i / (float)seg;
        for (int k = 0; k < seg; k++) {
            float th = 6.2831853f * (float)k / (float)seg;
            V3 v = v3(cosf(th) * sinf(phi) * r, cosf(phi) * r, sinf(th) * sinf(phi) * r);
            rows[i][k] = feattrans(f, v);
        }
    }
    for (int i = 0; i < seg; i++) {
        for (int k = 0; k < seg; k++) {
            int k2 = (k + 1) % seg;
            // quads between rows i and i+1
            V3 n = v3_add(rows[i][k], rows[i + 1][k2]);
            float nl = v3_len(n); if (nl > 0) n = v3_scale(n, 1.0f / nl);
            mesh_push_quad(m, rows[i][k], rows[i][k2], rows[i + 1][k2], rows[i + 1][k], n);
        }
    }
    for (int i = 0; i <= seg; i++) free(rows[i]);
    free(rows);
}

bool feature_make_mesh(DesignFeature *f, int material_index, CadMesh *out) {
    (void)material_index;
    if (!f || !out) return false;
    memset(out, 0, sizeof(*out));
    if (f->type == FEAT_BOX) push_box(f, out);
    else if (f->type == FEAT_CYLINDER) push_cylinder(f, out);
    else if (f->type == FEAT_SPHERE) push_sphere(f, out);
    else return false;
    mesh_bbox(out);
    return out->valid && out->nverts > 0;
}

void feature_free(DesignFeature *f) { (void)f; }

void design_build_assembly(Project *p, DesignFeature *feats, int nfeats) {
    if (!p || nfeats <= 0) return;
    float cursor = 0;
    for (int i = 0; i < nfeats; i++) {
        DesignFeature f = feats[i];
        f.x = cursor + (f.type == FEAT_BOX ? f.w * 0.5f : (f.type == FEAT_CYLINDER ? f.w : f.w));
        f.y = 0; f.z = 0; f.rx = 0; f.rz = 0;
        CadMesh mesh;
        if (!feature_make_mesh(&f, 0, &mesh)) continue;
        char name[128];
        const char *t = f.type == FEAT_BOX ? "box" : (f.type == FEAT_CYLINDER ? "cyl" : "sphere");
        snprintf(name, sizeof(name), "%s-%d", t, i + 1);
        CadModel cm; memset(&cm, 0, sizeof(cm));
        cm.id = str_dup(make_id());
        cm.name = str_dup(name);
        cm.source_file = str_dup("design:parametric");
        cm.mesh = mesh;
        arr_push(p->cad_models, cm);
        cursor += (f.type == FEAT_BOX ? f.w : 2.0f * f.w) + 3.0f;
    }
}

// ================= mass properties =================
void mesh_mass_props(CadMesh *m, float density_g_cm3, float *out_volume_mm3, float *out_mass_g, V3 *out_centroid) {
    double vol = 0, cx = 0, cy = 0, cz = 0;
    if (m && m->ntris > 0 && m->idx && m->pos) {
        for (int t = 0; t < m->ntris; t++) {
            int ia = m->idx[t * 3], ib = m->idx[t * 3 + 1], ic = m->idx[t * 3 + 2];
            if (ia < 0 || ib < 0 || ic < 0) continue;
            float *a = m->pos + ia * 3, *b = m->pos + ib * 3, *c = m->pos + ic * 3;
            // tetrahedron w.r.t. origin
            double v = (double)(a[0] * (b[1] * c[2] - b[2] * c[1]) - a[1] * (b[0] * c[2] - b[2] * c[0]) + a[2] * (b[0] * c[1] - b[1] * c[0])) / 6.0;
            vol += v;
            cx += (a[0] + b[0] + c[0]) * v;
            cy += (a[1] + b[1] + c[1]) * v;
            cz += (a[2] + b[2] + c[2]) * v;
        }
    }
    if (out_volume_mm3) *out_volume_mm3 = (float)fabs(vol);
    if (out_centroid && vol != 0) {
        double inv = 1.0 / (4.0 * vol);
        out_centroid->x = (float)(cx * inv); out_centroid->y = (float)(cy * inv); out_centroid->z = (float)(cz * inv);
    } else if (out_centroid) { *out_centroid = v3(0, 0, 0); }
    if (out_mass_g) *out_mass_g = (float)(fabs(vol) * 0.001 * density_g_cm3); // mm3 -> cm3 (/1000) then * g/cm3
}

// ================= export =================
bool mesh_write_stl(const char *path, CadMesh *m) {
    if (!m || !m->pos || m->ntris <= 0) return false;
    FILE *f = fopen(path, "wb");
    if (!f) return false;
    char header[80] = "Volt360";
    fwrite(header, 1, 80, f);
    unsigned int ntri = (unsigned int)m->ntris;
    fwrite(&ntri, 4, 1, f);
    for (int t = 0; t < m->ntris; t++) {
        int ia = m->idx[t * 3], ib = m->idx[t * 3 + 1], ic = m->idx[t * 3 + 2];
        float *a = m->pos + ia * 3, *b = m->pos + ib * 3, *c = m->pos + ic * 3;
        // normal
        float ux = b[0] - a[0], uy = b[1] - a[1], uz = b[2] - a[2];
        float vx = c[0] - a[0], vy = c[1] - a[1], vz = c[2] - a[2];
        float nx = uy * vz - uz * vy, ny = uz * vx - ux * vz, nz = ux * vy - uy * vx;
        float len = sqrtf(nx * nx + ny * ny + nz * nz);
        if (len > 0) { nx /= len; ny /= len; nz /= len; } else { nx = 0; ny = 0; nz = 1; }
        float tri[12] = { nx, ny, nz, a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2] };
        fwrite(tri, 4, 12, f);
        unsigned short attr = 0;
        fwrite(&attr, 2, 1, f);
    }
    fclose(f);
    return true;
}

char *project_bom_text(Project *p) {
    Buf b; buf_init(&b);
    buf_append_str(&b, "Bill of Materials\n=================\n\n");
    if (p->components.len == 0 && p->pcbs.len == 0) {
        buf_append_str(&b, "(no components or PCBs in project)\n");
    }
    for (int i = 0; i < p->components.len; i++) {
        Component *c = &p->components.v[i];
        buf_appendf(&b, "%d. %s", i + 1, c->name ? c->name : "?");
        if (c->manufacturer && c->manufacturer[0]) buf_appendf(&b, " (%s)", c->manufacturer);
        if (c->package && c->package[0]) buf_appendf(&b, "  pkg=%s", c->package);
        buf_append_str(&b, "\n");
        for (int s = 0; s < c->nspecs; s++)
            if (c->specs[s].key && c->specs[s].value && c->specs[s].key[0])
                buf_appendf(&b, "    %s: %s\n", c->specs[s].key, c->specs[s].value);
    }
    for (int i = 0; i < p->pcbs.len; i++) {
        Pcb *pcb = &p->pcbs.v[i];
        buf_appendf(&b, "\nPCB '%s': %d footprint(s)\n", pcb->name ? pcb->name : "?", pcb->nfps);
        for (int j = 0; j < pcb->nfps; j++)
            buf_appendf(&b, "    %s  %s\n", pcb->fps[j].ref ? pcb->fps[j].ref : "?", pcb->fps[j].footprint ? pcb->fps[j].footprint : "?");
    }
    char *out = str_dup(buf_cstr(&b));
    buf_free(&b);
    return out;
}
