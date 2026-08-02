#include "importer.h"
#include <ctype.h>

// ================= S-expression parser (KiCad file format) =================
typedef struct { const char *p; const char *end; } SXP;

static void sxp_ws(SXP *x) {
    while (x->p < x->end && isspace((unsigned char)*x->p)) x->p++;
}

static SExprNode *sxp_parse(SXP *x);

static SExprNode *node_atom(const char *s, size_t slen) {
    SExprNode *n = (SExprNode *)calloc(1, sizeof(SExprNode));
    n->atom = str_dup_len(s, slen);
    arr_init(n->kids);
    return n;
}

static SExprNode *node_list(void) {
    SExprNode *n = (SExprNode *)calloc(1, sizeof(SExprNode));
    n->is_list = 1;
    n->atom = NULL;
    arr_init(n->kids);
    return n;
}

void sexpr_free(SExprNode *n) {
    if (!n) return;
    free(n->atom);
    for (int i = 0; i < n->kids.len; i++) sexpr_free(n->kids.v[i]);
    arr_free(n->kids);
    free(n);
}

static SExprNode *sxp_parse(SXP *x) {
    sxp_ws(x);
    if (x->p >= x->end) return NULL;
    char c = *x->p;
    if (c == '(') {
        x->p++;
        SExprNode *list = node_list();
        for (;;) {
            sxp_ws(x);
            if (x->p >= x->end) break;
            if (*x->p == ')') { x->p++; break; }
            SExprNode *kid = sxp_parse(x);
            if (kid) arr_push(list->kids, kid);
            else break;
        }
        return list;
    }
    if (c == '"') {
        x->p++;
        Buf s; buf_init(&s);
        while (x->p < x->end && *x->p != '"') {
            if (*x->p == '\\' && x->p + 1 < x->end) {
                char e = x->p[1];
                if (e == 'n') buf_append(&s, "\n", 1);
                else if (e == 't') buf_append(&s, "\t", 1);
                else if (e == 'r') buf_append(&s, "\r", 1);
                else buf_append(&s, &e, 1);
                x->p += 2;
                continue;
            }
            buf_append(&s, x->p, 1);
            x->p++;
        }
        if (x->p < x->end) x->p++;
        SExprNode *n = node_atom(buf_cstr(&s), s.len);
        buf_free(&s);
        return n;
    }
    // atom until whitespace or parens
    const char *start = x->p;
    while (x->p < x->end && !isspace((unsigned char)*x->p) && *x->p != '(' && *x->p != ')') x->p++;
    if (x->p == start) { x->p++; return NULL; }
    SExprNode *n = node_atom(start, (size_t)(x->p - start));
    return n;
}

SExprNode *sexpr_parse(const char *data, size_t len) {
    SXP x = { data, data + len };
    sxp_ws(&x);
    if (x.p >= x.end || *x.p != '(') return NULL;
    return sxp_parse(&x);
}

const char *sexpr_atom(SExprNode *n, int i) {
    if (!n || i < 0 || i >= n->kids.len || n->kids.v[i]->is_list) return "";
    return n->kids.v[i]->atom ? n->kids.v[i]->atom : "";
}

int sexpr_child_count(SExprNode *n) { return n ? n->kids.len : 0; }
SExprNode *sexpr_get(SExprNode *n, int i) { return (n && i >= 0 && i < n->kids.len) ? n->kids.v[i] : NULL; }

const char *sexpr_name(SExprNode *n) { return n ? sexpr_atom(n, 0) : ""; }

double sexpr_num(SExprNode *n, int i) {
    const char *a = sexpr_atom(n, i);
    if (!*a) return 0.0;
    bool ok;
    return str_atof_any(a, &ok);
}

SExprNode *sexpr_find_child(SExprNode *n, const char *name) {
    if (!n) return NULL;
    for (int i = 0; i < n->kids.len; i++) {
        SExprNode *kid = n->kids.v[i];
        if (kid->is_list && strcmp(sexpr_name(kid), name) == 0) return kid;
    }
    return NULL;
}

const char *sexpr_str(SExprNode *n, int i) { return sexpr_atom(n, i); }
