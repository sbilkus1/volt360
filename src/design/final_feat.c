#include "final_feat.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

// ===== MEASUREMENTS =====
float measure_distance(MeasurePoint a, MeasurePoint b) { return sqrtf((b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y)); }
float measure_angle(MeasurePoint a, MeasurePoint b, MeasurePoint c) {
    float dx1=b.x-a.x,dy1=b.y-a.y,dx2=c.x-a.x,dy2=c.y-a.y;
    float dot=dx1*dx2+dy1*dy2,len1=sqrtf(dx1*dx1+dy1*dy1),len2=sqrtf(dx2*dx2+dy2*dy2);
    return len1>0&&len2>0?acosf(dot/(len1*len2))*180.0f/(float)M_PI:0;
}
char *measure_report(MeasurePoint a, MeasurePoint b) {
    char buf[64]; snprintf(buf,sizeof(buf),"Distance: %.2f mm",measure_distance(a,b)); return str_dup(buf);
}

// ===== CROSS-PROBE =====
const char *crossprobe_find_schematic_ref(Project *p, const char *fp_ref) {
    if(!p||!fp_ref)return NULL;
    for(int i=0;i<p->schematics.len;i++)for(int j=0;j<p->schematics.v[i].ninsts;j++)if(p->schematics.v[i].insts[j].ref&&strcmp(p->schematics.v[i].insts[j].ref,fp_ref)==0)return p->schematics.v[i].name;return NULL;
}
const char *crossprobe_find_pcb_fp(Project *p, const char *sch_ref) {
    if(!p||!sch_ref)return NULL;
    for(int i=0;i<p->pcbs.len;i++)for(int j=0;j<p->pcbs.v[i].nfps;j++)if(p->pcbs.v[i].fps[j].ref&&strcmp(p->pcbs.v[i].fps[j].ref,sch_ref)==0)return p->pcbs.v[i].name;return NULL;
}
char *crossprobe_highlight(Project *p, const char *ref) {
    if(!p)return str_dup("no project"); char buf[256];
    const char *sch=crossprobe_find_schematic_ref(p,ref); const char *pcb=crossprobe_find_pcb_fp(p,ref);
    snprintf(buf,sizeof(buf),"Cross-probe %s: SCH=%s PCB=%s",ref?ref:"?",sch?sch:"-",pcb?pcb:"-"); return str_dup(buf);
}

// ===== OBJECT SPLIT =====
int object_split_count(CadMesh *m) { return mesh_component_count(m); }
char *object_split_report(CadMesh *m) {
    int n=object_split_count(m); char buf[64]; snprintf(buf,sizeof(buf),"Components: %d (split with Object Split button)",n); return str_dup(buf);
}
int object_split_into_project(Project *p, CadMesh *m) {
    if(!p||!m)return 0; int n=mesh_component_count(m); if(n<=1)return n;
    CadMesh *comps=(CadMesh*)malloc(sizeof(CadMesh)*(size_t)n); int nc=mesh_split_components(m,comps);
    for(int i=0;i<nc;i++){ CadModel c; memset(&c,0,sizeof(c));c.id=str_dup(make_id());char nm[64];snprintf(nm,sizeof(nm),"part-%d",i+1);c.name=str_dup(nm);c.mesh=comps[i];arr_push(p->cad_models,c);}
    free(comps); return nc;
}

// ===== MAINTENANCE + FIRMWARE =====
static char g_maint_log[16][128]; static int g_mlog=0;
void maint_log_event(Farm *f, const char *printer, const char *task, float hrs) {
    (void)f; if(g_mlog>=16)return; snprintf(g_maint_log[g_mlog],128,"%s: %s @ %.0fh",printer?printer:"?",task?task:"?",hrs); g_mlog++;
}
char *maint_history_report(Farm *f) {
    (void)f; char buf[2048];int off=0; off+=snprintf(buf+off,sizeof(buf)-off,"Maintenance History:\n");
    for(int i=0;i<g_mlog;i++) off+=snprintf(buf+off,sizeof(buf)-off,"  %s\n",g_maint_log[i]);
    if(g_mlog==0) off+=snprintf(buf+off,sizeof(buf)-off,"  No events logged\n"); return str_dup(buf);
}
char *firmware_update_status(const char *printer, const char *ver) {
    char buf[128]; snprintf(buf,sizeof(buf),"%s: firmware %s (stub - Klipper auto-update via Moonraker)",printer?printer:"?",ver?ver:"latest"); return str_dup(buf);
}

// ===== DESIGN BLOCK + STACKUP =====
char *designblock_visual_preview(Pcb *pcb, V2 o, float w, float h) { return designblock_preview(pcb,o,w,h); }

char *stackup_default_4layer(void) {
    return str_dup("4-Layer Stackup:\n  L1: Signal (0.035mm Cu) | 0.2mm prepreg er=4.2\n  L2: GND (0.035mm Cu) | 1.2mm FR4 core er=4.6\n  L3: VCC (0.035mm Cu) | 0.2mm prepreg er=4.2\n  L4: Signal (0.035mm Cu)\n  Total: 1.6mm FR4");
}
char *stackup_impedance_report(LayerStack *ls, int n, float z0) {
    char buf[512];int off=0; off+=snprintf(buf+off,sizeof(buf)-off,"Impedance target: %.0f ohm\n",z0);
    for(int i=0;i<n;i++) off+=snprintf(buf+off,sizeof(buf)-off,"  %s: %.3fmm er=%.1f\n",ls[i].name,ls[i].thickness,ls[i].er);
    return str_dup(buf);
}

// ===== AI DESIGN =====
char *ai_explain_design(Pcb *pcb) {
    int r=0,c=0; for(int i=0;i<pcb->ntracks;i++){if(pcb->tracks[i].width<0.3f)r++;if(pcb->tracks[i].width>0.5f)c++;}
    char buf[512]; snprintf(buf,sizeof(buf),"Design Analysis:\n  %d tracks (%d thin high-speed, %d power)\n  %d components\n  Likely a mixed-signal board with %s",
        pcb->ntracks,r,c,pcb->nfps,pcb->nfps>5?"moderate complexity":"simple design"); return str_dup(buf);
}
char *ai_troubleshoot(Pcb *pcb, const char *issue) {
    char buf[256]; snprintf(buf,sizeof(buf),"Troubleshooting '%s':\n  Check DRC for clearance violations\n  Verify trace widths for current capacity\n  Ensure ground plane continuity",issue?issue:"?"); (void)pcb; return str_dup(buf);
}
char *ai_suggest_improvements(Pcb *pcb) {
    char buf[384]; snprintf(buf,sizeof(buf),"Improvement Suggestions:\n  - Add teardrops to pad connections\n  - Use copper pour for GND plane\n  - Consider 4-layer stackup for better SI\n  - Add test points for debugging",pcb->ntracks,pcb->nfps); (void)pcb; return str_dup(buf);
}

// ===== CURVATURE + T-SPLINE =====
char *curvature_visual_data(CadMesh *m) {
    if(!m||!m->valid)return str_dup("no mesh"); float *h=curvature_heatmap(m);
    if(!h)return str_dup("failed"); free(h);
    char buf[128]; snprintf(buf,sizeof(buf),"Curvature data: %d points ready for heatmap rendering",m->nverts); return str_dup(buf);
}
char *tspline_stub(void) { return str_dup("T-Splines: requires OpenCASCADE or similar kernel\nStub: surface subdivision not yet implemented"); }
