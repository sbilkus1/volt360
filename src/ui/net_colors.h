#pragma once
#include "../model/project.h"
#include "../../third_party/raylib/src/raylib.h"

typedef enum { NC_POWER, NC_GROUND, NC_SIGNAL, NC_CLOCK, NC_DIFFPAIR, NC_RF, NC_ANALOG, NC_CUSTOM } NetClass;

typedef struct {
    char net_name[32];
    NetClass cls;
    Color color;
    float min_width;
    float min_clearance;
} NetClassRule;

typedef struct {
    NetClassRule *rules;
    int nrules;
    Color default_color;
    int enabled;
} NetColorMap;

void netcolor_init(NetColorMap *ncm);
void netcolor_add_rule(NetColorMap *ncm, const char *pattern, NetClass cls, Color c);
void netcolor_auto_classify(NetColorMap *ncm, const Pcb *pcb);
Color netcolor_get(const NetColorMap *ncm, const char *net_name);
NetClass netcolor_guess_class(const char *net_name);
void netcolor_render_legend(const NetColorMap *ncm, Rectangle area);
