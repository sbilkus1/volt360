#pragma once
// bga_fanout.h - BGA ball fanout via pattern generator
#include "../model/project.h"

typedef struct {
    int rows;
    int cols;
    float pitch;
    float ball_dia;
    float via_dia;
    float via_pad;
    float trace_width;
    int layers;
    int fanout_layers;
} BgaPackage;

typedef struct {
    V2 *via_positions;
    V2 *trace_points;  /* pairs: [start0,end0, start1,end1, ...] */
    int nvias;
    int ntraces;
    int success;
    int balls_total;
    int balls_routed;
} BgaFanout;

void bga_fanout_init(BgaFanout *fo);
void bga_fanout_free(BgaFanout *fo);
BgaFanout bga_generate_fanout(const BgaPackage *pkg, V2 origin, int start_row, int start_col);
void bga_fanout_to_pcb(const BgaFanout *fo, Pcb *pcb, const char *net_prefix);
