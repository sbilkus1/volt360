#pragma once
// webcamview.h - Webcam snapshot viewer and image display.
#include <stdbool.h>

// Display a saved image file (stub - raylib can display textures).
bool webcam_show_image(const char *image_path);

// Capture a frame from a webcam and save to file.
// device_index: 0 = default camera
bool webcam_capture_frame(int device_index, const char *output_path);

// List available camera devices.
char *webcam_enum_devices(void);

// Live preview window text (indicates webcam status).
char *webcam_preview_status(void);
