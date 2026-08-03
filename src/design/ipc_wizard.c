#include "ipc_wizard.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ipc_params_default(IpcParams *p, IpcPackageType type) { memset(p,0,sizeof(*p)); p->type=type;
    switch(type){
    case IPC_SOIC: p->pin_count=8;p->pitch=1.27f;p->body_x=5.0f;p->body_y=6.0f;p->pad_x=1.6f;p->pad_y=0.6f;p->lead_span=6.2f;break;
    case IPC_QFP: p->pin_count=44;p->pitch=0.8f;p->body_x=10.0f;p->body_y=10.0f;p->pad_x=1.5f;p->pad_y=0.4f;p->lead_span=12.0f;break;
    case IPC_QFN: p->pin_count=32;p->pitch=0.5f;p->body_x=5.0f;p->body_y=5.0f;p->pad_x=0.7f;p->pad_y=0.3f;break;
    case IPC_BGA: p->bga_rows=8;p->bga_cols=8;p->pitch=1.0f;p->ball_dia=0.5f;p->body_x=8.0f;p->body_y=8.0f;break;
    case IPC_SOT23: p->pin_count=3;p->pitch=0.95f;p->body_x=2.9f;p->body_y=1.3f;p->pad_x=0.8f;p->pad_y=0.6f;break;
    case IPC_SOT223: p->pin_count=4;p->pitch=2.3f;p->body_x=6.5f;p->body_y=3.5f;p->pad_x=1.8f;p->pad_y=0.8f;break;
    case IPC_0603: p->pad_x=0.8f;p->pad_y=0.95f;p->body_x=1.6f;p->body_y=0.8f;break;
    case IPC_0805: p->pad_x=1.2f;p->pad_y=1.3f;p->body_x=2.0f;p->body_y=1.2f;break;
    case IPC_1206: p->pad_x=1.8f;p->pad_y=1.6f;p->body_x=3.2f;p->body_y=1.6f;break;
    case IPC_2512: p->pad_x=3.2f;p->pad_y=1.8f;p->body_x=6.4f;p->body_y=3.2f;break;
    case IPC_SOD123: p->pad_x=0.8f;p->pad_y=0.8f;p->body_x=2.5f;p->body_y=1.6f;break;
    case IPC_SOD323: p->pad_x=0.9f;p->pad_y=0.9f;p->body_x=1.7f;p->body_y=1.25f;break;
    case IPC_TANT_A: p->pad_x=1.0f;p->pad_y=1.6f;p->body_x=3.2f;p->body_y=1.6f;break;
    case IPC_TANT_B: p->pad_x=1.2f;p->pad_y=2.0f;p->body_x=3.5f;p->body_y=2.8f;break;
    case IPC_TANT_C: p->pad_x=2.0f;p->pad_y=2.5f;p->body_x=6.0f;p->body_y=3.2f;break;
    case IPC_OSCON: p->pin_count=4;p->pitch=2.54f;p->body_x=5.0f;p->body_y=7.0f;p->pad_x=1.5f;p->pad_y=1.0f;break;
    case IPC_CONN_TH: p->pin_count=6;p->pitch=2.54f;p->hole_dia=1.0f;p->outer_dia=2.0f;break;
    }
    if(p->courtyard==0)p->courtyard=0.25f;if(p->heel==0)p->heel=0.4f;if(p->side==0)p->side=0.1f;
}

void ipc_params_quick(IpcParams *p, IpcPackageType type, int pin_count, float pitch) {
    ipc_params_default(p, type); p->pin_count = pin_count; p->pitch = pitch;
}

static void add_pad(Footprint *fp, float x, float y, float sx, float sy, int shape, int layer, const char *name) {
    if(fp->npads>=256)return; Pad *pd=&fp->pads[fp->npads];memset(pd,0,sizeof(*pd));
    snprintf(pd->name=(char*)malloc(16),16,"%s",name?name:"");
    pd->pos=v2(x,y);pd->size=v2(sx,sy);pd->shape=shape;pd->layer=layer;pd->type=(layer<0)?PAD_THT:PAD_SMD;fp->npads++;
}

IpcResult ipc_generate_footprint(const IpcParams *p) {
    IpcResult r;memset(&r,0,sizeof(r));
    if(!p){snprintf(r.error,128,"NULL params");return r;}
    r.fp.pads=(Pad*)calloc(256,sizeof(Pad));r.fp.npads=0;r.fp.name=str_dup("IPC_GEN");
    switch(p->type){
    case IPC_SOIC:{float span=p->lead_span>0?p->lead_span:p->body_x+2.0f;int per=p->pin_count/2;
        for(int i=0;i<per;i++){float y=i*p->pitch-(per-1)*p->pitch/2;char nm[8];snprintf(nm,8,"%d",i+1);
        add_pad(&r.fp,-span/2,y,p->pad_x,p->pad_y,PAD_RECT,0,nm);
        snprintf(nm,8,"%d",p->pin_count-per+i+1);add_pad(&r.fp,span/2,y,p->pad_x,p->pad_y,PAD_RECT,0,nm);}
        break;}
    case IPC_QFP:{float span=p->lead_span>0?p->lead_span:p->body_x+p->pad_y*3;int per=p->pin_count/4;
        for(int k=0;k<4;k++)for(int i=0;i<per;i++){float t=i*p->pitch-(per-1)*p->pitch/2;float x,y;char nm[8];
        if(k==0){y=-span/2;x=t;}else if(k==1){x=span/2;y=t;}else if(k==2){y=span/2;x=-t;}else{x=-span/2;y=-t;}
        snprintf(nm,8,"%d",k*per+i+1);add_pad(&r.fp,x,y,k%2==0?p->pad_y:p->pad_x,k%2==0?p->pad_x:p->pad_y,PAD_RECT,0,nm);}
        break;}
    case IPC_QFN:{int per=p->pin_count/4;
        for(int k=0;k<4;k++)for(int i=0;i<per;i++){float t=i*p->pitch-(per-1)*p->pitch/2;float x,y;char nm[8];
        if(k==0){y=-p->body_y/2;x=t;}else if(k==1){x=p->body_x/2;y=t;}else if(k==2){y=p->body_y/2;x=-t;}else{x=-p->body_x/2;y=-t;}
        snprintf(nm,8,"%d",k*per+i+1);add_pad(&r.fp,x,y,p->pad_x,p->pad_y,PAD_RECT,0,nm);}
        add_pad(&r.fp,0,0,p->body_x*0.6f,p->body_y*0.6f,PAD_RECT,0,"TH");break;}
    case IPC_BGA:{for(int row=0;row<p->bga_rows;row++)for(int c=0;c<p->bga_cols;c++){char nm[8];
        float x=(c-(p->bga_cols-1)/2.0f)*p->pitch,y=(row-(p->bga_rows-1)/2.0f)*p->pitch;
        snprintf(nm,8,"%c%d",'A'+row,c+1);add_pad(&r.fp,x,y,p->ball_dia,p->ball_dia,PAD_CIRCLE,0,nm);}break;}
    case IPC_SOT23: case IPC_SOT223:{int n=p->pin_count;
        for(int i=0;i<n;i++){float x;if(i==0)x=0;else if(i==1)x=p->pitch;else x=-p->pitch;
        float y=i==0?-p->body_y/2:p->body_y/2;char nm[8];snprintf(nm,8,"%d",i+1);
        add_pad(&r.fp,x,y,p->pad_x,p->pad_y,PAD_RECT,0,nm);}break;}
    case IPC_SOD123: case IPC_SOD323:{float sp=p->body_x*0.7f;
        add_pad(&r.fp,-sp/2,0,p->pad_x,p->pad_y,PAD_RECT,0,"1");
        add_pad(&r.fp,sp/2,0,p->pad_x,p->pad_y,PAD_RECT,0,"2");break;}
    case IPC_0603: case IPC_0805: case IPC_1206: case IPC_2512:{float sp=p->body_x*0.7f;
        add_pad(&r.fp,-sp/2,0,p->pad_x,p->pad_y,PAD_RECT,0,"1");
        add_pad(&r.fp,sp/2,0,p->pad_x,p->pad_y,PAD_RECT,0,"2");break;}
    case IPC_TANT_A: case IPC_TANT_B: case IPC_TANT_C:{float sp=p->body_x*0.7f;
        add_pad(&r.fp,-sp/2,0,p->pad_x,p->pad_y,PAD_RECT,0,"1");
        add_pad(&r.fp,sp/2,0,p->pad_x,p->pad_y,PAD_RECT,0,"2");break;}
    case IPC_OSCON:{float sp=p->body_x/2;for(int i=0;i<p->pin_count;i++){float y=i*p->pitch-(p->pin_count-1)*p->pitch/2;char nm[8];snprintf(nm,8,"%d",i+1);
        add_pad(&r.fp,-sp,y,p->pad_x,p->pad_y,PAD_RECT,0,i==0?"1":nm);
        if(i==0)add_pad(&r.fp,sp,y,p->pad_x,p->pad_y,PAD_RECT,0,"4");}break;}
    case IPC_CONN_TH:{for(int i=0;i<p->pin_count;i++){float x=i*p->pitch-(p->pin_count-1)*p->pitch/2;char nm[8];snprintf(nm,8,"%d",i+1);
        add_pad(&r.fp,x,0,p->outer_dia,p->outer_dia,PAD_CIRCLE,-1,nm);}break;}
    }
    r.success=1;return r;
}

void ipc_result_free(IpcResult *r) {
    if(!r)return;for(int i=0;i<r->fp.npads;i++)free((char*)r->fp.pads[i].name);
    free(r->fp.pads);free(r->fp.name);free(r->fp.prims);free(r->fp.model3d);
}

char *ipc_params_summary(const IpcParams *p) {
    if(!p)return str_dup("NULL");
    char buf[256];snprintf(buf,sizeof(buf),"Type=%d Pins=%d Pitch=%.2fmm Body=%.1fx%.1f",p->type,p->pin_count,p->pitch,p->body_x,p->body_y);
    return str_dup(buf);
}
