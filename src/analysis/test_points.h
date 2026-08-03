#pragma once
#include "../model/project.h"

typedef struct {
    float pad_dia;
    float drill_dia;
    float min_spacing;
    float edge_margin;
    int max_per_net;
    int only_unconnected;
} TestPointConfig;

typedef struct {
    V2 *positions;
    char **net_names;
    int *via_indices;
    int npoints;
    int added;
} TestPointResult;

void testpoint_config_default(TestPointConfig *cfg);
TestPointResult testpoint_insert(Pcb *pcb, const TestPointConfig *cfg);
void testpoint_result_free(TestPointResult *r);
char *testpoint_report(const TestPointResult *r);
void testpoint_remove_all(Pcb *pcb);
