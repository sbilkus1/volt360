#pragma once
/* treeview.h - Project browser tree (left sidebar) */
#include <stdbool.h>
#include <raylib.h>

#define TREE_MAX_NODES 256
#define TREE_MAX_CHILDREN 64

typedef struct TreeNode {
    char label[64];
    int expanded;
    int selected;
    int parent_idx;
    int children[64];
    int nchildren;
    int depth;
    int id;
} TreeNode;

typedef struct {
    TreeNode nodes[TREE_MAX_NODES];
    int nnodes;
    int root_idx;
    Rectangle rect;
    int scroll_offset;
    Color bg;
    Color text;
    Color select_bg;
    char search[64];
    int sel_idx;
} ProjectTree;

void tree_init(ProjectTree *t, int x, int y, int w, int h);
int tree_render(ProjectTree *t);
int tree_add_node(ProjectTree *t, int parent_id, const char *name);
void tree_clear(ProjectTree *t);
int tree_get_selected(ProjectTree *t);
