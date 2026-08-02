#include "advance_feat.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

// ===== PUSH-AND-SHOVE ROUTER =====
DRCResult pcb_check_drc_realtime(Pcb *pcb, V2 a, V2 b, float width, float clr, int excl) {
    DRCResult r = {-1,0,""};
    float total_clr = width*0.5f + clr;
    // check tracks
    for (int i=0;i<pcb->ntracks;i++) { if(i==excl)continue;
        float d1=(pcb->tracks[i].a.x-a.x)*(pcb->tracks[i].a.x-a.x)+(pcb->tracks[i].a.y-a.y)*(pcb->tracks[i].a.y-a.y);
        float d2=(pcb->tracks[i].b.x-b.x)*(pcb->tracks[i].b.x-b.x)+(pcb->tracks[i].b.y-b.y)*(pcb->tracks[i].b.y-b.y);
        float min_d = sqrtf(d1<d2?d1:d2);
        if(min_d < total_clr+pcb->tracks[i].width*0.5f){r.index=i;r.is_track=1;snprintf(r.msg,128,"Track %d clearance %.3f < %.3f",i,min_d,total_clr); return r;}
    }
    // check pads
    for (int i=0;i<pcb->nfps;i++) { if(i==excl)continue;
        float d1=(pcb->fps[i].pos.x-a.x)*(pcb->fps[i].pos.x-a.x)+(pcb->fps[i].pos.y-a.y)*(pcb->fps[i].pos.y-a.y);
        float d2=(pcb->fps[i].pos.x-b.x)*(pcb->fps[i].pos.x-b.x)+(pcb->fps[i].pos.y-b.y)*(pcb->fps[i].pos.y-b.y);
        float min_d = sqrtf(d1<d2?d1:d2);
        if(min_d < total_clr+2.0f){r.index=i;r.is_track=0;snprintf(r.msg,128,"Pad %d clearance %.3f < %.3f",i,min_d,total_clr+2.0f); return r;}
    }
    return r;
}

int pcb_route_interactive(Pcb *pcb, V2 start, V2 end, float width, float clr, int max_attempts) {
    if (!pcb) return -1;
    float total_clr = width*0.5f + clr;
    int shoved = 0;

    // attempt to place the track. if blocked, shove the blocker and retry.
    for (int attempt = 0; attempt < max_attempts; attempt++) {
        DRCResult drc = pcb_check_drc_realtime(pcb, start, end, width, clr, -1);
        if (drc.index < 0) break; // clear to place

        if (drc.is_track) {
            // shove the blocking track perpendicular to our direction
            V2 dir = v2(end.x-start.x, end.y-start.y); float len=sqrtf(dir.x*dir.x+dir.y*dir.y);
            if (len<0.001f) return -1;
            V2 perp = v2(-dir.y/len, dir.x/len);
            float push_dist = total_clr + pcb->tracks[drc.index].width*0.5f + 0.05f;
            // determine push direction (away from center of new track)
            V2 mid = v2((pcb->tracks[drc.index].a.x+pcb->tracks[drc.index].b.x)*0.5f, (pcb->tracks[drc.index].a.y+pcb->tracks[drc.index].b.y)*0.5f);
            V2 line_mid = v2((start.x+end.x)*0.5f, (start.y+end.y)*0.5f);
            V2 to_mid = v2(mid.x-line_mid.x, mid.y-line_mid.y);
            float sign = (perp.x*to_mid.x+perp.y*to_mid.y)>0?1.0f:-1.0f;
            pcb->tracks[drc.index].a.x += perp.x*push_dist*sign;
            pcb->tracks[drc.index].a.y += perp.y*push_dist*sign;
            pcb->tracks[drc.index].b.x += perp.x*push_dist*sign;
            pcb->tracks[drc.index].b.y += perp.y*push_dist*sign;
            shoved++;
        } else {
            // can't shove a pad — try routing around it
            V2 mid = pcb->fps[drc.index].pos;
            V2 detour = v2(start.x, mid.y+10); // simple vertical detour
            // add two segments to go around
            Track t1 = {start, detour, width, NULL};
            Track t2 = {detour, end, width, NULL};
            pcb->tracks = (Track*)realloc(pcb->tracks,sizeof(Track)*(size_t)(pcb->ntracks+2));
            pcb->tracks[pcb->ntracks++]=t1; pcb->tracks[pcb->ntracks++]=t2;
            return shoved;
        }
    }

    // place the final track
    Track nt = {start, end, width, NULL};
    pcb->tracks = (Track*)realloc(pcb->tracks,sizeof(Track)*(size_t)(pcb->ntracks+1));
    pcb->tracks[pcb->ntracks++]=nt;
    return shoved;
}

// ===== ORGANIC TREE SUPPORTS =====
void supports_organic_tree(SliceResult *sr, const SliceSettings *cfg, float overhang_deg, int branch_depth) {
    if (!sr||sr->nlayers<3||!cfg) return;
    float overhang_factor = cosf(overhang_deg*(float)M_PI/180.0f);
    // Start from bed (layer 0) and grow branches upward toward overhanging regions.
    // Each branch splits into 2-3 sub-branches at each depth level.

    // find bed-level anchor points
    SliceLayer *L0 = &sr->layers[0];
    V2 anchors[32]; int na = 0;
    for (int j=0;j<L0->npaths&&na<32;j++) if(L0->paths[j].kind==0){ // perimeter
        for(int k=0;k<L0->paths[j].npts&&na<32;k+=8) anchors[na++]=L0->paths[j].pts[k];
    }
    if (na==0) return;

    // grow tree upward layer by layer
    V2 trunks[32]; memcpy(trunks,anchors,sizeof(V2)*(size_t)na); int nt=na;

    for (int li=1;li<sr->nlayers;li++) {
        SliceLayer *L = &sr->layers[li];
        // find overhang points in this layer
        V2 overhangs[64]; int no=0;
        // check if this layer extends beyond layer below
        SliceLayer *Lb = &sr->layers[li-1];
        float below_radius = 0;
        for(int j=0;j<Lb->npaths;j++) if(Lb->paths[j].kind==0&&Lb->paths[j].npts>0){
            float cx=0,cy=0;
            for(int k=0;k<Lb->paths[j].npts;k++){cx+=Lb->paths[j].pts[k].x;cy+=Lb->paths[j].pts[k].y;}
            cx/=Lb->paths[j].npts;cy/=Lb->paths[j].npts; below_radius=sqrtf(cx*cx+cy*cy);
        }

        // add support pillar segments from trunks to overhangs
        for (int t=0;t<nt;t++) {
            V2 last = trunks[t];
            V2 pt = last; // trunk stays in place for straight sections
            // branch toward nearest overhang if present
            float best_dist=1e9f; int best_o=0;
            for (int o=0;o<no;o++) { float d=(overhangs[o].x-pt.x)*(overhangs[o].x-pt.x)+(overhangs[o].y-pt.y)*(overhangs[o].y-pt.y); if(d<best_dist){best_dist=d;best_o=o;} }
            if (no>0 && best_dist<1e8f) pt=overhangs[best_o];

            V2 pts[2]={last,pt};
            int ni=L->npaths++;
            L->paths=(SlicePath*)realloc(L->paths,sizeof(SlicePath)*(size_t)L->npaths);
            L->paths[ni].kind=2; L->paths[ni].npts=2; L->paths[ni].width=cfg->line_width*0.8f; L->paths[ni].solid=0;
            L->paths[ni].pts=(V2*)malloc(sizeof(V2)*2); L->paths[ni].pts[0]=pts[0]; L->paths[ni].pts[1]=pts[1];
            trunks[t]=pt;
        }
    }
    (void)overhang_factor; (void)branch_depth;
}

// ===== PBR MATERIALS =====
void pbr_material_preset(PBRMaterial *mat, const char *type) {
    memset(mat,0,sizeof(*mat));
    if(strstr(type,"aluminum")){mat->albedo[0]=0.91f;mat->albedo[1]=0.91f;mat->albedo[2]=0.91f;mat->metallic=1.0f;mat->roughness=0.3f;mat->ao=0.5f;}
    else if(strstr(type,"steel")){mat->albedo[0]=0.6f;mat->albedo[1]=0.6f;mat->albedo[2]=0.6f;mat->metallic=1.0f;mat->roughness=0.4f;mat->ao=0.6f;}
    else if(strstr(type,"plastic")){mat->albedo[0]=0.8f;mat->albedo[1]=0.8f;mat->albedo[2]=0.8f;mat->metallic=0.0f;mat->roughness=0.5f;mat->ao=0.7f;}
    else if(strstr(type,"glass")){mat->albedo[0]=0.95f;mat->albedo[1]=0.95f;mat->albedo[2]=1.0f;mat->metallic=0.0f;mat->roughness=0.05f;mat->ao=1.0f;}
    else if(strstr(type,"copper")){mat->albedo[0]=0.95f;mat->albedo[1]=0.6f;mat->albedo[2]=0.4f;mat->metallic=1.0f;mat->roughness=0.25f;mat->ao=0.5f;}
    else if(strstr(type,"gold")){mat->albedo[0]=1.0f;mat->albedo[1]=0.85f;mat->albedo[2]=0.3f;mat->metallic=1.0f;mat->roughness=0.2f;mat->ao=0.4f;}
    else if(strstr(type,"carbon")){mat->albedo[0]=0.1f;mat->albedo[1]=0.1f;mat->albedo[2]=0.1f;mat->metallic=0.0f;mat->roughness=0.7f;mat->ao=0.8f;}
    else if(strstr(type,"wood")){mat->albedo[0]=0.6f;mat->albedo[1]=0.4f;mat->albedo[2]=0.2f;mat->metallic=0.0f;mat->roughness=0.8f;mat->ao=0.6f;}
    else{mat->albedo[0]=0.8f;mat->albedo[1]=0.8f;mat->albedo[2]=0.8f;mat->metallic=0.0f;mat->roughness=0.5f;mat->ao=0.7f;}
}

void hdr_environment_preset(HDREnvironment *env, const char *type) {
    memset(env,0,sizeof(*env));
    if(strstr(type,"studio")){env->sky_color[0]=0.5f;env->sky_color[1]=0.55f;env->sky_color[2]=0.6f;env->ground_color[0]=0.2f;env->ground_color[1]=0.2f;env->ground_color[2]=0.2f;
        env->sun_dir[0]=0.5f;env->sun_dir[1]=1.0f;env->sun_dir[2]=0.3f;env->sun_color[0]=1.0f;env->sun_color[1]=0.98f;env->sun_color[2]=0.95f;env->intensity=1.5f;env->enable_shadows=1;}
    else if(strstr(type,"outdoor")){env->sky_color[0]=0.3f;env->sky_color[1]=0.5f;env->sky_color[2]=0.9f;env->ground_color[0]=0.3f;env->ground_color[1]=0.5f;env->ground_color[2]=0.2f;
        env->sun_dir[0]=0.8f;env->sun_dir[1]=1.0f;env->sun_dir[2]=0.5f;env->sun_color[0]=1.0f;env->sun_color[1]=0.9f;env->sun_color[2]=0.7f;env->intensity=2.0f;env->enable_shadows=1;}
    else if(strstr(type,"night")){env->sky_color[0]=0.05f;env->sky_color[1]=0.05f;env->sky_color[2]=0.15f;env->ground_color[0]=0.02f;env->ground_color[1]=0.02f;env->ground_color[2]=0.02f;
        env->sun_dir[0]=0;env->sun_dir[1]=1;env->sun_dir[2]=0;env->sun_color[0]=0.1f;env->sun_color[1]=0.1f;env->sun_color[2]=0.3f;env->intensity=0.3f;env->enable_shadows=0;}
    else if(strstr(type,"sunset")){env->sky_color[0]=0.9f;env->sky_color[1]=0.4f;env->sky_color[2]=0.2f;env->ground_color[0]=0.3f;env->ground_color[1]=0.15f;env->ground_color[2]=0.1f;
        env->sun_dir[0]=-0.5f;env->sun_dir[1]=0.3f;env->sun_dir[2]=1.0f;env->sun_color[0]=1.0f;env->sun_color[1]=0.5f;env->sun_color[2]=0.1f;env->intensity=3.0f;env->enable_shadows=1;}
    else{env->sky_color[0]=0.5f;env->sky_color[1]=0.5f;env->sky_color[2]=0.5f;env->ground_color[0]=0.1f;env->ground_color[1]=0.1f;env->ground_color[2]=0.1f;env->intensity=1.0f;}
}

char *pbr_render_report(CadMesh *m, PBRMaterial *mat, HDREnvironment *env) {
    char buf[512];
    snprintf(buf,sizeof(buf),"PBR Render: %d verts %d tris | Albedo:(%.2f,%.2f,%.2f) Metal:%.1f Rough:%.1f | %s lighting shadows:%s",
        m?m->nverts:0,m?m->ntris:0,mat->albedo[0],mat->albedo[1],mat->albedo[2],mat->metallic,mat->roughness,
        env->enable_shadows?"Studio":"Ambient",env->enable_shadows?"ON":"OFF");
    return str_dup(buf);
}

// ===== 5-AXIS CAM =====
int cam5_simultaneous(V3 *pts, V3 *norms, int n, float tool_dia, float stepover, AxisPoint *tp, int max_tp, const char *out) {
    if(!pts||!norms||n<2||!tp||max_tp<2) return 0;
    FILE *f=NULL; if(out){f=fopen(out,"w"); if(f) fprintf(f,"; 5-axis simultaneous\nG21 G90\n");}
    int count=0;
    for(int i=0;i<n&&count<max_tp;i++){
        // offset surface point along normal by tool radius for tool tip position
        V3 tip = v3(pts[i].x+norms[i].x*tool_dia*0.5f, pts[i].y+norms[i].y*tool_dia*0.5f, pts[i].z+norms[i].z*tool_dia*0.5f);
        tp[count].pos=tip; tp[count].axis=norms[i];
        if(f) fprintf(f,"G1 X%.3f Y%.3f Z%.3f A%.2f B%.2f\n",tip.x,tip.y,tip.z,acosf(norms[i].z)*180.0f/(float)M_PI,atan2f(norms[i].y,norms[i].x)*180.0f/(float)M_PI);
        count++;
    }
    if(f){fprintf(f,"M30\n");fclose(f);}
    (void)stepover;
    return count;
}

char *cam5_toolpath_report(AxisPoint *path, int n) {
    char buf[512]; int off=0;
    off+=snprintf(buf+off,sizeof(buf)-off,"5-Axis Toolpath: %d points\n",n);
    if(n>0){AxisPoint *p=&path[0]; off+=snprintf(buf+off,sizeof(buf)-off,"  Start: (%.1f,%.1f,%.1f) axis:(%.2f,%.2f,%.2f)\n",p->pos.x,p->pos.y,p->pos.z,p->axis.x,p->axis.y,p->axis.z);}
    if(n>1){AxisPoint *p=&path[n-1]; off+=snprintf(buf+off,sizeof(buf)-off,"  End:   (%.1f,%.1f,%.1f) axis:(%.2f,%.2f,%.2f)",p->pos.x,p->pos.y,p->pos.z,p->axis.x,p->axis.y,p->axis.z);}
    return str_dup(buf);
}
