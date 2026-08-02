#include "webcamview.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

bool webcam_show_image(const char *image_path) {
    (void)image_path;
    // raylib: LoadTexture(image_path) + DrawTexture
    return false; // stub
}

bool webcam_capture_frame(int device_index, const char *output_path) {
    (void)device_index; (void)output_path;
    // Windows: Media Foundation / DirectShow capture
    return false; // stub
}

char *webcam_enum_devices(void) {
    return str_dup("Webcam: device enumeration stub\nWindows: use DirectShow or Media Foundation API");
}

char *webcam_preview_status(void) {
    return str_dup("Webcam: no camera connected (stub)");
}
