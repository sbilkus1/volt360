#pragma once
// slicer.h - 3D slicing engine (mesh -> toolpaths) inspired by OrcaSlicer.
// Slices a closed triangle mesh into layers and emits per-layer toolpaths
// (perimeters + infill) ready for the G-code writer.
#include "../model/project.h"

typedef struct {
    // geometry
    float layer_height;      // mm (default 0.2)
    float line_width;        // mm (default = nozzle * 1.2)
    float nozzle_diameter;   // mm (default 0.4)
    int perimeters;          // wall loops (default 2)
    int solid_top_layers;    // mm equivalent -> count = ceil(mm/layer) (default 3)
    int solid_bottom_layers; // default 3
    int infill_density;      // 0..100 (default 15)
    float infill_angle;      // deg (default 45)
    int infill_pattern;      // 0=grid, 1=grid2 (alternating), 2=honeycomb, 3=concentric, 4=gyroid
    bool ironing;            // top surface ironing pass
    bool adaptive_layer;     // variable layer height based on surface slope
    // temps / speeds
    int bed_temp;            // C (default 60)
    int hotend_temp;         // C (default 210)
    float print_speed;       // mm/s (default 60)
    float travel_speed;      // mm/s (default 150)
    float retract_mm;        // default 0.8
    float extrusion_mult;    // default 1.0
    float filament_dia;      // mm (default 1.75)
    // AMS / CFS multicolor
    int tools;               // number of AMS/CFS slots (0 = none / single tool)
    int random_color;        // pick a random tool per sliced model
    int color_change_layer;  // >=0: emit an extra M600 color change at this layer
    const char *color_change_cmd; // gcode to switch filament (default "M600")
    // print farm
    int farm_mode;           // emit farm control codes / completion markers
    int auto_eject;          // emit servo-eject sequence at print end
    float eject_angle;       // servo angle to push the part (deg, default 90)
    int eject_release_temp;  // wait for bed to cool to this temp before ejecting (default 40)
    const char *eject_gcode; // custom removal gcode override ("" / NULL = auto sequence)
    int forced_tool;         // -1 = auto (random when enabled), >=0 = pin to this AMS/CFS slot
    // OrcaSlicer-style quality controls
    int seam_position;       // 0 = nearest-to-origin, 1 = back (max +Y), 2 = fixed angle
    float seam_angle;        // deg for fixed seam (default 0 = +X)
    int scarf_joint;         // overlap the start/end of the outer wall to hide the seam
    float scarf_ratio;       // scarf overlap as fraction of perimeter length (0.1)
    float polyhole;          // hole-enlargement factor (0 = off, e.g. 0.05 => +5%)
    int fuzzy_skin;          // jitter the outer perimeter
    float fuzzy_points_mm;   // jitter wavelength along the wall (2.0)
    float fuzzy_mm;          // jitter amplitude (0.3)
    int sandwich_mode;       // alternate wall/infill ordering per layer
    int zaa;                 // approximate Z anti-aliasing: denser, inset top fill
    float wall_speed_mult;   // per-feature speed multipliers (1.0 default)
    float infill_speed_mult;
    float solid_speed_mult;
    int wall_temp;           // 0 = use hotend_temp
    int infill_temp;         // 0 = use hotend_temp
    float wall_layer_height; // 0 = use layer_height (extrusion only)
    // header templates (may be NULL)
    const char *printer_name;
    const char *start_gcode;
    const char *end_gcode;
} SliceSettings;

void slice_settings_default(SliceSettings *s);

// ---------------- toolpaths ----------------
typedef enum { SLICE_PATH_PERIMETER, SLICE_PATH_INFILL } SlicePathKind;

typedef struct {
    int kind;            // SlicePathKind
    V2 *pts;             // polyline (mm, already inset by line_width/2)
    int npts;
    float width;         // extrusion width used (mm)
    int solid;           // 1 if this infill is solid
} SlicePath;

typedef struct {
    float z;             // layer height (mm)
    SlicePath *paths;    // ordered: perimeters outer->inner, then infill
    int npaths;
} SliceLayer;

typedef struct {
    SliceLayer *layers;
    int nlayers;
    // stats
    float filament_mm;   // total filament length (mm)
    float est_time_s;    // rough print time (s)
    float total_dist;    // total toolpath distance (mm)
    float z_max;         // print height
    int total_paths;
} SliceResult;

// Slice a mesh (must be closed / manifold-ish). Z is auto-shifted so the lowest
// point sits at z=0. Returns true on success.
bool slice_mesh(CadMesh *mesh, const SliceSettings *cfg, SliceResult *out);
void slice_result_free(SliceResult *r);

// Small helpers exposed for tests / preview.
// Inset a closed polygon inward by `d` mm (miter offset). Output polygon in `out`.
bool poly_offset(V2 *poly, int n, float d, V2 *out, int *out_n, int maxn);

// Persist slice settings to/from a JSON file (for UI preference save).
bool slice_settings_save(const SliceSettings *cfg, const char *path);
bool slice_settings_load(SliceSettings *cfg, const char *path);
