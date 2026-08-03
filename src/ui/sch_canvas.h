#pragma once
#include "../model/project.h"
#include <raylib.h>

typedef enum { TOOL_SELECT, TOOL_WIRE, TOOL_PLACE, TOOL_LABEL, TOOL_DELETE } SchTool;

typedef struct {
    char *ref;
    char *value;
    char *symbol;
    V2 pos;
    float rotation;
    int mirrored;
} SchematicSymbol;

typedef struct {
    SchematicSymbol *symbols;
    int nsymbols;
    struct { V2 start, end; int net_index; } *wires;
    int nwires;
    V2 *junctions;
    int njunctions;
    SchTool tool;
    int dragging;
    V2 drag_offset;
    int wire_started;
    V2 wire_start_pos;
    V2 wire_end_pos;
    Vector2 pan_offset;
    float zoom;
    Rectangle viewport;
    int grid_on;
    float grid_size;
    struct {
        const char *name;
        const char *value;
        const char *mfg;
        Rectangle rect;
    } palette[16];
    int npalette;
    int placing;
    int place_index;
    V2 place_pos;
} SchCanvas;

void sch_canvas_init(SchCanvas *c, int x, int y, int w, int h);
void sch_canvas_update(SchCanvas *c, Project *p);
void sch_canvas_render(SchCanvas *c);
void sch_canvas_set_tool(SchCanvas *c, SchTool t);
void sch_canvas_zoom(SchCanvas *c, float delta);
void sch_canvas_add_symbol(SchCanvas *c, const SchematicSymbol *s);
void sch_canvas_clear(SchCanvas *c);
