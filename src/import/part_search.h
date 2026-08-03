#pragma once

typedef enum { PART_VENDOR_DIGIKEY, PART_VENDOR_LCSC, PART_VENDOR_MOUSER, PART_VENDOR_JLCPCB } PartVendor;

typedef struct {
    char mpn[64];
    char mfg[32];
    char desc[256];
    char package[32];
    float price_1;
    float price_100;
    float price_1000;
    int stock;
    char url[256];
    PartVendor vendor;
} PartResult;

typedef struct {
    PartResult *results;
    int nresults;
    char query[128];
    PartVendor vendor;
    int searching;
} PartSearch;

void part_search_init(PartSearch *ps);
void part_search_query(PartSearch *ps, const char *query, PartVendor vendor);
void part_search_free(PartSearch *ps);
int part_search_count(const PartSearch *ps);
const PartResult *part_search_get(const PartSearch *ps, int index);
char *part_search_summary(const PartSearch *ps);
