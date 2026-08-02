#include "fusion_lib.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// ===== TOOL LIBRARY (25 standard tools) =====
static FusionTool g_tools[] = {
    {"tool-001","flat end mill","1mm flat end mill",1, 1.0f,0,6,30,15,0,2, 15000,47,0.05f,1500,300,400, 0.3f,0.5f,1,"ER32",25,"aluminum,plastic"},
    {"tool-002","flat end mill","2mm flat end mill",2, 2.0f,0,10,40,20,0,2, 12000,75,0.07f,1680,400,500, 0.8f,1.0f,1,"ER32",25,"aluminum,plastic,wood"},
    {"tool-003","flat end mill","3mm flat end mill",3, 3.0f,0,15,50,25,0,3, 10000,94,0.08f,2400,500,600, 1.2f,1.5f,1,"ER32",30,"aluminum,steel,plastic"},
    {"tool-004","flat end mill","4mm flat end mill",4, 4.0f,0,20,60,30,0,3, 9000,113,0.10f,2700,600,800, 1.6f,2.0f,1,"ER32",30,"aluminum,steel,plastic"},
    {"tool-005","flat end mill","6mm flat end mill",5, 6.0f,0,25,75,40,0,4, 8000,151,0.12f,3840,800,1000, 2.4f,3.0f,1,"ER32",35,"aluminum,steel,titanium"},
    {"tool-006","flat end mill","8mm flat end mill",6, 8.0f,0,30,80,45,0,4, 7000,176,0.15f,4200,900,1200, 3.2f,4.0f,1,"ER32",40,"aluminum,steel"},
    {"tool-007","flat end mill","10mm flat end mill",7, 10.0f,0,35,90,50,0,4, 6000,188,0.18f,4320,1000,1400, 4.0f,5.0f,1,"BT40",45,"aluminum,steel"},
    {"tool-008","flat end mill","12mm flat end mill",8, 12.0f,0,40,100,55,0,4, 5000,188,0.20f,4000,1000,1500, 5.0f,6.0f,1,"BT40",50,"aluminum,steel"},
    {"tool-009","flat end mill","16mm flat end mill",9, 16.0f,0,45,120,65,0,4, 4000,201,0.25f,4000,1000,1600, 6.4f,8.0f,1,"BT40",60,"aluminum,steel"},
    {"tool-010","flat end mill","20mm flat end mill",10, 20.0f,0,50,140,80,0,5, 3000,188,0.30f,4500,1000,1800, 8.0f,10.0f,1,"BT40",70,"aluminum"},

    {"tool-011","ball end mill","R1.5 ball end mill",11, 3.0f,1.5f,15,50,25,0,2, 12000,113,0.05f,1200,300,400, 0.2f,0.3f,1,"ER32",25,"aluminum,steel"},
    {"tool-012","ball end mill","R2 ball end mill",12, 4.0f,2.0f,20,60,30,0,2, 10000,126,0.06f,1200,400,500, 0.3f,0.5f,1,"ER32",30,"aluminum,steel"},
    {"tool-013","ball end mill","R3 ball end mill",13, 6.0f,3.0f,25,75,40,0,2, 8000,151,0.08f,1280,500,600, 0.5f,0.8f,1,"ER32",35,"aluminum,steel"},
    {"tool-014","ball end mill","R5 ball end mill",14, 10.0f,5.0f,35,90,50,0,2, 6000,188,0.10f,1200,600,800, 0.8f,1.2f,1,"BT40",45,"aluminum,steel"},

    {"tool-015","face mill","50mm face mill",15, 50.0f,0,8,100,60,0,5, 2000,314,0.30f,3000,300,500, 35.0f,2.0f,3,"BT40",75,"aluminum,steel"},
    {"tool-016","face mill","80mm face mill",16, 80.0f,0,10,120,70,0,6, 1500,377,0.35f,3150,300,500, 55.0f,3.0f,3,"BT40",100,"aluminum,steel"},

    {"tool-017","drill","2mm drill",20, 2.0f,0,25,60,0,118,2, 4000,25,0.05f,400,200,0, 0,2.0f,1,"ER32",20,"aluminum,steel,plastic"},
    {"tool-018","drill","3mm drill",21, 3.0f,0,30,70,0,118,2, 3500,33,0.07f,490,200,0, 0,3.0f,1,"ER32",20,"aluminum,steel"},
    {"tool-019","drill","5mm drill",22, 5.0f,0,40,80,0,118,2, 2500,39,0.10f,500,200,0, 0,5.0f,1,"ER32",25,"aluminum,steel"},
    {"tool-020","drill","8mm drill",23, 8.0f,0,50,100,0,118,2, 2000,50,0.12f,480,200,0, 0,8.0f,1,"BT40",30,"aluminum,steel"},

    {"tool-021","chamfer mill","6mm 90° chamfer",30, 6.0f,0,15,60,15,45,3, 8000,151,0.08f,1920,500,600, 0.5f,0.5f,1,"ER32",30,"aluminum,steel,plastic"},
    {"tool-022","engrave","0.5mm engraver",35, 0.5f,0,5,40,10,30,1, 20000,31,0.02f,400,200,300, 0.1f,0.1f,0,"ER32",20,"aluminum,plastic"},
    {"tool-023","slot mill","3mm slot mill (T-slot)",40, 3.0f,0,10,50,20,0,2, 8000,75,0.04f,640,300,400, 3.0f,3.0f,1,"ER32",25,"aluminum"},
    {"tool-024","tap","M3 tap",50, 3.0f,0,15,50,15,0,3, 500,5,0,0,100,0, 0,3.0f,3,"ER32",20,"aluminum,steel"},
    {"tool-025","tap","M5 tap",51, 5.0f,0,20,60,20,0,3, 400,6,0,0,100,0, 0,5.0f,3,"ER32",25,"aluminum,steel"},
};
static int g_ntools = 25;
static FusionTool *g_custom_tools = NULL; static int g_ncustom = 0;

int fusion_tool_count(void) { return g_ntools + g_ncustom; }
FusionTool *fusion_tool_get(int i) { if(i<g_ntools)return &g_tools[i]; i-=g_ntools; return i<g_ncustom?&g_custom_tools[i]:NULL; }
FusionTool *fusion_tool_find_by_type(const char *type, float diam) {
    for(int i=0;i<g_ntools;i++) if(strcmp(g_tools[i].type,type)==0 && fabsf(g_tools[i].diameter-diam)<0.01f) return &g_tools[i];
    for(int i=0;i<g_ncustom;i++) if(strcmp(g_custom_tools[i].type,type)==0 && fabsf(g_custom_tools[i].diameter-diam)<0.01f) return &g_custom_tools[i];
    return NULL;
}
FusionTool *fusion_tool_find_by_number(int n) {
    for(int i=0;i<g_ntools;i++) if(g_tools[i].tool_number==n) return &g_tools[i];
    for(int i=0;i<g_ncustom;i++) if(g_custom_tools[i].tool_number==n) return &g_custom_tools[i];
    return NULL;
}
int fusion_tool_add(FusionTool t) { g_custom_tools=(FusionTool*)realloc(g_custom_tools,sizeof(FusionTool)*(size_t)(g_ncustom+1)); g_custom_tools[g_ncustom]=t; return g_ncustom++; }

char *fusion_tool_list_all(void) {
    char buf[4096]; int off=0;
    off+=snprintf(buf+off,sizeof(buf)-off,"=== FUSION CAM TOOL LIBRARY ===\n%-4s %-20s %-10s %-8s %-8s %-10s\n","#","Type","Dia(mm)","RPM","Feed","Coolant");
    for(int i=0;i<g_ntools;i++){FusionTool*t=&g_tools[i];
        off+=snprintf(buf+off,sizeof(buf)-off,"%-4d %-20s %-10.1f %-8d %-8.0f %-10s\n",t->tool_number,t->description,t->diameter,t->spindle_speed,t->feed_rate,t->coolant==1?"Flood":t->coolant==2?"Mist":"None");}
    return str_dup(buf);
}

char *fusion_tool_list_by_material(const char *mat) {
    char buf[2048]; int off=0,found=0;
    for(int i=0;i<g_ntools;i++){FusionTool*t=&g_tools[i]; if(strstr(t->materials,mat)){found++;
        off+=snprintf(buf+off,sizeof(buf)-off,"[%d] %s %.1fmm %dRPM %.0fmm/min\n",t->tool_number,t->description,t->diameter,t->spindle_speed,t->feed_rate);}}
    if(!found)off+=snprintf(buf+off,sizeof(buf)-off,"No tools for %s\n",mat);
    return str_dup(buf);
}

bool fusion_library_load(const char *path) { (void)path; return false; }
bool fusion_library_save(const char *path) {
    if(!path)return false; FILE*f=fopen(path,"w");if(!f)return false;
    fprintf(f,"{\"data\":[\n");
    for(int i=0;i<g_ntools;i++){FusionTool*t=&g_tools[i];
        fprintf(f,"{\"guid\":\"%s\",\"type\":\"%s\",\"number\":%d,\"diameter\":%.3f,\"rpm\":%d,\"feed\":%.0f,\"coolant\":%d,\"material\":\"%s\"}%s\n",
            t->guid,t->type,t->tool_number,t->diameter,t->spindle_speed,t->feed_rate,t->coolant,t->materials,i<g_ntools-1?",":"");}
    fprintf(f,"]}\n");fclose(f);return true;
}

// ===== MATERIAL LIBRARY =====
static FusionMaterial g_materials[] = {
    {"Aluminum 6061","metal",2.7f,95, 200,0.10f,40,25},
    {"Aluminum 7075","metal",2.81f,150, 150,0.08f,35,20},
    {"Steel 1018","metal",7.87f,130, 100,0.06f,30,15},
    {"Steel 4140","metal",7.85f,200, 70,0.05f,25,12},
    {"Stainless 304","metal",8.0f,200, 50,0.04f,20,10},
    {"Titanium Ti6Al4V","metal",4.43f,350, 40,0.03f,15,8},
    {"Brass","metal",8.5f,80, 250,0.12f,45,30},
    {"Copper","metal",8.96f,50, 120,0.08f,35,20},
    {"Acrylic","plastic",1.19f,30, 300,0.12f,50,40},
    {"Delrin/POM","plastic",1.41f,40, 350,0.15f,50,45},
    {"Nylon","plastic",1.14f,30, 300,0.12f,45,40},
    {"Polycarbonate","plastic",1.2f,50, 250,0.10f,45,35},
    {"Wood (Hard)","wood",0.7f,20, 400,0.20f,60,50},
    {"Carbon Fiber","composite",1.6f,60, 80,0.04f,25,10},
    {"Foam","foam",0.05f,2, 500,0.30f,70,60},
};
int fusion_material_count(void) { return 15; }
FusionMaterial *fusion_material_get(int i) { return i>=0&&i<15?&g_materials[i]:NULL; }
FusionMaterial *fusion_material_find(const char *nm) { for(int i=0;i<15;i++) if(strcmp(g_materials[i].name,nm)==0) return &g_materials[i]; return NULL; }
char *fusion_material_list_all(void) {
    char buf[2048]; int off=0;
    off+=snprintf(buf+off,sizeof(buf)-off,"=== FUSION MATERIAL LIBRARY ===\n%-20s %-10s %-8s %-8s %-8s\n","Name","Type","Density","SFM","FPT");
    for(int i=0;i<15;i++){FusionMaterial*m=&g_materials[i];
        off+=snprintf(buf+off,sizeof(buf)-off,"%-20s %-10s %-8.1f %-8.0f %-8.2f\n",m->name,m->type,m->density,m->surface_speed,m->feed_per_tooth);}
    return str_dup(buf);
}

// ===== MACHINE LIBRARY =====
static FusionMachine g_machines[] = {
    {"Tormach 440","mill", 250,160,250, 10000,1.5f,"BT30",10,1,0,3,"linuxcnc","linuxcnc.cps"},
    {"Haas VF-2","mill", 762,406,508, 8100,22.4f,"BT40",30,1,1,4,"haas","haas_vf.cps"},
    {"ShopBot PRSalpha","router", 2400,1500,200, 18000,3.7f,"ER32",1,0,0,3,"grbl","grbl.cps"},
    {"OMAX 2626","waterjet", 660,660,150, 0,30.0f,"",0,0,0,2,"omax","omax.cps"},
    {"Universal Laser","laser", 813,457,200, 0,0, "",0,0,0,2,"grbl","grbl_laser.cps"},
    {"Hypertherm Powermax45","plasma", 1500,3000,100, 0,0, "",0,0,0,2,"linuxcnc","plasma.cps"},
    {"Haas ST-10","lathe", 0,0,356, 6000,11.0f,"",12,1,0,2,"haas","haas_lathe.cps"},
};
int fusion_machine_count(void) { return 7; }
FusionMachine *fusion_machine_get(int i) { return i>=0&&i<7?&g_machines[i]:NULL; }
FusionMachine *fusion_machine_find(const char *nm) { for(int i=0;i<7;i++) if(strcmp(g_machines[i].name,nm)==0) return &g_machines[i]; return NULL; }
char *fusion_machine_list_all(void) {
    char buf[1024]; int off=0;
    off+=snprintf(buf+off,sizeof(buf)-off,"=== FUSION MACHINE LIBRARY ===\n");
    for(int i=0;i<7;i++){FusionMachine*m=&g_machines[i];
        off+=snprintf(buf+off,sizeof(buf)-off,"  %-20s %-10s %dx%dx%d %dRPM %s\n",m->name,m->type,(int)m->x_travel,(int)m->y_travel,(int)m->z_travel,(int)m->max_spindle_rpm,m->controller);}
    return str_dup(buf);
}

// ===== CAM RECOMMENDATIONS =====
char *fusion_cam_recommendations(const char *mat, const char *op) {
    FusionMaterial *m = fusion_material_find(mat);
    if(!m) return str_dup("Material not found");
    char buf[512]; int off=0;
    off+=snprintf(buf+off,sizeof(buf)-off,"=== CAM RECOMMENDATIONS ===\nMaterial: %s\nOperation: %s\n",mat,op?op:"?");
    off+=snprintf(buf+off,sizeof(buf)-off,"Surface speed: %.0f m/min\nFeed/tooth: %.2f mm\nStepover: %d%%\nStepdown: %d%%\n",
        m->surface_speed, m->feed_per_tooth, m->stepover_pct, m->stepdown_pct);
    // suggest tools
    off+=snprintf(buf+off,sizeof(buf)-off,"\nSuggested tools:\n");
    for(int i=0;i<g_ntools;i++) if(strstr(g_tools[i].materials,mat)) off+=snprintf(buf+off,sizeof(buf)-off,"  [%d] %s\n",g_tools[i].tool_number,g_tools[i].description);
    return str_dup(buf);
}
