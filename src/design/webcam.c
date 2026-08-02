#include "webcam.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

bool webcam_capture(const char *output_path) {
    (void)output_path;
    // Would use Windows Media Foundation / DirectShow to capture
    return false; // stub: no camera API implemented
}

char *webcam_list_devices(void) {
    return str_dup("Webcam support: use external viewer\nStub: camera API not yet implemented");
}

void render_material_default(RenderMaterial *rm, const char *name) {
    memset(rm, 0, sizeof(*rm));
    snprintf(rm->name, sizeof(rm->name), "%s", name ? name : "default");
    rm->diffuse[0] = 0.8f; rm->diffuse[1] = 0.8f; rm->diffuse[2] = 0.8f;
    rm->specular[0] = 0.5f; rm->specular[1] = 0.5f; rm->specular[2] = 0.5f;
    rm->roughness = 0.4f; rm->metallic = 0.0f; rm->clearcoat = 0.0f;
}

char *render_material_list(void) {
    return str_dup(
        "=== RENDER MATERIALS ===\n"
        "  Aluminum: diffuse(0.9,0.9,0.9) rough=0.3 metal=1.0\n"
        "  Steel:    diffuse(0.6,0.6,0.6) rough=0.4 metal=1.0\n"
        "  Plastic:  diffuse(0.8,0.8,0.8) rough=0.5 metal=0.0\n"
        "  Glass:    diffuse(0.9,0.9,1.0) rough=0.1 metal=0.0 clear=1.0\n"
        "  Copper:   diffuse(0.9,0.6,0.4) rough=0.3 metal=1.0\n");
}
