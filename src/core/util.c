#include "util.h"
#include <stdarg.h>
#include <math.h>
#include <ctype.h>

// ---------------- dynamic byte buffer ----------------
void buf_init(Buf *b) { b->data = NULL; b->len = 0; b->cap = 0; }
void buf_free(Buf *b) { free(b->data); b->data = NULL; b->len = b->cap = 0; }

void buf_reserve(Buf *b, size_t extra) {
    if (b->len + extra + 1 > b->cap) {
        size_t ncap = b->cap ? b->cap : 64;
        while (ncap < b->len + extra + 1) ncap *= 2;
        b->data = (char *)realloc(b->data, ncap);
        b->cap = ncap;
    }
}

void buf_append(Buf *b, const void *data, size_t len) {
    buf_reserve(b, len);
    memcpy(b->data + b->len, data, len);
    b->len += len;
    b->data[b->len] = '\0';
}

void buf_append_str(Buf *b, const char *s) { buf_append(b, s, strlen(s)); }

void buf_appendf(Buf *b, const char *fmt, ...) {
    va_list ap, ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);
    int n = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (n < 0) { va_end(ap2); return; }
    buf_reserve(b, (size_t)n);
    vsnprintf(b->data + b->len, (size_t)n + 1, fmt, ap2);
    b->len += (size_t)n;
    va_end(ap2);
}

char *buf_cstr(Buf *b) {
    if (!b->data) { b->data = (char *)calloc(1, 1); b->cap = 1; }
    b->data[b->len] = '\0';
    return b->data;
}

// ---------------- file IO ----------------
char *file_read(const char *path, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) { if (out_len) *out_len = 0; return NULL; }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz < 0) { fclose(f); if (out_len) *out_len = 0; return NULL; }
    char *data = (char *)malloc((size_t)sz + 1);
    size_t rd = fread(data, 1, (size_t)sz, f);
    fclose(f);
    data[rd] = '\0';
    if (out_len) *out_len = rd;
    return data;
}

bool file_write(const char *path, const void *data, size_t len) {
    FILE *f = fopen(path, "wb");
    if (!f) return false;
    size_t w = fwrite(data, 1, len, f);
    fclose(f);
    return w == len;
}

bool file_exists(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f) { fclose(f); return true; }
    return false;
}

#ifdef _WIN32
#include <windows.h>
bool dir_exists(const char *path) {
    DWORD a = GetFileAttributesA(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}
bool path_is_dir(const char *path) { return dir_exists(path); }
#else
#include <sys/stat.h>
bool dir_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}
bool path_is_dir(const char *path) { return dir_exists(path); }
#endif

// ---------------- string helpers ----------------
char *str_dup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char *d = (char *)malloc(n + 1);
    memcpy(d, s, n + 1);
    return d;
}

char *str_dup_len(const char *s, size_t n) {
    char *d = (char *)malloc(n + 1);
    memcpy(d, s, n);
    d[n] = '\0';
    return d;
}

bool str_ends_with(const char *s, const char *suffix) {
    size_t ls = strlen(s), lx = strlen(suffix);
    if (lx > ls) return false;
    return memcmp(s + ls - lx, suffix, lx) == 0;
}

bool str_ends_with_ci(const char *s, const char *suffix) {
    size_t ls = strlen(s), lx = strlen(suffix);
    if (lx > ls) return false;
    for (size_t i = 0; i < lx; i++) {
        if (tolower((unsigned char)s[ls - lx + i]) != tolower((unsigned char)suffix[i])) return false;
    }
    return true;
}

bool str_starts_with(const char *s, const char *prefix) {
    return strncmp(s, prefix, strlen(prefix)) == 0;
}

const char *path_ext(const char *path) {
    static char buf[16];
    const char *p = strrchr(path, '.');
    const char *slash = strrchr(path, '/');
    const char *bslash = strrchr(path, '\\');
    const char *sep = slash > bslash ? slash : bslash;
    if (!p || p == path || (sep && p < sep)) return "";
    size_t n = strlen(p + 1);
    if (n >= sizeof(buf)) n = sizeof(buf) - 1;
    for (size_t i = 0; i < n; i++) buf[i] = (char)tolower((unsigned char)p[1 + i]);
    buf[n] = '\0';
    return buf;
}

const char *path_ext_dot(const char *path) {
    static char buf[16];
    const char *e = path_ext(path);
    snprintf(buf, sizeof(buf), ".%s", e);
    return buf;
}

char *path_basename(const char *path) {
    const char *p = strrchr(path, '/');
    const char *b = strrchr(path, '\\');
    const char *sep = b > p ? b : p;
    return str_dup(sep ? sep + 1 : path);
}

char *path_noext(const char *path) {
    char *bn = path_basename(path);
    char *dot = strrchr(bn, '.');
    if (dot) *dot = '\0';
    return bn;
}

char *path_dirname(const char *path) {
    const char *p = strrchr(path, '/');
    const char *b = strrchr(path, '\\');
    const char *sep = b > p ? b : p;
    if (!sep) return str_dup(".");
    if (sep == path) return str_dup("/");
    return str_dup_len(path, (size_t)(sep - path));
}

char *path_join(const char *a, const char *b) {
    size_t la = strlen(a);
    Buf buf; buf_init(&buf);
    buf_append_str(&buf, a);
    if (la > 0 && a[la - 1] != '/' && a[la - 1] != '\\') buf_append_str(&buf, "/");
    buf_append_str(&buf, b);
    char *r = str_dup(buf_cstr(&buf));
    buf_free(&buf);
    return r;
}

char *path_normalize(const char *path) {
    Buf b; buf_init(&b);
    const char *p = path;
    while (*p) {
        if ((p[0] == '.' && p[1] == '/' ) || (p[0] == '.' && p[1] == '\\')) {
            p += 2;
            continue;
        }
        if (*p == '\\') { buf_append_str(&b, "/"); p++; continue; }
        if (p[0] == '/' && p[1] == '/') { p++; continue; }
        buf_append(&b, p, 1);
        p++;
    }
    char *r = str_dup(buf_cstr(&b));
    buf_free(&b);
    return r;
}

void str_trim(char *s) {
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1])) end--;
    *end = '\0';
    if (start != s) memmove(s, start, (size_t)(end - start) + 1);
}

void str_lower(char *s) {
    for (; *s; s++) *s = (char)tolower((unsigned char)*s);
}

char *str_lower_dup(const char *s) {
    char *d = str_dup(s);
    str_lower(d);
    return d;
}

int str_icmp(const char *a, const char *b) {
    while (*a && *b) {
        int ca = tolower((unsigned char)*a), cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

char **str_split(const char *s, const char *delims, int *count) {
    int n = 0;
    ARRAY(char *) parts; arr_init(parts);
    const char *p = s;
    while (*p) {
        while (*p && strchr(delims, *p)) p++;
        if (!*p) break;
        const char *start = p;
        while (*p && !strchr(delims, *p)) p++;
        arr_push(parts, str_dup_len(start, (size_t)(p - start)));
        n++;
    }
    *count = n;
    return parts.v;
}

void str_free_split(char **parts, int count) {
    for (int i = 0; i < count; i++) free(parts[i]);
    free(parts);
}

char *str_join(const char **parts, int count, const char *sep) {
    Buf b; buf_init(&b);
    for (int i = 0; i < count; i++) {
        if (i) buf_append_str(&b, sep);
        buf_append_str(&b, parts[i]);
    }
    char *r = str_dup(buf_cstr(&b));
    buf_free(&b);
    return r;
}

char *str_replace_all(const char *src, const char *from, const char *to) {
    size_t lf = strlen(from);
    if (!lf) return str_dup(src);
    Buf b; buf_init(&b);
    const char *p = src;
    const char *hit;
    while ((hit = strstr(p, from)) != NULL) {
        buf_append(&b, p, (size_t)(hit - p));
        buf_append_str(&b, to);
        p = hit + lf;
    }
    buf_append_str(&b, p);
    char *r = str_dup(buf_cstr(&b));
    buf_free(&b);
    return r;
}

int str_contains(const char *s, const char *sub) { return strstr(s, sub) != NULL; }

size_t str_utf8_len(const char *s) {
    size_t n = 0;
    for (; *s; s++) if ((unsigned char)*s < 0x80 || (*s & 0xC0) != 0x80) n++;
    return n;
}

bool str_is_number(const char *s) {
    if (!*s) return false;
    bool any = false;
    for (; *s; s++) {
        if (isdigit((unsigned char)*s)) { any = true; continue; }
        if (*s == '.' || *s == ',' || *s == '-') continue;
        if (*s == 'e' || *s == 'E') continue;
        return false;
    }
    return any;
}

double str_atof_any(const char *s, bool *ok) {
    // handles "1,5" style too
    char *buf = str_dup(s);
    for (char *p = buf; *p; p++) if (*p == ',') *p = '.';
    char *end = NULL;
    double v = strtod(buf, &end);
    bool good = end != buf;
    free(buf);
    if (ok) *ok = good;
    return good ? v : 0.0;
}

// ---------------- math ----------------
V3 v3_rot_x(V3 v, float a) {
    float c = cosf(a), s = sinf(a);
    return v3(v.x, v.y * c - v.z * s, v.y * s + v.z * c);
}
V3 v3_rot_y(V3 v, float a) {
    float c = cosf(a), s = sinf(a);
    return v3(v.x * c + v.z * s, v.y, -v.x * s + v.z * c);
}
V3 v3_rot_z(V3 v, float a) {
    float c = cosf(a), s = sinf(a);
    return v3(v.x * c - v.y * s, v.x * s + v.y * c, v.z);
}

uint32_t hash_str(const char *s) {
    uint32_t h = 2166136261u;
    for (; *s; s++) { h ^= (unsigned char)*s; h *= 16777619u; }
    return h;
}

char *make_id(void) {
    static uint32_t counter = 0x5eed1234;
    counter += 0x9e3779b9;
    char buf[40];
    snprintf(buf, sizeof(buf), "%08x%08x", counter, (uint32_t)(uintptr_t)&counter);
    return str_dup(buf);
}
