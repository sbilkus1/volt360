#pragma once
// moonraker.h - Klipper/Moonraker HTTP client for network printing.
#include <stdbool.h>

// Upload a G-code file and optionally start printing.
// Returns the HTTP status code (200 = success) or -1 on network error.
int mr_upload_and_print(const char *host, int port, const char *gcode_path, const char *display_name);

// Get printer state info. Fills `state` buffer (e.g. "printing", "ready", "error").
// Returns HTTP status code or -1.
int mr_status(const char *host, int port, char *state, int state_len);

// Send a G-code command directly (e.g. PAUSE, RESUME, CANCEL_PRINT).
int mr_gcode_cmd(const char *host, int port, const char *cmd);

// Cancel the current print.
int mr_cancel(const char *host, int port);

// Pause / Resume the current print.
int mr_pause(const char *host, int port);
int mr_resume(const char *host, int port);
