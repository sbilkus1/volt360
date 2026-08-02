#include "kicad_draw.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

static float g_grid = 1.0f;
static const float g_grids[] = {0.1f, 0.25f, 0.5f, 1.0f, 2.5f, 5.0f};
static int g_grid_idx = 3;

V2 grid_snap(V2 pos, float gs) { return v2(roundf(pos.x/gs)*gs, roundf(pos.y/gs)*gs); }
float grid_size_current(void) { return g_grid; }
void grid_set_size(float mm) { g_grid = mm > 0 ? mm : 1.0f; }
void grid_toggle(void) { g_grid_idx = (g_grid_idx+1)%6; g_grid = g_grids[g_grid_idx]; }

bool is_junction(Schematic *s, V2 pos, float tol) {
    if (!s) return false; int count = 0;
    for (int i = 0; i < s->nwires; i++) {
        float d1=(s->wires[i].a.x-pos.x)*(s->wires[i].a.x-pos.x)+(s->wires[i].a.y-pos.y)*(s->wires[i].a.y-pos.y);
        float d2=(s->wires[i].b.x-pos.x)*(s->wires[i].b.x-pos.x)+(s->wires[i].b.y-pos.y)*(s->wires[i].b.y-pos.y);
        if (d1<tol*tol||d2<tol*tol) count++;
    } return count>=3;
}

int junctions_find(Schematic *s, V2 *out, int max_out, float tol) {
    if (!s||!out) return 0; int n=0;
    for (int i=0;i<s->nwires&&n<max_out;i++) {
        V2 pts[2]={s->wires[i].a,s->wires[i].b};
        for (int p=0;p<2&&n<max_out;p++) if (is_junction(s,pts[p],tol)) {
            int dup=0; for (int j=0;j<n;j++) if(fabsf(out[j].x-pts[p].x)<tol&&fabsf(out[j].y-pts[p].y)<tol)dup=1;
            if(!dup)out[n++]=pts[p];}} return n;
}

static LayerColor g_layers[]={
    {200,52,52,255,"F.Cu"},{50,180,50,255,"B.Cu"},{200,200,52,255,"F.SilkS"},{52,200,200,255,"B.SilkS"},
    {200,52,200,255,"F.Mask"},{120,120,120,255,"Edge.Cuts"},{52,52,255,255,"F.Paste"},{255,128,0,255,"B.Paste"},
};
LayerColor layer_color_get(int id){if(id>=0&&id<8)return g_layers[id];return (LayerColor){180,180,180,255,"Unknown"};}
int layer_id_from_name(const char*nm){for(int i=0;i<8;i++)if(strcmp(g_layers[i].name,nm)==0)return i;return-1;}

int ratsnest_compute(Pcb *pcb, V2 *from, V2 *to, int max_lines) {
    if (!pcb||!from||!to) return 0; int n=0;
    for (int i=0;i<pcb->nfps&&n<max_lines;i++) for (int j=i+1;j<pcb->nfps&&n<max_lines;j++) {
        int connected=0;
        for (int t=0;t<pcb->ntracks&&!connected;t++){float d1=(pcb->tracks[t].a.x-pcb->fps[i].pos.x)*(pcb->tracks[t].a.x-pcb->fps[i].pos.x)+(pcb->tracks[t].a.y-pcb->fps[i].pos.y)*(pcb->tracks[t].a.y-pcb->fps[i].pos.y);float d2=(pcb->tracks[t].b.x-pcb->fps[i].pos.x)*(pcb->tracks[t].b.x-pcb->fps[i].pos.x)+(pcb->tracks[t].b.y-pcb->fps[i].pos.y)*(pcb->tracks[t].b.y-pcb->fps[i].pos.y);if(d1<2||d2<2)connected=1;}
        if(!connected){from[n]=pcb->fps[i].pos;to[n]=pcb->fps[j].pos;n++;}} return n;
}

void pad_draw_outline(V2 pos,V2 size,float angle,int shape,int layer,float zoom,int ox,int oy){(void)pos;(void)size;(void)angle;(void)shape;(void)layer;(void)zoom;(void)ox;(void)oy;}
void via_draw(V2 pos,float outer,float drill,int ox,int oy,float zoom){(void)pos;(void)outer;(void)drill;(void)ox;(void)oy;(void)zoom;}
void symbol_draw_outline(V2 pos,float w,float h,const char*ref,const char*val,int npins,int ox,int oy,float zoom){(void)pos;(void)w;(void)h;(void)ref;(void)val;(void)npins;(void)ox;(void)oy;(void)zoom;}

int wire_manhattan(V2 start,V2 end,V2*wp,int max){if(max<3)return 0;wp[0]=start;wp[1]=v2(end.x,start.y);wp[2]=end;return 3;}

int items_in_rect(V2*pos,int n,V2 r1,V2 r2,int*sel,int max_sel){
    if(!pos||!sel)return 0;float mnx=r1.x<r2.x?r1.x:r2.x,mxx=r1.x>r2.x?r1.x:r2.x,mny=r1.y<r2.y?r1.y:r2.y,mxy=r1.y>r2.y?r1.y:r2.y;
    int c=0;for(int i=0;i<n&&c<max_sel;i++)if(pos[i].x>=mnx&&pos[i].x<=mxx&&pos[i].y>=mny&&pos[i].y<=mxy)sel[c++]=i;return c;
}

void grid_draw_kiCad_style(int x,int y,int w,int h,float zoom,V2 pan,float gs){(void)x;(void)y;(void)w;(void)h;(void)zoom;(void)pan;(void)gs;}

char *coord_status_text(V2 world,float gs){char buf[64];snprintf(buf,sizeof(buf),"X:%.2f Y:%.2f Grid:%.2f mm",world.x,world.y,gs);return str_dup(buf);}
