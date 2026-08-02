#include "slicer.h"
#include "../core/util.h"
#include "../core/json.h"
#include "infill_ext.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SL_EPS 1e-5f
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

void slice_settings_default(SliceSettings *s) {
    memset(s, 0, sizeof(*s));
    s->layer_height = 0.2f;
    s->nozzle_diameter = 0.4f;
    s->line_width = 0.4f * 1.2f;
    s->perimeters = 2;
    s->solid_top_layers = 3;
    s->solid_bottom_layers = 3;
    s->infill_density = 15;
    s->infill_angle = 45;
    s->infill_pattern = 0; // grid
    s->bed_temp = 60;
    s->hotend_temp = 210;
    s->print_speed = 60;
    s->travel_speed = 150;
    s->retract_mm = 0.8f;
    s->extrusion_mult = 1.0f;
    s->filament_dia = 1.75f;
    s->tools = 0;
    s->random_color = 0;
    s->color_change_layer = -1;
    s->color_change_cmd = "M600";
    s->farm_mode = 0;
    s->auto_eject = 0;
    s->eject_angle = 90;
    s->eject_release_temp = 40;
    s->eject_gcode = NULL;
    s->forced_tool = -1;
    s->seam_position = 0;
    s->seam_angle = 0;
    s->scarf_joint = 0;
    s->scarf_ratio = 0.1f;
    s->polyhole = 0;
    s->fuzzy_skin = 0;
    s->fuzzy_points_mm = 2.0f;
    s->fuzzy_mm = 0.3f;
    s->sandwich_mode = 0;
    s->zaa = 0;
    s->wall_speed_mult = 1.0f;
    s->infill_speed_mult = 1.0f;
    s->solid_speed_mult = 1.0f;
    s->wall_temp = 0;
    s->infill_temp = 0;
    s->wall_layer_height = 0;
    s->printer_name = "Volt360 (Orca-style)";
}

// ================= triangle-plane intersection =================
static int tri_segment(float *pa, float *pb, float *pc, float z, V2 *out) {
    float *v[3] = { pa, pb, pc };
    float d[3];
    for (int i = 0; i < 3; i++) d[i] = v[i][2] - z;
    int above = 0, below = 0;
    for (int i = 0; i < 3; i++) { if (d[i] > SL_EPS) above++; else if (d[i] < -SL_EPS) below++; }
    if (above == 3 || below == 3) return 0;
    for (int i = 0; i < 3; i++) if (fabs(d[i]) <= SL_EPS) d[i] = SL_EPS;
    int n = 0;
    for (int i = 0; i < 3; i++) {
        int j = (i + 1) % 3;
        if ((d[i] > 0) != (d[j] > 0)) {
            float t = d[i] / (d[i] - d[j]);
            out[n++] = v2(v[i][0] + t * (v[j][0] - v[i][0]),
                          v[i][1] + t * (v[j][1] - v[i][1]));
        }
    }
    if (n == 2 && v2_dist(out[0], out[1]) < SL_EPS) n = 1;
    return n;
}

// ================= segment joining (hash map) =================
typedef struct { V2 a, b; int used; } Seg;

typedef struct { int64_t kx, ky; int seg; int next; } Node;

typedef struct {
    Node *nodes;
    int count, cap;
    int *head;
    int nbuckets;
} Hash;

static uint32_t hash2(int64_t kx, int64_t ky) {
    uint64_t h = (uint64_t)kx * 73856093ull ^ (uint64_t)ky * 19349663ull;
    h ^= h >> 33; h *= 0xff51afd7ed558ccdull; h ^= h >> 33;
    return (uint32_t)h;
}

static void hash_init(Hash *h, int approx) {
    h->cap = approx * 2 + 16;
    h->count = 0;
    h->nodes = (Node *)malloc(sizeof(Node) * (size_t)h->cap);
    h->nbuckets = approx + 16;
    h->head = (int *)malloc(sizeof(int) * (size_t)h->nbuckets);
    for (int i = 0; i < h->nbuckets; i++) h->head[i] = -1;
}

static void hash_free(Hash *h) { free(h->nodes); free(h->head); memset(h, 0, sizeof(*h)); }

static void hash_put(Hash *h, int64_t kx, int64_t ky, int seg) {
    if (h->count >= h->cap) { h->cap *= 2; h->nodes = (Node *)realloc(h->nodes, sizeof(Node) * (size_t)h->cap); }
    int idx = h->count++;
    h->nodes[idx].kx = kx; h->nodes[idx].ky = ky;
    h->nodes[idx].seg = seg;
    uint32_t b = hash2(kx, ky) % (uint32_t)h->nbuckets;
    h->nodes[idx].next = h->head[b];
    h->head[b] = idx;
}

static int hash_get(Hash *h, int64_t kx, int64_t ky, int *segs, int max) {
    uint32_t b = hash2(kx, ky) % (uint32_t)h->nbuckets;
    int n = 0;
    for (int i = h->head[b]; i >= 0; i = h->nodes[i].next)
        if (h->nodes[i].kx == kx && h->nodes[i].ky == ky)
            if (n < max) { segs[n++] = h->nodes[i].seg; }
    return n;
}

static float poly_area2(V2 *p, int n) {
    float a = 0;
    for (int i = 0; i < n; i++) { V2 x = p[i], y = p[(i + 1) % n]; a += x.x * y.y - x.y * y.x; }
    return 0.5f * a;
}

// Join segments into closed loops. Fills *out_flat (V2 array, caller frees),
// *out_offset (int array of nloop+1 starts) and returns nloop. Each loop's
// vertices are flat[offset[i] .. offset[i+1]). Duplicate closing vertex omitted.
static int join_loops(Seg *segs, int nseg, V2 **out_flat, int **out_offset, int *out_nloop) {
    *out_flat = NULL; *out_offset = NULL; *out_nloop = 0;
    if (nseg <= 0) return 0;
    Hash h; hash_init(&h, nseg);
    for (int i = 0; i < nseg; i++) {
        hash_put(&h, llround(segs[i].a.x * 100), llround(segs[i].a.y * 100), i);
        hash_put(&h, llround(segs[i].b.x * 100), llround(segs[i].b.y * 100), i);
    }
    V2 *flat = (V2 *)malloc(sizeof(V2) * (size_t)(nseg * 4 + 8));
    int *off = (int *)malloc(sizeof(int) * (size_t)(nseg + 2));
    int nloop = 0, npts = 0;
    int start_seg = 0;
    while (start_seg < nseg) {
        while (start_seg < nseg && segs[start_seg].used) start_seg++;
        if (start_seg >= nseg) break;
        off[nloop] = npts;
        int cur = start_seg;
        segs[cur].used = 1;
        flat[npts++] = segs[cur].a;
        V2 p = segs[cur].b;
        int guard = 0;
        while (guard++ < nseg * 2 + 4) {
            flat[npts++] = p;
            if (v2_dist(p, flat[off[nloop]]) < 1e-4f) break; // closed
            int cands[16];
            int m = hash_get(&h, llround(p.x * 100), llround(p.y * 100), cands, 16);
            int nxt = -1;
            for (int k = 0; k < m; k++) if (!segs[cands[k]].used) { nxt = cands[k]; break; }
            if (nxt < 0) break; // open chain
            segs[nxt].used = 1;
            V2 other = (segs[nxt].a.x == p.x && segs[nxt].a.y == p.y) ? segs[nxt].b : segs[nxt].a;
            p = other;
        }
        int len = npts - off[nloop];
        while (len > 1 && v2_dist(flat[off[nloop] + len - 1], flat[off[nloop]]) < 1e-4f) len--;
        if (len >= 3 && fabs(poly_area2(flat + off[nloop], len)) > 1e-3f) {
            npts = off[nloop] + len;
            nloop++;
        } else {
            npts = off[nloop]; // discard degenerate loop
        }
    }
    off[nloop] = npts; // end index of the last loop (and total point count)
    hash_free(&h);
    *out_flat = flat;
    *out_offset = off;
    *out_nloop = nloop;
    if (nloop == 0) { free(flat); free(off); *out_offset = NULL; *out_flat = NULL; }
    return nloop;
}

// ================= polygon offset (miter inset) =================
static V2 perp_v(V2 e) { return v2(-e.y, e.x); }
static V2 norm_v(V2 a) { float l = v2_len(a); return l > 1e-9f ? v2(a.x / l, a.y / l) : v2(0, 0); }

// Core offset: dir = +1 insets toward the loop interior (material loops),
// dir = -1 outsets away from the interior (hole loops).
static bool poly_offset_impl(V2 *poly, int n, float d, float dir, V2 *out, int *out_n, int maxn) {
    *out_n = 0;
    if (n < 3 || maxn < 3 || d <= 0) return false;
    float area = poly_area2(poly, n);
    if (fabs(area) < 1e-6f) return false;
    float s = (area > 0 ? 1.0f : -1.0f) * dir;
    int cnt = 0;
    for (int i = 0; i < n && cnt < maxn; i++) {
        V2 a = poly[(i + n - 1) % n];
        V2 b = poly[i];
        V2 c = poly[(i + 1) % n];
        V2 e1 = norm_v(v2_sub(b, a));
        V2 e2 = norm_v(v2_sub(c, b));
        if (v2_len(e1) < 1e-6f || v2_len(e2) < 1e-6f) continue;
        V2 n1 = v2_scale(perp_v(e1), s);
        V2 n2 = v2_scale(perp_v(e2), s);
        V2 bis = norm_v(v2_add(n1, n2));
        float den = v2_dot(bis, n1);
        if (fabs(den) < 1e-6f) continue;
        float t = d / den;
        if (t < 0 || !(t < d * 20.0f)) continue; // collapsed or extreme corner
        out[cnt++] = v2_add(b, v2_scale(bis, t));
    }
    if (cnt >= 3) {
        float oa = poly_area2(out, cnt);
        if (oa * area > 0 && fabs(oa) > 1e-4f) { *out_n = cnt; return true; }
    }
    *out_n = 0;
    return false;
}

bool poly_offset(V2 *poly, int n, float d, V2 *out, int *out_n, int maxn) {
    return poly_offset_impl(poly, n, d, 1.0f, out, out_n, maxn);
}

static bool poly_offset_out(V2 *poly, int n, float d, V2 *out, int *out_n, int maxn) {
    return poly_offset_impl(poly, n, d, -1.0f, out, out_n, maxn);
}

// ================= seam / scarf / fuzzy / helpers =================
static int seam_start(V2 *pts, int n, const SliceSettings *cfg) {
    if (n < 2) return 0;
    if (cfg->seam_position == 2) {
        float want = cfg->seam_angle * (float)(M_PI / 180.0);
        int best = 0; float bd = 1e30f;
        for (int i = 0; i < n; i++) {
            float a = atan2f(pts[i].y, pts[i].x);
            float dd = fabsf(a - want);
            if (dd > 3.14159f) dd = 6.28318f - dd;
            if (dd < bd) { bd = dd; best = i; }
        }
        return best;
    }
    if (cfg->seam_position == 1) { // back = max +Y
        int best = 0;
        for (int i = 1; i < n; i++) if (pts[i].y > pts[best].y) best = i;
        return best;
    }
    int best = 0; float bd = 1e30f; // nearest to origin
    for (int i = 0; i < n; i++) { float dd = v2_len(pts[i]); if (dd < bd) { bd = dd; best = i; } }
    return best;
}

static bool point_in_poly(V2 pt, V2 *poly, int n) {
    bool in = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((poly[i].y > pt.y) != (poly[j].y > pt.y)) &&
            (pt.x < (poly[j].x - poly[i].x) * (pt.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x))
            in = !in;
    }
    return in;
}

static void add_fuzzy_jitter(V2 *pts, int n, float wavelength, float amp) {
    if (n < 2 || amp <= 0) return;
    float run = 0;
    for (int i = 1; i < n; i++) {
        V2 d = v2_sub(pts[i], pts[i - 1]);
        float L = v2_len(d);
        run += L;
        if (run >= wavelength && L > 1e-4f) {
            V2 perp = v2_scale(perp_v(d), ((rand() % 1000) / 500.0f - 1.0f) * amp);
            pts[i].x += perp.x;
            pts[i].y += perp.y;
            run = 0;
        }
    }
}

// reorder layer paths so infill sits right after the outer perimeter (sandwich)
static void layer_sandwich_order(SliceLayer *L) {
    if (L->npaths < 3) return;
    SlicePath *np = (SlicePath *)malloc(sizeof(SlicePath) * (size_t)L->npaths);
    int npn = 0;
    // first perimeter
    int first = -1;
    for (int i = 0; i < L->npaths; i++) if (L->paths[i].kind == SLICE_PATH_PERIMETER) { first = i; break; }
    if (first < 0) { free(np); return; }
    np[npn++] = L->paths[first];
    for (int i = 0; i < L->npaths; i++) if (L->paths[i].kind == SLICE_PATH_INFILL) np[npn++] = L->paths[i];
    for (int i = 0; i < L->npaths; i++)
        if (L->paths[i].kind == SLICE_PATH_PERIMETER && i != first) np[npn++] = L->paths[i];
    free(L->paths);
    L->paths = np;
}

// ================= infill (parallel lines clipped to polygon) =================
typedef struct { V2 a, b; } Seg2;

static int infill_segments(V2 *poly, int n, float angle, float spacing, Seg2 *out, int maxseg) {
    if (n < 3 || spacing <= 0) return 0;
    int cap_n = n > 256 ? 256 : n;
    float rad = angle * (float)(M_PI / 180.0);
    float c = cosf(rad), s = sinf(rad);
    V2 *r = (V2 *)malloc(sizeof(V2) * (size_t)cap_n);
    float ymin = 1e30f, ymax = -1e30f;
    for (int i = 0; i < cap_n; i++) {
        r[i].x = poly[i].x * c + poly[i].y * s;   // rotate by -angle
        r[i].y = -poly[i].x * s + poly[i].y * c;
        if (r[i].y < ymin) ymin = r[i].y;
        if (r[i].y > ymax) ymax = r[i].y;
    }
    int cnt = 0;
    volatile int y_iter = 0;
    for (float y = ymin + spacing * 0.5f; y < ymax && cnt < maxseg && (++y_iter < 10000); y += spacing) {
        float xs[256];
        int nx = 0;
        volatile int nx_guard = 0;
        for (int i = 0; i < cap_n; i++) {
            int j = (i + 1) % cap_n;
            float y0 = r[i].y, y1 = r[j].y;
            if ((y0 <= y && y1 > y) || (y1 <= y && y0 > y)) {
                if (nx < 256) xs[nx++] = r[i].x + (y - y0) / (y1 - y0) * (r[j].x - r[i].x);
            }
        }
        nx_guard = nx;
        if (nx_guard > 256) nx_guard = 256;
        for (int k = 0; k < nx_guard; k++)
            for (int j2 = k + 1; j2 < nx_guard; j2++)
                if (xs[j2] < xs[k]) { float t = xs[j2]; xs[j2] = xs[k]; xs[k] = t; }
        for (int k = 0; k + 1 < nx_guard; k += 2) {
            if (xs[k + 1] - xs[k] < spacing * 0.25f) continue;
            out[cnt].a = v2(xs[k] * c - y * s, xs[k] * s + y * c);
            out[cnt].b = v2(xs[k + 1] * c - y * s, xs[k + 1] * s + y * c);
            cnt++;
            if (cnt >= maxseg) break;
        }
    }
    free(r);
    return cnt;
}

// ================= layer assembly =================
static void layer_add_path(SliceLayer *L, int kind, V2 *pts, int npts, float width, int solid) {
    if (npts < 1) return;
    int ni = L->npaths++;
    L->paths = (SlicePath *)realloc(L->paths, sizeof(SlicePath) * (size_t)L->npaths);
    SlicePath *p = &L->paths[ni];
    p->kind = kind; p->npts = npts; p->width = width; p->solid = solid;
    p->pts = (V2 *)malloc(sizeof(V2) * (size_t)npts);
    memcpy(p->pts, pts, sizeof(V2) * (size_t)npts);
}

void slice_result_free(SliceResult *r) {
    if (!r) return;
    for (int i = 0; i < r->nlayers; i++) {
        SliceLayer *L = &r->layers[i];
        for (int j = 0; j < L->npaths; j++) free(L->paths[j].pts);
        free(L->paths);
    }
    free(r->layers);
    memset(r, 0, sizeof(*r));
}

// ================= main slicing =================
bool slice_mesh(CadMesh *mesh, const SliceSettings *cfg, SliceResult *out) {
    memset(out, 0, sizeof(*out));
    if (!mesh || !cfg || mesh->ntris <= 0 || cfg->layer_height <= 0.001f) return false;
    mesh_bbox(mesh);
    if (!mesh->valid) return false;
    float zmin = mesh->bmin.z, zmax = mesh->bmax.z;
    if (zmax - zmin < 1e-4f) return false;

    int nlayers = (int)ceilf((zmax - zmin) / cfg->layer_height) + 1;
    if (nlayers < 1) nlayers = 1;
    SliceLayer *layers = (SliceLayer *)calloc((size_t)nlayers, sizeof(SliceLayer));

    double filament = 0, dist = 0;
    int total_paths = 0;
    float cross = (float)(M_PI * (cfg->filament_dia / 2.0) * (cfg->filament_dia / 2.0));

    for (int li = 0; li < nlayers; li++) {
        float zabs = zmin + (float)li * cfg->layer_height;
        SliceLayer *L = &layers[li];
        L->z = (float)li * cfg->layer_height;

        // gather intersection segments
        int maxsegs = mesh->ntris * 2 + 4;
        Seg *segs = (Seg *)malloc(sizeof(Seg) * (size_t)maxsegs);
        memset(segs, 0, sizeof(Seg) * (size_t)maxsegs);
        int nseg = 0;
        for (int t = 0; t < mesh->ntris; t++) {
            float *pa = &mesh->pos[mesh->idx[t * 3 + 0] * 3];
            float *pb = &mesh->pos[mesh->idx[t * 3 + 1] * 3];
            float *pc = &mesh->pos[mesh->idx[t * 3 + 2] * 3];
            if (zabs < pa[2] - 0.01f && zabs < pb[2] - 0.01f && zabs < pc[2] - 0.01f) continue;
            if (zabs > pa[2] + 0.01f && zabs > pb[2] + 0.01f && zabs > pc[2] + 0.01f) continue;
            V2 tmp[2];
            int k = tri_segment(pa, pb, pc, zabs, tmp);
            if (k == 2 && nseg < maxsegs) {
                segs[nseg].a = tmp[0];
                segs[nseg].b = tmp[1];
                nseg++;
            }
        }

        V2 *flat = NULL; int *off = NULL; int nloop = 0;
        join_loops(segs, nseg, &flat, &off, &nloop);
        free(segs);
        if (nloop <= 0) { free(flat); free(off); continue; }

        // pre-pass: find the outer loop (largest |area|) and collect holes
        int outer_i = 0; float outer_area = 0;
        V2 *holes[64]; int hole_n[64]; int nholes = 0;
        for (int li2 = 0; li2 < nloop; li2++) {
            V2 *pp = flat + off[li2];
            int nn = off[li2 + 1] - off[li2];
            if (nn < 3) continue;
            float aa = poly_area2(pp, nn);
            if (fabs(aa) > fabs(outer_area)) { outer_area = aa; outer_i = li2; }
        }
        for (int li2 = 0; li2 < nloop && nholes < 64; li2++) {
            V2 *pp = flat + off[li2];
            int nn = off[li2 + 1] - off[li2];
            if (nn < 3 || li2 == outer_i) continue;
            float aa = poly_area2(pp, nn);
            if (aa * outer_area < 0) { holes[nholes] = pp; hole_n[nholes] = nn; nholes++; }
        }

        for (int li2 = 0; li2 < nloop; li2++) {
            int nstart = off[li2];
            int nend = off[li2 + 1];
            V2 *poly = flat + nstart;
            int n = nend - nstart;
            if (n < 3) continue;
            float a = poly_area2(poly, n);
            int is_hole = (li2 != outer_i) && (a * outer_area < 0) && fabs(a) > 1e-6f;
            int is_outer = (li2 == outer_i);

            if (!is_hole) {
                // ---- material loop: contour + miter insets ----
                if (is_outer && (cfg->seam_position > 0 || cfg->scarf_joint || cfg->fuzzy_skin)) {
                    V2 *rot = (V2 *)malloc(sizeof(V2) * (size_t)n);
                    int s0 = seam_start(poly, n, cfg);
                    for (int i = 0; i < n; i++) rot[i] = poly[(s0 + i) % n];
                    if (cfg->scarf_joint && n >= 4) {
                        int k = (int)(cfg->scarf_ratio * (float)n);
                        if (k < 1) k = 1;
                        if (k > n / 2) k = n / 2;
                        V2 *sc = (V2 *)malloc(sizeof(V2) * (size_t)(n + k));
                        memcpy(sc, rot, sizeof(V2) * (size_t)n);
                        memcpy(sc + n, rot, sizeof(V2) * (size_t)k);
                        layer_add_path(L, SLICE_PATH_PERIMETER, sc, n + k, cfg->line_width, 1);
                        free(sc);
                    } else {
                        layer_add_path(L, SLICE_PATH_PERIMETER, rot, n, cfg->line_width, 1);
                    }
                    if (cfg->fuzzy_skin) {
                        SlicePath *lastp = &L->paths[L->npaths - 1];
                        add_fuzzy_jitter(lastp->pts, lastp->npts, cfg->fuzzy_points_mm, cfg->fuzzy_mm);
                    }
                    free(rot);
                } else {
                    layer_add_path(L, SLICE_PATH_PERIMETER, poly, n, cfg->line_width, 1);
                }
                V2 *bufA = (V2 *)malloc(sizeof(V2) * (size_t)(n * 2 + 8));
                V2 *bufB = (V2 *)malloc(sizeof(V2) * (size_t)(n * 2 + 8));
                V2 *src = poly; int src_n = n; V2 *dst = bufA; int dst_n = 0;
                for (int p = 1; p < cfg->perimeters; p++) {
                    int cap = src_n * 2 + 8;
                    if (!poly_offset(src, src_n, cfg->line_width, dst, &dst_n, cap) || dst_n < 3) break;
                    layer_add_path(L, SLICE_PATH_PERIMETER, dst, dst_n, cfg->line_width, 1);
                    V2 *sw = src; src = dst; dst = sw; src_n = dst_n;
                }
                free(bufA); free(bufB);
            } else {
                // ---- hole loop: contour + outward (into material) perimeters ----
                V2 *cur = poly; int cur_n = n; bool is_cur_owned = false;
                if (cfg->polyhole > 0) {
                    V2 *ph = (V2 *)malloc(sizeof(V2) * (size_t)(n * 2 + 8));
                    int phn = 0;
                    if (poly_offset_out(poly, n, cfg->line_width * cfg->polyhole, ph, &phn, n * 2 + 8) && phn >= 3) {
                        cur = ph; cur_n = phn; is_cur_owned = true;
                    } else free(ph);
                }
                layer_add_path(L, SLICE_PATH_PERIMETER, cur, cur_n, cfg->line_width, 1);
                V2 *bufA = (V2 *)malloc(sizeof(V2) * (size_t)(cur_n * 2 + 8));
                V2 *bufB = (V2 *)malloc(sizeof(V2) * (size_t)(cur_n * 2 + 8));
                V2 *src = cur; int src_n = cur_n; V2 *dst = bufA; int dst_n = 0;
                for (int p = 1; p < cfg->perimeters; p++) {
                    int cap = src_n * 2 + 8;
                    if (!poly_offset_out(src, src_n, cfg->line_width, dst, &dst_n, cap) || dst_n < 3) break;
                    layer_add_path(L, SLICE_PATH_PERIMETER, dst, dst_n, cfg->line_width, 1);
                    V2 *sw = src; src = dst; dst = sw; src_n = dst_n;
                }
                free(bufA); free(bufB);
                if (is_cur_owned) free(cur);
            }

            // ---- infill only inside the outermost material loop ----
            if (is_outer) {
                int idx_from_top = nlayers - 1 - li;
                int solid = (cfg->infill_density >= 100) || (li < cfg->solid_bottom_layers) || (idx_from_top < cfg->solid_top_layers);
                float spacing = solid ? cfg->line_width : cfg->line_width * 100.0f / (float)(cfg->infill_density > 0 ? cfg->infill_density : 1);
                V2 *clip = poly; int clip_n = n; V2 zaa_buf[64]; int zaa_n = 0;
                if (solid && cfg->zaa) {
                    spacing *= 0.92f;
                    if (n <= 62 && poly_offset(poly, n, 0.04f, zaa_buf, &zaa_n, 64) && zaa_n >= 3) { clip = zaa_buf; clip_n = zaa_n; }
                }
                float dy = 0; for (int i = 0; i < n; i++) { float yy = poly[i].y; if (yy > dy) dy = yy; }
                int maxseg = (int)((dy + spacing) / (spacing > 0 ? spacing : 0.2f)) + n * 2 + 64;
                if (maxseg < 64) maxseg = 64;
                Seg2 *inf = (Seg2 *)malloc(sizeof(Seg2) * (size_t)maxseg);
                int ni = infill_segments(clip, clip_n, cfg->infill_angle, spacing, inf, maxseg);
                // pattern-specific additional passes
                if (cfg->infill_pattern == 1) {
                    float a2 = cfg->infill_angle + 90.0f;
                    if (a2 >= 360.0f) a2 -= 360.0f;
                    Seg2 *inf2 = (Seg2 *)malloc(sizeof(Seg2) * (size_t)maxseg);
                    int ni2 = infill_segments(clip, clip_n, a2, spacing, inf2, maxseg);
                    for (int k2 = 0; k2 < ni2; k2++) {
                        if (ni + k2 < maxseg) inf[ni + k2] = inf2[k2];
                    }
                    ni += ni2;
                    free(inf2);
                } else if (cfg->infill_pattern == 2) {
                    // honeycomb: 3 angles
                    for (int pa = 0; pa < 3; pa++) {
                        float ha = cfg->infill_angle + pa * 60.0f;
                        if (ha >= 360.0f) ha -= 360.0f;
                        Seg2 *inf2 = (Seg2 *)malloc(sizeof(Seg2) * (size_t)maxseg);
                        int ni2 = infill_segments(clip, clip_n, ha, spacing * 1.15f, inf2, maxseg);
                        for (int k2 = 0; k2 < ni2; k2++) {
                            if (ni + k2 < maxseg) inf[ni + k2] = inf2[k2];
                        }
                        ni += ni2; free(inf2);
                    }
                } else if (cfg->infill_pattern == 3) {
                    // concentric: offset perimeters inward
                    free(inf); inf = NULL; ni = 0;
                    V2 *ring = clip; int rn = clip_n; bool ring_owned = false;
                    float inset = spacing;
                    for (int ri = 0; ri < 15; ri++) {
                        int cap = rn * 2 + 8;
                        V2 *next = (V2 *)malloc(sizeof(V2) * (size_t)cap);
                        int nnext = 0;
                        if (poly_offset(ring, rn, inset, next, &nnext, cap) && nnext >= 3) {
                            layer_add_path(L, SLICE_PATH_INFILL, next, nnext, cfg->line_width, solid);
                            if (ring_owned) free(ring);
                            ring = next; rn = nnext; ring_owned = true;
                        } else { free(next); break; }
                    }
                    if (ring_owned) free(ring);
                } else if (cfg->infill_pattern == 4) {
                    // gyroid: sine-wave crosshatch
                    free(inf); inf = (Seg2 *)malloc(sizeof(Seg2) * (size_t)maxseg);
                    ni = infill_gyroid(clip, clip_n, spacing, cfg->infill_angle, (V2 *)inf, maxseg);
                    ni /= 2; // gyroid returns point pairs
                } else if (cfg->infill_pattern == 5) {
                    // lightning: radial sparse
                    free(inf); inf = (Seg2 *)malloc(sizeof(Seg2) * (size_t)maxseg);
                    ni = infill_lightning(clip, clip_n, cfg->line_width, (V2 *)inf, maxseg);
                    ni /= 2;
                } else if (cfg->infill_pattern == 6) {
                    // cubic: alternating angles
                    free(inf); inf = (Seg2 *)malloc(sizeof(Seg2) * (size_t)maxseg);
                    ni = infill_cubic(clip, clip_n, spacing, li, (V2 *)inf, maxseg);
                    ni /= 2;
                }
                for (int k = 0; k < ni; k++) {
                    V2 mid = v2((inf[k].a.x + inf[k].b.x) * 0.5f, (inf[k].a.y + inf[k].b.y) * 0.5f);
                    bool in_hole = false;
                    for (int h = 0; h < nholes && !in_hole; h++)
                        if (point_in_poly(mid, holes[h], hole_n[h])) in_hole = true;
                    if (in_hole) continue;
                    V2 pp[2] = { inf[k].a, inf[k].b };
                    layer_add_path(L, SLICE_PATH_INFILL, pp, 2, cfg->line_width, solid);
                }
                free(inf);
            }
        }
        if (cfg->sandwich_mode) layer_sandwich_order(L);

        // accumulate stats
        for (int j = 0; j < L->npaths; j++) {
            SlicePath *p = &L->paths[j];
            double pl = 0;
            for (int k = 1; k < p->npts; k++) pl += v2_dist(p->pts[k - 1], p->pts[k]);
            if (p->npts == 2) pl = v2_dist(p->pts[0], p->pts[1]);
            dist += pl;
            filament += pl * p->width * cfg->layer_height * cfg->extrusion_mult;
        }
        total_paths += L->npaths;
        free(off);
        free(flat);
    }

    out->layers = layers;
    out->nlayers = nlayers;
    out->total_paths = total_paths;
    out->total_dist = (float)dist;
    out->z_max = (nlayers - 1) * cfg->layer_height;
    out->filament_mm = (cross > 1e-6f) ? (float)(filament / cross) : 0;
    out->est_time_s = (cfg->print_speed > 0) ? (float)(dist / cfg->print_speed) + total_paths * 0.1f + nlayers * 0.5f : 0;
    return true;
}

// ================= slice settings persistence =================
bool slice_settings_save(const SliceSettings *cfg, const char *path) {
    if (!cfg || !path) return false;
    JsonW w; jw_init(&w);
    jw_begin(&w);
    jw_key(&w, "layer_height");      jw_num(&w, cfg->layer_height);
    jw_key(&w, "line_width");        jw_num(&w, cfg->line_width);
    jw_key(&w, "nozzle_diameter");   jw_num(&w, cfg->nozzle_diameter);
    jw_key(&w, "perimeters");        jw_int(&w, cfg->perimeters);
    jw_key(&w, "solid_top_layers");  jw_int(&w, cfg->solid_top_layers);
    jw_key(&w, "solid_bottom_layers"); jw_int(&w, cfg->solid_bottom_layers);
    jw_key(&w, "infill_density");    jw_int(&w, cfg->infill_density);
    jw_key(&w, "infill_angle");      jw_num(&w, cfg->infill_angle);
    jw_key(&w, "infill_pattern");    jw_int(&w, cfg->infill_pattern);
    jw_key(&w, "ironing");           jw_bool(&w, cfg->ironing);
    jw_key(&w, "bed_temp");          jw_int(&w, cfg->bed_temp);
    jw_key(&w, "hotend_temp");       jw_int(&w, cfg->hotend_temp);
    jw_key(&w, "print_speed");       jw_num(&w, cfg->print_speed);
    jw_key(&w, "travel_speed");      jw_num(&w, cfg->travel_speed);
    jw_key(&w, "retract_mm");        jw_num(&w, cfg->retract_mm);
    jw_key(&w, "extrusion_mult");    jw_num(&w, cfg->extrusion_mult);
    jw_key(&w, "filament_dia");      jw_num(&w, cfg->filament_dia);
    jw_key(&w, "tools");             jw_int(&w, cfg->tools);
    jw_key(&w, "random_color");      jw_int(&w, cfg->random_color);
    jw_key(&w, "color_change_layer"); jw_int(&w, cfg->color_change_layer);
    jw_end(&w);
    bool ok = file_write(path, jw_cstr(&w), strlen(jw_cstr(&w)));
    jw_free(&w);
    return ok;
}

static int jg_int(JsonValue *root, const char *key, int def) {
    if (!json_has(root, key)) return def;
    const char *s = json_get_str(root, key);
    return s ? atoi(s) : def;
}
static float jg_float(JsonValue *root, const char *key, float def) {
    if (!json_has(root, key)) return def;
    return (float)json_get_num(root, key);
}

bool slice_settings_load(SliceSettings *cfg, const char *path) {
    if (!cfg || !path) return false;
    size_t len = 0;
    char *data = file_read(path, &len);
    if (!data) return false;
    JsonValue *root = json_parse(data, len);
    free(data);
    if (!root || root->type != J_OBJ) { json_free(root); return false; }
    cfg->layer_height       = jg_float(root, "layer_height", 0.2f);
    cfg->line_width         = jg_float(root, "line_width", 0.48f);
    cfg->nozzle_diameter    = jg_float(root, "nozzle_diameter", 0.4f);
    cfg->perimeters         = jg_int(root, "perimeters", 2);
    cfg->solid_top_layers   = jg_int(root, "solid_top_layers", 3);
    cfg->solid_bottom_layers = jg_int(root, "solid_bottom_layers", 3);
    cfg->infill_density     = jg_int(root, "infill_density", 15);
    cfg->infill_angle       = jg_float(root, "infill_angle", 45.0f);
    cfg->infill_pattern     = jg_int(root, "infill_pattern", 1);
    cfg->ironing            = jg_int(root, "ironing", 0);
    cfg->bed_temp           = jg_int(root, "bed_temp", 60);
    cfg->hotend_temp        = jg_int(root, "hotend_temp", 210);
    cfg->print_speed        = jg_float(root, "print_speed", 60.0f);
    cfg->travel_speed       = jg_float(root, "travel_speed", 150.0f);
    cfg->retract_mm         = jg_float(root, "retract_mm", 0.8f);
    cfg->extrusion_mult     = jg_float(root, "extrusion_mult", 1.0f);
    cfg->filament_dia       = jg_float(root, "filament_dia", 1.75f);
    cfg->tools              = jg_int(root, "tools", 0);
    cfg->random_color       = jg_int(root, "random_color", 0);
    cfg->color_change_layer = jg_int(root, "color_change_layer", -1);
    json_free(root);
    return true;
}
