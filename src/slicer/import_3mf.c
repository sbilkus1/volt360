#include "import_3mf.h"
#include "../core/util.h"
#include "../../third_party/miniz/miniz.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

typedef struct {
    const char *p;
    const char *end;
} XMLTok;

static void xml_skip_ws(XMLTok *x) {
    while (x->p < x->end && isspace((unsigned char)*x->p)) x->p++;
}

static int xml_match(XMLTok *x, const char *s) {
    int slen = (int)strlen(s);
    if (x->p + slen > x->end) return 0;
    if (memcmp(x->p, s, (size_t)slen) == 0) { x->p += slen; return 1; }
    return 0;
}

static int xml_read_to(XMLTok *x, const char *delim) {
    int dlen = (int)strlen(delim);
    while (x->p + dlen <= x->end) {
        if (memcmp(x->p, delim, (size_t)dlen) == 0) return 1;
        x->p++;
    }
    return 0;
}

static int xml_read_quoted(XMLTok *x, char *buf, int bufsz) {
    int len = 0;
    xml_skip_ws(x);
    if (x->p >= x->end) return 0;
    if (*x->p == '"') {
        x->p++;
        while (x->p < x->end && *x->p != '"' && len < bufsz - 1) {
            buf[len++] = *x->p;
            x->p++;
        }
        buf[len] = '\0';
        if (x->p < x->end && *x->p == '"') x->p++;
        return len;
    }
    if (*x->p == '\'') {
        x->p++;
        while (x->p < x->end && *x->p != '\'' && len < bufsz - 1) {
            buf[len++] = *x->p;
            x->p++;
        }
        buf[len] = '\0';
        if (x->p < x->end && *x->p == '\'') x->p++;
        return len;
    }
    return 0;
}

static float xml_read_float_attr(XMLTok *x) {
    char buf[64];
    if (xml_read_quoted(x, buf, 64) > 0) {
        return (float)atof(buf);
    }
    return 0.0f;
}

static int xml_find_tag(XMLTok *x, const char *tag) {
    char open[128];
    snprintf(open, sizeof(open), "<%s", tag);
    while (x->p < x->end) {
        if (xml_match(x, open)) return 1;
        x->p++;
    }
    return 0;
}

typedef struct {
    float *v;
    int len;
    int cap;
} FloatVec;

typedef struct {
    int *v;
    int len;
    int cap;
} IntVec;

static void fv_init(FloatVec *f) { f->v = NULL; f->len = 0; f->cap = 0; }
static void fv_push(FloatVec *f, float val) {
    if (f->len >= f->cap) { f->cap = f->cap ? f->cap * 2 : 256; f->v = (float *)realloc(f->v, sizeof(float) * (size_t)f->cap); }
    f->v[f->len++] = val;
}
static void fv_free(FloatVec *f) { free(f->v); f->v = NULL; f->len = f->cap = 0; }

static void iv_init(IntVec *f) { f->v = NULL; f->len = 0; f->cap = 0; }
static void iv_push(IntVec *f, int val) {
    if (f->len >= f->cap) { f->cap = f->cap ? f->cap * 2 : 256; f->v = (int *)realloc(f->v, sizeof(int) * (size_t)f->cap); }
    f->v[f->len++] = val;
}
static void iv_free(IntVec *f) { free(f->v); f->v = NULL; f->len = f->cap = 0; }

static int parse_3mf_xml(const char *data, size_t len, CadMesh *out) {
    XMLTok x;
    FloatVec verts_x, verts_y, verts_z;
    IntVec tri_v1, tri_v2, tri_v3;
    int i, nv, nt;
    float *pos;

    if (!data || len == 0) return 0;
    x.p = data;
    x.end = data + len;

    fv_init(&verts_x);
    fv_init(&verts_y);
    fv_init(&verts_z);
    iv_init(&tri_v1);
    iv_init(&tri_v2);
    iv_init(&tri_v3);

    /* Find <vertices> */
    if (!xml_find_tag(&x, "vertices")) goto fail;

    /* Parse <vertex x="..." y="..." z="..."/> entries */
    while (x.p < x.end) {
        xml_skip_ws(&x);
        if (x.p >= x.end) break;
        if (xml_match(&x, "</vertices>")) break;
        if (xml_match(&x, "<vertex")) {
            while (x.p < x.end && *x.p != '>' && *x.p != '/') {
                xml_skip_ws(&x);
                if (xml_match(&x, "x=")) fv_push(&verts_x, xml_read_float_attr(&x));
                else if (xml_match(&x, "y=")) fv_push(&verts_y, xml_read_float_attr(&x));
                else if (xml_match(&x, "z=")) fv_push(&verts_z, xml_read_float_attr(&x));
                else x.p++;
            }
            while (x.p < x.end && *x.p != '>') x.p++;
            if (x.p < x.end) x.p++;
            continue;
        }
        x.p++;
    }

    /* Find <triangles> */
    if (!xml_find_tag(&x, "triangles")) goto fail;

    while (x.p < x.end) {
        xml_skip_ws(&x);
        if (x.p >= x.end) break;
        if (xml_match(&x, "</triangles>")) break;
        if (xml_match(&x, "<triangle")) {
            int v1 = 0, v2 = 0, v3 = 0;
            int got = 0;
            while (x.p < x.end && *x.p != '>' && *x.p != '/') {
                xml_skip_ws(&x);
                if (xml_match(&x, "v1=")) { v1 = (int)xml_read_float_attr(&x); got++; }
                else if (xml_match(&x, "v2=")) { v2 = (int)xml_read_float_attr(&x); got++; }
                else if (xml_match(&x, "v3=")) { v3 = (int)xml_read_float_attr(&x); got++; }
                else x.p++;
            }
            while (x.p < x.end && *x.p != '>') x.p++;
            if (x.p < x.end) x.p++;
            if (got >= 3) {
                iv_push(&tri_v1, v1);
                iv_push(&tri_v2, v2);
                iv_push(&tri_v3, v3);
            }
            continue;
        }
        x.p++;
    }

    nv = verts_x.len;
    nt = tri_v1.len;
    if (nv < 3 || nt < 1) goto fail;

    /* Build CadMesh */
    memset(out, 0, sizeof(*out));
    out->nverts = nv;
    out->ntris = nt;
    pos = (float *)malloc(sizeof(float) * 3 * (size_t)nv);
    if (!pos) goto fail;
    out->pos = pos;
    for (i = 0; i < nv; i++) {
        pos[i * 3 + 0] = verts_x.v[i];
        pos[i * 3 + 1] = verts_y.v[i];
        pos[i * 3 + 2] = verts_z.v[i];
    }
    out->idx = (int *)malloc(sizeof(int) * 3 * (size_t)nt);
    if (!out->idx) { free(pos); out->pos = NULL; goto fail; }
    for (i = 0; i < nt; i++) {
        out->idx[i * 3 + 0] = tri_v1.v[i];
        out->idx[i * 3 + 1] = tri_v2.v[i];
        out->idx[i * 3 + 2] = tri_v3.v[i];
    }
    mesh_bbox(out);
    out->valid = 1;

    fv_free(&verts_x); fv_free(&verts_y); fv_free(&verts_z);
    iv_free(&tri_v1); iv_free(&tri_v2); iv_free(&tri_v3);
    return 1;

fail:
    fv_free(&verts_x); fv_free(&verts_y); fv_free(&verts_z);
    iv_free(&tri_v1); iv_free(&tri_v2); iv_free(&tri_v3);
    return 0;
}

bool import_3mf(const char *path, CadMesh *out) {
    mz_zip_archive zip;
    size_t fsize;
    void *fdata;
    int ok;

    if (!path || !out) return false;

    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, path, 0)) return false;

    fdata = mz_zip_reader_extract_file_to_heap(&zip, "3D/3dmodel.model", &fsize, 0);
    if (!fdata) {
        fdata = mz_zip_reader_extract_file_to_heap(&zip, "/3D/3dmodel.model", &fsize, 0);
    }
    if (!fdata) {
        mz_zip_reader_end(&zip);
        return false;
    }

    ok = parse_3mf_xml((const char *)fdata, fsize, out);

    mz_free(fdata);
    mz_zip_reader_end(&zip);
    return ok ? true : false;
}
