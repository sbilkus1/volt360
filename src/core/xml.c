#include "xml.h"
#include <ctype.h>

typedef struct { const char *p; const char *end; } XP;

static void xp_ws(XP *x) { while (x->p < x->end && isspace((unsigned char)*x->p)) x->p++; }

static XmlNode *node_new(const char *name, size_t nlen) {
    XmlNode *n = (XmlNode *)calloc(1, sizeof(XmlNode));
    n->name = str_dup_len(name, nlen);
    arr_init(n->attrs);
    arr_init(n->children);
    n->text = NULL;
    return n;
}

static void attr_add(XmlNode *n, const char *name, size_t nlen, const char *val, size_t vlen) {
    XmlAttr a;
    a.name = str_dup_len(name, nlen);
    a.value = str_dup_len(val, vlen);
    arr_push(n->attrs, a);
}

// parse attributes after tag name; 'p' points at first char after name
static void parse_attrs(XP *x, XmlNode *n) {
    for (;;) {
        xp_ws(x);
        if (x->p >= x->end) return;
        if (*x->p == '>' || *x->p == '/' || *x->p == '?') return;
        const char *ns = x->p;
        while (x->p < x->end && !isspace((unsigned char)*x->p) && *x->p != '=' && *x->p != '>' && *x->p != '/') x->p++;
        if (x->p == ns) { x->p++; continue; }
        const char *ne = x->p;
        xp_ws(x);
        if (x->p < x->end && *x->p == '=') {
            x->p++;
            xp_ws(x);
            if (x->p < x->end && *x->p == '"') {
                x->p++;
                const char *vs = x->p;
                while (x->p < x->end && *x->p != '"') x->p++;
                attr_add(n, ns, (size_t)(ne - ns), vs, (size_t)(x->p - vs));
                if (x->p < x->end) x->p++;
            } else if (x->p < x->end && *x->p == '\'') {
                x->p++;
                const char *vs = x->p;
                while (x->p < x->end && *x->p != '\'') x->p++;
                attr_add(n, ns, (size_t)(ne - ns), vs, (size_t)(x->p - vs));
                if (x->p < x->end) x->p++;
            } else {
                const char *vs = x->p;
                while (x->p < x->end && !isspace((unsigned char)*x->p) && *x->p != '>') x->p++;
                attr_add(n, ns, (size_t)(ne - ns), vs, (size_t)(x->p - vs));
            }
        } else {
            attr_add(n, ns, (size_t)(ne - ns), "", 0);
        }
    }
}

static XmlNode *xml_parse_element(XP *x);

static void skip_comment(XP *x) {
    const char *q = strstr(x->p, "-->");
    if (q) x->p = q + 3; else x->p = x->end;
}
static void skip_decl(XP *x) {
    const char *q = strstr(x->p, ">");
    if (q) x->p = q + 1; else x->p = x->end;
}

static XmlNode *xml_parse_element(XP *x) {
    // x->p at '<'
    x->p++;
    if (x->p < x->end && *x->p == '?') { skip_decl(x); return xml_parse_element(x); }
    if (x->p < x->end && *x->p == '!') {
        if (x->end - x->p >= 3 && strncmp(x->p, "!--", 3) == 0) { x->p += 3; skip_comment(x); return xml_parse_element(x); }
        skip_decl(x);
        return xml_parse_element(x);
    }
    const char *ns = x->p;
    while (x->p < x->end && !isspace((unsigned char)*x->p) && *x->p != '>' && *x->p != '/') x->p++;
    XmlNode *n = node_new(ns, (size_t)(x->p - ns));
    parse_attrs(x, n);
    xp_ws(x);
    if (x->p < x->end && x->p[0] == '/' && x->p + 1 < x->end && x->p[1] == '>') { x->p += 2; return n; } // self-closing
    if (x->p < x->end && *x->p == '>') x->p++;
    // inner content
    Buf text; buf_init(&text);
    for (;;) {
        if (x->p >= x->end) break;
        if (*x->p == '<') {
            if (x->end - x->p >= 4 && strncmp(x->p, "<!--", 4) == 0) { x->p += 4; skip_comment(x); continue; }
            if (x->end - x->p >= 9 && strncmp(x->p, "<![CDATA[", 9) == 0) {
                x->p += 9;
                const char *q = strstr(x->p, "]]>");
                size_t ln = q ? (size_t)(q - x->p) : (size_t)(x->end - x->p);
                buf_append(&text, x->p, ln);
                x->p += ln + (q ? 3 : 0);
                continue;
            }
            if (x->end - x->p >= 2 && x->p[1] == '/') {
                // closing tag
                const char *q = strchr(x->p, '>');
                if (q) x->p = q + 1;
                break;
            }
            if (x->end - x->p >= 2 && x->p[1] == '?') { skip_decl(x); continue; }
            if (x->end - x->p >= 2 && x->p[1] == '!') { skip_decl(x); continue; }
            XmlNode *child = xml_parse_element(x);
            if (child) arr_push(n->children, child);
            continue;
        }
        // text
        const char *ts = x->p;
        while (x->p < x->end && *x->p != '<') x->p++;
        buf_append(&text, ts, (size_t)(x->p - ts));
    }
    // trim text
    {
        char *s = buf_cstr(&text);
        str_trim(s);
        if (*s) n->text = str_dup(s);
    }
    buf_free(&text);
    return n;
}

XmlNode *xml_parse(const char *data, size_t len) {
    XP x = { data, data + len };
    xp_ws(&x);
    if (x.p >= x.end || *x.p != '<') return NULL;
    XmlNode *root = xml_parse_element(&x);
    return root;
}

void xml_free(XmlNode *n) {
    if (!n) return;
    for (int i = 0; i < n->attrs.len; i++) { free(n->attrs.v[i].name); free(n->attrs.v[i].value); }
    arr_free(n->attrs);
    for (int i = 0; i < n->children.len; i++) xml_free(n->children.v[i]);
    arr_free(n->children);
    free(n->name);
    free(n->text);
    free(n);
}

const char *xml_attr(XmlNode *n, const char *name) {
    if (!n) return NULL;
    for (int i = 0; i < n->attrs.len; i++) {
        if (strcmp(n->attrs.v[i].name, name) == 0) return n->attrs.v[i].value;
    }
    return NULL;
}

XmlNode *xml_child(XmlNode *n, const char *name) {
    if (!n) return NULL;
    for (int i = 0; i < n->children.len; i++) {
        if (strcmp(n->children.v[i]->name, name) == 0) return n->children.v[i];
    }
    return NULL;
}

void xml_children(XmlNode *n, const char *name, XmlNodeList *out) {
    if (!n) return;
    for (int i = 0; i < n->children.len; i++) {
        if (strcmp(n->children.v[i]->name, name) == 0) arr_push(*out, n->children.v[i]);
    }
}

const char *xml_local_name(XmlNode *n) {
    const char *c = strchr(n->name, ':');
    return c ? c + 1 : n->name;
}

bool xml_name_is(XmlNode *n, const char *local) {
    return strcmp(xml_local_name(n), local) == 0;
}

static void collect_text(XmlNode *n, Buf *b) {
    if (n->text) { buf_append_str(b, n->text); if (b->len && b->data[b->len-1] != '\n') buf_append_str(b, "\n"); }
    for (int i = 0; i < n->children.len; i++) collect_text(n->children.v[i], b);
}

char *xml_text_all(XmlNode *n) {
    if (!n) return str_dup("");
    Buf b; buf_init(&b);
    collect_text(n, &b);
    char *r = str_dup(buf_cstr(&b));
    buf_free(&b);
    return r;
}
