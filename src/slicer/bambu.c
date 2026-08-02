#include "bambu.h"
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

bool bambu_upload(const char *host, int port, const char *code, const char *gcode_path) {
    (void)host; (void)port; (void)code; (void)gcode_path;
    // Bambu uses FTP at port 990 with TLS. Simplified: note the path.
    // Real implementation would use WinINet FTP or a third-party FTP lib.
    return true; // stub
}

int bambu_status(const char *host, int port, const char *code, char *state, int state_len) {
    (void)host; (void)port; (void)code;
    // Query via HTTP / MQTT. Stub.
    snprintf(state, state_len, "online");
    return 200;
}

bool usb_print(const char *com_port, const char *gcode_path) {
#ifdef _WIN32
    if (!com_port || !gcode_path) return false;
    char full_port[32];
    snprintf(full_port, sizeof(full_port), "\\\\.\\%s", com_port);
    HANDLE h = CreateFileA(full_port, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return false;

    DCB dcb = {0}; dcb.DCBlength = sizeof(dcb);
    GetCommState(h, &dcb);
    dcb.BaudRate = CBR_115200; dcb.ByteSize = 8; dcb.Parity = NOPARITY; dcb.StopBits = ONESTOPBIT;
    SetCommState(h, &dcb);

    FILE *gf = fopen(gcode_path, "r");
    if (!gf) { CloseHandle(h); return false; }
    char line[256];
    while (fgets(line, sizeof(line), gf)) {
        DWORD written;
        WriteFile(h, line, (DWORD)strlen(line), &written, NULL);
    }
    fclose(gf);
    CloseHandle(h);
    return true;
#else
    (void)com_port; (void)gcode_path; return false;
#endif
}
