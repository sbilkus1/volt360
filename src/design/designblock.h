#pragma once
// designblock.h - PCB design blocks: save/restore layout regions.
#include <stdbool.h>
#include "../model/project.h"

// A design block captures tracks + footprints within a rectangular region.
typedef struct {
    char name[64];
    V2 origin;          // block origin in board coordinates
    float width, height;
    Track *tracks; int ntracks;
    int *fp_indices; int nfps; // indices into original footprint array
} DesignBlock;

// Save a rectangular region from a PCB as a design block.
DesignBlock *block_save(Pcb *pcb, V2 origin, float w, float h, const char *name);

// Restore a design block into a PCB at a new position.
int block_restore(Pcb *pcb, DesignBlock *blk, V2 new_origin);

// Free a design block.
void block_free(DesignBlock *blk);
