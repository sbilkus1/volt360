#pragma once
#include "../model/project.h"

typedef struct {
    float amplitude;       /* mm, default 1.0 */
    float gap;             /* mm between meander peaks, default 0.5 */
    float min_segment;     /* mm minimum straight segment, default 0.5 */
    int max_meanders;      /* cap on meander sections */
} MeanderConfig;

typedef struct {
    V2 *meander_points;
    int npoints;
    float original_length;
    float tuned_length;
    float target_length;
    int success;
} MeanderResult;

void meander_config_default(MeanderConfig *cfg);
MeanderResult meander_generate(const V2 *original_path, int npoints, float target_length, const MeanderConfig *cfg);
void meander_result_free(MeanderResult *r);
MeanderResult meander_diff_pair(const V2 *trace_a, int na, const V2 *trace_b, int nb, const MeanderConfig *cfg);
void meander_apply_to_pcb(Pcb *pcb, int track_idx, const MeanderResult *mr);
