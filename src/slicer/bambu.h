#pragma once
// bambu.h - BambuLab/Creality printer connectivity (FTP/HTTP).
#include <stdbool.h>

// Upload G-code to a Bambu printer via FTP.
// host: printer IP, code: printer access code
bool bambu_upload(const char *host, int port, const char *code, const char *gcode_path);

// Check Bambu printer status.
int bambu_status(const char *host, int port, const char *code, char *state, int state_len);

// USB serial printing: send G-code over a COM port.
bool usb_print(const char *com_port, const char *gcode_path);
