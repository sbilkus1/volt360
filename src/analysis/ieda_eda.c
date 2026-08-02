#include "ieda_eda.h"
#include "../core/util.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

// ===== SIMULATED ANNEALING PLACEMENT =====
float place_total_wirelength(Pcb *pcb) {
    if (!pcb) return 0;
    float total = 0;
    // estimate wirelength as half-perimeter of connected pads (tracks)
    for (int i = 0; i < pcb->ntracks; i++)
        total += fabsf(pcb->tracks[i].a.x - pcb->tracks[i].b.x) + fabsf(pcb->tracks[i].a.y - pcb->tracks[i].b.y);
    return total;
}

int place_simulated_annealing(Pcb *pcb, float T_start, float T_end, float cool, int moves_per_temp) {
    if (!pcb || pcb->nfps < 2) return 0;
    float T = T_start;
    srand((unsigned)time(NULL));
    int moves = 0;

    while (T > T_end) {
        for (int m = 0; m < moves_per_temp; m++) {
            // random swap of two footprint positions
            int i = rand() % pcb->nfps, j = rand() % pcb->nfps;
            if (i == j) continue;
            float old_cost = place_total_wirelength(pcb);
            // swap positions
            V2 tmp = pcb->fps[i].pos; pcb->fps[i].pos = pcb->fps[j].pos; pcb->fps[j].pos = tmp;
            float new_cost = place_total_wirelength(pcb);
            float delta = new_cost - old_cost;
            if (delta > 0 && ((float)rand() / (float)RAND_MAX) > expf(-delta / T)) {
                // reject: swap back
                tmp = pcb->fps[i].pos; pcb->fps[i].pos = pcb->fps[j].pos; pcb->fps[j].pos = tmp;
            }
            moves++;
        }
        T *= cool;
    }
    return moves;
}

// ===== A* MAZE ROUTER =====
#define GRID_MAX 1024
typedef struct { int x, y; int g, h; int parent; int closed; } AStarNode;

static int astar_heuristic(int cx, int cy, int gx, int gy) { return abs(cx-gx) + abs(cy-gy); }

int route_astar(Pcb *pcb, V2 start, V2 end, float grid_res, float clearance, V2 *path, int max_points) {
    if (!pcb || grid_res <= 0 || !path || max_points < 2) return 0;
    int gx = (int)(end.x / grid_res), gy = (int)(end.y / grid_res);
    int sx = (int)(start.x / grid_res), sy = (int)(start.y / grid_res);
    int w = GRID_MAX, h = GRID_MAX;
    if (sx < 0 || sy < 0 || gx < 0 || gy < 0 || sx >= w || sy >= h || gx >= w || gy >= h) return 0;

    AStarNode *nodes = (AStarNode *)calloc((size_t)(w * h), sizeof(AStarNode));
    int *open = (int *)malloc(sizeof(int) * (size_t)(w * h));
    int open_count = 0;

    // mark obstacles
    for (int t = 0; t < pcb->ntracks; t++) {
        int x1=(int)(pcb->tracks[t].a.x/grid_res), y1=(int)(pcb->tracks[t].a.y/grid_res);
        int x2=(int)(pcb->tracks[t].b.x/grid_res), y2=(int)(pcb->tracks[t].b.y/grid_res);
        // Bresenham line marking
        int dx=abs(x2-x1),dy=abs(y2-y1),sx2=x1<x2?1:-1,sy2=y1<y2?1:-1,err=dx-dy;
        for(;;){ if(x1>=0&&x1<w&&y1>=0&&y1<h)nodes[x1*h+y1].closed=2; if(x1==x2&&y1==y2)break;
            int e2=err*2; if(e2>-dy){err-=dy;x1+=sx2;} if(e2<dx){err+=dx;y1+=sy2;} }
    }
    // mark pads
    for (int f = 0; f < pcb->nfps; f++) {
        int cx=(int)(pcb->fps[f].pos.x/grid_res), cy=(int)(pcb->fps[f].pos.y/grid_res);
        int r=(int)(clearance*2/grid_res); if(r<1)r=1;
        for (int dx=-r;dx<=r;dx++) for(int dy=-r;dy<=r;dy++) { int nx=cx+dx,ny=cy+dy; if(nx>=0&&nx<w&&ny>=0&&ny<h)nodes[nx*h+ny].closed=2; }
    }

    nodes[sx*h+sy].g = 0; nodes[sx*h+sy].h = astar_heuristic(sx,sy,gx,gy);
    open[open_count++] = sx * h + sy;

    int found = 0, goal_idx = -1;
    while (open_count > 0) {
        // find lowest f = g+h
        int best = 0;
        for (int i = 1; i < open_count; i++) {
            int ai = open[i], bi = open[best];
            if (nodes[ai].g + nodes[ai].h < nodes[bi].g + nodes[bi].h) best = i;
        }
        int cur = open[best];
        open[best] = open[--open_count];
        int cx = cur / h, cy = cur % h;
        if (nodes[cur].closed) continue;
        nodes[cur].closed = 1;
        if (cx == gx && cy == gy) { found = 1; goal_idx = cur; break; }

        int neighbors[8][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
        for (int n = 0; n < 8; n++) {
            int nx = cx + neighbors[n][0], ny = cy + neighbors[n][1];
            if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;
            int ni = nx * h + ny;
            if (nodes[ni].closed > 0) continue;
            int ng = nodes[cur].g + (n < 4 ? 10 : 14);
            if (!nodes[ni].closed || ng < nodes[ni].g) {
                nodes[ni].g = ng; nodes[ni].h = astar_heuristic(nx, ny, gx, gy);
                nodes[ni].parent = cur; nodes[ni].closed = 0;
                open[open_count++] = ni;
            }
        }
    }

    int count = 0;
    if (found) {
        int back = goal_idx;
        while (back != sx * h + sy && count < max_points) {
            path[count++] = v2((back / h) * grid_res, (back % h) * grid_res);
            back = nodes[back].parent;
        }
        path[count++] = start;
        // reverse
        for (int i = 0; i < count / 2; i++) { V2 t = path[i]; path[i] = path[count-1-i]; path[count-1-i] = t; }
    }
    free(nodes); free(open);
    return count;
}

int route_all_nets(Pcb *pcb, float grid_res, float clearance) {
    if (!pcb) return 0;
    int routed = 0;
    // route each unrouteable pair (simplified: just existing tracks)
    for (int i = 0; i < pcb->ntracks; i++) {
        V2 path[256];
        int n = route_astar(pcb, pcb->tracks[i].a, pcb->tracks[i].b, grid_res, clearance, path, 256);
        if (n >= 2) {
            // add the routed segments
            for (int j = 1; j < n; j++) {
                Track t; t.a = path[j-1]; t.b = path[j]; t.width = pcb->tracks[i].width; t.net = pcb->tracks[i].net ? str_dup(pcb->tracks[i].net) : NULL;
                pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track)*(size_t)(pcb->ntracks+1));
                pcb->tracks[pcb->ntracks++] = t;
            }
            routed++;
        }
    }
    return routed;
}

// ===== CLOCK TREE SYNTHESIS (H-TREE) =====
int cts_build_htree(Pcb *pcb, V2 src, V2 *sinks, int n, float width) {
    if (!pcb || !sinks || n < 1) return 0;
    int added = 0;
    // build binary tree: recursively connect sinks pairwise to their midpoint, then to src
    V2 *nodes = (V2 *)malloc(sizeof(V2)*(size_t)n);
    memcpy(nodes, sinks, sizeof(V2)*(size_t)n);
    int remaining = n;

    while (remaining > 1) {
        for (int i = 0; i < remaining - 1; i += 2) {
            V2 mid = v2((nodes[i].x+nodes[i+1].x)*0.5f, (nodes[i].y+nodes[i+1].y)*0.5f);
            Track t1 = {nodes[i], mid, width, str_dup("CLK")};
            Track t2 = {nodes[i+1], mid, width, str_dup("CLK")};
            pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track)*(size_t)(pcb->ntracks+2));
            pcb->tracks[pcb->ntracks++]=t1; pcb->tracks[pcb->ntracks++]=t2;
            added += 2;
            nodes[i/2] = mid;
        }
        if (remaining & 1) nodes[remaining/2] = nodes[remaining-1];
        remaining = (remaining + 1) / 2;
    }
    // connect final node to source
    Track tf = {nodes[0], src, width, str_dup("CLK")};
    pcb->tracks = (Track *)realloc(pcb->tracks, sizeof(Track)*(size_t)(pcb->ntracks+1));
    pcb->tracks[pcb->ntracks++] = tf; added++;
    free(nodes);
    return added;
}

float cts_measure_skew(Pcb *pcb) {
    float max_d = 0, min_d = 1e9f;
    for (int i=0;i<pcb->ntracks;i++) {
        float dx=pcb->tracks[i].b.x-pcb->tracks[i].a.x, dy=pcb->tracks[i].b.y-pcb->tracks[i].a.y;
        float d = sqrtf(dx*dx+dy*dy);
        if (d > max_d) max_d = d; if (d < min_d) min_d = d;
    }
    return max_d - min_d;
}

// ===== B*-TREE FLOORPLANNING =====
int floorplan_bstar(float *bw, float *bh, int n, V2 *pos, float board_w, float board_h) {
    (void)board_w; (void)board_h;
    // simple greedy: place blocks left-to-right, new row when full
    float cx = 0, cy = 0, row_h = 0;
    for (int i = 0; i < n; i++) {
        if (cx + bw[i] > board_w) { cx = 0; cy += row_h; row_h = 0; }
        pos[i] = v2(cx, cy);
        cx += bw[i];
        if (bh[i] > row_h) row_h = bh[i];
    }
    return n;
}

// ===== TIMING OPTIMIZATION =====
V2 timing_optimal_buffer_pos(V2 a, V2 b, float max_len) {
    float dx = b.x-a.x, dy = b.y-a.y;
    float len = sqrtf(dx*dx+dy*dy);
    if (len <= max_len) return v2(-1,-1);
    float t = max_len / len;
    return v2(a.x + dx*t, a.y + dy*t);
}

// ===== ENHANCED DRC =====
int drc_enhanced_check(Pcb *pcb, float min_s, float min_w, float min_a, DRCViolation *viols, int maxv) {
    if (!pcb || !viols) return 0; int n = 0;

    // width check
    for (int i=0;i<pcb->ntracks&&n<maxv;i++)if(pcb->tracks[i].width<min_w){viols[n].pos=pcb->tracks[i].a;viols[n].type=1;snprintf(viols[n].msg,128,"Width %.2f < %.2f",pcb->tracks[i].width,min_w);n++;}

    // spacing check
    for (int i=0;i<pcb->ntracks&&n<maxv;i++)for(int j=i+1;j<pcb->ntracks&&n<maxv;j++){
        float d1=sqrtf((pcb->tracks[i].a.x-pcb->tracks[j].a.x)*(pcb->tracks[i].a.x-pcb->tracks[j].a.x)+(pcb->tracks[i].a.y-pcb->tracks[j].a.y)*(pcb->tracks[i].a.y-pcb->tracks[j].a.y));
        if(d1<min_s){viols[n].pos=pcb->tracks[i].a;viols[n].type=2;snprintf(viols[n].msg,128,"Spacing %.3f < %.3f",d1,min_s);n++;}
    }
    return n;
}

// ===== GDS EXPORT STUB =====
char *gds_export_stub(Pcb *pcb, const char *out) {
    char buf[512];
    snprintf(buf, sizeof(buf), "GDSII Export: %s\nTracks: %d  Pads: %d\nCells: 1  Layers: 2\nStatus: stub (needs full GDSII binary writer)", out?out:"?", pcb?pcb->ntracks:0, pcb?pcb->nfps:0);
    return str_dup(buf);
}

// ===== POWER GRID =====
char *power_grid_analysis(Pcb *pcb, float Vdd, float Imax) {
    if (!pcb) return str_dup("no PCB");
    char buf[1024]; int off=0;
    float R=0.0172f*50.0f/(1.0f*35e-6f*1000.0f); // 50mm 1oz trace
    float Vdrop=Imax*R;
    float margin=Vdd*0.05f;
    off+=snprintf(buf+off,sizeof(buf)-off,"=== POWER GRID ===\nVdd=%.1fV Imax=%.0fA\nIR drop: %.3fV (%.1f%%)\nMargin: %.3fV (%s)\nEM: %s\n",
        Vdd,Imax,Vdrop,Vdrop/Vdd*100,margin,Vdrop<margin?"OK":"VIOLATION",Imax>5?"Check trace width":"OK");
    return str_dup(buf);
}
