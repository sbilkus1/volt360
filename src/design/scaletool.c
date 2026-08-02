#include "scaletool.h"
#include "../import/gerber_out.h"
#include "../import/pdfout.h"
#include "../design/design.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

void mesh_scale(CadMesh *m, float sx, float sy, float sz) {
    if (!m || !m->valid) return;
    for (int i = 0; i < m->nverts; i++) {
        m->pos[i*3] *= sx; m->pos[i*3+1] *= sy; m->pos[i*3+2] *= sz;
    }
    mesh_bbox(m);
}

void mesh_scale_about(CadMesh *m, V3 ref, float sx, float sy, float sz) {
    if (!m || !m->valid) return;
    for (int i = 0; i < m->nverts; i++) {
        m->pos[i*3] = ref.x + (m->pos[i*3] - ref.x) * sx;
        m->pos[i*3+1] = ref.y + (m->pos[i*3+1] - ref.y) * sy;
        m->pos[i*3+2] = ref.z + (m->pos[i*3+2] - ref.z) * sz;
    }
    mesh_bbox(m);
}

bool mesh_to_solid(CadMesh *in, CadMesh *out, float thickness) {
    if (!in || !out || !in->valid || thickness <= 0) return false;
    memset(out, 0, sizeof(*out)); out->valid = 1;
    out->nverts = in->nverts * 2;
    out->ntris = in->ntris * 2 + in->nverts * 2; // faces + side walls
    out->pos = (float *)malloc(sizeof(float)*3*(size_t)out->nverts);
    out->nrm = (float *)calloc((size_t)out->nverts*3, sizeof(float));
    out->idx = (int *)malloc(sizeof(int)*3*(size_t)out->ntris);

    // copy original + offset copy
    for (int i = 0; i < in->nverts; i++) {
        float nx = in->nrm ? in->nrm[i*3] : 0, ny = in->nrm ? in->nrm[i*3+1] : 0, nz = in->nrm ? in->nrm[i*3+2] : 0;
        float len = sqrtf(nx*nx+ny*ny+nz*nz); if (len<0.001f) len=1;
        out->pos[i*3]=in->pos[i*3]; out->pos[i*3+1]=in->pos[i*3+1]; out->pos[i*3+2]=in->pos[i*3+2];
        out->pos[(in->nverts+i)*3]=in->pos[i*3]+nx/len*thickness;
        out->pos[(in->nverts+i)*3+1]=in->pos[i*3+1]+ny/len*thickness;
        out->pos[(in->nverts+i)*3+2]=in->pos[i*3+2]+nz/len*thickness;
    }
    // copy triangles for both sides
    int ti = 0;
    for (int t = 0; t < in->ntris; t++) {
        out->idx[ti*3]=in->idx[t*3]; out->idx[ti*3+1]=in->idx[t*3+1]; out->idx[ti*3+2]=in->idx[t*3+2]; ti++;
        out->idx[ti*3]=in->nverts+in->idx[t*3+2]; out->idx[ti*3+1]=in->nverts+in->idx[t*3+1]; out->idx[ti*3+2]=in->nverts+in->idx[t*3]; ti++;
    }
    out->ntris = ti;
    mesh_bbox(out);
    return true;
}

float corner_relief_diameter(float thickness, float bend_radius) {
    return thickness * 1.5f + bend_radius * 0.5f;
}

BendTableEntry bend_table_lookup(float thickness, float radius) {
    BendTableEntry e = {thickness, radius, 0.4f, 0};
    if (thickness < 1.0f) e.k_factor = 0.33f;
    else if (thickness < 2.0f) e.k_factor = 0.4f;
    else e.k_factor = 0.45f;
    e.deduction_90 = 2.0f*(radius+thickness)*tanf((float)M_PI/4.0f) - (float)M_PI/180.0f*(radius+e.k_factor*thickness)*90.0f;
    return e;
}

void eject_profile_default(EjectProfile *ep, const char *material) {
    memset(ep, 0, sizeof(*ep));
    snprintf(ep->material, sizeof(ep->material), "%s", material ? material : "PLA");
    ep->release_temp = 40.0f; ep->eject_angle = 90;
    snprintf(ep->gcode_suffix, sizeof(ep->gcode_suffix), "M280 P0 S%d ; servo eject", ep->eject_angle);
}

char *eject_profile_report(EjectProfile *ep) {
    if (!ep) return str_dup("no profile");
    char buf[256];
    snprintf(buf, sizeof(buf), "Eject: %s @%dC angle=%d gcode=%s",
        ep->material, (int)ep->release_temp, ep->eject_angle, ep->gcode_suffix);
    return str_dup(buf);
}

char *predictive_maintenance_report(Farm *f) {
    if (!f) return str_dup("no farm");
    char buf[2048]; int off = 0, alerts = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== PREDICTIVE MAINTENANCE ===\n");
    for (int i = 0; i < f->n_printers; i++) {
        if (f->printers[i].total_jobs_done > 100) {
            off += snprintf(buf+off, sizeof(buf)-off, "  %s: 100+ jobs, check belts/nozzle\n", f->printers[i].name);
            alerts++;
        }
        if (f->printers[i].hours_running > 500) {
            off += snprintf(buf+off, sizeof(buf)-off, "  %s: 500+ hours, lubricate/calibrate\n", f->printers[i].name);
            alerts++;
        }
    }
    for (int i = 0; i < f->n_maint; i++)
        if (f->maint[i].interval_hours > 0) {
            float hrs = 0;
            for (int p = 0; p < f->n_printers; p++)
                if (strcmp(f->printers[p].name, f->maint[i].printer_name)==0) hrs=f->printers[p].hours_running;
            if (hrs - f->maint[i].last_service_hours > f->maint[i].interval_hours * 0.8f) {
                off += snprintf(buf+off, sizeof(buf)-off, "  %s: %s due soon\n", f->maint[i].printer_name, f->maint[i].task);
                alerts++;
            }
        }
    if (alerts==0) off += snprintf(buf+off, sizeof(buf)-off, "  All printers up to date\n");
    return str_dup(buf);
}

// simple favorites: store as string list in project notes
void favorites_add(Project *p, const char *item_type, const char *item_id) {
    if (!p || !item_type || !item_id) return;
    char note[256]; snprintf(note, sizeof(note), "favorite:%s:%s", item_type, item_id);
    arr_push(p->notes, str_dup(note));
}

char *favorites_list(Project *p) {
    if (!p) return str_dup("no project");
    char buf[1024]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== FAVORITES ===\n");
    int count = 0;
    for (int i = 0; i < p->notes.len; i++)
        if (strncmp(p->notes.v[i], "favorite:", 9) == 0) {
            off += snprintf(buf+off, sizeof(buf)-off, "  %s\n", p->notes.v[i]+9);
            count++;
        }
    if (count == 0) off += snprintf(buf+off, sizeof(buf)-off, "  No favorites yet\n");
    return str_dup(buf);
}

char *export_all_formats(Project *p, const char *output_dir) {
    if (!p) return str_dup("no project");
    char buf[512];
    int count = 0;
    // export all PCBs as gerber + dxf + pdf
    for (int i = 0; i < p->pcbs.len; i++) {
        Pcb *pcb = &p->pcbs.v[i];
        char path[512];
        snprintf(path, sizeof(path), "%s/%s.gbr", output_dir, pcb->name?pcb->name:"board");
        gerber_write_all(pcb, output_dir); count+=3;
        snprintf(path, sizeof(path), "%s/%s.dxf", output_dir, pcb->name?pcb->name:"board");
        dxf_export_pcb(pcb, path); count++;
        snprintf(path, sizeof(path), "%s/%s.pdf", output_dir, pcb->name?pcb->name:"board");
        pdf_export_drawing(pcb, path); count++;
    }
    // export all CAD as STL
    for (int i = 0; i < p->cad_models.len; i++) {
        if (p->cad_models.v[i].mesh.valid) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s.stl", output_dir, p->cad_models.v[i].name?p->cad_models.v[i].name:"model");
            mesh_write_stl(path, &p->cad_models.v[i].mesh); count++;
        }
    }
    snprintf(buf, sizeof(buf), "Exported %d files to %s", count, output_dir);
    return str_dup(buf);
}
