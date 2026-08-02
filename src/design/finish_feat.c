#include "finish_feat.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// ===== MESH REPAIR =====
MeshRepairResult mesh_repair_full(CadMesh *m) {
    MeshRepairResult r={0};if(!m||!m->valid){snprintf(r.status,128,"invalid mesh");return r;}
    r.degenerates=mesh_remove_degenerate(m,0.001f);r.normals_fixed=mesh_fix_normals(m);snprintf(r.status,128,"Repaired: %d degenerates, %d normals",r.degenerates,r.normals_fixed);return r;
}
char *mesh_repair_report(MeshRepairResult r) { return str_dup(r.status); }

// ===== HOLLOWING =====
char *hollow_preview(CadMesh *m, float wt) {
    if(!m||!m->valid)return str_dup("no mesh");char buf[128];
    snprintf(buf,sizeof(buf),"Hollow: %.1fmm wall | %d verts %d tris -> %.0f%% smaller",wt,m->nverts,m->ntris,100.0f-100.0f*(wt/wt+1.0f));return str_dup(buf);
}

// ===== AUTO-ORIENT =====
char *auto_orient_report(CadMesh *m) {
    float rx,ry,rz; calib_auto_orient(m,&rx,&ry,&rz);char buf[64];snprintf(buf,sizeof(buf),"Best orientation: Rx=%.0f Ry=%.0f Rz=%.0f",rx,ry,rz);return str_dup(buf);
}

// ===== SIGNAL TRACING =====
char *signal_trace(Schematic *s, const char *pin) {
    if(!s)return str_dup("no schematic");char buf[1024];int off=0;off+=snprintf(buf+off,sizeof(buf)-off,"Signal trace from '%s':\n",pin?pin:"?");
    // find wires connected to this pin, trace through junctions
    for(int i=0;i<s->nwires;i++){ V2 end=s->wires[i].a; int found=0;
        for(int j=0;j<s->ninsts;j++){float d=(end.x-s->insts[j].pos.x)*(end.x-s->insts[j].pos.x)+(end.y-s->insts[j].pos.y)*(end.y-s->insts[j].pos.y);if(d<100.0f){found=1;off+=snprintf(buf+off,sizeof(buf)-off,"  -> %s\n",s->insts[j].ref?s->insts[j].ref:"?");}}if(found)break;}
    return str_dup(buf);
}

// ===== AI LEARN =====
char *ai_learn_topic(const char *t) {
    if(!t)return str_dup("no topic");
    if(strstr(t,"impedance"))return str_dup("Impedance: set by trace width/dielectric height. Use 50 ohm for general, 90 ohm for USB diff pairs. Tools > DRC > check trace Z0");
    if(strstr(t,"ground"))return str_dup("Ground planes: fill empty copper areas with GND pour. Reduces EMI, provides return path. Tools > Copper Pour > Fill");
    if(strstr(t,"routing"))return str_dup("PCB routing: use 45/90deg angles. Keep power traces wide. Route clock first, then data, then power. Use differential pairs for USB/HDMI.");
    if(strstr(t,"stackup"))return str_dup("4-layer stackup: Sig-GND-VCC-Sig is standard. 2-layer: keep ground plane intact. Use Tools > Stackup to configure.");
    return str_dup("Enter a topic: impedance, ground, routing, stackup, crosstalk, thermal");
}
char *ai_explain_pcb_concept(const char *c) { return ai_learn_topic(c); }

// ===== REMOTE ACCESS =====
char *remote_access_tunnel_status(const char *id) { char b[128];snprintf(b,sizeof(b),"Remote access for %s: stub (SSH tunnel / Tailscale / WireGuard)",id?id:"farm");return str_dup(b);}

// ===== BATCH CONTROL =====
char *batch_control_start_all(Farm *f) { if(!f)return str_dup("no farm");for(int i=0;i<f->n_printers;i++){f->printers[i].busy=1;snprintf(f->printers[i].status,16,"busy");}return str_dup("All printers started");}
char *batch_control_stop_all(Farm *f) { if(!f)return str_dup("no farm");for(int i=0;i<f->n_printers;i++){f->printers[i].busy=0;snprintf(f->printers[i].status,16,"idle");}return str_dup("All printers stopped");}
char *batch_status_report(Farm *f) { if(!f)return str_dup("no farm");int b=0;for(int i=0;i<f->n_printers;i++)if(f->printers[i].busy)b++;char buf[64];snprintf(buf,sizeof(buf),"Batch: %d/%d busy",b,f->n_printers);return str_dup(buf);}

// ===== SECTION VIEW =====
char *section_view_report(CadMesh *m, float z) {
    if(!m||!m->valid)return str_dup("no mesh");char buf[256];
    int above=0,below=0;for(int i=0;i<m->nverts;i++){if(m->pos[i*3+2]>z)above++;else below++;}
    snprintf(buf,sizeof(buf),"Section at Z=%.1f: %d above, %d below (%d verts)",z,above,below,m->nverts);return str_dup(buf);
}
char *section_view_slice(CadMesh *m, float z) { return mesh_xsection_text(m,z); }

// ===== NET CLASSES =====
int netclass_define(Pcb *pcb, NetClassDef nc) { return netclass_assign_by_pattern(pcb,nc.name,nc.track_width,nc.clearance); }
char *netclass_list_report(Pcb *pcb) {
    char buf[256];int off=0;off+=snprintf(buf+off,sizeof(buf)-off,"Net Classes:\n");
    const char* nets[32];int nn=0;for(int i=0;i<pcb->ntracks;i++)if(pcb->tracks[i].net){int dup=0;for(int j=0;j<nn;j++)if(nets[j]&&strcmp(nets[j],pcb->tracks[i].net)==0)dup=1;if(!dup&&nn<32)nets[nn++]=pcb->tracks[i].net;}
    for(int i=0;i<nn;i++)off+=snprintf(buf+off,sizeof(buf)-off,"  %s\n",nets[i]);return str_dup(buf);
}
