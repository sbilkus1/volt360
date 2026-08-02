#include "dxf_import.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool dxf_import(const char *path, Pcb *out) {
    if (!path || !out) return false;
    FILE *f = fopen(path, "r"); if (!f) return false;
    memset(out, 0, sizeof(*out));
    out->id = str_dup(make_id());
    const char *slash = strrchr(path, '/'); const char *bs = strrchr(path, '\\');
    if (bs && bs > slash) slash = bs;
    out->name = str_dup(slash ? slash + 1 : path);

    float x1 = 0, y1 = 0, x2 = 0, y2 = 0, cx = 0, cy = 0, cr = 0;
    int in_entity = 0;
    char ent_type[32] = "";
    float vx[64]; float vy[64]; int nv = 0;
    char line[256]; int group = 0;
    while (fgets(line, sizeof(line), f)) {
        // trim
        char *nl = line + strlen(line) - 1;
        while (nl >= line && (*nl == '\r' || *nl == '\n')) *nl-- = '\0';
        if (group == 0) { group = atoi(line); continue; }

        if (group == 0) {
            if (strcmp(line, "LINE") == 0 || strcmp(line, "LWPOLYLINE") == 0 || strcmp(line, "CIRCLE") == 0) {
                strncpy(ent_type, line, sizeof(ent_type)); in_entity = 1; x1=y1=x2=y2=0; nv=0;
            }
        } else if (in_entity) {
            switch (group) {
                case 10: x1 = (float)atof(line); break;
                case 20: y1 = (float)atof(line); break;
                case 11: x2 = (float)atof(line); break;
                case 21: y2 = (float)atof(line); break;
                case 40: cr = (float)atof(line); break;
            }
            // LWPOLYLINE vertex
            if (strcmp(ent_type, "LWPOLYLINE") == 0) {
                if (group == 10) { if (nv < 64) { vx[nv] = (float)atof(line); vy[nv] = 0; nv++; } }
                else if (group == 20 && nv > 0) vy[nv-1] = (float)atof(line);
            }
        }
        // finalize entity
        if (strcmp(line, "ENDSEC") == 0 || strcmp(line, "EOF") == 0) {
            if (in_entity && strcmp(ent_type, "LINE") == 0) {
                Track t; t.a = v2(x1,y1); t.b = v2(x2,y2); t.width = 0.1f; t.net = NULL;
                out->tracks = (Track *)realloc(out->tracks, sizeof(Track)*(size_t)(out->ntracks+1));
                out->tracks[out->ntracks++] = t;
            } else if (in_entity && strcmp(ent_type, "LWPOLYLINE") == 0 && nv >= 2) {
                for (int i = 0; i < nv-1; i++) {
                    Track t; t.a = v2(vx[i],vy[i]); t.b = v2(vx[i+1],vy[i+1]); t.width = 0.1f; t.net = NULL;
                    out->tracks = (Track *)realloc(out->tracks, sizeof(Track)*(size_t)(out->ntracks+1));
                    out->tracks[out->ntracks++] = t;
                }
                // add to outline if large enough
                if (nv >= 3 && !out->noutline) {
                    out->noutline = nv;
                    out->outline = (V2 *)malloc(sizeof(V2)*(size_t)nv);
                    for (int i = 0; i < nv; i++) out->outline[i] = v2(vx[i], vy[i]);
                }
            }
            in_entity = 0;
        }
        group = 0;
    }
    fclose(f);
    if (out->noutline == 0 && out->ntracks > 0) {
        // compute from tracks bbox
        float mnx=out->tracks[0].a.x, mny=out->tracks[0].a.y, mxx=mnx, mxy=mny;
        for (int i=0;i<out->ntracks;i++) {
            if(out->tracks[i].a.x<mnx)mnx=out->tracks[i].a.x; if(out->tracks[i].a.x>mxx)mxx=out->tracks[i].a.x;
            if(out->tracks[i].a.y<mny)mny=out->tracks[i].a.y; if(out->tracks[i].a.y>mxy)mxy=out->tracks[i].a.y;
        }
        out->bmin=v2(mnx,mny); out->bmax=v2(mxx,mxy); out->has_bbox=true;
    } else pcb_bbox(out);
    return out->ntracks > 0 || out->noutline > 0;
}
