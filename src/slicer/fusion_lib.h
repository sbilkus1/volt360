#pragma once
// fusion_lib.h - Autodesk Fusion CAM library: tools, materials, machines, post-processors.
#include <stdbool.h>
#include "../model/project.h"

// ===== FUSION TOOL LIBRARY =====
typedef struct {
    char guid[64];         // unique tool ID
    char type[32];         // "flat end mill","ball end mill","face mill","chamfer mill","drill","tap","reamer","bore","thread mill","slot mill","engrave","laser","plasma","waterjet"
    char description[128];
    int tool_number;
    // geometry
    float diameter;        // mm
    float corner_radius;   // 0 = sharp
    float flute_length;
    float overall_length;
    float shoulder_length;
    float taper_angle;
    int num_flutes;
    // feeds & speeds
    float spindle_speed;   // RPM
    float surface_speed;   // m/min
    float feed_per_tooth;  // mm/tooth
    float feed_rate;       // mm/min
    float plunge_feedrate; // mm/min
    float ramp_feedrate;
    // cutting
    float stepover;        // mm or fraction
    float stepdown;        // mm per pass
    // coolant
    int coolant;           // 0=none, 1=flood, 2=mist, 3=through
    // holder
    char holder_type[32];  // "BT30","BT40","HSK63","CAT40","R8","ER32"
    float holder_diameter;
    // material suitability
    char materials[256];   // comma-separated: "aluminum,steel,titanium,plastic"
} FusionTool;

// ===== FUSION MATERIAL LIBRARY =====
typedef struct {
    char name[64];
    char type[32];         // "metal","plastic","wood","composite","foam"
    float density;         // g/cm³
    float hardness;
    // machining properties
    float surface_speed;   // m/min recommended
    float feed_per_tooth;  // mm recommended
    float stepover_pct;    // % of tool diameter
    float stepdown_pct;
} FusionMaterial;

// ===== FUSION MACHINE DEFINITION =====
typedef struct {
    char name[64];
    char type[32];         // "mill","lathe","mill-turn","router","laser","plasma","waterjet","edm"
    // work volume
    float x_travel, y_travel, z_travel;
    // spindle
    float max_spindle_rpm;
    float max_spindle_power_kw;
    char spindle_type[16]; // "BT30","BT40","HSK63","R8"
    // capabilities
    int max_tools;
    int has_coolant;
    int has_probe;
    int num_axes;          // 3, 4, 5
    char controller[32];   // "grbl","mach3","linuxcnc","haas","fanuc","siemens","heidenhain"
    char post_processor[128]; // path to post config
} FusionMachine;

// ===== API =====

// Tool library: load/save/query.
int fusion_tool_count(void);
FusionTool *fusion_tool_get(int index);
FusionTool *fusion_tool_find_by_type(const char *type, float diameter);
FusionTool *fusion_tool_find_by_number(int tool_number);
char *fusion_tool_list_all(void);
char *fusion_tool_list_by_material(const char *material);

// Import/Export
bool fusion_library_load(const char *json_path);
bool fusion_library_save(const char *json_path);

// Material library
int fusion_material_count(void);
FusionMaterial *fusion_material_get(int index);
FusionMaterial *fusion_material_find(const char *name);
char *fusion_material_list_all(void);

// Machine definitions
int fusion_machine_count(void);
FusionMachine *fusion_machine_get(int index);
FusionMachine *fusion_machine_find(const char *name);
char *fusion_machine_list_all(void);

// Add custom tools
int fusion_tool_add(FusionTool tool);

// Generate toolpath recommendations for a material + operation.
char *fusion_cam_recommendations(const char *material, const char *operation);
