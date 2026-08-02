#include "real_algo.h"
#include "../core/util.h"
#include "../design/sketcher.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

// ===== KICAD CONNECTION GRAPH =====
// Union-find for wire endpoint grouping
typedef struct { int parent, rank; } UFNode;
static int uf_find(UFNode *uf, int x) { while(uf[x].parent!=x){uf[x].parent=uf[uf[x].parent].parent;x=uf[x].parent;} return x; }
static void uf_union(UFNode *uf, int a, int b) { int ra=uf_find(uf,a),rb=uf_find(uf,b); if(ra==rb)return;
    if(uf[ra].rank<uf[rb].rank)uf[ra].parent=rb; else if(uf[ra].rank>uf[rb].rank)uf[rb].parent=ra; else{uf[rb].parent=ra;uf[ra].rank++;} }

int conngraph_build(Schematic *s, ConnNode *nodes, int max_nodes) {
    if (!s || !nodes || max_nodes < 1) return 0;
    int np = s->nwires * 2 + s->ninsts * 2 + s->nlabels;
    V2 *pts = (V2 *)malloc(sizeof(V2)*(size_t)np);
    int *type = (int *)malloc(sizeof(int)*(size_t)np); // 0=wire, 1=pin, 2=label
    int *ref = (int *)malloc(sizeof(int)*(size_t)np);  // wire index or inst index
    int npts = 0;

    // wire endpoints
    for (int i=0;i<s->nwires;i++) { pts[npts]=s->wires[i].a; type[npts]=0; ref[npts]=i; npts++; pts[npts]=s->wires[i].b; type[npts]=0; ref[npts]=i; npts++; }
    // component pins (approximate pin positions near component center)
    for (int i=0;i<s->ninsts;i++) { pts[npts]=s->insts[i].pos; type[npts]=1; ref[npts]=i; npts++; pts[npts]=v2(s->insts[i].pos.x+15,s->insts[i].pos.y); type[npts]=1; ref[npts]=i; npts++; }
    // labels
    for (int i=0;i<s->nlabels;i++) { pts[npts]=s->labels[i].pos; type[npts]=2; ref[npts]=i; npts++; }

    // union-find merge coincident points
    UFNode *uf = (UFNode *)malloc(sizeof(UFNode)*(size_t)npts);
    for (int i=0;i<npts;i++){uf[i].parent=i;uf[i].rank=0;}
    for (int i=0;i<npts;i++) for(int j=i+1;j<npts;j++) {
        float dx=pts[i].x-pts[j].x,dy=pts[i].y-pts[j].y;
        if(sqrtf(dx*dx+dy*dy)<1.0f) uf_union(uf,i,j);
    }

    // assign net IDs
    int net_id = 0;
    int *net = (int *)malloc(sizeof(int)*(size_t)npts);
    for (int i=0;i<npts;i++) net[i]=-1;
    for (int i=0;i<npts;i++) {
        int root=uf_find(uf,i);
        if (net[root] < 0) net[root] = net_id++;
        net[i] = net[root];
    }

    // build node output
    int out = 0;
    for (int i=0;i<npts&&out<max_nodes;i++) {
        ConnNode cn; cn.net_id=net[i]; cn.pos=pts[i]; cn.pin_count=0;
        // count pins on this net
        for (int j=0;j<npts;j++) if(net[j]==net[i]&&type[j]==1) cn.pin_count++;
        nodes[out++]=cn;
    }

    free(pts); free(type); free(ref); free(uf); free(net);
    return net_id;
}

int conngraph_trace_net(Schematic *s, ConnNode *nodes, int net_id, int *wires, int max_w, int *pins, int max_p) {
    (void)s; (void)nodes; (void)net_id; (void)wires; (void)max_w; (void)pins; (void)max_p; return 0;
}

// ===== PUSH-AND-SHOVE ROUTER =====
void pushshove_shove_track(Track *t, V2 dir, float amount) {
    t->a.x += dir.x * amount; t->a.y += dir.y * amount;
    t->b.x += dir.x * amount; t->b.y += dir.y * amount;
}

bool pushshove_is_clear(Pcb *pcb, V2 pos, float clr, int excl) {
    for (int i=0;i<pcb->ntracks;i++) { if(i==excl)continue;
        float d1=sqrtf((pcb->tracks[i].a.x-pos.x)*(pcb->tracks[i].a.x-pos.x)+(pcb->tracks[i].a.y-pos.y)*(pcb->tracks[i].a.y-pos.y));
        float d2=sqrtf((pcb->tracks[i].b.x-pos.x)*(pcb->tracks[i].b.x-pos.x)+(pcb->tracks[i].b.y-pos.y)*(pcb->tracks[i].b.y-pos.y));
        if(d1<clr||d2<clr)return false;
    }
    for (int i=0;i<pcb->nfps;i++) { if(i==excl)continue;
        float d=sqrtf((pcb->fps[i].pos.x-pos.x)*(pcb->fps[i].pos.x-pos.x)+(pcb->fps[i].pos.y-pos.y)*(pcb->fps[i].pos.y-pos.y));
        if(d<clr)return false;
    }
    return true;
}

int pushshove_route(Pcb *pcb, V2 a, V2 b, float width, float clr, int max_shoves) {
    if (!pcb) return 0;
    float total_clr = width * 0.5f + clr;
    int shoved = 0;
    // compute direction of new track
    V2 dir = v2(b.x-a.x, b.y-a.y); float len = sqrtf(dir.x*dir.x+dir.y*dir.y);
    if (len < 0.001f) return 0;
    V2 perp = v2(-dir.y/len, dir.x/len);

    // check each existing track, shove if too close
    for (int i = 0; i < pcb->ntracks && shoved < max_shoves; i++) {
        Track *t = &pcb->tracks[i];
        // check both endpoints of new track against existing track
        float d1 = sqrtf((t->a.x-a.x)*(t->a.x-a.x)+(t->a.y-a.y)*(t->a.y-a.y));
        float d2 = sqrtf((t->b.x-b.x)*(t->b.x-b.x)+(t->b.y-b.y)*(t->b.y-b.y));
        if (d1 < total_clr + t->width*0.5f || d2 < total_clr + t->width*0.5f) {
            float push = total_clr + t->width*0.5f - (d1 < d2 ? d1 : d2) + 0.1f;
            pushshove_shove_track(t, perp, push);
            shoved++;
        }
    }

    // add the new track
    Track nt; nt.a=a; nt.b=b; nt.width=width; nt.net=NULL;
    pcb->tracks=(Track*)realloc(pcb->tracks,sizeof(Track)*(size_t)(pcb->ntracks+1));
    pcb->tracks[pcb->ntracks++]=nt;
    return shoved;
}

// ===== ADAPTIVE LAYER HEIGHT =====
float surface_angle_at_z(CadMesh *mesh, float z) {
    if (!mesh || !mesh->valid || mesh->ntris < 1) return 0;
    float total_angle = 0; int count = 0;
    for (int t = 0; t < mesh->ntris; t++) {
        int i0=mesh->idx[t*3],i1=mesh->idx[t*3+1],i2=mesh->idx[t*3+2];
        float z0=mesh->pos[i0*3+2],z1=mesh->pos[i1*3+2],z2=mesh->pos[i2*3+2];
        if ((z0 <= z && z1 >= z) || (z1 <= z && z0 >= z) ||
            (z0 <= z && z2 >= z) || (z2 <= z && z0 >= z)) {
            float e1x=mesh->pos[i1*3]-mesh->pos[i0*3],e1y=mesh->pos[i1*3+1]-mesh->pos[i0*3+1],e1z=z1-z0;
            float e2x=mesh->pos[i2*3]-mesh->pos[i0*3],e2y=mesh->pos[i2*3+1]-mesh->pos[i0*3+1],e2z=z2-z0;
            float nx=e1y*e2z-e1z*e2y, ny=e1z*e2x-e1x*e2z, nz=e1x*e2y-e1y*e2x;
            float nlen=sqrtf(nx*nx+ny*ny+nz*nz);
            if (nlen > 1e-6f) { float angle=acosf(fabsf(nz)/nlen)*180.0f/(float)M_PI; total_angle+=angle; count++; }
        }
    }
    return count > 0 ? total_angle / (float)count : 0;
}

int adaptive_layer_heights(CadMesh *mesh, float base_h, float min_h, float max_h, float *out_h, int max_l) {
    if (!mesh || !mesh->valid || !out_h || max_l < 1) return 0;
    float zmin=mesh->bmin.z, zmax=mesh->bmax.z;
    int n_layers = (int)((zmax-zmin)/base_h) + 1;
    if (n_layers > max_l) n_layers = max_l;
    for (int li = 0; li < n_layers; li++) {
        float z = zmin + li * base_h;
        float angle = surface_angle_at_z(mesh, z);
        // steep surfaces -> finer layers
        float ratio = 1.0f - (angle / 90.0f) * 0.7f;
        float h = base_h * ratio;
        if (h < min_h) h = min_h; if (h > max_h) h = max_h;
        out_h[li] = h;
    }
    return n_layers;
}

// ===== BRIDGE DETECTION =====
int bridge_detect(V2 *poly, int n, V2 *below, int nb, float lw, V2 *bridges, int max_b) {
    if (!poly||n<3||!bridges||max_b<1) return 0;
    // find poly vertices NOT supported by below polygon
    int count = 0;
    for (int i = 0; i < n && count < max_b; i++) {
        // check if poly vertex is inside below polygon
        int inside = 0;
        for (int j = 0, k = nb-1; j < nb && nb > 2; k=j++)
            if (((below[j].y > poly[i].y) != (below[k].y > poly[i].y)) &&
                (poly[i].x < (below[k].x-below[j].x)*(poly[i].y-below[j].y)/(below[k].y-below[j].y)+below[j].x)) inside=!inside;
        if (!inside && nb > 2) { V2 next = poly[(i+1)%n]; bridges[count*2]=poly[i]; bridges[count*2+1]=next; count++; }
    }
    return count;
}

// ===== CONSTRAINT SOLVER =====
int sketch_dof_count(Sketch *sk) {
    if (!sk) return 0;
    int dof = sk->n_entities * 2; // each point has 2 DOF (x,y)
    for (int i = 0; i < sk->n_constraints; i++) {
        switch (sk->constraints[i].type) {
            case CT_HORIZONTAL: case CT_VERTICAL: case CT_FIX: dof -= 1; break;
            case CT_COINCIDENT: dof -= 2; break;
            case CT_DISTANCE: dof -= 1; break;
            default: break;
        }
    }
    return dof > 0 ? dof : 0;
}

int constraint_iterative_solve(Sketch *sk, int max_iter, float tol) {
    if (!sk) return 0;
    for (int iter = 0; iter < max_iter; iter++) {
        float max_move = 0;
        for (int ci = 0; ci < sk->n_constraints; ci++) {
            SkConstraint *c = &sk->constraints[ci];
            SkEntity *a = c->entity_a>=0&&c->entity_a<sk->n_entities ? &sk->entities[c->entity_a] : NULL;
            if (!a) continue;
            switch (c->type) {
                case CT_HORIZONTAL: if(a->type==SK_LINE){float dy=a->p2.y-a->p1.y;a->p2.y=a->p1.y;if(fabsf(dy)>max_move)max_move=fabsf(dy);}break;
                case CT_VERTICAL: if(a->type==SK_LINE){float dx=a->p2.x-a->p1.x;a->p2.x=a->p1.x;if(fabsf(dx)>max_move)max_move=fabsf(dx);}break;
                default: break;
            }
        }
        if (max_move < tol) { sk->solved = 1; return iter + 1; }
    }
    sk->solved = (sketch_dof_count(sk) == 0);
    return max_iter;
}

// ===== SA COOLING SCHEDULE =====
float sa_cooling_schedule(float T0, float Tend, int n, float *T, int max_T) {
    float alpha = powf(Tend / T0, 1.0f / (float)n);
    int count = 0; float Tk = T0;
    while (Tk > Tend && count < max_T) { T[count++] = Tk; Tk *= alpha; }
    return alpha;
}

// ===== NETLIST CONNECTIVITY =====
char *netlist_full_connectivity(Project *p) {
    if (!p) return str_dup("no project");
    char buf[2048]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "=== FULL NETLIST ===\nProject: %s\n\n", p->name ? p->name : "?");
    for (int si = 0; si < p->schematics.len; si++) {
        Schematic *s = &p->schematics.v[si];
        ConnNode nodes[256]; int nc = conngraph_build(s, nodes, 256);
        off += snprintf(buf+off, sizeof(buf)-off, "Sheet: %s (%d nets)\n", s->name ? s->name : "?", nc);
        for (int i = 0; i < s->ninsts; i++)
            off += snprintf(buf+off, sizeof(buf)-off, "  %s %s\n", s->insts[i].ref ? s->insts[i].ref : "?",
                s->insts[i].value ? s->insts[i].value : "?");
    }
    return str_dup(buf);
}

// ===== FEATURE DEPENDENCY GRAPH =====
void feature_dep_graph(DesignFeature *feats, int n, FeatureDep *deps, int max_deps) {
    if (!feats||!deps||n<1) return;
    for (int i = 0; i < n && i < max_deps; i++) {
        deps[i].feature_id = i; deps[i].n_deps = 0;
        if (i > 0) { deps[i].depends_on[deps[i].n_deps++] = i - 1; } // chain dependency
    }
}

char *feature_dep_report(DesignFeature *feats, int n, FeatureDep *deps, int ndeps) {
    char buf[512]; int off = 0;
    off += snprintf(buf+off, sizeof(buf)-off, "Feature Dependency Graph:\n");
    for (int i = 0; i < ndeps && i < n; i++) {
        off += snprintf(buf+off, sizeof(buf)-off, "  [%d] depends on:", i);
        for (int j = 0; j < deps[i].n_deps; j++) off += snprintf(buf+off, sizeof(buf)-off, " %d", deps[i].depends_on[j]);
        off += snprintf(buf+off, sizeof(buf)-off, "\n");
    }
    return str_dup(buf);
}
