#include "kicad_parser.h"
#include "../core/util.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

typedef struct {
    const char *p;
    const char *end;
    int error;
} Tokenizer;

typedef enum { TOK_NONE, TOK_LPAREN, TOK_RPAREN, TOK_SYMBOL, TOK_STRING, TOK_NUMBER } TokenType;

typedef struct {
    int type;
    char text[256];
    int len;
} Token;

static void tok_init(Tokenizer *t, const char *data, int len) {
    t->p = data;
    t->end = data + len;
    t->error = 0;
}

static void skip_ws(Tokenizer *t) {
    while (t->p < t->end && isspace((unsigned char)*t->p)) t->p++;
}

static Token tok_next(Tokenizer *t) {
    Token tok;
    memset(&tok, 0, sizeof(tok));
    skip_ws(t);
    if (t->p >= t->end) return tok;
    if (*t->p == '(') { tok.type = TOK_LPAREN; tok.text[0] = '('; tok.len = 1; t->p++; return tok; }
    if (*t->p == ')') { tok.type = TOK_RPAREN; tok.text[0] = ')'; tok.len = 1; t->p++; return tok; }
    if (*t->p == '"') {
        tok.type = TOK_STRING;
        t->p++;
        while (t->p < t->end && *t->p != '"') {
            if (tok.len < 254) {
                if (*t->p == '\\' && t->p + 1 < t->end) {
                    t->p++;
                    if (*t->p == 'n') tok.text[tok.len++] = '\n';
                    else if (*t->p == 't') tok.text[tok.len++] = '\t';
                    else tok.text[tok.len++] = *t->p;
                } else {
                    tok.text[tok.len++] = *t->p;
                }
            }
            t->p++;
        }
        if (t->p < t->end) t->p++;
        tok.text[tok.len] = '\0';
        return tok;
    }
    {
        const char *start = t->p;
        while (t->p < t->end && !isspace((unsigned char)*t->p) && *t->p != '(' && *t->p != ')') t->p++;
        tok.len = (int)(t->p - start);
        if (tok.len > 255) tok.len = 255;
        memcpy(tok.text, start, (size_t)tok.len);
        tok.text[tok.len] = '\0';
        tok.type = TOK_SYMBOL;
        {
            int all_digit = 1, has_dot = 0, has_minus = 0;
            for (int i = 0; i < tok.len; i++) {
                if (i == 0 && tok.text[i] == '-') { has_minus = 1; continue; }
                if (tok.text[i] == '.') { has_dot = 1; continue; }
                if (!isdigit((unsigned char)tok.text[i])) { all_digit = 0; break; }
            }
            if (all_digit && tok.len > 0 - (has_minus * 0)) tok.type = TOK_NUMBER;
        }
        return tok;
    }
}

static double tok_numval(const char *s) {
    return atof(s);
}

static Token tok_peek(Tokenizer *t);

static void tok_skip_list(Tokenizer *t) {
    Token tok = tok_next(t);
    if (tok.type == TOK_LPAREN) {
        int depth = 1;
        while (depth > 0) {
            tok = tok_next(t);
            if (tok.type == TOK_LPAREN) depth++;
            else if (tok.type == TOK_RPAREN) depth--;
            else if (tok.type == TOK_NONE) { t->error = 1; return; }
        }
    }
}

static char *tok_strdup(Token *tok) {
    if (tok->len == 0) return str_dup("");
    char *s = (char *)malloc((size_t)tok->len + 1);
    if (!s) return NULL;
    memcpy(s, tok->text, (size_t)tok->len);
    s[tok->len] = '\0';
    return s;
}

static void sch_add_inst(Schematic *s, const char *ref, const char *sym, const char *val, V2 pos, float rot) {
    SymInst *si;
    s->insts = (SymInst *)realloc(s->insts, sizeof(SymInst) * (size_t)(s->ninsts + 1));
    si = &s->insts[s->ninsts++];
    memset(si, 0, sizeof(*si));
    si->id = make_id();
    si->ref = str_dup(ref ? ref : "U?");
    si->symbol = str_dup(sym ? sym : "");
    si->value = str_dup(val ? val : "");
    si->pos = pos;
    si->rotation = rot;
}

static void sch_add_wire(Schematic *s, V2 a, V2 b) {
    s->wires = (Wire *)realloc(s->wires, sizeof(Wire) * (size_t)(s->nwires + 1));
    s->wires[s->nwires].a = a;
    s->wires[s->nwires].b = b;
    s->nwires++;
}

static void sch_add_label(Schematic *s, const char *name, V2 pos) {
    NetLabel *nl;
    s->labels = (NetLabel *)realloc(s->labels, sizeof(NetLabel) * (size_t)(s->nlabels + 1));
    nl = &s->labels[s->nlabels++];
    nl->name = str_dup(name ? name : "");
    nl->pos = pos;
}

static void sch_add_text(Schematic *s, const char *text, V2 pos) {
    SchText *st;
    s->texts = (SchText *)realloc(s->texts, sizeof(SchText) * (size_t)(s->ntexts + 1));
    st = &s->texts[s->ntexts++];
    st->text = str_dup(text ? text : "");
    st->pos = pos;
    st->size = 12;
}

static void sch_add_junction(Schematic *s, V2 pos) {
    (void)s; (void)pos;
}

static int parse_property(Tokenizer *t, char **key, char **val) {
    Token tok;
    *key = NULL; *val = NULL;
    tok = tok_next(t);
    if (tok.type != TOK_LPAREN || t->error) return 0;
    tok = tok_next(t);
    if (tok.type != TOK_SYMBOL || strcmp(tok.text, "property") != 0) { tok_skip_list(t); return 0; }
    /* skip to values: (property "Name" "Value" ...) */
    tok = tok_next(t);
    if (tok.type == TOK_STRING) {
        *key = tok_strdup(&tok);
        tok = tok_next(t);
        if (tok.type == TOK_STRING) {
            *val = tok_strdup(&tok);
            tok_skip_list(t);
        } else {
            free(*key); *key = NULL;
            tok_skip_list(t);
        }
    } else {
        tok_skip_list(t);
    }
    return (*key && *val) ? 1 : 0;
}

static int parse_xy(Tokenizer *t, V2 *out) {
    Token tok;
    tok = tok_next(t);
    if (tok.type != TOK_LPAREN || t->error) return 0;
    tok = tok_next(t);
    if (tok.type != TOK_SYMBOL || strcmp(tok.text, "xy") != 0) { tok_skip_list(t); return 0; }
    tok = tok_next(t);
    if (tok.type != TOK_NUMBER) { tok_skip_list(t); return 0; }
    out->x = (float)tok_numval(tok.text);
    tok = tok_next(t);
    if (tok.type != TOK_NUMBER) { tok_skip_list(t); return 0; }
    out->y = (float)tok_numval(tok.text);
    tok = tok_next(t);
    if (tok.type != TOK_RPAREN) { tok_skip_list(t); return 0; }
    return 1;
}

static int parse_pts(Tokenizer *t, V2 **pts, int *npts) {
    Token tok;
    int cap = 0;
    *pts = NULL; *npts = 0;
    tok = tok_next(t);
    if (tok.type != TOK_LPAREN || t->error) return 0;
    tok = tok_next(t);
    if (tok.type != TOK_SYMBOL || strcmp(tok.text, "pts") != 0) { tok_skip_list(t); return 0; }
    while (1) {
        tok = tok_next(t);
        if (tok.type == TOK_RPAREN || tok.type == TOK_NONE) break;
        if (tok.type == TOK_LPAREN) {
            V2 p;
            tok = tok_next(t);
            if (tok.type != TOK_SYMBOL || strcmp(tok.text, "xy") != 0) { tok_skip_list(t); return 0; }
            tok = tok_next(t);
            if (tok.type != TOK_NUMBER) { tok_skip_list(t); return 0; }
            p.x = (float)tok_numval(tok.text);
            tok = tok_next(t);
            if (tok.type != TOK_NUMBER) { tok_skip_list(t); return 0; }
            p.y = (float)tok_numval(tok.text);
            tok = tok_next(t);
            if (tok.type != TOK_RPAREN) { tok_skip_list(t); return 0; }
            if (*npts >= cap) {
                cap = cap ? cap * 2 : 8;
                *pts = (V2 *)realloc(*pts, sizeof(V2) * (size_t)cap);
            }
            (*pts)[(*npts)++] = p;
        }
    }
    return *npts;
}

static int parse_at(Tokenizer *t, float *x, float *y, float *rot) {
    Token tok;
    *rot = 0.0f;
    tok = tok_next(t);
    if (tok.type != TOK_LPAREN || t->error) return 0;
    tok = tok_next(t);
    if (tok.type != TOK_SYMBOL || strcmp(tok.text, "at") != 0) { tok_skip_list(t); return 0; }
    tok = tok_next(t);
    if (tok.type != TOK_NUMBER) { tok_skip_list(t); return 0; }
    *x = (float)tok_numval(tok.text);
    tok = tok_next(t);
    if (tok.type != TOK_NUMBER) { tok_skip_list(t); return 0; }
    *y = (float)tok_numval(tok.text);
    tok = tok_peek(t);
    if (tok.type == TOK_NUMBER) {
        (void)tok_next(t);
        *rot = (float)tok_numval(tok.text);
        tok = tok_next(t);
    }
    if (tok.type != TOK_RPAREN) { return 1; }
    return 1;
}

static Token tok_peek(Tokenizer *t) {
    const char *save = t->p;
    int save_err = t->error;
    Token tok = tok_next(t);
    t->p = save;
    t->error = save_err;
    return tok;
}

static void parse_symbol(Tokenizer *t, Schematic *s) {
    Token tok;
    char *libid = NULL, *ref = NULL, *val = NULL;
    float x = 0, y = 0, rot = 0;
    int has_pos = 0;

    tok = tok_next(t);
    if (tok.type != TOK_SYMBOL) return;
    libid = tok_strdup(&tok);

    while (1) {
        tok = tok_next(t);
        if (tok.type == TOK_RPAREN || tok.type == TOK_NONE) break;
        if (tok.type == TOK_LPAREN) {
            tok = tok_next(t);
            if (tok.type != TOK_SYMBOL) { tok_skip_list(t); continue; }
            if (strcmp(tok.text, "at") == 0) {
                tok = tok_next(t);
                if (tok.type == TOK_NUMBER) x = (float)tok_numval(tok.text);
                tok = tok_next(t);
                if (tok.type == TOK_NUMBER) y = (float)tok_numval(tok.text);
                tok = tok_next(t);
                if (tok.type == TOK_NUMBER) rot = (float)tok_numval(tok.text);
                else if (tok.type == TOK_RPAREN) { has_pos = 1; continue; }
                tok = tok_next(t);
                if (tok.type == TOK_RPAREN) has_pos = 1;
            } else if (strcmp(tok.text, "property") == 0) {
                tok = tok_next(t);
                if (tok.type == TOK_STRING) {
                    char *key = tok_strdup(&tok);
                    tok = tok_next(t);
                    if (tok.type == TOK_STRING) {
                        if (!ref && strcmp(key, "Reference") == 0) ref = tok_strdup(&tok);
                        if (!val && strcmp(key, "Value") == 0) val = tok_strdup(&tok);
                    }
                    free(key);
                    tok_skip_list(t);
                } else {
                    tok_skip_list(t);
                }
            } else {
                tok_skip_list(t);
            }
        }
    }
    if (has_pos) {
        if (!ref) ref = str_dup("U?");
        if (!val) val = str_dup("");
        sch_add_inst(s, ref, libid, val, v2(x, y), rot);
    }
    free(libid); free(ref); free(val);
}

static void parse_wire(Tokenizer *t, Schematic *s) {
    Token tok;
    V2 *pts = NULL; int npts = 0;
    tok = tok_peek(t);
    while (tok.type == TOK_LPAREN) {
        tok_next(t);
        tok = tok_next(t);
        if (tok.type == TOK_SYMBOL && strcmp(tok.text, "pts") == 0) {
            tok = tok_next(t);
            while (1) {
                tok = tok_next(t);
                if (tok.type == TOK_RPAREN || tok.type == TOK_NONE) break;
                if (tok.type == TOK_LPAREN) {
                    V2 p;
                    tok = tok_next(t);
                    if (tok.type == TOK_SYMBOL && strcmp(tok.text, "xy") == 0) {
                        tok = tok_next(t);
                        if (tok.type == TOK_NUMBER) p.x = (float)tok_numval(tok.text);
                        tok = tok_next(t);
                        if (tok.type == TOK_NUMBER) p.y = (float)tok_numval(tok.text);
                        tok = tok_next(t);
                        if (npts == 0) { pts = (V2 *)malloc(sizeof(V2)); *pts = p; npts = 1; }
                        else { pts = (V2 *)realloc(pts, sizeof(V2) * (size_t)(npts + 1)); pts[npts++] = p; }
                    }
                }
            }
        } else {
            tok_skip_list(t);
        }
        tok = tok_peek(t);
    }
    for (int i = 0; i + 1 < npts; i++) sch_add_wire(s, pts[i], pts[i + 1]);
    free(pts);
    tok = tok_next(t);
    (void)tok;
}

static void parse_label(Tokenizer *t, Schematic *s) {
    Token tok;
    char *name = NULL;
    V2 pos; int has_pos = 0;
    memset(&pos, 0, sizeof(pos));
    tok = tok_next(t);
    if (tok.type == TOK_STRING) name = tok_strdup(&tok);
    else if (tok.type == TOK_SYMBOL) name = tok_strdup(&tok);
    while (1) {
        tok = tok_next(t);
        if (tok.type == TOK_RPAREN || tok.type == TOK_NONE) break;
        if (tok.type == TOK_LPAREN) {
            tok = tok_next(t);
            if (tok.type == TOK_SYMBOL && strcmp(tok.text, "at") == 0) {
                tok = tok_next(t);
                if (tok.type == TOK_NUMBER) pos.x = (float)tok_numval(tok.text);
                tok = tok_next(t);
                if (tok.type == TOK_NUMBER) pos.y = (float)tok_numval(tok.text);
                has_pos = 1;
                tok_skip_list(t);
            } else {
                tok_skip_list(t);
            }
        }
    }
    if (name && has_pos) sch_add_label(s, name, pos);
    free(name);
}

static void parse_text(Tokenizer *t, Schematic *s) {
    Token tok;
    char *text = NULL;
    V2 pos; int has_pos = 0;
    memset(&pos, 0, sizeof(pos));
    tok = tok_next(t);
    if (tok.type == TOK_STRING) text = tok_strdup(&tok);
    else if (tok.type == TOK_SYMBOL) text = tok_strdup(&tok);
    while (1) {
        tok = tok_next(t);
        if (tok.type == TOK_RPAREN || tok.type == TOK_NONE) break;
        if (tok.type == TOK_LPAREN) {
            tok = tok_next(t);
            if (tok.type == TOK_SYMBOL && strcmp(tok.text, "at") == 0) {
                tok = tok_next(t);
                if (tok.type == TOK_NUMBER) pos.x = (float)tok_numval(tok.text);
                tok = tok_next(t);
                if (tok.type == TOK_NUMBER) pos.y = (float)tok_numval(tok.text);
                has_pos = 1;
                tok_skip_list(t);
            } else {
                tok_skip_list(t);
            }
        }
    }
    if (text && has_pos) sch_add_text(s, text, pos);
    free(text);
}

static void parse_junction(Tokenizer *t, Schematic *s) {
    Token tok;
    V2 pos; int has_pos = 0;
    memset(&pos, 0, sizeof(pos));
    while (1) {
        tok = tok_next(t);
        if (tok.type == TOK_RPAREN || tok.type == TOK_NONE) break;
        if (tok.type == TOK_LPAREN) {
            tok = tok_next(t);
            if (tok.type == TOK_SYMBOL && strcmp(tok.text, "at") == 0) {
                tok = tok_next(t);
                if (tok.type == TOK_NUMBER) pos.x = (float)tok_numval(tok.text);
                tok = tok_next(t);
                if (tok.type == TOK_NUMBER) pos.y = (float)tok_numval(tok.text);
                has_pos = 1;
                tok_skip_list(t);
            } else {
                tok_skip_list(t);
            }
        }
    }
    if (has_pos) sch_add_junction(s, pos);
}

static void parse_title_block(Tokenizer *t, Schematic *s) {
    (void)s;
    Token tok;
    while (1) {
        tok = tok_next(t);
        if (tok.type == TOK_RPAREN || tok.type == TOK_NONE) break;
        if (tok.type == TOK_LPAREN) {
            tok_skip_list(t);
        }
    }
}

bool kicad_parse_sch(const char *path, Schematic *out) {
    size_t file_len;
    char *data;
    Tokenizer t;
    Token tok;

    if (!path || !out) return false;
    memset(out, 0, sizeof(*out));
    data = file_read(path, &file_len);
    if (!data) return false;
    if (file_len == 0) { free(data); return false; }

    out->id = make_id();
    out->name = path_noext(path);

    tok_init(&t, data, (int)file_len);

    tok = tok_next(&t);
    if (tok.type != TOK_LPAREN) { free(data); return false; }
    tok = tok_next(&t);
    if (tok.type != TOK_SYMBOL || strcmp(tok.text, "kicad_sch") != 0) { free(data); return false; }

    while (1) {
        tok = tok_peek(&t);
        if (tok.type == TOK_RPAREN || tok.type == TOK_NONE) break;
        if (tok.type != TOK_LPAREN) { tok_next(&t); continue; }
        tok_next(&t);
        tok = tok_next(&t);
        if (tok.type != TOK_SYMBOL) { tok_skip_list(&t); continue; }
        if (strcmp(tok.text, "version") == 0) { tok_skip_list(&t); }
        else if (strcmp(tok.text, "title_block") == 0) { parse_title_block(&t, out); }
        else if (strcmp(tok.text, "page") == 0) { tok_skip_list(&t); }
        else if (strcmp(tok.text, "lib_symbols") == 0) { tok_skip_list(&t); }
        else if (strcmp(tok.text, "symbol") == 0) { parse_symbol(&t, out); }
        else if (strcmp(tok.text, "wire") == 0) { parse_wire(&t, out); }
        else if (strcmp(tok.text, "label") == 0) { parse_label(&t, out); }
        else if (strcmp(tok.text, "text") == 0) { parse_text(&t, out); }
        else if (strcmp(tok.text, "junction") == 0) { parse_junction(&t, out); }
        else { tok_skip_list(&t); }
    }
    tok = tok_next(&t);

    free(data);
    if (out->ninsts == 0 && out->nwires == 0 && out->nlabels == 0 && out->ntexts == 0) {
        schematic_free_contents(out);
        return false;
    }
    return true;
}
