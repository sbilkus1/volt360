#include "moonraker.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

static char *mr_http_post(const char *host, int port, const char *endpoint, const char *body, int *out_status) {
    *out_status = -1;
    wchar_t wh[256], wep[512];
    MultiByteToWideChar(CP_UTF8, 0, host, -1, wh, 256);
    HINTERNET s = WinHttpOpen(L"Volt360/1.0", WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, 0);
    HINTERNET c = WinHttpConnect(s, wh, (INTERNET_PORT)port, 0);
    MultiByteToWideChar(CP_UTF8, 0, endpoint, -1, wep, 512);
    HINTERNET r = WinHttpOpenRequest(c, L"POST", wep, NULL, NULL, NULL, 0);
    LPCWSTR hdr = L"Content-Type: application/json\r\n";
    DWORD blen = (DWORD)strlen(body);
    WinHttpSendRequest(r, hdr, (DWORD)wcslen(hdr), (LPVOID)body, blen, blen, 0);
    if (!WinHttpReceiveResponse(r, NULL)) goto done;
    DWORD sc = 0, sz = sizeof(sc);
    WinHttpQueryHeaders(r, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &sc, &sz, NULL);
    *out_status = (int)sc;
    char *buf = (char *)malloc(8192);
    int total = 0; DWORD rd;
    while (WinHttpReadData(r, buf + total, 8191 - total, &rd) && rd > 0) { total += (int)rd; if (total >= 8190) break; }
    buf[total] = '\0';
done:
    WinHttpCloseHandle(r); WinHttpCloseHandle(c); WinHttpCloseHandle(s);
    return buf;
}

static char *mr_http_get(const char *host, int port, const char *endpoint, int *out_status) {
    *out_status = -1;
    wchar_t wh[256], wep[512];
    MultiByteToWideChar(CP_UTF8, 0, host, -1, wh, 256);
    HINTERNET s = WinHttpOpen(L"Volt360/1.0", WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, 0);
    HINTERNET c = WinHttpConnect(s, wh, (INTERNET_PORT)port, 0);
    MultiByteToWideChar(CP_UTF8, 0, endpoint, -1, wep, 512);
    HINTERNET r = WinHttpOpenRequest(c, L"GET", wep, NULL, NULL, NULL, WINHTTP_FLAG_SECURE);
    WinHttpSendRequest(r, NULL, 0, NULL, 0, 0, 0);
    if (!WinHttpReceiveResponse(r, NULL)) goto done;
    DWORD sc = 0, sz = sizeof(sc);
    WinHttpQueryHeaders(r, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &sc, &sz, NULL);
    *out_status = (int)sc;
    char *buf = (char *)malloc(8192);
    int total = 0; DWORD rd;
    while (WinHttpReadData(r, buf + total, 8191 - total, &rd) && rd > 0) { total += (int)rd; if (total >= 8190) break; }
    buf[total] = '\0';
done:
    WinHttpCloseHandle(r); WinHttpCloseHandle(c); WinHttpCloseHandle(s);
    return buf;
}

static const char *mr_json_str(const char *json, const char *key) {
    static char b[256];
    char k[128]; snprintf(k, sizeof(k), "\"%s\":\"", key);
    const char *p = strstr(json, k);
    if (!p) { snprintf(k, sizeof(k), "\"%s\": \"", key); p = strstr(json, k); if (!p) return ""; }
    p += strlen(k);
    int n = 0;
    while (*p && *p != '"' && n < 250) b[n++] = *p++;
    b[n] = '\0';
    return b;
}

int mr_status(const char *host, int port, char *state, int state_len) {
    int sc;
    char *r = mr_http_get(host, port, "/printer/objects/query?print_stats", &sc);
    if (sc > 0 && r) {
        const char *st = mr_json_str(r, "state");
        snprintf(state, state_len, "%s", st[0] ? st : "offline");
    } else { snprintf(state, state_len, "offline"); }
    free(r);
    return sc;
}

int mr_gcode_cmd(const char *host, int port, const char *cmd) {
    char body[512];
    snprintf(body, sizeof(body), "{\"script\":\"%s\"}", cmd);
    int sc;
    char *r = mr_http_post(host, port, "/printer/gcode/script", body, &sc);
    free(r);
    return sc;
}

int mr_upload_and_print(const char *host, int port, const char *gcode_path, const char *display_name) {
    // For full upload use the file API; simple version: send print_start with filename
    // Moonraker expects files to already be in the virtual SD.
    // For basic integration, we just note the path and suggest manual upload.
    (void)gcode_path; (void)display_name;
    // Send START_PRINT via gcode script as a fallback
    char cmd[256];
    const char *fn = display_name ? display_name : "print.gcode";
    snprintf(cmd, sizeof(cmd), "SDCARD_PRINT_FILE FILENAME=%s", fn);
    return mr_gcode_cmd(host, port, cmd);
}

int mr_cancel(const char *host, int port) {
    return mr_gcode_cmd(host, port, "CANCEL_PRINT");
}

int mr_pause(const char *host, int port) {
    return mr_gcode_cmd(host, port, "PAUSE");
}

int mr_resume(const char *host, int port) {
    return mr_gcode_cmd(host, port, "RESUME");
}

#else
// stub for non-Windows
int mr_status(const char *host, int port, char *state, int state_len) { (void)host; (void)port; snprintf(state, state_len, "unsupported"); return -1; }
int mr_gcode_cmd(const char *host, int port, const char *cmd) { (void)host; (void)port; (void)cmd; return -1; }
int mr_upload_and_print(const char *host, int port, const char *gcode_path, const char *display_name) { (void)host; (void)port; (void)gcode_path; (void)display_name; return -1; }
int mr_cancel(const char *host, int port) { (void)host; (void)port; return -1; }
int mr_pause(const char *host, int port) { (void)host; (void)port; return -1; }
int mr_resume(const char *host, int port) { (void)host; (void)port; return -1; }
#endif
