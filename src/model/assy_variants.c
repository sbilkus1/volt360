#include "assy_variants.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void variant_set_init(VariantSet *vs, const char *pcb_id) {
    memset(vs, 0, sizeof(*vs));
    vs->pcb_id = str_dup(pcb_id ? pcb_id : "");
}

void variant_set_free(VariantSet *vs) {
    for (int i = 0; i < vs->nvariants; i++) {
        free(vs->variants[i].parts);
    }
    free(vs->variants);
    free(vs->pcb_id);
    memset(vs, 0, sizeof(*vs));
}

int variant_add(VariantSet *vs, const char *name, const char *desc) {
    AssemblyVariant *tmp = (AssemblyVariant *)realloc(vs->variants,
        (size_t)(vs->nvariants + 1) * sizeof(AssemblyVariant));
    if (!tmp) return -1;
    vs->variants = tmp;
    int idx = vs->nvariants++;
    memset(&vs->variants[idx], 0, sizeof(AssemblyVariant));
    snprintf(vs->variants[idx].name, sizeof(vs->variants[idx].name), "%s", name);
    snprintf(vs->variants[idx].desc, sizeof(vs->variants[idx].desc), "%s", desc ? desc : "");
    vs->variants[idx].total_cost = 0.0f;
    vs->variants[idx].total_populated = 0;
    return idx;
}

void variant_set_part(VariantSet *vs, int variant_idx, const char *designator,
                      const char *value, const char *mfg, int populated) {
    if (variant_idx < 0 || variant_idx >= vs->nvariants) return;
    AssemblyVariant *av = &vs->variants[variant_idx];

    for (int i = 0; i < av->nparts; i++) {
        if (strcmp(av->parts[i].designator, designator) == 0) {
            snprintf(av->parts[i].variant_value, sizeof(av->parts[i].variant_value), "%s", value ? value : "");
            snprintf(av->parts[i].variant_mfg, sizeof(av->parts[i].variant_mfg), "%s", mfg ? mfg : "");
            av->parts[i].populated = populated ? 1 : 0;
            return;
        }
    }

    VariantPart *tmp = (VariantPart *)realloc(av->parts,
        (size_t)(av->nparts + 1) * sizeof(VariantPart));
    if (!tmp) return;
    av->parts = tmp;
    int idx = av->nparts++;
    memset(&av->parts[idx], 0, sizeof(VariantPart));
    snprintf(av->parts[idx].designator, sizeof(av->parts[idx].designator), "%s", designator ? designator : "");
    snprintf(av->parts[idx].variant_value, sizeof(av->parts[idx].variant_value), "%s", value ? value : "");
    snprintf(av->parts[idx].variant_mfg, sizeof(av->parts[idx].variant_mfg), "%s", mfg ? mfg : "");
    av->parts[idx].populated = populated ? 1 : 0;
}

void variant_import_from_pcb(VariantSet *vs, const Pcb *pcb) {
    if (!pcb) return;

    int idx = variant_add(vs, "Full Population", "Default production variant with all parts fitted");
    if (idx < 0) return;

    AssemblyVariant *av = &vs->variants[idx];
    av->total_populated = pcb->nfps;

    for (int i = 0; i < pcb->nfps; i++) {
        FpInst *fp = &pcb->fps[i];
        variant_set_part(vs, idx,
            fp->ref ? fp->ref : "?",
            fp->value ? fp->value : "",
            "",
            1);
    }
}

char *variant_bom_export(const AssemblyVariant *av) {
    if (!av) return str_dup("");
    Buf b; buf_init(&b);
    buf_appendf(&b, "Designator,Value,Manufacturer,Populated,Notes\n");
    for (int i = 0; i < av->nparts; i++) {
        VariantPart *vp = &av->parts[i];
        buf_appendf(&b, "%s,%s,%s,%s,%s\n",
            vp->designator,
            vp->variant_value,
            vp->variant_mfg[0] ? vp->variant_mfg : "N/A",
            vp->populated ? "Yes" : "DNP",
            vp->notes[0] ? vp->notes : "");
    }
    return buf_cstr(&b);
}

static const char *part_category(const char *value) {
    if (!value || !value[0]) return "Other";
    char tmp[64]; strncpy(tmp, value, sizeof(tmp)); str_lower(tmp);
    if (strstr(tmp, "res") || strstr(tmp, "ohm")) return "Passives";
    if (strstr(tmp, "cap") || strstr(tmp, "uf") || strstr(tmp, "nf") || strstr(tmp, "pf")) return "Passives";
    if (strstr(tmp, "ind") || strstr(tmp, "uh") || strstr(tmp, "mh") || strstr(tmp, "ferrite")) return "Passives";
    if (strstr(tmp, "crystal") || strstr(tmp, "mhz") || strstr(tmp, "osc")) return "Passives";
    if (strstr(tmp, "ic ") || strstr(tmp, "mcu") || strstr(tmp, "stm32") ||
        strstr(tmp, "esp32") || strstr(tmp, "reg ") || strstr(tmp, "ldo") ||
        strstr(tmp, "soc") || strstr(tmp, "uart") || strstr(tmp, "usb-")) return "ICs";
    if (strstr(tmp, "diode") || strstr(tmp, "transistor") || strstr(tmp, "mosfet") ||
        strstr(tmp, "npn") || strstr(tmp, "pnp") || strstr(tmp, "n-ch") || strstr(tmp, "tvs")) return "Semiconductors";
    if (strstr(tmp, "conn") || strstr(tmp, "header") || strstr(tmp, "socket") ||
        strstr(tmp, "pin ") || strstr(tmp, "receptacle") || strstr(tmp, "plug")) return "Connectors";
    return "Other";
}

char *variant_compare_costs(const VariantSet *vs) {
    Buf b; buf_init(&b);
    buf_appendf(&b, "%-24s %10s %6s\n", "Variant", "Total Cost", "Parts");
    buf_append_str(&b, "-----------------------------------------------\n");

    for (int i = 0; i < vs->nvariants; i++) {
        AssemblyVariant *av = &vs->variants[i];
        float ic_cost = 0.0f, passives_cost = 0.0f, semi_cost = 0.0f, conn_cost = 0.0f, other_cost = 0.0f;
        int ic_cnt = 0, passives_cnt = 0, semi_cnt = 0, conn_cnt = 0, other_cnt = 0;

        for (int j = 0; j < av->nparts; j++) {
            if (!av->parts[j].populated) continue;
            float cpart = 0.10f; /* default estimate */
            const char *cat = part_category(av->parts[j].variant_value);
            if (strcmp(cat, "ICs") == 0) { ic_cost += cpart; ic_cnt++; }
            else if (strcmp(cat, "Passives") == 0) { passives_cost += cpart; passives_cnt++; }
            else if (strcmp(cat, "Semiconductors") == 0) { semi_cost += cpart; semi_cnt++; }
            else if (strcmp(cat, "Connectors") == 0) { conn_cost += cpart; conn_cnt++; }
            else { other_cost += cpart; other_cnt++; }
        }

        float total = ic_cost + passives_cost + semi_cost + conn_cost + other_cost;
        buf_appendf(&b, "%-24s $%8.2f %3d parts\n", av->name, total, av->total_populated);
        if (ic_cnt > 0)         buf_appendf(&b, "  ICs:            $%8.2f (%2d)\n", ic_cost, ic_cnt);
        if (passives_cnt > 0)   buf_appendf(&b, "  Passives:       $%8.2f (%2d)\n", passives_cost, passives_cnt);
        if (semi_cnt > 0)       buf_appendf(&b, "  Semiconductors: $%8.2f (%2d)\n", semi_cost, semi_cnt);
        if (conn_cnt > 0)       buf_appendf(&b, "  Connectors:     $%8.2f (%2d)\n", conn_cost, conn_cnt);
        if (other_cnt > 0)      buf_appendf(&b, "  Other:          $%8.2f (%2d)\n", other_cost, other_cnt);
        if (i < vs->nvariants - 1) buf_append_str(&b, "\n");
    }
    return buf_cstr(&b);
}
