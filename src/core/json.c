#include "json.h"
#include <ctype.h>
#include <math.h>

// ================= PARSER =================

typedef struct { const char *p; const char *end; } JP;

static void jp_ws(JP *j) { while (j->p < j->end && isspace((unsigned char)*j->p)) j->p++; }

static bool jp_match(JP *j, const char *lit) {
    size_t n = strlen(lit);
    if ((size_t)(j->end - j->p) < n) return false;
    if (memcmp(j->p, lit, n) == 0) { j->p += n; return true; }
    return false;
}

static JsonValue *jp_value(JP *j);

static JsonValue *make(JsonType t) {
    JsonValue *v = (JsonValue *)calloc(1, sizeof(JsonValue));
    v->type = t;
    return v;
}

static JsonValue *jp_string(JP *j) {
    // assume at '"'
    j->p++;
    Buf b; buf_init(&b);
    while (j->p < j->end && *j->p != '"') {
        char c = *j->p++;
        if (c == '\\') {
            if (j->p >= j->end) break;
            char e = *j->p++;
            switch (e) {
                case 'n': buf_append(&b, "\n", 1); break;
                case 't': buf_append(&b, "\t", 1); break;
                case 'r': buf_append(&b, "\r", 1); break;
                case 'b': buf_append(&b, "\b", 1); break;
                case 'f': buf_append(&b, "\f", 1); break;
                case 'u': {
                    char hex[5] = {0};
                    if (j->end - j->p >= 4) { memcpy(hex, j->p, 4); j->p += 4; }
                    char tmp[8];
                    unsigned cp = (unsigned)strtoul(hex, NULL, 16);
                    if (cp < 0x80) { tmp[0] = (char)cp; tmp[1] = 0; }
                    else if (cp < 0x800) { tmp[0] = (char)(0xC0 | (cp >> 6)); tmp[1] = (char)(0x80 | (cp & 0x3F)); tmp[2] = 0; }
                    else { tmp[0] = (char)(0xE0 | (cp >> 12)); tmp[1] = (char)(0x80 | ((cp >> 6) & 0x3F)); tmp[2] = (char)(0x80 | (cp & 0x3F)); tmp[3] = 0; }
                    buf_append_str(&b, tmp);
                    break;
                }
                default: buf_append(&b, &e, 1); break;
            }
        } else {
            buf_append(&b, &c, 1);
        }
    }
    if (j->p < j->end && *j->p == '"') j->p++;
    JsonValue *v = make(J_STR);
    v->u.str = str_dup(buf_cstr(&b));
    buf_free(&b);
    return v;
}

static JsonValue *jp_number(JP *j) {
    const char *start = j->p;
    if (*j->p == '-') j->p++;
    while (j->p < j->end && (isdigit((unsigned char)*j->p) || *j->p == '.' || *j->p == 'e' || *j->p == 'E' || *j->p == '+' || *j->p == '-')) j->p++;
    JsonValue *v = make(J_NUM);
    v->u.num = strtod(start, NULL);
    return v;
}

static JsonValue *jp_object(JP *j) {
    j->p++; // '{'
    JsonValue *v = make(J_OBJ);
    arr_init(v->u.obj.pairs);
    jp_ws(j);
    if (j->p < j->end && *j->p == '}') { j->p++; return v; }
    while (j->p < j->end) {
        jp_ws(j);
        if (j->p >= j->end || *j->p != '"') break;
        JsonValue *k = jp_string(j);
        jp_ws(j);
        if (j->p < j->end && *j->p == ':') j->p++;
        jp_ws(j);
        JsonValue *val = jp_value(j);
        if (!val) { json_free(k); break; }
        JsonPair pair = { str_dup(k->u.str), val };
        arr_push(v->u.obj.pairs, pair);
        json_free(k);
        jp_ws(j);
        if (j->p < j->end && *j->p == ',') { j->p++; continue; }
        break;
    }
    jp_ws(j);
    if (j->p < j->end && *j->p == '}') j->p++;
    return v;
}

static JsonValue *jp_array(JP *j) {
    j->p++; // '['
    JsonValue *v = make(J_ARR);
    arr_init(v->u.arr.items);
    jp_ws(j);
    if (j->p < j->end && *j->p == ']') { j->p++; return v; }
    while (j->p < j->end) {
        jp_ws(j);
        JsonValue *val = jp_value(j);
        if (!val) break;
        arr_push(v->u.arr.items, val);
        jp_ws(j);
        if (j->p < j->end && *j->p == ',') { j->p++; continue; }
        break;
    }
    jp_ws(j);
    if (j->p < j->end && *j->p == ']') j->p++;
    return v;
}

static JsonValue *jp_value(JP *j) {
    jp_ws(j);
    if (j->p >= j->end) return NULL;
    char c = *j->p;
    if (c == '{') return jp_object(j);
    if (c == '[') return jp_array(j);
    if (c == '"') return jp_string(j);
    if (c == '-' || isdigit((unsigned char)c)) return jp_number(j);
    if (jp_match(j, "true")) { JsonValue *v = make(J_BOOL); v->u.b = true; return v; }
    if (jp_match(j, "false")) { JsonValue *v = make(J_BOOL); v->u.b = false; return v; }
    if (jp_match(j, "null")) { return make(J_NULL); }
    return NULL;
}

JsonValue *json_parse(const char *text, size_t len) {
    JP j = { text, text + len };
    JsonValue *v = jp_value(&j);
    return v;
}

void json_free(JsonValue *v) {
    if (!v) return;
    switch (v->type) {
        case J_STR: free(v->u.str); break;
        case J_ARR:
            for (int i = 0; i < v->u.arr.items.len; i++) json_free(v->u.arr.items.v[i]);
            arr_free(v->u.arr.items);
            break;
        case J_OBJ:
            for (int i = 0; i < v->u.obj.pairs.len; i++) { free(v->u.obj.pairs.v[i].key); json_free(v->u.obj.pairs.v[i].val); }
            arr_free(v->u.obj.pairs);
            break;
        default: break;
    }
    free(v);
}

// ---------------- accessors ----------------
JsonValue *json_get(JsonValue *obj, const char *key) {
    if (!obj || obj->type != J_OBJ) return NULL;
    for (int i = 0; i < obj->u.obj.pairs.len; i++) {
        if (strcmp(obj->u.obj.pairs.v[i].key, key) == 0) return obj->u.obj.pairs.v[i].val;
    }
    return NULL;
}

JsonValue *json_has(JsonValue *obj, const char *key) { return json_get(obj, key); }

const char *json_get_str(JsonValue *obj, const char *key) {
    JsonValue *v = json_get(obj, key);
    if (v && v->type == J_STR) return v->u.str;
    return NULL;
}

double json_get_num(JsonValue *obj, const char *key) {
    JsonValue *v = json_get(obj, key);
    if (v && v->type == J_NUM) return v->u.num;
    return 0.0;
}

bool json_get_bool(JsonValue *obj, const char *key) {
    JsonValue *v = json_get(obj, key);
    if (v && v->type == J_BOOL) return v->u.b;
    return false;
}

JsonValue *json_get_arr(JsonValue *obj, const char *key) {
    JsonValue *v = json_get(obj, key);
    if (v && v->type == J_ARR) return v;
    return NULL;
}

JsonValue *json_get_obj(JsonValue *obj, const char *key) {
    JsonValue *v = json_get(obj, key);
    if (v && v->type == J_OBJ) return v;
    return NULL;
}

int json_arr_len(JsonValue *arr) { return (arr && arr->type == J_ARR) ? arr->u.arr.items.len : 0; }
JsonValue *json_arr_get(JsonValue *arr, int i) {
    if (!arr || arr->type != J_ARR || i < 0 || i >= arr->u.arr.items.len) return NULL;
    return arr->u.arr.items.v[i];
}

const char *json_get_display_str(JsonValue *obj, const char *key) {
    static char buf[64];
    JsonValue *v = json_get(obj, key);
    if (!v) return "";
    switch (v->type) {
        case J_STR: return v->u.str;
        case J_NUM: snprintf(buf, sizeof(buf), "%.10g", v->u.num); return buf;
        case J_BOOL: return v->u.b ? "true" : "false";
        case J_NULL: return "";
        default: return "";
    }
}

double json_num_value(JsonValue *v) {
    if (v && v->type == J_NUM) return v->u.num;
    if (v && v->type == J_STR) { bool ok; return str_atof_any(v->u.str, &ok); }
    return 0.0;
}

const char *json_str_value(JsonValue *v) {
    if (v && v->type == J_STR) return v->u.str;
    return NULL;
}

// ================= WRITER =================
void jw_init(JsonW *w) { buf_init(&w->b); w->depth = 0; w->first[0] = true; }
const char *jw_cstr(JsonW *w) { return buf_cstr(&w->b); }
void jw_free(JsonW *w) { buf_free(&w->b); }

static void jw_begin_item(JsonW *w) {
    int d = w->depth;
    if (!w->first[d]) buf_append_str(&w->b, ",");
    w->first[d] = false;
}

void jw_begin(JsonW *w) {
    buf_append_str(&w->b, "{");
    w->depth = 0;
    w->first[0] = true;
    w->close[0] = '}';
}

void jw_key(JsonW *w, const char *key) {
    jw_begin_item(w);
    jw_str(w, key);
    buf_append_str(&w->b, ":");
}

static void esc(Buf *b, const char *s) {
    for (const unsigned char *p = (const unsigned char *)s; *p; p++) {
        switch (*p) {
            case '"': buf_append_str(b, "\\\""); break;
            case '\\': buf_append_str(b, "\\\\"); break;
            case '\n': buf_append_str(b, "\\n"); break;
            case '\r': buf_append_str(b, "\\r"); break;
            case '\t': buf_append_str(b, "\\t"); break;
            case '\b': buf_append_str(b, "\\b"); break;
            case '\f': buf_append_str(b, "\\f"); break;
            default:
                if (*p < 0x20) buf_appendf(b, "\\u%04x", *p);
                else buf_append(b, p, 1);
        }
    }
}

void jw_str(JsonW *w, const char *s) {
    if (!s) { jw_null(w); return; }
    buf_append_str(&w->b, "\"");
    esc(&w->b, s);
    buf_append_str(&w->b, "\"");
}

void jw_num(JsonW *w, double n) {
    if (isnan(n) || isinf(n)) { buf_append_str(&w->b, "0"); return; }
    buf_appendf(&w->b, "%.10g", n);
}

void jw_int(JsonW *w, long long n) { buf_appendf(&w->b, "%lld", n); }
void jw_bool(JsonW *w, bool b) { buf_append_str(&w->b, b ? "true" : "false"); }
void jw_null(JsonW *w) { buf_append_str(&w->b, "null"); }

void jw_obj(JsonW *w) {
    buf_append_str(&w->b, "{");
    w->depth++;
    w->first[w->depth] = true;
    w->close[w->depth] = '}';
}

void jw_arr(JsonW *w) {
    buf_append_str(&w->b, "[");
    w->depth++;
    w->first[w->depth] = true;
    w->close[w->depth] = ']';
}

void jw_end(JsonW *w) {
    char c = (w->depth >= 0) ? w->close[w->depth] : '}';
    buf_append_str(&w->b, (c == ']') ? "]" : "}");
    if (w->depth > 0) w->first[w->depth - 1] = false;
    w->depth--;
}

void jw_arr_item_str(JsonW *w, const char *s) { jw_begin_item(w); jw_str(w, s); }
void jw_arr_item_num(JsonW *w, double n) { jw_begin_item(w); jw_num(w, n); }
void jw_arr_item_obj(JsonW *w) {
    jw_begin_item(w);
    buf_append_str(&w->b, "{");
    w->depth++;
    w->first[w->depth] = true;
    w->close[w->depth] = '}';
}
