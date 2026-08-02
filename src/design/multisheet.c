#include "multisheet.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void sheet_add_hierarchical(Schematic *parent, const char *child_name) {
    if (!parent || !child_name) return;
    // store child reference as a label or annotation
    NetLabel nl; nl.name = str_dup(child_name); nl.pos = v2(0, 0);
    parent->labels = (NetLabel *)realloc(parent->labels, sizeof(NetLabel) * (size_t)(parent->nlabels + 1));
    parent->labels[parent->nlabels++] = nl;
}

int sheet_children(Project *p, const char *parent_name, char ***out_names) {
    if (!p || !parent_name || !out_names) return 0;
    int count = 0;
    *out_names = NULL;
    for (int i = 0; i < p->schematics.len; i++) {
        Schematic *s = &p->schematics.v[i];
        for (int j = 0; j < s->nlabels; j++) {
            if (s->labels[j].name && strcmp(s->labels[j].name, parent_name) == 0) {
                *out_names = (char **)realloc(*out_names, sizeof(char *) * (size_t)(count + 1));
                (*out_names)[count++] = str_dup(s->name ? s->name : "?");
            }
        }
    }
    return count;
}

Schematic *sheet_find_root(Project *p) {
    if (!p || p->schematics.len == 0) return NULL;
    if (p->schematics.len == 1) return &p->schematics.v[0];
    // find schematic that's not referenced as a child by any other
    for (int i = 0; i < p->schematics.len; i++) {
        int refd = 0;
        for (int j = 0; j < p->schematics.len; j++) {
            if (i == j) continue;
            for (int k = 0; k < p->schematics.v[j].nlabels; k++)
                if (strcmp(p->schematics.v[j].labels[k].name, p->schematics.v[i].name) == 0) { refd = 1; break; }
            if (refd) break;
        }
        if (!refd) return &p->schematics.v[i];
    }
    return &p->schematics.v[0]; // fallback
}

char *sheet_hierarchy_report(Project *p) {
    if (!p) return str_dup("no project");
    char buf[2048]; int off = 0;
    Schematic *root = sheet_find_root(p);
    off += snprintf(buf+off, sizeof(buf)-off,
        "=== HIERARCHY ===\n"
        "Root: %s\n"
        "Total sheets: %d\n",
        root ? (root->name ? root->name : "?") : "none", p->schematics.len);
    for (int i = 0; i < p->schematics.len; i++) {
        Schematic *s = &p->schematics.v[i];
        off += snprintf(buf+off, sizeof(buf)-off,
            "  [%d] %s  (%d insts, %d wires, %d labels)\n",
            i+1, s->name ? s->name : "?", s->ninsts, s->nwires, s->nlabels);
    }
    return str_dup(buf);
}
