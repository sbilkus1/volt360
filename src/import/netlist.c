#include "importer.h"

#define N_PUSH(ptr, count, val) do { \
    (ptr) = (__typeof__(ptr))realloc((ptr), sizeof(*(ptr)) * ((size_t)(count) + 1)); \
    (ptr)[(count)] = (val); (count)++; \
} while (0)

static char *netlist_sym_name(const char *ref, const char *value) {
    // guess a symbol name from the value; fall back to ref prefix
    char *sym = str_dup(value && value[0] ? value : "net");
    return sym;
}

// KiCad .net (s-expression)
static bool netlist_from_sexpr(const char *data, size_t len, Project *p, Schematic *out) {
    SExprNode *root = sexpr_parse(data, len);
    if (!root) return false;
    schematic_free_contents(out);
    memset(out, 0, sizeof(*out));
    out->id = make_id();
    out->name = str_dup("netlist");

    SExprNode *comps = sexpr_find_child(root, "components");
    if (comps) {
        for (int i = 0; i < sexpr_child_count(comps); i++) {
            SExprNode *comp = sexpr_get(comps, i);
            if (!comp->is_list || strcmp(sexpr_name(comp), "comp") != 0) continue;
            SymInst si; memset(&si, 0, sizeof(si));
            si.id = make_id();
            const char *ref = "";
            const char *val = "";
            for (int k = 0; k < sexpr_child_count(comp); k++) {
                SExprNode *kv = sexpr_get(comp, k);
                if (!kv->is_list) continue;
                const char *kname = sexpr_name(kv);
                if (strcmp(kname, "ref") == 0) ref = sexpr_str(kv, 0) ? sexpr_str(kv, 0) : "";
                else if (strcmp(kname, "value") == 0) val = sexpr_str(kv, 0) ? sexpr_str(kv, 0) : "";
            }
            si.ref = str_dup(ref[0] ? ref : "?");
            si.value = str_dup(val);
            si.symbol = netlist_sym_name(ref, val);
            si.pos = v2((float)(out->ninsts % 6) * 12.7f, (float)(out->ninsts / 6) * 12.7f);
            N_PUSH(out->insts, out->ninsts, si);
        }
    }
    SExprNode *nets = sexpr_find_child(root, "nets");
    if (nets) {
        for (int i = 0; i < sexpr_child_count(nets); i++) {
            SExprNode *net = sexpr_get(nets, i);
            if (!net->is_list || strcmp(sexpr_name(net), "net") != 0) continue;
            const char *name = "";
            for (int k = 1; k < sexpr_child_count(net); k++) {
                SExprNode *kv = sexpr_get(net, k);
                if (!kv->is_list) continue;
                if (strcmp(sexpr_name(kv), "name") == 0) name = sexpr_str(kv, 0) ? sexpr_str(kv, 0) : "";
            }
            NetLabel nl; memset(&nl, 0, sizeof(nl));
            nl.name = str_dup(name);
            nl.pos = v2(60.0f, (float)(i % 40) * 2.54f);
            N_PUSH(out->labels, out->nlabels, nl);
        }
    }
    sexpr_free(root);
    return out->ninsts > 0 || out->nlabels > 0;
}

// SPICE .cir / LTSpice .asc
static bool netlist_from_spice(const char *data, Project *p, Schematic *out) {
    schematic_free_contents(out);
    memset(out, 0, sizeof(*out));
    out->id = make_id();
    out->name = str_dup("netlist");
    ARRAY(char *) nodes; arr_init(nodes);
    const char *line = data;
    int lineno = 0;
    while (line && *line) {
        const char *nl = strchr(line, '\n');
        size_t len = nl ? (size_t)(nl - line) : strlen(line);
        char buf[512];
        size_t cl = len < sizeof(buf) - 1 ? len : sizeof(buf) - 1;
        memcpy(buf, line, cl);
        buf[cl] = '\0';
        line = nl ? nl + 1 : NULL;
        lineno++;
        char *s = buf;
        str_trim(s);
        if (!*s || *s == '*' || *s == ';' || *s == '$') continue;
        // tokenize whitespace / equals
        int ntok = 0;
        char **tok = str_split(s, " \t", &ntok);
        if (ntok < 2) { str_free_split(tok, ntok); continue; }
        // first token: ref (may include leading _ or .subckt lines); skip directives
        if (tok[0][0] == '.') { str_free_split(tok, ntok); continue; }
        // reference designator = strip leading non-alpha
        char *ref = str_dup(tok[0]);
        char *rp = ref;
        while (*rp && !((*rp >= 'A' && *rp <= 'Z') || (*rp >= 'a' && *rp <= 'z'))) rp++;
        // node tokens are middle tokens that look like node names (start with letter, digit, or /)
        ARRAY(char *) node_toks; arr_init(node_toks);
        for (int i = 1; i < ntok - 1; i++) {
            if (tok[i][0] == '\0') continue;
            arr_push(node_toks, tok[i]);
        }
        // value = last token
        const char *value = ntok >= 2 ? tok[ntok - 1] : "";
        SymInst si; memset(&si, 0, sizeof(si));
        si.id = make_id();
        si.ref = str_dup(rp);
        si.value = str_dup(value);
        si.symbol = netlist_sym_name(rp, value);
        si.pos = v2((float)(out->ninsts % 6) * 12.7f, (float)(out->ninsts / 6) * 12.7f);
        N_PUSH(out->insts, out->ninsts, si);
        // collect nodes
        for (int i = 0; i < node_toks.len; i++) {
            bool seen = false;
            for (int k = 0; k < nodes.len; k++) if (strcmp(nodes.v[k], node_toks.v[i]) == 0) { seen = true; break; }
            if (!seen) arr_push(nodes, str_dup(node_toks.v[i]));
        }
        arr_free(node_toks);
        str_free_split(tok, ntok);
        free(ref);
    }
    for (int i = 0; i < nodes.len; i++) {
        NetLabel nl; memset(&nl, 0, sizeof(nl));
        nl.name = str_dup(nodes.v[i]);
        nl.pos = v2(60.0f, (float)(i % 40) * 2.54f);
        N_PUSH(out->labels, out->nlabels, nl);
    }
    str_free_split(nodes.v, nodes.len);
    return out->ninsts > 0 || out->nlabels > 0;
}

bool import_netlist(const char *path, Project *p) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    const char *ext = path_ext(path);
    Schematic sc; memset(&sc, 0, sizeof(sc));
    bool ok = false;
    if (strcmp(ext, "net") == 0) {
        ok = netlist_from_sexpr(data, len, p, &sc);
    } else {
        ok = netlist_from_spice(data, p, &sc);
    }
    free(data);
    if (ok) arr_push(p->schematics, sc);
    else schematic_free_contents(&sc);
    return ok;
}
