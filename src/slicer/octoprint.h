#pragma once
// octoprint.h - OctoPrint/PrusaLink printer connectivity.
#include <stdbool.h>

// OctoPrint API (HTTP REST, port 80/5000 typically)
// Uses the same API pattern as Moonraker with different endpoints.

// Upload G-code to OctoPrint and optionally start printing.
int octo_upload(const char *host, int port, const char *api_key, const char *gcode_path, const char *filename);

// Get printer status from OctoPrint.
int octo_status(const char *host, int port, const char *api_key, char *state, int state_len);

// PrusaLink (Prusa Connect local): similar REST API on port 80.
int prusalink_status(const char *host, int port, const char *api_key, char *state, int state_len);
int prusalink_upload(const char *host, int port, const char *api_key, const char *gcode_path);
