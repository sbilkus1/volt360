#pragma once
#include "../model/project.h"

typedef enum {
    IPC_SOIC, IPC_QFP, IPC_QFN, IPC_BGA, IPC_SOT23, IPC_SOT223,
    IPC_0603, IPC_0805, IPC_1206, IPC_2512,
    IPC_SOD123, IPC_SOD323,
    IPC_TANT_A, IPC_TANT_B, IPC_TANT_C,
    IPC_OSCON, IPC_CONN_TH,
} IpcPackageType;

typedef struct {
    IpcPackageType type;
    int pin_count; float pitch; float body_x; float body_y;
    float pad_x; float pad_y; float heel; float side; float courtyard;
    float lead_span; float lead_width;
    int bga_rows; int bga_cols; float ball_dia;
    int chip_pads;
    float hole_dia; float outer_dia;
} IpcParams;

typedef struct {
    Footprint fp;
    int success;
    char error[128];
} IpcResult;

void ipc_params_default(IpcParams *p, IpcPackageType type);
void ipc_params_quick(IpcParams *p, IpcPackageType type, int pin_count, float pitch);
IpcResult ipc_generate_footprint(const IpcParams *params);
void ipc_result_free(IpcResult *r);
char *ipc_params_summary(const IpcParams *p);
