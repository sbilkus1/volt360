#include "drawing2d.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>

char *drawing2d_from_mesh(CadMesh *mesh, const char *title) {
    if (!mesh || !mesh->valid) return str_dup("No mesh for drawing");
    float w = mesh->bmax.x - mesh->bmin.x;
    float d = mesh->bmax.y - mesh->bmin.y;
    float h = mesh->bmax.z - mesh->bmin.z;
    char buf[2048]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off,
        "==========================================\n"
        "  2D DRAWING: %s\n"
        "==========================================\n"
        "  Scale: 1:1   Units: mm\n"
        "\n"
        "  FRONT VIEW (XY)\n"
        "  +-------------+      \n"
        "  |              |     W=%.1f mm\n"
        "  |              |     H=%.1f mm\n"
        "  |              |     \n"
        "  +-------------+      \n"
        "  |--- %.1f mm ----|\n"
        "\n"
        "  TOP VIEW (XZ)\n"
        "  +-------------+      \n"
        "  |              |     W=%.1f mm\n"
        "  |              |     D=%.1f mm\n"
        "  |              |     \n"
        "  +-------------+      \n"
        "  |--- %.1f mm ----|\n"
        "\n"
        "  RIGHT VIEW (YZ)\n"
        "  +-------------+      \n"
        "  |              |     D=%.1f mm\n"
        "  |              |     H=%.1f mm\n"
        "  |              |     \n"
        "  +-------------+      \n"
        "  |--- %.1f mm ----|\n"
        "\n"
        "  DIMENSIONS SUMMARY:\n"
        "  Width  (X): %.1f mm\n"
        "  Depth  (Y): %.1f mm\n"
        "  Height (Z): %.1f mm\n"
        "  Volume:     (use mesh_mass_props)\n"
        "  Vertices:   %d   Triangles: %d\n"
        "==========================================\n",
        title ? title : "Untitled",
        w, h, w,
        w, d, w,
        d, h, d,
        w, d, h,
        mesh->nverts, mesh->ntris);
    return str_dup(buf);
}

char *drawing2d_outline(CadMesh *mesh) {
    if (!mesh || !mesh->valid || mesh->ntris < 1) return str_dup("No mesh");
    char buf[4096]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off, "DXF-LIKE OUTLINE\n================\n");
    // project all vertices to XY, find convex outline (simple bbox)
    off += snprintf(buf + off, sizeof(buf) - off,
        "Bounding Box: (%.1f,%.1f) - (%.1f,%.1f)\n",
        mesh->bmin.x, mesh->bmin.y, mesh->bmax.x, mesh->bmax.y);
    off += snprintf(buf + off, sizeof(buf) - off,
        "Outline: %.1fx%.1f mm rectangle\n",
        mesh->bmax.x - mesh->bmin.x, mesh->bmax.y - mesh->bmin.y);
    return str_dup(buf);
}

char *drawing2d_bom_sheet(Project *p, const char *title) {
    if (!p) return str_dup("No project");
    char buf[4096]; int off = 0;
    off += snprintf(buf + off, sizeof(buf) - off,
        "==========================================\n"
        "  BILL OF MATERIALS: %s\n"
        "==========================================\n"
        "  #   Component        Value        Package\n"
        "  --- ------------     ---------    ---------\n",
        title ? title : p->name);
    for (int i = 0; i < p->components.len; i++) {
        Component *c = &p->components.v[i];
        off += snprintf(buf + off, sizeof(buf) - off, "  %-3d %-16s %-12s %-12s\n",
            i + 1,
            c->name ? c->name : "?",
            c->manufacturer ? c->manufacturer : "-",
            c->package ? c->package : "-");
    }
    if (p->components.len == 0)
        off += snprintf(buf + off, sizeof(buf) - off, "  (no components)\n");
    off += snprintf(buf + off, sizeof(buf) - off,
        "==========================================\n"
        "  Total items: %d\n"
        "==========================================\n", p->components.len);
    return str_dup(buf);
}
