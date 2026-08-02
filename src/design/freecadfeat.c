#include "freecadfeat.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

// === FEM ===
char *fem_static_analysis(CadMesh *m, float E_GPa, float nu, FEMLoad *loads, int nloads, FEMBC *bcs, int nbcs) {
    (void)loads; (void)nloads; (void)bcs; (void)nbcs;
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[1024];
    float vol = 0;
    for (int t=0; t<m->ntris; t++) { int i0=m->idx[t*3],i1=m->idx[t*3+1],i2=m->idx[t*3+2];
        float e1x=m->pos[i1*3]-m->pos[i0*3],e1y=m->pos[i1*3+1]-m->pos[i0*3+1],e1z=m->pos[i1*3+2]-m->pos[i0*3+2];
        float e2x=m->pos[i2*3]-m->pos[i0*3],e2y=m->pos[i2*3+1]-m->pos[i0*3+1],e2z=m->pos[i2*3+2]-m->pos[i0*3+2];
        vol += fabsf(e1x*(e1y*e2z-e1z*e2y) - e1y*(e1x*e2z-e1z*e2x) + e1z*(e1x*e2y-e1y*e2x))/6.0f;
    }
    snprintf(buf, sizeof(buf), "=== FEM STATIC ANALYSIS ===\nMesh: %d verts, %d tris, vol=%.1f mm³\nE=%.0f GPa, nu=%.2f\nLoads: %d  BCs: %d\nStiffness: ~%.0f N/mm (estimated)\n",
        m->nverts, m->ntris, vol, E_GPa, nu, nloads, nbcs, E_GPa*vol/100.0f);
    return str_dup(buf);
}

char *fem_modal_analysis(CadMesh *m, float E_GPa, float nu, float density, int n_modes) {
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[512];
    float w = m->bmax.x-m->bmin.x, h = m->bmax.y-m->bmin.y, t = m->bmax.z-m->bmin.z;
    float I = w*h*h*h/12.0f, L = t>0?t:w, mass = density*w*h*t*1e-9f;
    float f1 = (float)(M_PI*M_PI)*sqrtf(E_GPa*1e9f*I*1e-12f/(mass*L*L*L*1e-9f))/(2.0f*(float)M_PI);
    snprintf(buf, sizeof(buf), "=== FEM MODAL ===\nMesh: %s\nMode 1: %.1f Hz\nE=%.0f GPa  density=%.0f kg/m³\nModes requested: %d\n",
        "analysis", f1, E_GPa, density, n_modes);
    return str_dup(buf);
}

// === TECHDRAW ===
char *techdraw_front_view(CadMesh *m) {
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[256];
    snprintf(buf, sizeof(buf), "Front (XY): %.0f x %.0f mm", m->bmax.x-m->bmin.x, m->bmax.y-m->bmin.y);
    return str_dup(buf);
}
char *techdraw_top_view(CadMesh *m) {
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[256];
    snprintf(buf, sizeof(buf), "Top (XZ): %.0f x %.0f mm", m->bmax.x-m->bmin.x, m->bmax.z-m->bmin.z);
    return str_dup(buf);
}
char *techdraw_right_view(CadMesh *m) {
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[256];
    snprintf(buf, sizeof(buf), "Right (YZ): %.0f x %.0f mm", m->bmax.y-m->bmin.y, m->bmax.z-m->bmin.z);
    return str_dup(buf);
}
char *techdraw_all_views(CadMesh *m, const char *title) {
    if (!m || !m->valid) return str_dup("no mesh");
    char buf[1024];
    snprintf(buf, sizeof(buf),
        "=== %s ===\nFront: %.0fx%.0f  Top: %.0fx%.0f  Right: %.0fx%.0f (mm)\nScale: 1:1 | Material: %d verts, %d faces",
        title ? title : "Drawing", m->bmax.x-m->bmin.x, m->bmax.y-m->bmin.y,
        m->bmax.x-m->bmin.x, m->bmax.z-m->bmin.z, m->bmax.y-m->bmin.y, m->bmax.z-m->bmin.z,
        m->nverts, m->ntris);
    return str_dup(buf);
}

// === ROBOT ===
void robot_forward_kinematics(DHParam *dh, int n, V3 *ee) {
    if (!dh || !ee || n < 1) return;
    float T[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    for (int i=0;i<n;i++) {
        float ct=cosf(dh[i].theta), st=sinf(dh[i].theta), ca=cosf(dh[i].alpha), sa=sinf(dh[i].alpha);
        float m[16]={ct,-st*ca,st*sa,dh[i].a*ct, st,ct*ca,-ct*sa,dh[i].a*st, 0,sa,ca,dh[i].d, 0,0,0,1};
        float tm[16]; for(int r=0;r<4;r++)for(int c=0;c<4;c++) tm[r*4+c]=T[r*4]*m[c]+T[r*4+1]*m[4+c]+T[r*4+2]*m[8+c]+T[r*4+3]*m[12+c];
        memcpy(T,tm,sizeof(T));
    }
    ee->x=T[3]; ee->y=T[7]; ee->z=T[11];
}

char *robot_kinematics_report(DHParam *dh, int n, float angles[6]) {
    char buf[512]; int off=0;
    for (int i=0;i<n;i++) dh[i].theta=angles[i];
    V3 ee; robot_forward_kinematics(dh,n,&ee);
    off+=snprintf(buf+off,sizeof(buf)-off,"Robot FK (%d joints): EE=(%.1f,%.1f,%.1f)\n",n,ee.x,ee.y,ee.z);
    return str_dup(buf);
}

// === PYTHON API ===
char *python_api_docs(void) {
    return str_dup(
        "=== PYTHON API ===\n"
        "llama_cad.project.load(path) -> Project\n"
        "llama_cad.mesh.bbox(mesh) -> (min,max)\n"
        "llama_cad.slice.mesh(mesh,settings) -> SliceResult\n"
        "llama_cad.farm.add_printer(name,slots) -> int\n"
        "llama_cad.spice.solve_dc(schematic) -> voltages\n"
        "Status: Python bindings planned (pybind11/CFFI)");
}

// === STATIC TIMING ===
float timing_propagation_delay(float L_mm, float er) {
    return L_mm * 1e-3f / (3e8f / sqrtf(er)) * 1e9f;
}
float timing_setup_margin(float period, float data_delay, float skew, float setup) {
    return period - data_delay - skew - setup;
}
float timing_hold_margin(float data_delay, float skew, float hold) {
    return data_delay - skew - hold;
}
char *timing_report(Pcb *pcb, float freq_MHz) {
    if (!pcb) return str_dup("no PCB");
    char buf[1024]; int off=0;
    float T=1e3f/freq_MHz, max_len=0;
    for (int i=0;i<pcb->ntracks;i++) { float dx=pcb->tracks[i].b.x-pcb->tracks[i].a.x,dy=pcb->tracks[i].b.y-pcb->tracks[i].a.y; float l=sqrtf(dx*dx+dy*dy); if(l>max_len)max_len=l; }
    float delay=timing_propagation_delay(max_len,4.2f);
    float setup=timing_setup_margin(T,delay,0.1f,0.5f);
    off+=snprintf(buf+off,sizeof(buf)-off,"=== TIMING @%.0f MHz ===\nPeriod: %.2f ns  Max track: %.0f mm\nDelay: %.3f ns  Setup margin: %.2f ns\nStatus: %s\n",
        freq_MHz,T,max_len,delay,setup,setup>0?"OK":"VIOLATION");
    return str_dup(buf);
}

// === OPENCASCADE ===
char *opencascade_compat_notes(void) {
    return str_dup("OpenCASCADE compatibility: STEP/B-Rep kernel.\nMesh-only currently. OCCT integration would add: B-Rep solids, fillets, chamfers, booleans, NURBS surfaces.");
}

// === SPREADSHEET ===
char *spreadsheet_create_sample(void) {
    return str_dup(
        "=== SPREADSHEET ===\n"
        "| Parameter | Value | Unit |\n"
        "|-----------|-------|------|\n"
        "| length    | 100   | mm   |\n"
        "| width     | 50    | mm   |\n"
        "| height    | 20    | mm   |\n"
        "| material  | PLA   | -    |\n"
        "Status: FreeCAD Spreadsheet workbench stub");
}
