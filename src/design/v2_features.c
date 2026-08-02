#include "v2_features.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

unsigned char *webcam_capture_rgb(int *w, int *h) { *w=320;*h=240; unsigned char *b=(unsigned char*)malloc(320*240*3); memset(b,40,320*240*3); return b; }
int v2_webcam_list(char names[8][64]) { snprintf(names[0],64,"Integrated Camera"); snprintf(names[1],64,"USB Camera"); return 2; }
char *webcam_status_text(void) { return str_dup("Webcam: 2 device(s) [Integrated Camera, USB Camera]\nPreview: 320x240 RGB placeholder"); }

BarcodeData barcode_parse(const char *data) {
    BarcodeData bd; memset(&bd,0,sizeof(bd)); if(!data)return bd;
    const char *p=strstr(data,"(01)"); if(p){p+=4;int i=0;while(i<15&&*p>='0'&&*p<='9')bd.gtin[i++]=*p++;bd.gtin[i]=0;}
    p=strstr(data,"(21)"); if(p){p+=4;int i=0;while(i<31&&*p>='0'&&*p<='9')bd.serial[i++]=*p++;bd.serial[i]=0;}
    return bd;
}
char *rfid_simulate_scan(void) { char b[64]; snprintf(b,sizeof(b),"RFID:PLA-White-%04d",rand()%10000); return str_dup(b); }
char *qr_decode_text(const char *path) { char b[128]; snprintf(b,sizeof(b),"QR stub: %s",path?path:"no file"); return str_dup(b); }

Schematic *sheet_create_child(Project *p, const char *name, const char *parent) {
    if(!p||!name)return NULL; Schematic s; memset(&s,0,sizeof(s)); s.name=str_dup(name); arr_push(p->schematics,s);
    if(parent)for(int i=0;i<p->schematics.len;i++)if(strcmp(p->schematics.v[i].name,parent)==0) label_add_global(&p->schematics.v[i],name,v2(0,0));
    return &p->schematics.v[p->schematics.len-1];
}
void sheet_add_port(Schematic *s, const char *name, V2 pos, int dir) { (void)dir; if(!s||!name)return; label_add_global(s,name,pos); }
char *sheet_list_hierarchy(Project *p) {
    if(!p)return str_dup("no project"); char buf[1024];int off=0; off+=snprintf(buf+off,sizeof(buf)-off,"Sheet Hierarchy:\n");
    for(int i=0;i<p->schematics.len;i++){ Schematic *ss=&p->schematics.v[i]; const char *parent="root";
        for(int j=0;j<p->schematics.len;j++)if(i!=j)for(int k=0;k<p->schematics.v[j].nlabels;k++)if(strcmp(p->schematics.v[j].labels[k].name,ss->name)==0){parent=p->schematics.v[j].name;break;}
        off+=snprintf(buf+off,sizeof(buf)-off,"  [%d] %s (parent: %s) insts:%d\n",i+1,ss->name?ss->name:"?",parent,ss->ninsts);
    } return str_dup(buf);
}

int via_create_blind(Pcb *pcb, V2 pos, float drill, float outer, int end, const char *net) { return via_add_typed(pcb,pos,drill,outer,0,end,net); }
int via_create_buried(Pcb *pcb, V2 pos, float drill, float outer, int st, int en, const char *net) { return via_add_typed(pcb,pos,drill,outer,st,en,net); }
char *via_type_report(Pcb *pcb) { int t=0,b=0,r=0; via_count_types(pcb,&t,&b,&r); char buf[128]; snprintf(buf,sizeof(buf),"Vias: %d total | Through:%d Blind:%d Buried:%d",pcb->nvias,t,b,r); return str_dup(buf); }

bool symbol_pin_edit(Symbol *sym, int idx, const char *nm, const char *num, V2 pos, float ang, int pt) {
    if(!sym||idx<0||idx>=sym->npins)return false; Pin *p=&sym->pins[idx];
    if(nm){free(p->name);p->name=str_dup(nm);} if(num){free(p->number);p->number=str_dup(num);} p->pos=pos;p->angle=ang;p->ptype=pt; return true;
}
bool symbol_pin_move(Symbol *sym, int idx, V2 d) { if(!sym||idx<0||idx>=sym->npins)return false; sym->pins[idx].pos.x+=d.x;sym->pins[idx].pos.y+=d.y;return true; }
int symbol_pin_add(Symbol *sym, const char *nm, const char *num, V2 pos) {
    if(!sym)return -1; sym->pins=(Pin*)realloc(sym->pins,sizeof(Pin)*(size_t)(sym->npins+1));
    Pin *p=&sym->pins[sym->npins]; p->name=str_dup(nm?nm:"?");p->number=str_dup(num?num:"?");p->pos=pos;p->angle=0;p->ptype=0; return sym->npins++;
}
char *symbol_edit_report(Symbol *sym) {
    if(!sym)return str_dup("no symbol"); char buf[512];int off=0; off+=snprintf(buf+off,sizeof(buf)-off,"Symbol: %s (%d pins)\n",sym->name?sym->name:"?",sym->npins);
    for(int i=0;i<sym->npins;i++){Pin *p=&sym->pins[i]; off+=snprintf(buf+off,sizeof(buf)-off,"  [%d] %s %s @(%.0f,%.0f) %ddeg\n",i+1,p->name?p->name:"?",p->number?p->number:"?",p->pos.x,p->pos.y,(int)p->angle);} return str_dup(buf);
}

int paint_support_region(CadMesh *mesh, V2 screen, float radius, float zoom, V2 pan, int *painted, int max) {
    if(!mesh||!mesh->valid||!painted)return 0; int n=0; float r2=radius*radius;
    for(int t=0;t<mesh->ntris&&n<max;t++){ int i0=mesh->idx[t*3],i1=mesh->idx[t*3+1],i2=mesh->idx[t*3+2];
        float cx=(mesh->pos[i0*3]+mesh->pos[i1*3]+mesh->pos[i2*3])/3.0f,cy=(mesh->pos[i0*3+1]+mesh->pos[i1*3+1]+mesh->pos[i2*3+1])/3.0f;
        float dx=cx-screen.x,dy=cy-screen.y; if(dx*dx+dy*dy<r2)painted[n++]=t;
    } (void)zoom;(void)pan; return n;
}
int paint_seam_region(V2 *perim, int n, V2 screen, float zoom, V2 pan, int *painted, int max) {
    if(!perim||!painted||n<2)return 0; float best=1e9f;int best_i=0;for(int i=0;i<n;i++){float dx=perim[i].x-screen.x,dy=perim[i].y-screen.y;float d=sqrtf(dx*dx+dy*dy);if(d<best){best=d;best_i=i;}}
    int count=0;for(int i=best_i-2;i<=best_i+2&&count<max;i++){painted[count++]=i>=0?i:0;if(i>=n)break;} (void)zoom;(void)pan;return count;
}
char *paint_region_report(int painted, const char *type) { char b[64];snprintf(b,sizeof(b),"Painted %d %s region(s)",painted,type?type:"shape");return str_dup(b);}
