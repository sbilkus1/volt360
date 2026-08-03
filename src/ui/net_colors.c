#include "net_colors.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int str_case_startswith(const char *s, const char *prefix) {
    if (!s || !prefix) return 0;
    size_t slen = strlen(s), plen = strlen(prefix);
    if (slen < plen) return 0;
    for (size_t i = 0; i < plen; i++) {
        if (toupper((unsigned char)s[i]) != toupper((unsigned char)prefix[i])) return 0;
    }
    if (slen == plen) return 1;
    if (!isalnum((unsigned char)s[plen]) && s[plen] != '_') return 0;
    return 1;
}

void netcolor_init(NetColorMap *ncm) {
    memset(ncm, 0, sizeof(*ncm));
    ncm->default_color = (Color){ 180, 180, 180, 255 };
    ncm->enabled = 1;

    netcolor_add_rule(ncm, "VCC", NC_POWER, (Color){ 220, 40, 40, 255 });
    netcolor_add_rule(ncm, "VDD", NC_POWER, (Color){ 220, 50, 50, 255 });
    netcolor_add_rule(ncm, "3V3", NC_POWER, (Color){ 230, 30, 30, 255 });
    netcolor_add_rule(ncm, "3.3V", NC_POWER, (Color){ 230, 30, 30, 255 });
    netcolor_add_rule(ncm, "5V", NC_POWER, (Color){ 240, 20, 20, 255 });
    netcolor_add_rule(ncm, "VBAT", NC_POWER, (Color){ 200, 60, 20, 255 });
    netcolor_add_rule(ncm, "PWR", NC_POWER, (Color){ 210, 45, 45, 255 });
    netcolor_add_rule(ncm, "VIN", NC_POWER, (Color){ 215, 40, 40, 255 });
    netcolor_add_rule(ncm, "VREF", NC_POWER, (Color){ 220, 60, 60, 255 });
    netcolor_add_rule(ncm, "VCCINT", NC_POWER, (Color){ 225, 45, 45, 255 });
    netcolor_add_rule(ncm, "VCORE", NC_POWER, (Color){ 215, 35, 35, 255 });
    netcolor_add_rule(ncm, "+12V", NC_POWER, (Color){ 235, 25, 25, 255 });
    netcolor_add_rule(ncm, "+5V", NC_POWER, (Color){ 240, 20, 20, 255 });
    netcolor_add_rule(ncm, "+3.3V", NC_POWER, (Color){ 230, 30, 30, 255 });

    netcolor_add_rule(ncm, "GND", NC_GROUND, (Color){ 40, 140, 40, 255 });
    netcolor_add_rule(ncm, "AGND", NC_GROUND, (Color){ 50, 150, 50, 255 });
    netcolor_add_rule(ncm, "DGND", NC_GROUND, (Color){ 45, 145, 45, 255 });
    netcolor_add_rule(ncm, "PGND", NC_GROUND, (Color){ 40, 135, 40, 255 });
    netcolor_add_rule(ncm, "VSS", NC_GROUND, (Color){ 55, 145, 55, 255 });

    netcolor_add_rule(ncm, "CLK", NC_CLOCK, (Color){ 30, 200, 220, 255 });
    netcolor_add_rule(ncm, "SCK", NC_CLOCK, (Color){ 40, 210, 230, 255 });
    netcolor_add_rule(ncm, "OSC", NC_CLOCK, (Color){ 50, 190, 210, 255 });
    netcolor_add_rule(ncm, "XTAL", NC_CLOCK, (Color){ 35, 205, 225, 255 });
    netcolor_add_rule(ncm, "MCLK", NC_CLOCK, (Color){ 45, 200, 215, 255 });

    netcolor_add_rule(ncm, "TX+", NC_DIFFPAIR, (Color){ 255, 140, 40, 255 });
    netcolor_add_rule(ncm, "TX-", NC_DIFFPAIR, (Color){ 250, 130, 30, 255 });
    netcolor_add_rule(ncm, "RX+", NC_DIFFPAIR, (Color){ 255, 150, 50, 255 });
    netcolor_add_rule(ncm, "RX-", NC_DIFFPAIR, (Color){ 250, 145, 45, 255 });
    netcolor_add_rule(ncm, "DP", NC_DIFFPAIR, (Color){ 245, 135, 35, 255 });
    netcolor_add_rule(ncm, "DN", NC_DIFFPAIR, (Color){ 248, 138, 38, 255 });
    netcolor_add_rule(ncm, "D+", NC_DIFFPAIR, (Color){ 245, 135, 35, 255 });
    netcolor_add_rule(ncm, "D-", NC_DIFFPAIR, (Color){ 248, 138, 38, 255 });

    netcolor_add_rule(ncm, "RF", NC_RF, (Color){ 200, 50, 200, 255 });
    netcolor_add_rule(ncm, "ANT", NC_RF, (Color){ 210, 60, 210, 255 });
    netcolor_add_rule(ncm, "PA", NC_RF, (Color){ 190, 45, 190, 255 });
    netcolor_add_rule(ncm, "LNA", NC_RF, (Color){ 195, 55, 195, 255 });

    netcolor_add_rule(ncm, "AIN", NC_ANALOG, (Color){ 60, 140, 240, 255 });
    netcolor_add_rule(ncm, "ADC", NC_ANALOG, (Color){ 70, 150, 250, 255 });
    netcolor_add_rule(ncm, "SENSE", NC_ANALOG, (Color){ 50, 130, 230, 255 });
    netcolor_add_rule(ncm, "TEMP", NC_ANALOG, (Color){ 65, 145, 245, 255 });
}

void netcolor_add_rule(NetColorMap *ncm, const char *pattern, NetClass cls, Color c) {
    int n = ncm->nrules;
    NetClassRule *r = (NetClassRule *)realloc(ncm->rules, sizeof(NetClassRule) * (size_t)(n + 1));
    if (!r) return;
    ncm->rules = r;
    memset(&ncm->rules[n], 0, sizeof(NetClassRule));
    strncpy(ncm->rules[n].net_name, pattern, sizeof(ncm->rules[n].net_name) - 1);
    ncm->rules[n].cls = cls;
    ncm->rules[n].color = c;
    ncm->rules[n].min_width = 0;
    ncm->rules[n].min_clearance = 0;
    ncm->nrules = n + 1;
}

static int net_seen(const char **seen, int nseen, const char *net) {
    if (!net || !net[0]) return 1;
    for (int i = 0; i < nseen; i++) {
        if (seen[i] && strcmp(seen[i], net) == 0) return 1;
    }
    return 0;
}

void netcolor_auto_classify(NetColorMap *ncm, const Pcb *pcb) {
    if (!pcb) return;
    const char **seen = NULL;
    int nseen = 0;
    for (int i = 0; i < pcb->ntracks; i++) {
        const char *net = pcb->tracks[i].net;
        if (!net || !net[0]) continue;
        if (net_seen(seen, nseen, net)) continue;
        int found = 0;
        for (int j = 0; j < ncm->nrules; j++) {
            if (str_case_startswith(net, ncm->rules[j].net_name)) {
                found = 1; break;
            }
        }
        if (!found) {
            NetClass cls = netcolor_guess_class(net);
            Color c = ncm->default_color;
            if (cls == NC_SIGNAL) c = (Color){ 160, 170, 185, 255 };
            netcolor_add_rule(ncm, net, cls, c);
        }
        const char **s = (const char **)realloc(seen, sizeof(char *) * (size_t)(nseen + 1));
        if (!s) continue;
        seen = s;
        seen[nseen++] = net;
    }
    free(seen);
}

NetClass netcolor_guess_class(const char *net_name) {
    if (!net_name) return NC_SIGNAL;
    const char *ups[] = { "VCC", "VDD", "3V3", "3.3V", "5V", "VBAT", "PWR", "VIN", "VREF", "VCCINT", "VCORE", "+12V", "+5V", "+3.3V" };
    for (int i = 0; i < 14; i++) { if (str_case_startswith(net_name, ups[i])) return NC_POWER; }
    const char *gnds[] = { "GND", "AGND", "DGND", "PGND", "VSS" };
    for (int i = 0; i < 5; i++) { if (str_case_startswith(net_name, gnds[i])) return NC_GROUND; }
    const char *clks[] = { "CLK", "SCK", "OSC", "XTAL", "MCLK" };
    for (int i = 0; i < 5; i++) { if (str_case_startswith(net_name, clks[i])) return NC_CLOCK; }
    const char *diffs[] = { "TX+", "TX-", "RX+", "RX-", "DP", "DN", "D+", "D-" };
    for (int i = 0; i < 8; i++) { if (str_case_startswith(net_name, diffs[i])) return NC_DIFFPAIR; }
    const char *rfs[] = { "RF", "ANT", "PA", "LNA" };
    for (int i = 0; i < 4; i++) { if (str_case_startswith(net_name, rfs[i])) return NC_RF; }
    const char *anas[] = { "AIN", "ADC", "SENSE", "TEMP" };
    for (int i = 0; i < 4; i++) { if (str_case_startswith(net_name, anas[i])) return NC_ANALOG; }
    return NC_SIGNAL;
}

Color netcolor_get(const NetColorMap *ncm, const char *net_name) {
    if (!ncm || !net_name || !net_name[0]) return ncm ? ncm->default_color : GRAY;
    for (int i = ncm->nrules - 1; i >= 0; i--) {
        if (str_case_startswith(net_name, ncm->rules[i].net_name))
            return ncm->rules[i].color;
    }
    return ncm->default_color;
}

void netcolor_render_legend(const NetColorMap *ncm, Rectangle area) {
    if (!ncm || ncm->nrules == 0) return;
    int x = (int)area.x + 10, y = (int)area.y + 10;
    int box_size = 12;
    int spacing = 18;
    const char *class_labels[] = { "POWER", "GND", "SIGNAL", "CLOCK", "DIFF", "RF", "ANALOG", "CUSTOM" };
    Color class_colors[8];
    int class_seen[8] = { 0 };
    for (int i = 0; i < 8; i++) class_colors[i] = BLANK;
    for (int i = 0; i < ncm->nrules; i++) {
        int cls = (int)ncm->rules[i].cls;
        if (cls >= 0 && cls < 8 && !class_seen[cls]) {
            class_colors[cls] = ncm->rules[i].color;
            class_seen[cls] = 1;
        }
    }
    int cols = 0;
    int max_per_col = 4;
    for (int i = 0; i < 8; i++) {
        if (!class_seen[i]) continue;
        int col = cols / max_per_col;
        int row = cols % max_per_col;
        int lx = x + col * 100;
        int ly = y + row * spacing;
        DrawRectangle(lx, ly, box_size, box_size, class_colors[i]);
        DrawText(class_labels[i], lx + box_size + 4, ly, 12, WHITE);
        cols++;
    }
}
