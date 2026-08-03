#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

#include "plugin_market.h"
#include "../core/util.h"
#include "../core/json.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* ---- built-in catalog entries ---- */

static const char *BUILTIN_CATALOG_JSON =
"{\"plugins\":["
"{\"name\":\"RF Impedance Calculator\",\"author\":\"VoltLab\",\"version\":\"1.2.0\","
 "\"desc\":\"Calculates trace impedance for RF designs - microstrip, stripline, coplanar waveguide\","
 "\"category\":\"EDA\",\"url\":\"\",\"icon\":\"RF\",\"downloads\":1240,\"rating\":4.5},"
"{\"name\":\"SnapEDA Part Finder\",\"author\":\"SnapEDA\",\"version\":\"2.1.3\","
 "\"desc\":\"Search and import parts from SnapEDA library with footprint and symbol\","
 "\"category\":\"EDA\",\"url\":\"\",\"icon\":\"SP\",\"downloads\":3200,\"rating\":4.8},"
"{\"name\":\"Teardrop Generator\",\"author\":\"VoltLab\",\"version\":\"1.0.1\","
 "\"desc\":\"Auto-generate teardrops for all vias and pads to improve reliability\","
 "\"category\":\"EDA\",\"url\":\"\",\"icon\":\"TD\",\"downloads\":890,\"rating\":4.2},"
"{\"name\":\"Gear Generator\",\"author\":\"MechCraft\",\"version\":\"3.0.0\","
 "\"desc\":\"Parametric gear CAD generation - spur, helical, bevel, worm gears\","
 "\"category\":\"CAD\",\"url\":\"\",\"icon\":\"GE\",\"downloads\":2150,\"rating\":4.7},"
"{\"name\":\"Thread Generator\",\"author\":\"MechCraft\",\"version\":\"2.0.1\","
 "\"desc\":\"ISO and metric threaded hole generator with clearance/tap drill sizes\","
 "\"category\":\"CAD\",\"url\":\"\",\"icon\":\"TH\",\"downloads\":1780,\"rating\":4.4},"
"{\"name\":\"Screw Library\",\"author\":\"FastenDB\",\"version\":\"1.5.0\","
 "\"desc\":\"Standard screw models M2-M12 with socket head, countersunk, hex types\","
 "\"category\":\"CAD\",\"url\":\"\",\"icon\":\"SC\",\"downloads\":2900,\"rating\":4.6},"
"{\"name\":\"Organic Support Generator\",\"author\":\"SliceLab\",\"version\":\"2.3.1\","
 "\"desc\":\"Tree-like organic supports for prints with custom branch settings\","
 "\"category\":\"Slicer\",\"url\":\"\",\"icon\":\"OS\",\"downloads\":4100,\"rating\":4.9},"
"{\"name\":\"Print Time Estimator\",\"author\":\"SliceLab\",\"version\":\"1.8.0\","
 "\"desc\":\"Accurate print time calculator with acceleration and jerk profiles\","
 "\"category\":\"Slicer\",\"url\":\"\",\"icon\":\"PT\",\"downloads\":3500,\"rating\":4.5},"
"{\"name\":\"Material Database\",\"author\":\"FilamentDB\",\"version\":\"4.0.2\","
 "\"desc\":\"50+ filament profiles with temperatures, speeds, retraction settings\","
 "\"category\":\"Slicer\",\"url\":\"\",\"icon\":\"MA\",\"downloads\":5200,\"rating\":4.8},"
"{\"name\":\"Dark OLED Theme\",\"author\":\"UXCraft\",\"version\":\"1.3.0\","
 "\"desc\":\"Ultra-dark UI theme optimized for AMOLED displays - true blacks\","
 "\"category\":\"Theme\",\"url\":\"\",\"icon\":\"DK\",\"downloads\":6700,\"rating\":4.9},"
"{\"name\":\"Solarized Theme\",\"author\":\"UXCraft\",\"version\":\"1.1.2\","
 "\"desc\":\"Light sepia UI theme based on the popular Solarized palette\","
 "\"category\":\"Theme\",\"url\":\"\",\"icon\":\"SO\",\"downloads\":3100,\"rating\":4.3},"
"{\"name\":\"Excel BOM Export\",\"author\":\"UtilKit\",\"version\":\"2.0.0\","
 "\"desc\":\"Export BOM directly to .xlsx format with supplier part numbers\","
 "\"category\":\"Utility\",\"url\":\"\",\"icon\":\"XL\",\"downloads\":4500,\"rating\":4.6},"
"{\"name\":\"PCB Panelization\",\"author\":\"FabPro\",\"version\":\"1.9.1\","
 "\"desc\":\"Advanced panelization with rails, tooling holes, fiducials, V-score\","
 "\"category\":\"Utility\",\"url\":\"\",\"icon\":\"PN\",\"downloads\":2800,\"rating\":4.4},"
"{\"name\":\"GitDiff Integration\",\"author\":\"DevKit\",\"version\":\"0.9.0\","
 "\"desc\":\"Git-aware project diff - visual diff of schematics, layouts, 3D models\","
 "\"category\":\"Utility\",\"url\":\"\",\"icon\":\"GD\",\"downloads\":1600,\"rating\":4.1},"
"{\"name\":\"Macro Recorder\",\"author\":\"DevKit\",\"version\":\"1.0.3\","
 "\"desc\":\"Record and replay keystroke macros for repetitive design tasks\","
 "\"category\":\"Utility\",\"url\":\"\",\"icon\":\"MR\",\"downloads\":2200,\"rating\":4.0}"
"]}";

/* ---- internal helpers ---- */

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} DownloadBuf;

static void dl_buf_init(DownloadBuf *db) {
    db->data = NULL;
    db->len = 0;
    db->cap = 0;
}

static void dl_buf_append(DownloadBuf *db, const void *buf, size_t len) {
    if (db->len + len > db->cap) {
        db->cap = db->cap ? db->cap * 2 : 65536;
        if (db->len + len > db->cap) db->cap = db->len + len + 4096;
        db->data = (char *)realloc(db->data, db->cap);
    }
    memcpy(db->data + db->len, buf, len);
    db->len += len;
}

static void dl_buf_free(DownloadBuf *db) {
    free(db->data);
    db->data = NULL;
    db->len = db->cap = 0;
}

static bool winhttp_download(const char *url, DownloadBuf *out) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    DWORD dwSize, dwDownloaded;
    char buf[8192];
    wchar_t wurl[2048];
    wchar_t whost[256];
    wchar_t wpath[2048];
    URL_COMPONENTSW url_comp;
    BOOL ok;

    if (!url || !out) return false;

    dl_buf_init(out);

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

    do {
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
        if (dwSize == 0) break;
        if (dwSize > sizeof(buf)) dwSize = sizeof(buf);
        if (!WinHttpReadData(hRequest, buf, dwSize, &dwDownloaded)) break;
        dl_buf_append(out, buf, dwDownloaded);
    } while (dwDownloaded > 0);

cleanup:
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return out->len > 0;
}

static bool winhttp_download_file(const char *url, const char *filepath) {
    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    DWORD dwSize, dwDownloaded;
    char buf[8192];
    wchar_t wurl[2048];
    wchar_t whost[256];
    wchar_t wpath[2048];
    URL_COMPONENTSW url_comp;
    BOOL ok;
    FILE *f = NULL;
    bool result = false;

    if (!url || !filepath) return false;

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

    f = fopen(filepath, "wb");
    if (!f) goto cleanup;

    do {
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
        if (dwSize == 0) break;
        if (dwSize > sizeof(buf)) dwSize = sizeof(buf);
        if (!WinHttpReadData(hRequest, buf, dwSize, &dwDownloaded)) break;
        fwrite(buf, 1, dwDownloaded, f);
    } while (dwDownloaded > 0);

    result = true;
cleanup:
    if (f) fclose(f);
    if (!result) DeleteFileA(filepath);
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return result;
}

static void parse_entry(PluginEntry *entry, JsonValue *obj) {
    memset(entry, 0, sizeof(*entry));

    {
        const char *s = json_get_str(obj, "name");
        if (s) { strncpy(entry->name, s, sizeof(entry->name) - 1); entry->name[sizeof(entry->name)-1] = 0; }
    }
    {
        const char *s = json_get_str(obj, "author");
        if (s) { strncpy(entry->author, s, sizeof(entry->author) - 1); entry->author[sizeof(entry->author)-1] = 0; }
    }
    {
        const char *s = json_get_str(obj, "version");
        if (s) { strncpy(entry->version, s, sizeof(entry->version) - 1); entry->version[sizeof(entry->version)-1] = 0; }
    }
    {
        const char *s = json_get_str(obj, "desc");
        if (s) { strncpy(entry->desc, s, sizeof(entry->desc) - 1); entry->desc[sizeof(entry->desc)-1] = 0; }
    }
    {
        const char *s = json_get_str(obj, "category");
        if (s) { strncpy(entry->category, s, sizeof(entry->category) - 1); entry->category[sizeof(entry->category)-1] = 0; }
    }
    {
        const char *s = json_get_str(obj, "url");
        if (s) { strncpy(entry->url, s, sizeof(entry->url) - 1); entry->url[sizeof(entry->url)-1] = 0; }
    }
    {
        const char *s = json_get_str(obj, "icon");
        if (s) { strncpy(entry->icon, s, sizeof(entry->icon) - 1); entry->icon[sizeof(entry->icon)-1] = 0; }
    }
    entry->downloads = (int)json_get_num(obj, "downloads");
    entry->rating = (float)json_get_num(obj, "rating");
    entry->installed = 0;
}

static int parse_catalog(const char *json_text, PluginEntry **out_entries) {
    JsonValue *root;
    JsonValue *arr;
    int i, count;

    if (!json_text || !out_entries) return 0;

    root = json_parse(json_text, strlen(json_text));
    if (!root) return 0;

    arr = json_get_arr(root, "plugins");
    if (!arr || arr->type != J_ARR) {
        json_free(root);
        return 0;
    }

    count = json_arr_len(arr);
    if (count <= 0) {
        json_free(root);
        return 0;
    }

    *out_entries = (PluginEntry *)calloc((size_t)count, sizeof(PluginEntry));
    if (!*out_entries) {
        json_free(root);
        return 0;
    }

    for (i = 0; i < count; i++) {
        JsonValue *item = json_arr_get(arr, i);
        if (item && item->type == J_OBJ) {
            parse_entry(&(*out_entries)[i], item);
        }
    }

    json_free(root);
    return count;
}

static int str_contains_ci(const char *s, const char *sub) {
    size_t slen, sublen, i;
    if (!s || !sub) return 0;
    slen = strlen(s);
    sublen = strlen(sub);
    if (sublen == 0) return 1;
    if (sublen > slen) return 0;
    for (i = 0; i <= slen - sublen; i++) {
        size_t j;
        int match = 1;
        for (j = 0; j < sublen; j++) {
            if (tolower((unsigned char)s[i+j]) != tolower((unsigned char)sub[j])) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

/* ---- public API ---- */

void plugin_market_init(PluginMarket *pm) {
    memset(pm, 0, sizeof(*pm));
    pm->entries = NULL;
    pm->nentries = 0;
    pm->loading = 0;
    pm->repo_url[0] = 0;
}

void plugin_market_fetch(PluginMarket *pm, const char *repo_url) {
    DownloadBuf db;
    int parsed_count;

    if (!pm) return;

    /* free existing entries */
    free(pm->entries);
    pm->entries = NULL;
    pm->nentries = 0;

    if (repo_url) {
        strncpy(pm->repo_url, repo_url, sizeof(pm->repo_url) - 1);
        pm->repo_url[sizeof(pm->repo_url) - 1] = 0;
    }

    pm->loading = 1;

    /* try to fetch from URL */
    if (pm->repo_url[0] && winhttp_download(pm->repo_url, &db)) {
        parsed_count = parse_catalog(db.data, &pm->entries);
        pm->nentries = parsed_count;
        dl_buf_free(&db);
        if (parsed_count > 0) {
            pm->loading = 0;
            return;
        }
    }

    /* fallback to built-in catalog */
    parsed_count = parse_catalog(BUILTIN_CATALOG_JSON, &pm->entries);
    pm->nentries = parsed_count;
    pm->loading = 0;
}

void plugin_market_free(PluginMarket *pm) {
    if (!pm) return;
    free(pm->entries);
    memset(pm, 0, sizeof(*pm));
}

int plugin_market_search(PluginMarket *pm, const char *query, PluginEntry *results, int max_results) {
    int i, found = 0;
    if (!pm || !results || max_results <= 0) return 0;

    for (i = 0; i < pm->nentries && found < max_results; i++) {
        PluginEntry *e = &pm->entries[i];
        if (!query || query[0] == 0 ||
            str_contains_ci(e->name, query) ||
            str_contains_ci(e->desc, query) ||
            str_contains_ci(e->author, query) ||
            str_contains_ci(e->category, query)) {
            results[found] = *e;
            found++;
        }
    }
    return found;
}

bool plugin_market_install(PluginMarket *pm, int index, const char *plugins_dir) {
    char filepath[512];
    PluginEntry *e;

    if (!pm || index < 0 || index >= pm->nentries || !plugins_dir) return false;

    e = &pm->entries[index];

    /* if already installed, treat as success */
    if (e->installed) return true;

    /* if no URL, it is a built-in stub - mark as installed */
    if (!e->url || e->url[0] == 0) {
        e->installed = 1;
        return true;
    }

    snprintf(filepath, sizeof(filepath), "%s\\%s.dll", plugins_dir, e->name);

    /* create plugins directory if needed */
    CreateDirectoryA(plugins_dir, NULL);

    if (!winhttp_download_file(e->url, filepath)) return false;

    /* try to load the DLL to verify */
    {
        HMODULE h = LoadLibraryA(filepath);
        if (h) {
            FreeLibrary(h);
            e->installed = 1;
            return true;
        }
    }

    /* failed to load - clean up */
    DeleteFileA(filepath);
    return false;
}

bool plugin_market_uninstall(int index, const char *plugins_dir) {
    char filepath[512];
    char search[512];
    WIN32_FIND_DATAA fd;
    HANDLE hFind;
    char **plugin_names = NULL;
    int nnames = 0;
    bool result = false;
    int i;

    (void)index;
    (void)plugins_dir;

    if (!plugins_dir) return false;

    snprintf(search, sizeof(search), "%s\\*.dll", plugins_dir);
    hFind = FindFirstFileA(search, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return false;

    do {
        char *name = str_dup(fd.cFileName);
        char **tmp = (char **)realloc(plugin_names, (size_t)(nnames + 1) * sizeof(char *));
        if (!tmp) { free(name); continue; }
        plugin_names = tmp;
        plugin_names[nnames++] = name;
    } while (FindNextFileA(hFind, &fd));
    FindClose(hFind);

    if (index < 0 || index >= nnames) {
        for (i = 0; i < nnames; i++) free(plugin_names[i]);
        free(plugin_names);
        return false;
    }

    snprintf(filepath, sizeof(filepath), "%s\\%s", plugins_dir, plugin_names[index]);
    result = DeleteFileA(filepath) ? true : false;

    for (i = 0; i < nnames; i++) free(plugin_names[i]);
    free(plugin_names);
    return result;
}

char *plugin_market_status(const PluginMarket *pm) {
    char buf[256];
    int installed = 0;
    int i;

    if (!pm) return str_dup("0 plugins");

    for (i = 0; i < pm->nentries; i++) {
        if (pm->entries[i].installed) installed++;
    }

    snprintf(buf, sizeof(buf), "%d plugins | %d installed | %d available",
             pm->nentries, installed, pm->nentries - installed);
    return str_dup(buf);
}

/* ---- rendering ---- */

void plugin_market_render(const PluginMarket *pm, Rectangle viewport) {
    int card_w = 150;
    int card_h = 100;
    int margin = 8;
    int search_h = 32;
    int tab_h = 28;
    int status_h = 24;
    int y = (int)viewport.y + margin;
    int x = (int)viewport.x + margin;
    int cols;
    int i;

    if (!pm) return;

    /* status text at bottom */
    {
        char *status = plugin_market_status(pm);
        DrawText(status, (int)viewport.x + 5, (int)(viewport.y + viewport.height - status_h),
                 14, DARKGRAY);
        free(status);
    }

    /* search bar placeholder */
    DrawRectangleLines((int)viewport.x + margin, y, (int)viewport.width - margin * 2, search_h, GRAY);
    DrawText("Search plugins...", (int)viewport.x + margin + 5, y + 6, 14, LIGHTGRAY);
    y += search_h + margin;

    /* category filter tabs */
    {
        const char *cats[] = {"All", "EDA", "CAD", "Slicer", "Theme", "Utility"};
        int ncats = 6;
        int tab_w = 60;
        for (i = 0; i < ncats; i++) {
            Rectangle tab = {(float)((int)viewport.x + margin + i * (tab_w + 4)),
                             (float)y, (float)tab_w, (float)tab_h};
            DrawRectangleRec(tab, (i == 0) ? BLUE : LIGHTGRAY);
            DrawText(cats[i], (int)tab.x + 4, (int)tab.y + 4, 12,
                     (i == 0) ? WHITE : DARKGRAY);
        }
    }
    y += tab_h + margin;

    /* plugin cards grid */
    cols = (int)((viewport.width - margin) / (float)(card_w + margin));
    if (cols < 1) cols = 1;

    for (i = 0; i < pm->nentries; i++) {
        PluginEntry *e = &pm->entries[i];
        int col = i % cols;
        int row = i / cols;
        int cx = (int)viewport.x + margin + col * (card_w + margin);
        int cy = y + row * (card_h + margin);

        /* card background */
        Color card_bg = e->installed ? (Color){200, 255, 200, 255} : RAYWHITE;
        DrawRectangle(cx, cy, card_w, card_h, card_bg);
        DrawRectangleLines(cx, cy, card_w, card_h, GRAY);

        /* icon */
        {
            char icon_text[4];
            snprintf(icon_text, sizeof(icon_text), "%s", e->icon[0] ? e->icon : "??");
            DrawText(icon_text, cx + 4, cy + 4, 16, DARKGRAY);
        }

        /* name (truncated) */
        {
            char name_display[32];
            strncpy(name_display, e->name, sizeof(name_display) - 1);
            name_display[sizeof(name_display) - 1] = 0;
            if (strlen(e->name) > sizeof(name_display) - 1) {
                name_display[sizeof(name_display) - 4] = '.';
                name_display[sizeof(name_display) - 3] = '.';
                name_display[sizeof(name_display) - 2] = '.';
                name_display[sizeof(name_display) - 3] = 0;
            }
            DrawText(name_display, cx + 24, cy + 4, 10, BLACK);
        }

        /* author and version */
        {
            char info[64];
            snprintf(info, sizeof(info), "%s v%s", e->author[0] ? e->author : "?", e->version);
            DrawText(info, cx + 4, cy + 22, 9, DARKGRAY);
        }

        /* category badge */
        {
            Color cat_color;
            if (strcmp(e->category, "EDA") == 0) cat_color = (Color){100, 149, 237, 255};
            else if (strcmp(e->category, "CAD") == 0) cat_color = (Color){60, 179, 113, 255};
            else if (strcmp(e->category, "Slicer") == 0) cat_color = (Color){218, 165, 32, 255};
            else if (strcmp(e->category, "Theme") == 0) cat_color = (Color){147, 112, 219, 255};
            else cat_color = (Color){119, 136, 153, 255};
            DrawRectangle(cx + 4, cy + 34, 50, 14, cat_color);
            DrawText(e->category, cx + 6, cy + 35, 8, WHITE);
        }

        /* star rating */
        {
            int stars = (int)(e->rating + 0.5f);
            if (stars > 5) stars = 5;
            {
                int si;
                int sx = cx + 4;
                for (si = 0; si < stars && si < 5; si++) {
                    DrawText("*", sx, cy + 50, 10, GOLD);
                    sx += 7;
                }
            }
        }

        /* install/uninstall button */
        {
            Rectangle btn = {(float)(cx + card_w - 55), (float)(cy + card_h - 20), 50, 16};
            if (e->installed) {
                DrawRectangleRec(btn, GREEN);
                DrawText("Installed", cx + card_w - 52, cy + card_h - 18, 8, WHITE);
            } else {
                DrawRectangleRec(btn, BLUE);
                DrawText("Install", cx + card_w - 46, cy + card_h - 18, 8, WHITE);
            }
        }

        /* installed checkmark */
        if (e->installed) {
            DrawText("v", cx + card_w - 16, cy + 4, 12, GREEN);
        }
    }
}
