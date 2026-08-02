#pragma once
// webcam.h - Webcam/materials placeholder for farm monitoring.
#include <stdbool.h>

// Capture a webcam snapshot (stub — returns false on no camera).
bool webcam_capture(const char *output_path);

// List available webcam devices (stub).
char *webcam_list_devices(void);

// Materials library extended with render properties.
typedef struct {
    char name[64];
    float diffuse[3];    // RGB 0-1
    float specular[3];
    float roughness;
    float metallic;
    float clearcoat;
} RenderMaterial;

void render_material_default(RenderMaterial *rm, const char *name);
char *render_material_list(void);
