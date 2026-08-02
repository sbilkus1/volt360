#include "painting.h"
#include <stdlib.h>
#include <string.h>

void paint_data_free(PaintData *pd) {
    if (!pd) return;
    for (int i = 0; i < pd->n_regions; i++) free(pd->regions[i].tri_indices);
    free(pd->regions);
    memset(pd, 0, sizeof(*pd));
}

void paint_region_add(PaintData *pd, int *tris, int n_tris, PaintType type, int color) {
    if (!pd || n_tris < 1) return;
    pd->regions = (PaintedRegion *)realloc(pd->regions, sizeof(PaintedRegion) * (size_t)(pd->n_regions + 1));
    PaintedRegion *pr = &pd->regions[pd->n_regions++];
    pr->n_tris = n_tris;
    pr->tri_indices = (int *)malloc(sizeof(int) * (size_t)n_tris);
    memcpy(pr->tri_indices, tris, sizeof(int) * (size_t)n_tris);
    pr->type = type;
    pr->color_index = color;
}

void paint_region_clear(PaintData *pd, PaintType type) {
    if (!pd) return;
    for (int i = pd->n_regions - 1; i >= 0; i--)
        if (pd->regions[i].type == type) {
            free(pd->regions[i].tri_indices);
            for (int j = i; j < pd->n_regions - 1; j++) pd->regions[j] = pd->regions[j+1];
            pd->n_regions--;
        }
}

bool paint_is_painted(PaintData *pd, int tri_idx, PaintType type) {
    if (!pd) return false;
    for (int i = 0; i < pd->n_regions; i++)
        if (pd->regions[i].type == type)
            for (int j = 0; j < pd->regions[i].n_tris; j++)
                if (pd->regions[i].tri_indices[j] == tri_idx) return true;
    return false;
}
