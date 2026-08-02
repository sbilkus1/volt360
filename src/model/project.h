#pragma once
// project.h - unified tool-agnostic CAD/EDA model
#include "../core/util.h"

// ---------------- schematic / footprint graphics ----------------
typedef enum { PRIM_RECT, PRIM_CIRCLE, PRIM_ARC, PRIM_LINE, PRIM_POLY, PRIM_TEXT } PrimType;

typedef struct {
    int type;
    V2 p1, p2;          // rect: two corners; line: endpoints; circle: center=p1 radius=p2.x
    float radius;       // circle radius (p2.x fallback)
    float a1, a2;       // arc start/end angles (degrees, CCW)
    float thickness;
    char *layer;        // "F.SilkS", "Edge.Cuts", "F.Fab", "Symbols" ...
    char *text;         // PRIM_TEXT content
    V2 *poly;           // PRIM_POLY points
    int npoly;
    int filled;         // rect/circle filled
} Prim;

// ---------------- symbol ----------------
typedef enum { PIN_INPUT = 0, PIN_OUTPUT, PIN_BIDIR, PIN_PASSIVE, PIN_POWER, PIN_NC } PinType;

typedef struct {
    char *name;         // e.g. "SCL"
    char *number;       // e.g. "5"
    V2 pos;             // mm relative to symbol anchor
    float angle;        // degrees, direction the pin points
    int ptype;
} Pin;

typedef struct {
    char *name;         // e.g. "LM358"
    char *reference;    // ref prefix e.g. "U"
    Pin *pins; int npins;
    Prim *prims; int nprims;
    V2 bmin, bmax;      // geometry bbox (mm)
    int has_bbox;
} Symbol;

// ---------------- footprint ----------------
typedef enum { PAD_THT, PAD_SMD } PadType;
typedef enum { PAD_RECT, PAD_CIRCLE, PAD_OVAL, PAD_ROUNDRECT } PadShape;

typedef struct {
    char *name;         // pad number e.g. "1"
    V2 pos;             // mm from footprint origin
    V2 size;            // mm
    float angle;        // degrees
    int shape;
    int layer;          // 0 front, 1 back, -1 both (THT)
    PadType type;
} Pad;

typedef struct {
    char *name;         // e.g. "SOIC-8"
    Pad *pads; int npads;
    Prim *prims; int nprims;
    char *model3d;      // relative 3D model path
    V2 bmin, bmax;
    int has_bbox;
} Footprint;

// ---------------- component (datasheet / library) ----------------
typedef struct { char *key; char *value; } Spec;
typedef struct { char *name; char *number; char *function; } PinMap;

typedef struct {
    char *id;
    char *name;
    char *description;
    char *source_file;
    char *manufacturer;
    char *package;      // package name
    char *footprint;    // default footprint name
    V3 size_mm;         // body size from datasheet
    float height_mm;
    Spec *specs; int nspecs;
    PinMap *pins; int npins;
    char *doc_text;     // datasheet text (truncated)
    char *doc_id;       // link to Doc
} Component;

// ---------------- schematic ----------------
typedef struct {
    char *id;
    char *ref;          // "U1"
    char *symbol;       // symbol name
    char *value;
    V2 pos;             // mm
    float rotation;     // degrees
    int mirrored;
} SymInst;

typedef struct { V2 a, b; } Wire;
typedef struct { char *name; V2 pos; } NetLabel;
typedef struct { char *text; V2 pos; int size; } SchText;

typedef struct {
    char *id;
    char *name;
    V2 origin;
    SymInst *insts; int ninsts;
    Wire *wires; int nwires;
    NetLabel *labels; int nlabels;
    SchText *texts; int ntexts;
} Schematic;

// ---------------- pcb ----------------
typedef struct {
    char *id;
    char *ref;          // "R1"
    char *footprint;    // footprint name
    char *value;
    char *component_id;
    V2 pos;             // mm
    float rotation;     // degrees
    int side;           // 0 front, 1 back
} FpInst;

typedef struct { V2 a, b; float width; char *net; } Track;
typedef struct { V2 pos; float drill; float outer; char *net; } Via;
typedef struct { V2 *pts; int npts; char *net; char *layer; } Zone;

typedef struct {
    char *id;
    char *name;
    float thickness;    // mm
    V2 *outline; int noutline; // closed loop, Edge.Cuts
    FpInst *fps; int nfps;
    Track *tracks; int ntracks;
    Via *vias; int nvias;
    Zone *zones; int nzones;
    Prim *prims; int nprims; // silk/fab graphics
    V2 bmin, bmax;
    int has_bbox;
} Pcb;

// ---------------- CAD model ----------------
typedef struct {
    float *pos;         // 3 * nverts
    float *nrm;         // 3 * nverts (may be NULL)
    int *idx;           // 3 * ntris
    int nverts, ntris;
    V3 bmin, bmax;
    int valid;
} CadMesh;

typedef struct {
    char *id;
    char *name;
    char *source_file;
    CadMesh mesh;
} CadModel;

// ---------------- assembly (mechanical + electrical co-design) ----------------
typedef enum { PART_CAD, PART_PCB } PartKind;

typedef struct {
    int kind;           // PART_CAD / PART_PCB
    char *part_id;      // cad model id or pcb id
    char *label;
    V3 pos;             // mm translation
    V3 rot;             // degrees (X,Y,Z)
    int flipped;
} PlaceItem;

// motion joint between two placed parts
typedef enum { JOINT_REVOLUTE, JOINT_PRISMATIC } JointType;

typedef struct {
    int type;           // JointType
    int a, b;           // indices into Assembly.items (b may be -1 = ground/world)
    V3 axis;            // rotation axis (revolute) or slide direction (prismatic)
    V3 pivot;           // pivot point in world mm
    float angle;        // current angle (rad) or displacement (mm)
    float speed;        // rad/s or mm/s
    float amp;          // oscillation amplitude
    int anim;           // 1 = animated in 3D view
} Joint;

typedef struct {
    char *id;
    char *name;
    PlaceItem *items; int nitems;
    Joint *joints; int njoints;
    int animated;       // 1 = drive joints from a time clock
    float clock;        // simulation time (s), advanced by assembly_tick
} Assembly;

// ---------------- document (datasheet) ----------------
typedef struct {
    char *id;
    char *name;
    char *source_file;
    char *text;
    int parsed;
} Doc;

// ---------------- project ----------------
typedef struct {
    char *name;
    char *path;         // project directory
    ARRAY(Symbol) symbols;
    ARRAY(Footprint) footprints;
    ARRAY(Component) components;
    ARRAY(CadModel) cad_models;
    ARRAY(Schematic) schematics;
    ARRAY(Pcb) pcbs;
    ARRAY(Assembly) assemblies;
    ARRAY(Doc) docs;
    ARRAY(char *) notes;
    char *llm_model;    // default "llama3"
    char *llm_url;      // "http://127.0.0.1:11434"
} Project;

// ---------------- lifecycle ----------------
void project_init(Project *p, const char *name, const char *path);
void project_free(Project *p);
bool project_save(Project *p);                    // writes <path>/project.json
bool project_load(Project *p, const char *dir);   // reads <dir>/project.json
char *project_save_path(Project *p);              // malloc'd

// ---------------- accessors by id ----------------
Component *project_find_component(Project *p, const char *id);
CadModel *project_find_cad(Project *p, const char *id);
Pcb *project_find_pcb(Project *p, const char *id);
Schematic *project_find_sch(Project *p, const char *id);
Assembly *project_find_assy(Project *p, const char *id);
Doc *project_find_doc(Project *p, const char *id);
Symbol *project_find_symbol(Project *p, const char *name);
Footprint *project_find_footprint(Project *p, const char *name);

// ---------------- geometry helpers ----------------
void prim_bbox(Prim *pr, V2 *bmin, V2 *bmax);
void symbol_bbox(Symbol *s);
void footprint_bbox(Footprint *f);
void pcb_bbox(Pcb *p);
void mesh_bbox(CadMesh *m);                    // compute bmin/bmax + set valid
void mesh_transform(CadMesh *src, CadMesh *dst, V3 pos, V3 rotdeg); // creates new mesh
V3 mesh_center(CadMesh *m);
float mesh_radius(CadMesh *m);                 // bounding sphere radius around center
void mesh_free(CadMesh *m);

// ---------------- shallow move helpers ----------------
void prim_free(Prim *p);
void prim_copy(Prim *dst, Prim *src);
void symbol_free_contents(Symbol *s);
void footprint_free_contents(Footprint *f);
void component_free_contents(Component *c);
void schematic_free_contents(Schematic *s);
void pcb_free_contents(Pcb *p);
void cad_free_contents(CadModel *c);
void assembly_free_contents(Assembly *a);
void doc_free_contents(Doc *d);
