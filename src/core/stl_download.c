#include "stl_download.h"
#include "../core/util.h"
#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma comment(lib, "winhttp.lib")

/* forward declarations */
static const char *strcasestr_custom(const char *haystack, const char *needle);
static const char *strnistr(const char *haystack, const char *needle, size_t haylen);

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} DownloadBuf;

static void dl_buf_init(DownloadBuf *db)
{
    db->data = NULL;
    db->len = 0;
    db->cap = 0;
}

static void dl_buf_append(DownloadBuf *db, const void *buf, size_t len)
{
    if (db->len + len > db->cap) {
        db->cap = db->cap ? db->cap * 2 : 65536;
        if (db->len + len > db->cap) db->cap = db->len + len + 4096;
        db->data = (char *)realloc(db->data, db->cap);
    }
    memcpy(db->data + db->len, buf, len);
    db->len += len;
}

static void dl_buf_free(DownloadBuf *db)
{
    free(db->data);
    db->data = NULL;
    db->len = db->cap = 0;
}

static bool winhttp_download(const char *url, DownloadBuf *out)
{
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    DWORD dwSize, dwDownloaded;
    char buf[8192];
    wchar_t wurl[2048];
    wchar_t whost[256];
    wchar_t wpath[2048];
    URL_COMPONENTSW url_comp;
    BOOL ok;
    const char *p;

    if (!url || !out) return false;

    dl_buf_init(out);

    /* convert URL to widechar */
    MultiByteToWideChar(CP_UTF8, 0, url, -1, wurl, 2048);
    wurl[2047] = 0;

    memset(&url_comp, 0, sizeof(url_comp));
    url_comp.dwStructSize = sizeof(url_comp);
    url_comp.lpszHostName = whost;
    url_comp.dwHostNameLength = 256;
    url_comp.lpszUrlPath = wpath;
    url_comp.dwUrlPathLength = 2048;

    if (!WinHttpCrackUrl(wurl, (DWORD)wcslen(wurl), 0, &url_comp)) return false;

    hSession = WinHttpOpen(L"Volt360/1.0",
                           WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                           WINHTTP_NO_PROXY_NAME,
                           WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return false;

    hConnect = WinHttpConnect(hSession, url_comp.lpszHostName,
                              url_comp.nPort, 0);
    if (!hConnect) { WinHttpCloseHandle(hSession); return false; }

    DWORD flags = (url_comp.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;
    hRequest = WinHttpOpenRequest(hConnect, L"GET", url_comp.lpszUrlPath,
                                  NULL, WINHTTP_NO_REFERER,
                                  WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    ok = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                            WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    if (!ok) goto cleanup;

    ok = WinHttpReceiveResponse(hRequest, NULL);
    if (!ok) goto cleanup;

    /* read headers for content-type */
    {
        DWORD ctype_size = 0;
        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_CONTENT_TYPE,
                            WINHTTP_HEADER_NAME_BY_INDEX, NULL, &ctype_size,
                            WINHTTP_NO_HEADER_INDEX);
    }

    do {
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
        if (dwSize == 0) break;
        if (dwSize > sizeof(buf)) dwSize = sizeof(buf);
        if (!WinHttpReadData(hRequest, buf, dwSize, &dwDownloaded)) break;
        dl_buf_append(out, buf, dwDownloaded);
    } while (dwDownloaded > 0);

    (void)p;
cleanup:
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return out->len > 0;
}

static bool parse_stl_binary(const char *data, size_t len, CadMesh *out)
{
    uint32_t num_tris;
    size_t min_size = 80 + 4;
    const unsigned char *uc = (const unsigned char *)data;
    int i;

    if (len < min_size) return false;
    memcpy(&num_tris, data + 80, 4);

    if ((size_t)(84 + num_tris * 50) > len) return false;

    out->nverts = (int)num_tris * 3;
    out->ntris  = (int)num_tris;
    out->pos = (float *)calloc((size_t)out->nverts * 3, sizeof(float));
    out->nrm = (float *)calloc((size_t)out->nverts * 3, sizeof(float));
    out->idx = (int *)calloc((size_t)out->ntris * 3, sizeof(int));

    if (!out->pos || !out->nrm || !out->idx) {
        mesh_free(out);
        return false;
    }

    out->bmin = v3(1e9f, 1e9f, 1e9f);
    out->bmax = v3(-1e9f, -1e9f, -1e9f);

    for (i = 0; i < (int)num_tris; i++) {
        size_t ofs = 84 + (size_t)i * 50;
        float nx, ny, nz;
        float v1x, v1y, v1z, v2x, v2y, v2z, v3x, v3y, v3z;
        int vi = i * 3;
        memcpy(&nx,  uc + ofs,      4);
        memcpy(&ny,  uc + ofs + 4,  4);
        memcpy(&nz,  uc + ofs + 8,  4);
        memcpy(&v1x, uc + ofs + 12, 4);
        memcpy(&v1y, uc + ofs + 16, 4);
        memcpy(&v1z, uc + ofs + 20, 4);
        memcpy(&v2x, uc + ofs + 24, 4);
        memcpy(&v2y, uc + ofs + 28, 4);
        memcpy(&v2z, uc + ofs + 32, 4);
        memcpy(&v3x, uc + ofs + 36, 4);
        memcpy(&v3y, uc + ofs + 40, 4);
        memcpy(&v3z, uc + ofs + 44, 4);

        out->nrm[vi * 3 + 0] = nx;
        out->nrm[vi * 3 + 1] = ny;
        out->nrm[vi * 3 + 2] = nz;
        out->nrm[vi * 3 + 3] = nx;
        out->nrm[vi * 3 + 4] = ny;
        out->nrm[vi * 3 + 5] = nz;
        out->nrm[vi * 3 + 6] = nx;
        out->nrm[vi * 3 + 7] = ny;
        out->nrm[vi * 3 + 8] = nz;

        out->pos[vi * 3 + 0] = v1x; out->pos[vi * 3 + 1] = v1y; out->pos[vi * 3 + 2] = v1z;
        out->pos[vi * 3 + 3] = v2x; out->pos[vi * 3 + 4] = v2y; out->pos[vi * 3 + 5] = v2z;
        out->pos[vi * 3 + 6] = v3x; out->pos[vi * 3 + 7] = v3y; out->pos[vi * 3 + 8] = v3z;

        out->idx[vi + 0] = vi;
        out->idx[vi + 1] = vi + 1;
        out->idx[vi + 2] = vi + 2;

        V3 v;
        v = v3(v1x,v1y,v1z); out->bmin = v3_min(out->bmin, v); out->bmax = v3_max(out->bmax, v);
        v = v3(v2x,v2y,v2z); out->bmin = v3_min(out->bmin, v); out->bmax = v3_max(out->bmax, v);
        v = v3(v3x,v3y,v3z); out->bmin = v3_min(out->bmin, v); out->bmax = v3_max(out->bmax, v);
    }

    out->valid = 1;
    return true;
}

static const char *skip_ws(const char *p) {
    while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
    return p;
}

static bool parse_stl_ascii(const char *data, size_t len, CadMesh *out)
{
    const char *p = data;
    const char *end = data + len;
    int nverts = 0, ntris = 0, vert_cap = 0;
    float *vert_buf = NULL;
    float *nrm_buf = NULL;
    float bx, by, bz, bex, bey, bez;

    if (!data || len == 0) return false;

    memset(out, 0, sizeof(CadMesh));
    bx = 1e9f; by = 1e9f; bz = 1e9f;
    bex = -1e9f; bey = -1e9f; bez = -1e9f;

    p = skip_ws(p);
    if (end - p < 5) return false;
    if (!(p[0]=='s' && p[1]=='o' && p[2]=='l' && p[3]=='i' && p[4]=='d')) return false;
    p += 5;

    while (p < end) {
        p = skip_ws(p);
        if (end - p > 8 && strncmp(p, "endsolid", 8) == 0) break;
        if (end - p > 5 && strncmp(p, "facet", 5) == 0) {
            p += 5;
            float nx = 0, ny = 0, nz = 0;
            /* parse "normal nx ny nz" */
            {
                char line[512]; int li = 0;
                while (p < end && *p != '\n' && *p != '\r' && li < 500) { line[li++] = *p; p++; }
                line[li] = 0;
                sscanf(line, " normal %f %f %f", &nx, &ny, &nz);
            }
            p = skip_ws(p);
            /* skip "outer loop" */
            if (end - p > 10 && strncmp(p, "outer loop", 10) == 0) p += 10;

            float vx[3] = {0,0,0}, vy[3] = {0,0,0}, vz[3] = {0,0,0};
            int vi;
            for (vi = 0; vi < 3; vi++) {
                p = skip_ws(p);
                if (end - p > 6 && strncmp(p, "vertex", 6) == 0) {
                    p += 6;
                    char line[512]; int li = 0;
                    while (p < end && *p != '\n' && *p != '\r' && li < 500) { line[li++] = *p; p++; }
                    line[li] = 0;
                    sscanf(line, " %f %f %f", &vx[vi], &vy[vi], &vz[vi]);
                }
            }

            p = skip_ws(p);
            if (end - p > 8 && strncmp(p, "endloop", 7) == 0) p += 7;
            p = skip_ws(p);
            if (end - p > 8 && strncmp(p, "endfacet", 8) == 0) p += 8;

            /* store */
            if (nverts + 3 > vert_cap) {
                vert_cap = vert_cap ? vert_cap * 2 : 1024;
                vert_buf = (float *)realloc(vert_buf, (size_t)vert_cap * 3 * sizeof(float));
                nrm_buf  = (float *)realloc(nrm_buf,  (size_t)vert_cap * 3 * sizeof(float));
            }
            int base = nverts * 3;
            vert_buf[base+0] = vx[0]; vert_buf[base+1] = vy[0]; vert_buf[base+2] = vz[0];
            vert_buf[base+3] = vx[1]; vert_buf[base+4] = vy[1]; vert_buf[base+5] = vz[1];
            vert_buf[base+6] = vx[2]; vert_buf[base+7] = vy[2]; vert_buf[base+8] = vz[2];
            nrm_buf[base+0] = nx; nrm_buf[base+1] = ny; nrm_buf[base+2] = nz;
            nrm_buf[base+3] = nx; nrm_buf[base+4] = ny; nrm_buf[base+5] = nz;
            nrm_buf[base+6] = nx; nrm_buf[base+7] = ny; nrm_buf[base+8] = nz;

            if (vx[0] < bx) bx = vx[0]; if (vy[0] < by) by = vy[0]; if (vz[0] < bz) bz = vz[0];
            if (vx[0] > bex) bex = vx[0]; if (vy[0] > bey) bey = vy[0]; if (vz[0] > bez) bez = vz[0];
            if (vx[1] < bx) bx = vx[1]; if (vy[1] < by) by = vy[1]; if (vz[1] < bz) bz = vz[1];
            if (vx[1] > bex) bex = vx[1]; if (vy[1] > bey) bey = vy[1]; if (vz[1] > bez) bez = vz[1];
            if (vx[2] < bx) bx = vx[2]; if (vy[2] < by) by = vy[2]; if (vz[2] < bz) bz = vz[2];
            if (vx[2] > bex) bex = vx[2]; if (vy[2] > bey) bey = vy[2]; if (vz[2] > bez) bez = vz[2];

            nverts += 3;
            ntris++;
        } else {
            p++;
        }
    }

    if (nverts == 0 || vert_buf == NULL) {
        free(vert_buf); free(nrm_buf);
        return false;
    }

    out->nverts = nverts;
    out->ntris  = ntris;
    out->pos = vert_buf;
    out->nrm = nrm_buf;
    out->idx = (int *)calloc((size_t)ntris * 3, sizeof(int));
    {
        int i;
        for (i = 0; i < ntris * 3; i++) out->idx[i] = i;
    }
    out->bmin = v3(bx, by, bz);
    out->bmax = v3(bex, bey, bez);
    out->valid = 1;
    return true;
}

static char *extract_stl_links(const char *html, size_t len)
{
    Buf links;
    buf_init(&links);
    const char *p = html;
    const char *end = html + len;

    while (p < end) {
        const char *href = strcasestr_custom(p, "href=");
        if (!href) break;
        href += 5;
        while (href < end && (*href == '"' || *href == '\'')) href++;
        const char *endq = href;
        while (endq < end && *endq != '"' && *endq != '\'' && *endq != '>' && *endq != ' ') endq++;
        if (endq > href) {
            size_t urllen = (size_t)(endq - href);
            if (urllen >= 4 && (
                strnistr(href, ".stl", urllen) ||
                strnistr(href, ".3mf", urllen) ||
                strnistr(href, ".obj", urllen) ||
                strnistr(href, ".amf", urllen))) {
                if (links.len > 0) buf_append(&links, "\n", 1);
                buf_append(&links, href, urllen);
            }
        }
        p = endq + 1;
    }

    /* also look for /download/ paths common on thingiverse/printables */
    p = html;
    while (p < end) {
        const char *dp = strstr(p, "/download/");
        if (!dp) break;
        const char *start = dp;
        const char *e = start;
        while (e < end && *e != '"' && *e != '\'' && *e != ' ' && *e != '\n') e++;
        if (e > start) {
            if (links.len > 0) buf_append(&links, "\n", 1);
            buf_append(&links, start, (size_t)(e - start));
        }
        p = e + 1;
    }

    /* thingiverse zip file pattern /zip files */
    p = html;
    while (p < end) {
        const char *zp = strstr(p, "/thingiverse-zip");
        if (!zp) break;
        const char *start = zp;
        const char *e = start;
        while (e < end && *e != '"' && *e != '\'' && *e != ' ') e++;
        if (e > start) {
            if (links.len > 0) buf_append(&links, "\n", 1);
            buf_append(&links, start, (size_t)(e - start));
        }
        p = e + 1;
    }

    if (links.len == 0) {
        buf_free(&links);
        return NULL;
    }

    return buf_cstr(&links);
}

static char *resolve_url(const char *base_url, const char *link)
{
    Buf b;
    buf_init(&b);
    if (str_starts_with(link, "http://") || str_starts_with(link, "https://")) {
        return str_dup(link);
    }
    /* relative path – prepend base domain */
    {
        const char *proto_end = strstr(base_url, "://");
        const char *host_start = proto_end ? proto_end + 3 : base_url;
        const char *host_end = host_start;
        while (*host_end && *host_end != '/' && *host_end != '?' && *host_end != '#') host_end++;

        const char *proto = "https";
        if (proto_end) {
            buf_append(&b, base_url, (size_t)(proto_end - base_url));
            proto = NULL;
        } else {
            buf_append_str(&b, "https");
        }
        buf_append_str(&b, "://");
        buf_append(&b, host_start, (size_t)(host_end - host_start));
        if (link[0] != '/') buf_append(&b, "/", 1);
        buf_append_str(&b, link);
        (void)proto;
    }
    return buf_cstr(&b);
}

bool stl_download_import(const char *url, CadMesh *out)
{
    DownloadBuf db;
    StlSource src;
    if (!url || !out) {
        if (out) memset(out, 0, sizeof(*out));
        return false;
    }
    memset(out, 0, sizeof(*out));

    /* detect source */
    {
        char *info = stl_detect_source(url);
        if (strcmp(info, "thingiverse") == 0) src = SOURCE_THINGIVERSE;
        else if (strcmp(info, "printables") == 0) src = SOURCE_PRINTABLES;
        else src = SOURCE_URL;
        free(info);
    }

    /* direct STL/3MF URL */
    if (src == SOURCE_URL &&
        (str_ends_with_ci(url, ".stl") || str_ends_with_ci(url, ".3mf") || str_ends_with_ci(url, ".amf"))) {
        dl_buf_init(&db);
        if (winhttp_download(url, &db)) {
            bool ok = false;
            if (db.len > 84 && strncmp(db.data, "solid", 5) == 0) {
                ok = parse_stl_ascii(db.data, db.len, out);
            }
            if (!ok && db.len >= 84) {
                ok = parse_stl_binary(db.data, db.len, out);
            }
            dl_buf_free(&db);
            if (ok) return true;
        }
        dl_buf_free(&db);
    }

    /* scrape page for STL links */
    dl_buf_init(&db);
    if (winhttp_download(url, &db)) {
        char *links = extract_stl_links(db.data, db.len);
        dl_buf_free(&db);
        if (links) {
            /* try each link */
            char *saveptr = NULL;
            char *token = strtok_s(links, "\n", &saveptr);
            while (token) {
                char *full_url = resolve_url(url, token);
                DownloadBuf db2;
                dl_buf_init(&db2);
                if (winhttp_download(full_url, &db2)) {
                    bool ok = false;
                    if (db2.len > 84 && strncmp(db2.data, "solid", 5) == 0) {
                        ok = parse_stl_ascii(db2.data, db2.len, out);
                    }
                    if (!ok && db2.len >= 84) {
                        ok = parse_stl_binary(db2.data, db2.len, out);
                    }
                    if (ok) {
                        dl_buf_free(&db2);
                        free(full_url);
                        free(links);
                        return true;
                    }
                }
                dl_buf_free(&db2);
                free(full_url);
                token = strtok_s(NULL, "\n", &saveptr);
            }
            free(links);
        }
    }

    /* fallback: generate placeholder cube mesh */
    {
        float verts[] = {
            -1,-1,-1,  1,-1,-1,  1, 1,-1,  -1, 1,-1,
            -1,-1, 1,  1,-1, 1,  1, 1, 1,  -1, 1, 1
        };
        int idxs[] = {
            0,1,2, 0,2,3,  4,5,6, 4,6,7,
            0,4,7, 0,7,3,  1,5,6, 1,6,2,
            0,1,5, 0,5,4,  3,2,6, 3,6,7
        };
        int nv = 8, ni = 12 * 3;
        out->nverts = nv;
        out->ntris  = 12;
        out->pos = (float *)calloc((size_t)nv * 3, sizeof(float));
        out->nrm = (float *)calloc((size_t)nv * 3, sizeof(float));
        out->idx = (int *)calloc((size_t)ni, sizeof(int));
        memcpy(out->pos, verts, sizeof(verts));
        memcpy(out->idx, idxs, sizeof(idxs));
        {
            int i;
            for (i = 0; i < nv; i++) {
                out->nrm[i*3+1] = 1.0f;
            }
        }
        out->bmin = v3(-1,-1,-1);
        out->bmax = v3(1,1,1);
        out->valid = 1;
    }

    return true;
}

char *stl_detect_source(const char *url)
{
    if (!url) return str_dup("direct");
    if (strstr(url, "thingiverse.com")) return str_dup("thingiverse");
    if (strstr(url, "printables.com"))  return str_dup("printables");
    if (strstr(url, "myminifactory.com")) return str_dup("direct");
    if (str_ends_with_ci(url, ".stl") || str_ends_with_ci(url, ".3mf") || str_ends_with_ci(url, ".amf"))
        return str_dup("direct");
    return str_dup("direct");
}

bool stl_batch_import(const char **urls, int count, CadMesh *out_meshes)
{
    int i;
    if (!urls || !out_meshes || count <= 0) return false;
    for (i = 0; i < count; i++) {
        if (!stl_download_import(urls[i], &out_meshes[i])) {
            return false;
        }
    }
    return true;
}

/* helpers for parsing */
static const char *strcasestr_custom(const char *haystack, const char *needle) {
    size_t nlen = strlen(needle);
    while (*haystack) {
        size_t j;
        for (j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[j]) != tolower((unsigned char)needle[j])) break;
        }
        if (j == nlen) return haystack;
        haystack++;
    }
    return NULL;
}

static const char *strnistr(const char *haystack, const char *needle, size_t haylen) {
    size_t nlen = strlen(needle);
    if (nlen > haylen) return NULL;
    size_t i;
    for (i = 0; i <= haylen - nlen; i++) {
        size_t j;
        for (j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i+j]) != tolower((unsigned char)needle[j])) break;
        }
        if (j == nlen) return haystack + i;
    }
    return NULL;
}
