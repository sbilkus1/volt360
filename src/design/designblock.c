#include "designblock.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

DesignBlock *block_save(Pcb *pcb, V2 origin, float w, float h, const char *name) {
    if (!pcb) return NULL;
    DesignBlock *blk = (DesignBlock *)calloc(1, sizeof(DesignBlock));
    snprintf(blk->name, sizeof(blk->name), "%s", name ? name : "block");
    blk->origin = origin; blk->width = w; blk->height = h;

    // capture tracks within region
    for (int i = 0; i < pcb->ntracks; i++) {
        Track *t = &pcb->tracks[i];
        if (t->a.x >= origin.x && t->a.x <= origin.x + w &&
            t->a.y >= origin.y && t->a.y <= origin.y + h &&
            t->b.x >= origin.x && t->b.x <= origin.x + w &&
            t->b.y >= origin.y && t->b.y <= origin.y + h) {
            blk->tracks = (Track *)realloc(blk->tracks, sizeof(Track)*(size_t)(blk->ntracks+1));
            blk->tracks[blk->ntracks] = *t;
            blk->tracks[blk->ntracks].a.x -= origin.x;
            blk->tracks[blk->ntracks].a.y -= origin.y;
            blk->tracks[blk->ntracks].b.x -= origin.x;
            blk->tracks[blk->ntracks].b.y -= origin.y;
            if (t->net) blk->tracks[blk->ntracks].net = str_dup(t->net);
            blk->ntracks++;
        }
    }
    // capture footprints within region
    for (int i = 0; i < pcb->nfps; i++) {
        if (pcb->fps[i].pos.x >= origin.x && pcb->fps[i].pos.x <= origin.x+w &&
            pcb->fps[i].pos.y >= origin.y && pcb->fps[i].pos.y <= origin.y+h) {
            blk->fp_indices = (int *)realloc(blk->fp_indices, sizeof(int)*(size_t)(blk->nfps+1));
            blk->fp_indices[blk->nfps++] = i;
        }
    }
    return blk;
}

int block_restore(Pcb *pcb, DesignBlock *blk, V2 new_origin) {
    if (!pcb || !blk) return 0;
    int added = 0;
    for (int i = 0; i < blk->ntracks; i++) {
        Track t = blk->tracks[i];
        t.a.x += new_origin.x; t.a.y += new_origin.y;
        t.b.x += new_origin.x; t.b.y += new_origin.y;
        if (t.net) t.net = str_dup(t.net);
        pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track)*(size_t)(pcb->ntracks+1));
        pcb->tracks[pcb->ntracks++] = t;
        added++;
    }
    return added;
}

void block_free(DesignBlock *blk) {
    if (!blk) return;
    for (int i = 0; i < blk->ntracks; i++) free(blk->tracks[i].net);
    free(blk->tracks); free(blk->fp_indices); free(blk);
}
