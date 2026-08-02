#pragma once
// json.h - minimal JSON parser + incremental writer (no deps)
#include "util.h"

typedef enum { J_NULL, J_BOOL, J_NUM, J_STR, J_ARR, J_OBJ } JsonType;

typedef struct JsonValue JsonValue;
typedef struct { char *key; JsonValue *val; } JsonPair;
typedef struct {
    ARRAY(JsonPair) pairs;
} JsonObj;
typedef struct {
    ARRAY(JsonValue *) items;
} JsonArr;

struct JsonValue {
    JsonType type;
    union {
        bool b;
        double num;
        char *str;
        JsonObj obj;
        JsonArr arr;
    } u;
};

// ---- parse ----
JsonValue *json_parse(const char *text, size_t len); // returns NULL on failure (root must be object/array)
void json_free(JsonValue *v);

// ---- accessors ----
JsonValue *json_get(JsonValue *obj, const char *key);            // obj must be J_OBJ
const char *json_get_str(JsonValue *obj, const char *key);       // NULL if missing / not string
double json_get_num(JsonValue *obj, const char *key);            // 0 if missing
bool json_get_bool(JsonValue *obj, const char *key);             // false if missing
JsonValue *json_get_arr(JsonValue *obj, const char *key);        // NULL if missing / not array
JsonValue *json_get_obj(JsonValue *obj, const char *key);        // NULL if missing / not object
int json_arr_len(JsonValue *arr);
JsonValue *json_arr_get(JsonValue *arr, int i);
JsonValue *json_has(JsonValue *obj, const char *key);

// string value as-is if string, else formatted number/true/false
const char *json_get_display_str(JsonValue *obj, const char *key); // static-ish: returns internal for str, formatted static buf for others
double json_num_value(JsonValue *v);      // value if J_NUM, else 0
const char *json_str_value(JsonValue *v); // string if J_STR, else NULL

// ---- incremental writer ----
typedef struct { Buf b; int depth; bool first[64]; char close[64]; } JsonW;
void jw_init(JsonW *w);
const char *jw_cstr(JsonW *w);     // NUL-terminated result
void jw_free(JsonW *w);
void jw_begin(JsonW *w);           // root object
void jw_end(JsonW *w);
void jw_key(JsonW *w, const char *key);
void jw_str(JsonW *w, const char *s);
void jw_num(JsonW *w, double n);
void jw_int(JsonW *w, long long n);
void jw_bool(JsonW *w, bool b);
void jw_null(JsonW *w);
void jw_obj(JsonW *w);             // open nested object (after jw_key)
void jw_arr(JsonW *w);             // open nested array (after jw_key)
void jw_arr_item_str(JsonW *w, const char *s); // convenience within array
void jw_arr_item_num(JsonW *w, double n);
void jw_arr_item_obj(JsonW *w);    // open object item in array
