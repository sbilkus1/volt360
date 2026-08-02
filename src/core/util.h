#pragma once
// util.h - shared string/buffer/array/math helpers for Volt360
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef _MSC_VER
#define INLINE static __inline
#else
#define INLINE static inline
#endif

// ---------------- dynamic byte buffer ----------------
typedef struct { char *data; size_t len, cap; } Buf;
void buf_init(Buf *b);
void buf_free(Buf *b);
void buf_reserve(Buf *b, size_t extra);
void buf_append(Buf *b, const void *data, size_t len);
void buf_append_str(Buf *b, const char *s);
void buf_appendf(Buf *b, const char *fmt, ...);
void buf_append_printf(Buf *b, const char *fmt, ...); // alias kept for clarity
char *buf_cstr(Buf *b); // ensure NUL-terminated, return data

// ---------------- file IO ----------------
char *file_read(const char *path, size_t *out_len);          // malloc'd
bool file_write(const char *path, const void *data, size_t len);
bool file_exists(const char *path);
bool dir_exists(const char *path);
bool path_is_dir(const char *path);

// ---------------- string helpers (malloc'd results) ----------------
char *str_dup(const char *s);
char *str_dup_len(const char *s, size_t n);
bool str_ends_with_ci(const char *s, const char *suffix);
bool str_ends_with(const char *s, const char *suffix);
bool str_starts_with(const char *s, const char *prefix);
const char *path_ext(const char *path);                       // lowercase ext w/o dot, static buf
const char *path_ext_dot(const char *path);                   // lowercase ext with dot
char *path_basename(const char *path);                        // malloc'd
char *path_noext(const char *path);                           // malloc'd
char *path_dirname(const char *path);                         // malloc'd
char *path_join(const char *a, const char *b);                // malloc'd
char *path_normalize(const char *path);                       // malloc'd, forward slashes + remove ./
void str_trim(char *s);                                       // in place
void str_lower(char *s);                                      // in place
char *str_lower_dup(const char *s);
int str_icmp(const char *a, const char *b);
char **str_split(const char *s, const char *delims, int *count); // malloc'd array of malloc'd strs
void str_free_split(char **parts, int count);
char *str_join(const char **parts, int count, const char *sep); // malloc'd
char *str_replace_all(const char *src, const char *from, const char *to); // malloc'd
int str_contains(const char *s, const char *sub);
size_t str_utf8_len(const char *s); // number of code points (approx)
bool str_is_number(const char *s);
double str_atof_any(const char *s, bool *ok); // parses comma decimal too

// ---------------- dynamic arrays ----------------
#define ARRAY(T) struct { T *v; int len; int cap; }
#define arr_init(a) do { (a).v = NULL; (a).len = 0; (a).cap = 0; } while (0)
#define arr_free(a) do { free((a).v); (a).v = NULL; (a).len = (a).cap = 0; } while (0)
#define arr_push(a, x) do { if ((a).len >= (a).cap) { (a).cap = (a).cap ? (a).cap * 2 : 8; (a).v = (__typeof__((a).v))realloc((a).v, (size_t)(a).cap * sizeof(*(a).v)); } (a).v[(a).len++] = (x); } while (0)
#define arr_count(a) ((a).len)
#define arr_get(a, i) ((a).v[(i)])
#define arr_last(a) ((a).v[(a).len - 1])
#define arr_clear(a) do { (a).len = 0; } while (0)
#define arr_remove_at(a, i) do { if ((i) < (a).len) { memmove(&(a).v[(i)], &(a).v[(i)+1], sizeof(*(a).v) * ((size_t)(a).len - (i) - 1)); (a).len--; } } while (0)

// ---------------- math ----------------
typedef struct { float x, y; } V2;
typedef struct { float x, y, z; } V3;

INLINE V2 v2(float x, float y) { V2 v = { x, y }; return v; }
INLINE V3 v3(float x, float y, float z) { V3 v = { x, y, z }; return v; }
INLINE float v2_dot(V2 a, V2 b) { return a.x * b.x + a.y * b.y; }
INLINE V2 v2_sub(V2 a, V2 b) { return v2(a.x - b.x, a.y - b.y); }
INLINE V2 v2_add(V2 a, V2 b) { return v2(a.x + b.x, a.y + b.y); }
INLINE V2 v2_scale(V2 a, float s) { return v2(a.x * s, a.y * s); }
INLINE float v2_len(V2 a) { return sqrtf(v2_dot(a, a)); }
INLINE float v2_dist(V2 a, V2 b) { return v2_len(v2_sub(a, b)); }
INLINE V3 v3_add(V3 a, V3 b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
INLINE V3 v3_sub(V3 a, V3 b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }
INLINE V3 v3_scale(V3 a, float s) { return v3(a.x * s, a.y * s, a.z * s); }
INLINE float v3_dot(V3 a, V3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
INLINE float v3_len(V3 a) { return sqrtf(v3_dot(a, a)); }
INLINE V3 v3_cross(V3 a, V3 b) { return v3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }
INLINE V3 v3_min(V3 a, V3 b) { return v3(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z); }
INLINE V3 v3_max(V3 a, V3 b) { return v3(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z); }
INLINE V3 v3_lerp(V3 a, V3 b, float t) { return v3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t); }

// rotate vector about X/Y/Z axis by angle radians
V3 v3_rot_x(V3 v, float a);
V3 v3_rot_y(V3 v, float a);
V3 v3_rot_z(V3 v, float a);

// ---------------- misc ----------------
uint32_t hash_str(const char *s);
char *make_id(void); // malloc'd unique id
