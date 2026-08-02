#pragma once
// advance_feat.h - Advanced features: push-shove router, organic supports, PBR, 5-axis CAM.
#include <stdbool.h>
#include "../model/project.h"
#include "../slicer/slicer.h"
#include "../slicer/supports.h"

// ===== INTERACTIVE PUSH-AND-SHOVE ROUTER =====
// Route a track from start to end, interactively pushing all obstacles.
// Maintains minimum clearance. Returns number of tracks shoved.
// route_attempts: maximum shove iterations before giving up.
int pcb_route_interactive(Pcb *pcb, V2 start, V2 end, float width, float clearance, int max_attempts);

// Check if a new track at given position would violate DRC.
// Returns NULL if OK, otherwise returns the violating track/pad index and type.
typedef struct { int index; int is_track; // 1=track, 0=pad
    char msg[128]; } DRCResult;
DRCResult pcb_check_drc_realtime(Pcb *pcb, V2 a, V2 b, float width, float clearance, int exclude_index);

// ===== ORGANIC TREE SUPPORTS =====
// Generate organic/dendritic support trees (OrcaSlicer-style).
// These branch outward from a trunk, minimizing material while supporting overhangs.
void supports_organic_tree(SliceResult *sr, const SliceSettings *cfg, float overhang_angle, int branch_depth);

// ===== PBR MATERIALS + LIGHTING =====
// Apply PBR material properties to a model for rendering.
typedef struct {
    float albedo[3];    // base color
    float metallic;     // 0=dielectric, 1=metal
    float roughness;    // 0=mirror, 1=matte
    float ao;           // ambient occlusion
    float emissive[3];  // self-illumination
} PBRMaterial;

// HDR environment lighting configuration.
typedef struct {
    float sky_color[3];     // top hemisphere
    float ground_color[3];  // bottom hemisphere
    float sun_dir[3];       // directional light direction
    float sun_color[3];
    float intensity;
    int enable_shadows;
} HDREnvironment;

void pbr_material_preset(PBRMaterial *mat, const char *type); // "aluminum","steel","plastic","glass","copper","gold","carbon","wood"
void hdr_environment_preset(HDREnvironment *env, const char *type); // "studio","outdoor","night","sunset","neutral"
char *pbr_render_report(CadMesh *m, PBRMaterial *mat, HDREnvironment *env);

// ===== 5-AXIS SIMULTANEOUS CAM =====
// Generate 5-axis simultaneous toolpath (X,Y,Z,A,B axes moving together).
// toolpath points are in world space, tool_axis is the tool orientation at each point.
typedef struct { V3 pos; V3 axis; } AxisPoint;
int cam5_simultaneous(V3 *surface_points, V3 *surface_normals, int n_points, float tool_diameter, float stepover,
                      AxisPoint *toolpath, int max_points, const char *output_path);
char *cam5_toolpath_report(AxisPoint *path, int n);
