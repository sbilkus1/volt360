#include "more_414.h"
#include "designblock.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// ===== OBJECT MANIPULATION =====
void object_scale(CadModel *cm, float sx, float sy, float sz) { if(cm&&cm->mesh.valid)mesh_scale(&cm->mesh,sx,sy,sz); }
void object_rotate(CadModel *cm, float rx, float ry, float rz) {
    if(!cm||!cm->mesh.valid)return; float ax=rx*0.0174533f,ay=ry*0.0174533f,az=rz*0.0174533f;
    for(int i=0;i<cm->mesh.nverts;i++){float x=cm->mesh.pos[i*3],y=cm->mesh.pos[i*3+1],z=cm->mesh.pos[i*3+2];
        float nx=x*cosf(az)-y*sinf(az),ny=x*sinf(az)+y*cosf(az);x=nx;y=ny;
        nx=x*cosf(ay)+z*sinf(ay);float nz=-x*sinf(ay)+z*cosf(ay);z=nz;
        ny=y*cosf(ax)-z*sinf(ax);nz=y*sinf(ax)+z*cosf(ax);
        cm->mesh.pos[i*3]=nx;cm->mesh.pos[i*3+1]=ny;cm->mesh.pos[i*3+2]=nz;}
    mesh_bbox(&cm->mesh);
}
int object_split_add(Project *p, CadModel *cm) { return object_split_into_project(p,&cm->mesh); }
char *object_manip_report(CadModel *cm) { char b[128];snprintf(b,sizeof(b),"Model: %d verts %d tris bbox %.0fx%.0fx%.0f",cm?cm->mesh.nverts:0,cm?cm->mesh.ntris:0,cm?cm->mesh.bmax.x-cm->mesh.bmin.x:0,cm?cm->mesh.bmax.y-cm->mesh.bmin.y:0,cm?cm->mesh.bmax.z-cm->mesh.bmin.z:0);return str_dup(b);}

// ===== OVERHANG =====
float overhang_detect_pct(SliceResult *sr) {
    if(!sr||sr->nlayers<2)return 0;int over=0,total=0;
    for(int li=1;li<sr->nlayers;li++){SliceLayer*L=&sr->layers[li];SliceLayer*Lb=&sr->layers[li-1];
        for(int j=0;j<L->npaths;j++)if(L->paths[j].kind==0){total+=L->paths[j].npts;
            V2*c=L->paths[j].pts;V2*pb=NULL;int pn=0;for(int k=0;k<Lb->npaths;k++)if(Lb->paths[k].kind==0&&Lb->paths[k].npts>pn){pn=Lb->paths[k].npts;pb=Lb->paths[k].pts;}
            if(pb)for(int k=0;k<L->paths[j].npts;k++){int in=0;for(int m=0,nn=pn-1;m<pn;nn=m++)if(((pb[m].y>c[k].y)!=(pb[nn].y>c[k].y))&&(c[k].x<(pb[nn].x-pb[m].x)*(c[k].y-pb[m].y)/(pb[nn].y-pb[m].y)+pb[m].x))in=!in;if(!in)over++;}}}
    return total>0?over*100.0f/total:0;
}
char *overhang_report(SliceResult *sr) { float p=overhang_detect_pct(sr);char b[64];snprintf(b,sizeof(b),"Overhang: %.0f%% of perimeter",p);return str_dup(b);}

// ===== PURCHASE FORECASTING =====
int purchase_forecast_run(Farm *f, PurchaseForecast *fcs, int max) {
    if(!f||!fcs)return 0;int n=0;
    for(int i=0;i<f->n_spools&&n<max;i++){int dup=0;for(int j=0;j<n;j++)if(strcmp(fcs[j].filament,f->spools[i].filament)==0)dup=1;if(!dup){PurchaseForecast*pf=&fcs[n];snprintf(pf->filament,32,"%s",f->spools[i].filament);pf->used_per_week=f->spools[i].used_m*0.5f;pf->spool_cost=25.0f;pf->weeks_remaining=(int)(f->spools[i].remaining_m/(pf->used_per_week>0?pf->used_per_week:10.0f));n++;}}
    return n;
}
char *purchase_forecast_report(PurchaseForecast *fcs, int n) {
    char buf[1024];int off=0;off+=snprintf(buf+off,sizeof(buf)-off,"Purchase Forecast:\n");
    for(int i=0;i<n;i++)off+=snprintf(buf+off,sizeof(buf)-off,"  %s: %.0fm/wk %d wks remaining ~$%.2f/spool\n",fcs[i].filament,fcs[i].used_per_week,fcs[i].weeks_remaining,fcs[i].spool_cost);
    return str_dup(buf);
}

// ===== SMARTTAGS =====
char *smarttags_search(Farm *f, const char *q) { return farm_tag_find(f,q,(char*)malloc(512),512); }
char *smarttags_list(Farm *f) {
    char buf[1024];int off=0;off+=snprintf(buf+off,sizeof(buf)-off,"Tags:\n");
    for(int i=0;i<f->n_files;i++)for(int j=0;j<f->files[i].n_tags;j++)off+=snprintf(buf+off,sizeof(buf)-off,"  %s: %s\n",f->files[i].name,f->files[i].tags[j]);
    return str_dup(buf);
}

// ===== DESIGN BLOCKS =====
static DesignBlock g_blocks[16];static int g_nb=0;
char *designblock_save_region(Pcb *pcb, V2 o, float w, float h, const char *nm) {
    if(g_nb>=16)return str_dup("block limit reached");
    DesignBlock *b = block_save(pcb,o,w,h,nm); if(!b)return str_dup("save failed");
    g_blocks[g_nb]=*b;g_nb++;char buf[64];snprintf(buf,sizeof(buf),"Block '%s' saved",nm?nm:"?");free(b);return str_dup(buf);
}
char *designblock_list(void) { char buf[512];int off=0;off+=snprintf(buf+off,sizeof(buf)-off,"Design Blocks:\n");for(int i=0;i<g_nb;i++)off+=snprintf(buf+off,sizeof(buf)-off,"  [%d] %s (%.0fx%.0f)\n",i+1,g_blocks[i].name,g_blocks[i].width,g_blocks[i].height);return str_dup(buf);}
int designblock_apply(Pcb *pcb, const char *nm, V2 pos) {
    for(int i=0;i<g_nb;i++)if(strcmp(g_blocks[i].name,nm)==0)return block_restore(pcb,&g_blocks[i],pos);
    return 0;
}

// ===== FOOTPRINT WIZARD =====
Footprint *footprint_wizard_smd(Project *p, const char *nm, int npads, float pitch, float pw, float ph) {
    if(!p)return NULL;Footprint fp;memset(&fp,0,sizeof(fp));fp.name=str_dup(nm?nm:"SMD");fp.npads=npads;fp.pads=(Pad*)calloc((size_t)npads,sizeof(Pad));
    for(int i=0;i<npads;i++){char n[8];snprintf(n,sizeof(n),"%d",i+1);fp.pads[i].name=str_dup(n);fp.pads[i].pos=v2(i*pitch-pitch*(npads-1)*0.5f,0);fp.pads[i].size=v2(pw,ph);fp.pads[i].shape=PAD_RECT;fp.pads[i].type=PAD_SMD;}
    fp.bmin=v2(-pitch*npads*0.5f-ph*0.5f,-pw*0.5f);fp.bmax=v2(pitch*npads*0.5f+ph*0.5f,pw*0.5f);fp.has_bbox=1;arr_push(p->footprints,fp);return &p->footprints.v[p->footprints.len-1];
}
Footprint *footprint_wizard_dip(Project *p, const char *nm, int npins, float pitch, float rs) {
    if(!p)return NULL;Footprint fp;memset(&fp,0,sizeof(fp));fp.name=str_dup(nm?nm:"DIP");fp.npads=npins*2;fp.pads=(Pad*)calloc((size_t)(npins*2),sizeof(Pad));
    for(int i=0;i<npins;i++){char n[8];snprintf(n,sizeof(n),"%d",i+1);fp.pads[i*2].name=str_dup(n);fp.pads[i*2].pos=v2(i*pitch-pitch*(npins-1)*0.5f,-rs*0.5f);fp.pads[i*2].size=v2(1.5f,2.5f);fp.pads[i*2].shape=PAD_RECT;fp.pads[i*2].type=PAD_THT;
        snprintf(n,sizeof(n),"%d",npins+i+1);fp.pads[i*2+1].name=str_dup(n);fp.pads[i*2+1].pos=v2(i*pitch-pitch*(npins-1)*0.5f,rs*0.5f);fp.pads[i*2+1].size=v2(1.5f,2.5f);fp.pads[i*2+1].shape=PAD_RECT;fp.pads[i*2+1].type=PAD_THT;}
    fp.bmin=v2(-pitch*npins*0.5f-2,-rs*0.5f-2);fp.bmax=v2(pitch*npins*0.5f+2,rs*0.5f+2);fp.has_bbox=1;arr_push(p->footprints,fp);return &p->footprints.v[p->footprints.len-1];
}
char *footprint_wizard_report(Footprint *fp) { char b[128];snprintf(b,sizeof(b),"Footprint: %s, %d pads",fp?fp->name:"?",fp?fp->npads:0);return str_dup(b);}

// ===== DIFFERENTIAL PAIR RULES =====
static DiffPairRules g_dprules={0.2f,0.3f,5.0f,100.0f};
void difpair_rules_set(Pcb *pcb, DiffPairRules r) { g_dprules=r;(void)pcb;}
DiffPairRules difpair_rules_get(void) { return g_dprules; }
char *difpair_rules_report(DiffPairRules r) { char b[128];snprintf(b,sizeof(b),"DiffPair: %.2fmm/%.2fmm gap %.1fmm unpaired Z0=%.0fohm",r.track_width,r.track_spacing,r.max_unpaired_length,r.target_impedance);return str_dup(b);}

// ===== 3MF + IRONING =====
bool mesh_import_3mf_stub(const char *path, CadMesh *out) { (void)path;(void)out;return false; }
char *ironing_adaptive_settings(SliceResult *sr, const SliceSettings *cfg) {
    char buf[128];snprintf(buf,sizeof(buf),"Adaptive ironing: %d layers. Ironing on top %d layers",sr?sr->nlayers:0,cfg?cfg->solid_top_layers:3);return str_dup(buf);
}
