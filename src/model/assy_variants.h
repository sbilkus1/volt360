#pragma once
#include "project.h"

typedef struct {
    char designator[16];
    char variant_value[64];
    char variant_mfg[32];
    int populated;
    char notes[128];
} VariantPart;

typedef struct {
    char name[64];
    char desc[256];
    VariantPart *parts;
    int nparts;
    float total_cost;
    int total_populated;
} AssemblyVariant;

typedef struct {
    AssemblyVariant *variants;
    int nvariants;
    char *pcb_id;
} VariantSet;

void variant_set_init(VariantSet *vs, const char *pcb_id);
void variant_set_free(VariantSet *vs);
int variant_add(VariantSet *vs, const char *name, const char *desc);
void variant_set_part(VariantSet *vs, int variant_idx, const char *designator, const char *value, const char *mfg, int populated);
void variant_import_from_pcb(VariantSet *vs, const Pcb *pcb);
char *variant_bom_export(const AssemblyVariant *av);
char *variant_compare_costs(const VariantSet *vs);
