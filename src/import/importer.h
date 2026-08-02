#pragma once
// importer.h - all file import entry points (mutate Project in place)
#include "../model/project.h"

typedef struct ImportResult {
    int ok;
    int ncomponents, nsymbols, nfootprints, ncad, nsch, npcb, ndocs;
    char message[512];
} ImportResult;

void import_result_init(ImportResult *r);

// import a single file based on extension
ImportResult importer_import_file(Project *p, const char *path);
// import a whole directory tree (recursive)
ImportResult importer_import_dir(Project *p, const char *dir);
// import a zip: extract to temp, import every supported file inside
ImportResult importer_import_zip(Project *p, const char *zip_path);
// batch: dispatch on extension incl. zip
ImportResult importer_import_any(Project *p, const char *path);

// ---- datasheet text extraction (malloc'd out) ----
bool importer_md_to_text(const char *path, char **out_text);
bool importer_csv_to_text(const char *path, char **out_text);
bool importer_docx_to_text(const char *path, char **out_text);
char *importer_text_from_file(const char *path); // by extension, NULL if unsupported

// ---- meshes ----
bool mesh_load_stl(const char *path, CadMesh *out);
bool mesh_load_obj(const char *path, CadMesh *out);
bool mesh_load_step(const char *path, CadMesh *out);
bool mesh_load_any(const char *path, CadMesh *out);

// ---- kicad ----
bool import_kicad_sch(const char *path, Project *p, Schematic *out);
bool import_kicad_pcb(const char *path, Project *p, Pcb *out);
bool import_kicad_sym(const char *path, Project *p, Symbol *out);
bool import_kicad_mod(const char *path, Project *p, Footprint *out);

// ---- s-expression ----
typedef struct SExprNode {
    char *atom;                 // NULL for lists
    ARRAY(struct SExprNode *) kids; // list children
    int is_list;
} SExprNode;

SExprNode *sexpr_parse(const char *data, size_t len);
void sexpr_free(SExprNode *n);
const char *sexpr_atom(SExprNode *n, int i);        // atom of child i ("" if list)
int sexpr_child_count(SExprNode *n);
SExprNode *sexpr_get(SExprNode *n, int i);          // child i
const char *sexpr_name(SExprNode *n);               // first child atom (name)
double sexpr_num(SExprNode *n, int i);              // numeric value of child i
SExprNode *sexpr_find_child(SExprNode *n, const char *name); // first child list/atom with that name (by first atom)
const char *sexpr_str(SExprNode *n, int i);         // atom child i quoted-string content

// ---- eagle ----
bool import_eagle_sch(const char *path, Project *p, Schematic *out);
bool import_eagle_brd(const char *path, Project *p, Pcb *out);

// ---- librepcb (JSON) ----
bool import_librepcb_project(const char *path, Project *p); // .lpp
bool import_librepcb_lib(const char *path, Project *p);     // .lp_sym/.lp_fp/.lp_cmp/.lp_dev/.lp_pkg

// ---- netlists ----
bool import_netlist(const char *path, Project *p);  // .net/.cir/.asc -> Schematic as netlist

// ---- svg/dxf ----
bool svg_import_pcb(const char *path, Pcb *out);
bool dxf_import(const char *path, Pcb *out);

// ---- additional meshes ----
bool mesh_import_ply(const char *path, CadMesh *out);
