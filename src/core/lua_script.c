#include "lua_script.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

/* ===== AST NODE TYPES ===== */
typedef enum {
    N_NUM, N_STRING, N_VAR, N_BINOP, N_ASSIGN,
    N_IF, N_FOR, N_CALL, N_BLOCK
} NodeType;

typedef struct AstNode AstNode;

typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV,
    OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE
} BinOp;

struct AstNode {
    NodeType type;
    union {
        struct { double val; }            num;
        struct { char *val; }             str;
        struct { char *name; }            var;
        struct { BinOp op; AstNode *l, *r; } binop;
        struct { char *name; AstNode *rhs; } assign;
        struct { AstNode *cond, *thn, *els; } iff;
        struct { char *var; AstNode *from, *to, *body; } forr;
        struct { char *name; AstNode **args; int nargs; } call;
        struct { AstNode **stmts; int nstmts; } block;
    } u;
};

/* ===== TK ===== */
typedef enum {
    TK_EOF, TK_NUM, TK_STRING, TK_ID, TK_EQ, TK_NE, TK_LE, TK_GE,
    TK_LT, TK_GT, TK_ASSIGN, TK_PLUS, TK_MINUS, TK_STAR, TK_SLASH,
    TK_LPAREN, TK_RPAREN, TK_COMMA, TK_SEMI, TK_IF, TK_THEN, TK_ELSE,
    TK_FOR, TK_DO, TK_END, TK_PRINT, TK_NEWLINE
} TokenType;

typedef struct {
    TokenType type;
    char *str_val;
    double num_val;
} Token;

/* ===== ENGINE ===== */
#define MAX_VARS 256
#define MAX_OUTPUT 65536

typedef struct {
    char *name;
    double val;
} VarEntry;

typedef struct {
    VarEntry vars[MAX_VARS];
    int nvars;
    char output[MAX_OUTPUT];
    int outlen;
    void *app_ptr;
} LuaState;

/* ===== LEXER ===== */
typedef struct {
    const char *p;
    const char *end;
    int line;
} Lexer;

static const char *keywords[] = {
    "if", "then", "else", "for", "do", "end", "print", NULL
};

static TokenType keyword_type(const char *s) {
    if (strcmp(s, "if")    == 0) return TK_IF;
    if (strcmp(s, "then")  == 0) return TK_THEN;
    if (strcmp(s, "else")  == 0) return TK_ELSE;
    if (strcmp(s, "for")   == 0) return TK_FOR;
    if (strcmp(s, "do")    == 0) return TK_DO;
    if (strcmp(s, "end")   == 0) return TK_END;
    if (strcmp(s, "print") == 0) return TK_PRINT;
    return TK_ID;
}

static void lexer_init(Lexer *l, const char *src, size_t len) {
    l->p = src;
    l->end = src + len;
    l->line = 1;
}

static void skip_ws_l(Lexer *l) {
    while (l->p < l->end) {
        if (*l->p == ' ' || *l->p == '\t' || *l->p == '\r') {
            l->p++;
        } else if (*l->p == '\n') {
            l->p++;
            l->line++;
        } else if (l->p + 1 < l->end && l->p[0] == '-' && l->p[1] == '-') {
            while (l->p < l->end && *l->p != '\n') l->p++;
        } else {
            break;
        }
    }
}

static Token lex_next(Lexer *l) {
    Token tok;
    memset(&tok, 0, sizeof(tok));
    skip_ws_l(l);
    if (l->p >= l->end) { tok.type = TK_EOF; return tok; }

    char c = *l->p;

    /* numbers */
    if (isdigit((unsigned char)c) || (c == '.' && l->p + 1 < l->end && isdigit((unsigned char)l->p[1]))) {
        char *endptr = NULL;
        tok.num_val = strtod(l->p, &endptr);
        if (endptr > l->p) { l->p = endptr; }
        tok.type = TK_NUM;
        return tok;
    }

    /* strings */
    if (c == '"' || c == '\'') {
        char quote = c;
        l->p++;
        Buf b; buf_init(&b);
        while (l->p < l->end && *l->p != quote) {
            if (*l->p == '\\' && l->p + 1 < l->end) {
                l->p++;
                switch (*l->p) {
                    case 'n': buf_append(&b, "\n", 1); break;
                    case 't': buf_append(&b, "\t", 1); break;
                    case '\\': buf_append(&b, "\\", 1); break;
                    case '"': buf_append(&b, "\"", 1); break;
                    case '\'': buf_append(&b, "'", 1); break;
                    default: buf_append(&b, l->p, 1); break;
                }
            } else {
                buf_append(&b, l->p, 1);
            }
            l->p++;
        }
        if (l->p < l->end) l->p++; /* skip closing quote */
        tok.type = TK_STRING;
        tok.str_val = buf_cstr(&b);
        return tok;
    }

    /* identifiers / keywords */
    if (isalpha((unsigned char)c) || c == '_') {
        const char *start = l->p;
        while (l->p < l->end && (isalnum((unsigned char)*l->p) || *l->p == '_' || *l->p == '.')) l->p++;
        size_t len = (size_t)(l->p - start);
        tok.str_val = str_dup_len(start, len);
        tok.type = keyword_type(tok.str_val);
        return tok;
    }

    /* operators and punct */
    l->p++;
    switch (c) {
        case '=':
            if (l->p < l->end && *l->p == '=') { l->p++; tok.type = TK_EQ; }
            else tok.type = TK_ASSIGN;
            break;
        case '~':
            if (l->p < l->end && *l->p == '=') { l->p++; tok.type = TK_NE; }
            else tok.type = TK_EOF;
            break;
        case '<':
            if (l->p < l->end && *l->p == '=') { l->p++; tok.type = TK_LE; }
            else tok.type = TK_LT;
            break;
        case '>':
            if (l->p < l->end && *l->p == '=') { l->p++; tok.type = TK_GE; }
            else tok.type = TK_GT;
            break;
        case '+': tok.type = TK_PLUS; break;
        case '-': tok.type = TK_MINUS; break;
        case '*': tok.type = TK_STAR; break;
        case '/': tok.type = TK_SLASH; break;
        case '(': tok.type = TK_LPAREN; break;
        case ')': tok.type = TK_RPAREN; break;
        case ',': tok.type = TK_COMMA; break;
        case ';': case '\n': tok.type = TK_NEWLINE; break;
        default:  tok.type = TK_EOF; break;
    }
    return tok;
}

/* ===== PARSER ===== */
typedef struct {
    Token cur;
    Lexer lex;
} Parser;

static void parser_advance(Parser *p) {
    if (p->cur.type == TK_STRING || p->cur.type == TK_ID ||
        p->cur.type == TK_IF || p->cur.type == TK_THEN || p->cur.type == TK_ELSE ||
        p->cur.type == TK_FOR || p->cur.type == TK_DO || p->cur.type == TK_END ||
        p->cur.type == TK_PRINT) {
        free(p->cur.str_val);
    }
    p->cur = lex_next(&p->lex);
}

static int parser_skip_newlines(Parser *p) {
    int skipped = 0;
    while (p->cur.type == TK_NEWLINE) {
        skipped++;
        parser_advance(p);
    }
    return skipped;
}

static AstNode *new_node(NodeType t) {
    AstNode *n = (AstNode *)calloc(1, sizeof(AstNode));
    n->type = t;
    return n;
}

static AstNode *parse_expr(Parser *p);

static AstNode *parse_atom(Parser *p) {
    AstNode *n = NULL;
    parser_skip_newlines(p);

    if (p->cur.type == TK_NUM) {
        n = new_node(N_NUM);
        n->u.num.val = p->cur.num_val;
        parser_advance(p);
    } else if (p->cur.type == TK_STRING) {
        n = new_node(N_STRING);
        n->u.str.val = str_dup(p->cur.str_val);
        parser_advance(p);
    } else if (p->cur.type == TK_ID) {
        /* function call or variable */
        char *name = str_dup(p->cur.str_val);
        parser_advance(p);
        if (p->cur.type == TK_LPAREN) {
            n = new_node(N_CALL);
            n->u.call.name = name;
            n->u.call.args = NULL;
            n->u.call.nargs = 0;
            parser_advance(p); /* skip ( */
            while (p->cur.type != TK_RPAREN && p->cur.type != TK_EOF) {
                AstNode *arg = parse_expr(p);
                n->u.call.nargs++;
                n->u.call.args = (AstNode **)realloc(n->u.call.args, (size_t)n->u.call.nargs * sizeof(AstNode *));
                n->u.call.args[n->u.call.nargs - 1] = arg;
                if (p->cur.type == TK_COMMA) parser_advance(p);
            }
            if (p->cur.type == TK_RPAREN) parser_advance(p);
        } else {
            n = new_node(N_VAR);
            n->u.var.name = name;
        }
    } else if (p->cur.type == TK_MINUS) {
        parser_advance(p);
        n = new_node(N_BINOP);
        n->u.binop.op = OP_MUL;
        n->u.binop.l = new_node(N_NUM);
        n->u.binop.l->u.num.val = -1.0;
        n->u.binop.r = parse_atom(p);
    } else if (p->cur.type == TK_LPAREN) {
        parser_advance(p);
        n = parse_expr(p);
        if (p->cur.type == TK_RPAREN) parser_advance(p);
    } else {
        n = new_node(N_NUM);
        n->u.num.val = 0.0;
    }

    return n;
}

static AstNode *parse_factor(Parser *p) {
    AstNode *left = parse_atom(p);
    while (p->cur.type == TK_STAR || p->cur.type == TK_SLASH) {
        TokenType op = p->cur.type;
        parser_advance(p);
        AstNode *n = new_node(N_BINOP);
        n->u.binop.op = (op == TK_STAR) ? OP_MUL : OP_DIV;
        n->u.binop.l = left;
        n->u.binop.r = parse_atom(p);
        left = n;
    }
    return left;
}

static AstNode *parse_term(Parser *p) {
    AstNode *left = parse_factor(p);
    while (p->cur.type == TK_PLUS || p->cur.type == TK_MINUS) {
        TokenType op = p->cur.type;
        parser_advance(p);
        AstNode *n = new_node(N_BINOP);
        n->u.binop.op = (op == TK_PLUS) ? OP_ADD : OP_SUB;
        n->u.binop.l = left;
        n->u.binop.r = parse_factor(p);
        left = n;
    }
    return left;
}

static AstNode *parse_cmp(Parser *p) {
    AstNode *left = parse_term(p);
    while (p->cur.type == TK_EQ || p->cur.type == TK_NE ||
           p->cur.type == TK_LT || p->cur.type == TK_LE ||
           p->cur.type == TK_GT || p->cur.type == TK_GE) {
        BinOp op;
        switch (p->cur.type) {
            case TK_EQ: op = OP_EQ; break;
            case TK_NE: op = OP_NE; break;
            case TK_LT: op = OP_LT; break;
            case TK_LE: op = OP_LE; break;
            case TK_GT: op = OP_GT; break;
            case TK_GE: op = OP_GE; break;
            default:    op = OP_EQ; break;
        }
        parser_advance(p);
        AstNode *n = new_node(N_BINOP);
        n->u.binop.op = op;
        n->u.binop.l = left;
        n->u.binop.r = parse_term(p);
        left = n;
    }
    return left;
}

static AstNode *parse_expr(Parser *p) {
    return parse_cmp(p);
}

static AstNode *parse_stmt(Parser *p);

static AstNode *parse_assignment_or_call(Parser *p) {
    if (p->cur.type == TK_ID) {
        char *name = str_dup(p->cur.str_val);
        Parser saved = *p;
        parser_advance(p);

        /* function call? */
        if (p->cur.type == TK_LPAREN) {
            *p = saved;
            return parse_expr(p);
        }

        /* assignment */
        if (p->cur.type == TK_ASSIGN) {
            parser_advance(p);
            AstNode *rhs = parse_expr(p);
            AstNode *n = new_node(N_ASSIGN);
            n->u.assign.name = name;
            n->u.assign.rhs = rhs;
            return n;
        }

        *p = saved;
    }
    return parse_expr(p);
}

static AstNode *parse_stmt(Parser *p) {
    parser_skip_newlines(p);

    if (p->cur.type == TK_IF) {
        parser_advance(p); /* skip 'if' */
        AstNode *cond = parse_expr(p);
        if (p->cur.type == TK_THEN) parser_advance(p);

        AstNode *body = new_node(N_BLOCK);
        while (p->cur.type != TK_ELSE && p->cur.type != TK_END && p->cur.type != TK_EOF) {
            AstNode *s = NULL;
            parser_skip_newlines(p);
            if (p->cur.type == TK_ELSE || p->cur.type == TK_END || p->cur.type == TK_EOF) break;
            if (p->cur.type == TK_PRINT) {
                parser_advance(p);
                if (p->cur.type == TK_LPAREN) parser_advance(p);
                s = new_node(N_CALL);
                s->u.call.name = str_dup("print");
                s->u.call.args = (AstNode **)malloc(sizeof(AstNode *));
                if (p->cur.type == TK_STRING) {
                    AstNode *ns = new_node(N_STRING);
                    ns->u.str.val = str_dup(p->cur.str_val);
                    s->u.call.args[0] = ns;
                    s->u.call.nargs = 1;
                    parser_advance(p);
                } else {
                    s->u.call.args[0] = parse_expr(p);
                    s->u.call.nargs = 1;
                }
                if (p->cur.type == TK_RPAREN) parser_advance(p);
            } else if (p->cur.type == TK_FOR) {
                s = parse_stmt(p);
            } else if (p->cur.type == TK_IF) {
                s = parse_stmt(p);
            } else {
                s = parse_assignment_or_call(p);
            }
            if (s) {
                body->u.block.nstmts++;
                body->u.block.stmts = (AstNode **)realloc(body->u.block.stmts, (size_t)body->u.block.nstmts * sizeof(AstNode *));
                body->u.block.stmts[body->u.block.nstmts - 1] = s;
            }
            parser_skip_newlines(p);
        }

        AstNode *else_body = NULL;
        if (p->cur.type == TK_ELSE) {
            parser_advance(p);
            else_body = new_node(N_BLOCK);
            while (p->cur.type != TK_END && p->cur.type != TK_EOF) {
                parser_skip_newlines(p);
                if (p->cur.type == TK_END || p->cur.type == TK_EOF) break;
                AstNode *s = NULL;
                if (p->cur.type == TK_PRINT) {
                    parser_advance(p);
                    if (p->cur.type == TK_LPAREN) parser_advance(p);
                    s = new_node(N_CALL);
                    s->u.call.name = str_dup("print");
                    s->u.call.args = (AstNode **)malloc(sizeof(AstNode *));
                    if (p->cur.type == TK_STRING) {
                        AstNode *ns = new_node(N_STRING);
                        ns->u.str.val = str_dup(p->cur.str_val);
                        s->u.call.args[0] = ns;
                        s->u.call.nargs = 1;
                        parser_advance(p);
                    } else {
                        s->u.call.args[0] = parse_expr(p);
                        s->u.call.nargs = 1;
                    }
                    if (p->cur.type == TK_RPAREN) parser_advance(p);
                } else {
                    s = parse_assignment_or_call(p);
                }
                if (s) {
                    else_body->u.block.nstmts++;
                    else_body->u.block.stmts = (AstNode **)realloc(else_body->u.block.stmts, (size_t)else_body->u.block.nstmts * sizeof(AstNode *));
                    else_body->u.block.stmts[else_body->u.block.nstmts - 1] = s;
                }
                parser_skip_newlines(p);
            }
        }

        if (p->cur.type == TK_END) parser_advance(p);

        AstNode *n = new_node(N_IF);
        n->u.iff.cond = cond;
        n->u.iff.thn = body;
        n->u.iff.els = else_body;
        return n;
    }

    if (p->cur.type == TK_FOR) {
        parser_advance(p); /* skip 'for' */
        if (p->cur.type != TK_ID) return new_node(N_NUM);
        char *var_name = str_dup(p->cur.str_val);
        parser_advance(p);
        if (p->cur.type == TK_ASSIGN) parser_advance(p);
        AstNode *from = parse_expr(p);
        /* skip ',' or next operand in for i=a,b syntax */
        if (p->cur.type == TK_COMMA) parser_advance(p);
        AstNode *to = parse_expr(p);
        if (p->cur.type == TK_DO) parser_advance(p);

        AstNode *body = new_node(N_BLOCK);
        while (p->cur.type != TK_END && p->cur.type != TK_EOF) {
            parser_skip_newlines(p);
            if (p->cur.type == TK_END || p->cur.type == TK_EOF) break;
            AstNode *s = NULL;
            if (p->cur.type == TK_PRINT) {
                parser_advance(p);
                if (p->cur.type == TK_LPAREN) parser_advance(p);
                s = new_node(N_CALL);
                s->u.call.name = str_dup("print");
                s->u.call.args = (AstNode **)malloc(sizeof(AstNode *));
                if (p->cur.type == TK_STRING) {
                    AstNode *ns = new_node(N_STRING);
                    ns->u.str.val = str_dup(p->cur.str_val);
                    s->u.call.args[0] = ns;
                    s->u.call.nargs = 1;
                    parser_advance(p);
                } else {
                    s->u.call.args[0] = parse_expr(p);
                    s->u.call.nargs = 1;
                }
                if (p->cur.type == TK_RPAREN) parser_advance(p);
            } else {
                s = parse_assignment_or_call(p);
            }
            if (s) {
                body->u.block.nstmts++;
                body->u.block.stmts = (AstNode **)realloc(body->u.block.stmts, (size_t)body->u.block.nstmts * sizeof(AstNode *));
                body->u.block.stmts[body->u.block.nstmts - 1] = s;
            }
            parser_skip_newlines(p);
        }
        if (p->cur.type == TK_END) parser_advance(p);

        AstNode *n = new_node(N_FOR);
        n->u.forr.var = var_name;
        n->u.forr.from = from;
        n->u.forr.to = to;
        n->u.forr.body = body;
        return n;
    }

    if (p->cur.type == TK_PRINT) {
        parser_advance(p);
        AstNode *n = new_node(N_CALL);
        n->u.call.name = str_dup("print");
        n->u.call.args = (AstNode **)malloc(sizeof(AstNode *));
        if (p->cur.type == TK_LPAREN) parser_advance(p);
        if (p->cur.type == TK_STRING) {
            AstNode *ns = new_node(N_STRING);
            ns->u.str.val = str_dup(p->cur.str_val);
            n->u.call.args[0] = ns;
            n->u.call.nargs = 1;
            parser_advance(p);
        } else {
            n->u.call.args[0] = parse_expr(p);
            n->u.call.nargs = 1;
        }
        if (p->cur.type == TK_RPAREN) parser_advance(p);
        return n;
    }

    return parse_assignment_or_call(p);
}

static AstNode *parse_program(Parser *p) {
    AstNode *prog = new_node(N_BLOCK);
    parser_skip_newlines(p);

    while (p->cur.type != TK_EOF) {
        AstNode *stmt = parse_stmt(p);
        if (stmt) {
            prog->u.block.nstmts++;
            prog->u.block.stmts = (AstNode **)realloc(prog->u.block.stmts, (size_t)prog->u.block.nstmts * sizeof(AstNode *));
            prog->u.block.stmts[prog->u.block.nstmts - 1] = stmt;
        }
        parser_skip_newlines(p);
    }

    return prog;
}

/* ===== EXECUTOR ===== */

static void ast_free(AstNode *n);
static double eval_expr(LuaState *st, AstNode *n);
static void exec_stmt(LuaState *st, AstNode *n);

static double get_var(LuaState *st, const char *name) {
    int i;
    for (i = 0; i < st->nvars; i++) {
        if (strcmp(st->vars[i].name, name) == 0) return st->vars[i].val;
    }
    return 0.0;
}

static void set_var(LuaState *st, const char *name, double val) {
    int i;
    for (i = 0; i < st->nvars; i++) {
        if (strcmp(st->vars[i].name, name) == 0) {
            st->vars[i].val = val;
            return;
        }
    }
    if (st->nvars < MAX_VARS) {
        st->vars[st->nvars].name = str_dup(name);
        st->vars[st->nvars].val = val;
        st->nvars++;
    }
}

static void output_append(LuaState *st, const char *s) {
    size_t slen = strlen(s);
    if (st->outlen + slen < MAX_OUTPUT) {
        memcpy(st->output + st->outlen, s, slen);
        st->outlen += (int)slen;
        st->output[st->outlen] = '\0';
    }
}

static void output_append_num(LuaState *st, double v) {
    char buf[64];
    if (v == floor(v) && fabs(v) < 1e12) {
        snprintf(buf, sizeof(buf), "%.0f", v);
    } else {
        snprintf(buf, sizeof(buf), "%g", v);
    }
    output_append(st, buf);
}

static double eval_expr(LuaState *st, AstNode *n) {
    if (!n) return 0.0;
    switch (n->type) {
        case N_NUM: return n->u.num.val;
        case N_VAR: return get_var(st, n->u.var.name);
        case N_STRING: return 0.0;
        case N_BINOP: {
            double l = eval_expr(st, n->u.binop.l);
            double r = eval_expr(st, n->u.binop.r);
            switch (n->u.binop.op) {
                case OP_ADD: return l + r;
                case OP_SUB: return l - r;
                case OP_MUL: return l * r;
                case OP_DIV: return r != 0.0 ? l / r : 0.0;
                case OP_EQ:  return (l == r) ? 1.0 : 0.0;
                case OP_NE:  return (l != r) ? 1.0 : 0.0;
                case OP_LT:  return (l < r)  ? 1.0 : 0.0;
                case OP_LE:  return (l <= r) ? 1.0 : 0.0;
                case OP_GT:  return (l > r)  ? 1.0 : 0.0;
                case OP_GE:  return (l >= r) ? 1.0 : 0.0;
            }
            return 0.0;
        }
        case N_CALL: {
            double arg_vals[4]; int i;
            for (i = 0; i < n->u.call.nargs && i < 4; i++) {
                arg_vals[i] = eval_expr(st, n->u.call.args[i]);
            }
            if (strcmp(n->u.call.name, "print") == 0) {
                for (i = 0; i < n->u.call.nargs; i++) {
                    if (i > 0) output_append(st, "\t");
                    /* check if the arg was a string literal */
                    if (n->u.call.args[i]->type == N_STRING) {
                        output_append(st, n->u.call.args[i]->u.str.val);
                    } else {
                        output_append_num(st, arg_vals[i]);
                    }
                }
                output_append(st, "\n");
                return 0.0;
            }
            /* volt.* API functions */
            if (st->app_ptr && strncmp(n->u.call.name, "volt.", 5) == 0) {
                const char *cmd = n->u.call.name + 5;
                if (strcmp(cmd, "create_box") == 0) {
                    output_append(st, "[volt] create_box\n");
                } else if (strcmp(cmd, "move") == 0) {
                    output_append(st, "[volt] move\n");
                } else if (strcmp(cmd, "rotate") == 0) {
                    output_append(st, "[volt] rotate\n");
                } else if (strcmp(cmd, "message") == 0) {
                    output_append(st, "[volt] message\n");
                } else if (strcmp(cmd, "slice") == 0) {
                    output_append(st, "[volt] slice\n");
                } else if (strcmp(cmd, "info") == 0) {
                    output_append(st, "[volt] info: Volt360 project\n");
                } else {
                    output_append(st, "[volt] unknown: ");
                    output_append(st, cmd);
                    output_append(st, "\n");
                }
            }
            return 0.0;
        }
        default: return 0.0;
    }
}

static void exec_stmt(LuaState *st, AstNode *n) {
    if (!n) return;
    switch (n->type) {
        case N_ASSIGN: {
            double v = eval_expr(st, n->u.assign.rhs);
            set_var(st, n->u.assign.name, v);
            break;
        }
        case N_IF: {
            double cond = eval_expr(st, n->u.iff.cond);
            if (cond != 0.0) {
                exec_stmt(st, n->u.iff.thn);
            } else if (n->u.iff.els) {
                exec_stmt(st, n->u.iff.els);
            }
            break;
        }
        case N_FOR: {
            double from = eval_expr(st, n->u.forr.from);
            double to   = eval_expr(st, n->u.forr.to);
            double step = (to >= from) ? 1.0 : -1.0;
            double i;
            int iters = 0;
            for (i = from; (step > 0 ? i <= to : i >= to) && iters < 10000; i += step, iters++) {
                set_var(st, n->u.forr.var, i);
                exec_stmt(st, n->u.forr.body);
            }
            break;
        }
        case N_BLOCK: {
            int i;
            for (i = 0; i < n->u.block.nstmts; i++) {
                exec_stmt(st, n->u.block.stmts[i]);
            }
            break;
        }
        case N_CALL: {
            eval_expr(st, n);
            break;
        }
        default:
            eval_expr(st, n);
            break;
    }
}

static void ast_free(AstNode *n) {
    if (!n) return;
    switch (n->type) {
        case N_STRING: free(n->u.str.val); break;
        case N_VAR:    free(n->u.var.name); break;
        case N_BINOP:  ast_free(n->u.binop.l); ast_free(n->u.binop.r); break;
        case N_ASSIGN: free(n->u.assign.name); ast_free(n->u.assign.rhs); break;
        case N_IF:     ast_free(n->u.iff.cond); ast_free(n->u.iff.thn); ast_free(n->u.iff.els); break;
        case N_FOR: {
            free(n->u.forr.var);
            ast_free(n->u.forr.from);
            ast_free(n->u.forr.to);
            ast_free(n->u.forr.body);
            break;
        }
        case N_CALL: {
            free(n->u.call.name);
            int i;
            for (i = 0; i < n->u.call.nargs; i++) ast_free(n->u.call.args[i]);
            free(n->u.call.args);
            break;
        }
        case N_BLOCK: {
            int i;
            for (i = 0; i < n->u.block.nstmts; i++) ast_free(n->u.block.stmts[i]);
            free(n->u.block.stmts);
            break;
        }
        default: break;
    }
    free(n);
}

/* ===== PUBLIC API ===== */

bool lua_init(LuaEngine *eng) {
    if (!eng) return false;
    LuaState *st = (LuaState *)calloc(1, sizeof(LuaState));
    if (!st) return false;
    st->nvars = 0;
    st->outlen = 0;
    st->output[0] = '\0';
    st->app_ptr = (void *)eng;
    eng->L = st;
    eng->initialized = 1;

    /* pre-register math constants */
    set_var(st, "pi", 3.141592653589793);
    return true;
}

void lua_close(LuaEngine *eng) {
    if (!eng || !eng->L) return;
    LuaState *st = (LuaState *)eng->L;
    int i;
    for (i = 0; i < st->nvars; i++) free(st->vars[i].name);
    free(st);
    eng->L = NULL;
    eng->initialized = 0;
}

bool lua_run_file(LuaEngine *eng, const char *path) {
    if (!eng || !eng->L || !path) return false;
    size_t len = 0;
    char *src = file_read(path, &len);
    if (!src) return false;
    bool result = lua_run_string(eng, src);
    free(src);
    return result;
}

bool lua_run_string(LuaEngine *eng, const char *code) {
    if (!eng || !eng->L || !code) return false;
    LuaState *st = (LuaState *)eng->L;

    /* reset output */
    st->outlen = 0;
    st->output[0] = '\0';

    Lexer lex;
    lexer_init(&lex, code, strlen(code));

    Parser p;
    memset(&p, 0, sizeof(p));
    p.lex = lex;
    parser_advance(&p);

    AstNode *prog = parse_program(&p);
    if (prog) {
        exec_stmt(st, prog);
        ast_free(prog);
        return true;
    }
    return false;
}

char *lua_get_output(LuaEngine *eng) {
    if (!eng || !eng->L) return str_dup("");
    LuaState *st = (LuaState *)eng->L;
    return str_dup(st->output);
}

void lua_register_api(LuaEngine *eng) {
    if (!eng || !eng->L) return;
    LuaState *st = (LuaState *)eng->L;

    /* register volt.* wrapper as internal variables to avoid lookup failures */
    set_var(st, "volt_create_box", 0);
    set_var(st, "volt_move", 0);
    set_var(st, "volt_rotate", 0);
    set_var(st, "volt_message", 0);
    set_var(st, "volt_slice", 0);
    set_var(st, "volt_info", 0);
}
