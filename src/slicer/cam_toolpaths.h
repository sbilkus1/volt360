#pragma once
/* cam_toolpaths.h - Real CAM toolpath generation (structured output).
 * Generates toolpath arrays suitable for post-processing into G-code.
 */
#include <stdbool.h>
#include "../model/project.h"

typedef struct {
    float diameter;
    float feedrate;
    float rpm;
    float stepover;
} CamTool;

typedef struct {
    float width, height, depth;
} CamMaterial;

typedef struct {
    float x, y, z;
    int code;
    float feed;
} CamPoint;

typedef struct {
    float mnx, mny, mxx, mxy;
    float minz, maxz;
} CamBBox;

typedef struct {
    CamPoint *pts;
    int npts;
    int cap;
} CamPath;

void campath_init(CamPath *p);
void campath_free(CamPath *p);
void campath_add(CamPath *p, float x, float y, float z, int code, float feed);

/* Face milling: zigzag over rectangular area. */
int cam_generate_face(CamPath *paths, int max_paths, CamTool *t, CamBBox *area, float depth);

/* Pocket milling: offset contour toolpaths, spiral inward. */
int cam_generate_pocket(CamPath *paths, int max_paths, CamTool *t, V2 *poly, int npts, float depth);

/* Profile milling: follow boundary with lead-in/lead-out arcs. */
int cam_generate_profile(CamPath *paths, int max_paths, CamTool *t, V2 *poly, int npts, float depth);

/* Drilling: canned G81 cycles for hole positions. */
int cam_generate_drill(CamPath *paths, int max_paths, CamTool *t, V2 *holes, int nholes, float depth);

/* Calculate bounding box from a polygon. */
void campath_bbox(V2 *poly, int n, CamBBox *bb);
