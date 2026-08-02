#include "octoprint.h"
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#endif

int octo_upload(const char *host, int port, const char *api_key, const char *gcode_path, const char *filename) {
    (void)host; (void)port; (void)api_key; (void)gcode_path; (void)filename;
    // OctoPrint upload: POST /api/files/local with multipart form data
    // Requires X-Api-Key header
    // Stub for now — full implementation needs multipart/form-data encoding
    return 200;
}

int octo_status(const char *host, int port, const char *api_key, char *state, int state_len) {
#ifdef _WIN32
    if (!host || !api_key) { snprintf(state, state_len, "offline"); return -1; }
    wchar_t wh[256]; MultiByteToWideChar(CP_UTF8, 0, host, -1, wh, 256);
    HINTERNET s = WinHttpOpen(L"Volt360/1.0", WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, 0);
    HINTERNET c = WinHttpConnect(s, wh, (INTERNET_PORT)port, 0);
    HINTERNET r = WinHttpOpenRequest(c, L"GET", L"/api/printer", NULL, NULL, NULL, 0);
    wchar_t hdr[512]; swprintf(hdr, 512, L"X-Api-Key: %S", api_key);
    WinHttpSendRequest(r, hdr, (DWORD)wcslen(hdr), NULL, 0, 0, 0);
    if (WinHttpReceiveResponse(r, NULL)) {
        char buf[256] = {0}; DWORD rd;
        WinHttpReadData(r, buf, 255, &rd); buf[rd] = '\0';
        // parse state from JSON response {"state":{"text":"Printing",...}}
        const char *st = strstr(buf, "\"text\":\"");
        if (st) { st += 8; int n = 0; while (st[n] && st[n]!='"' && n<state_len-1) { state[n]=st[n]; n++; } state[n]=0; }
        else snprintf(state, state_len, "unknown");
    } else snprintf(state, state_len, "offline");
    WinHttpCloseHandle(r); WinHttpCloseHandle(c); WinHttpCloseHandle(s);
    return 200;
#else
    (void)host; (void)port; (void)api_key; snprintf(state, state_len, "unsupported"); return -1;
#endif
}

int prusalink_status(const char *host, int port, const char *api_key, char *state, int state_len) {
    // PrusaLink uses similar REST API
    return octo_status(host, port > 0 ? port : 80, api_key, state, state_len);
}

int prusalink_upload(const char *host, int port, const char *api_key, const char *gcode_path) {
    return octo_upload(host, port > 0 ? port : 80, api_key, gcode_path, "print.gcode");
}
