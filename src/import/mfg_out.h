#pragma once
// mfg_out.h - Manufacturing outputs: pick & place, IPC-356, ODB++.
#include <stdbool.h>
#include "../model/project.h"

// Generate pick & place file (CSV format).
bool mfg_pickplace(Pcb *pcb, const char *path);

// Generate IPC-356 netlist.
bool mfg_ipc356(Pcb *pcb, const char *path);

// Generate ODB++ directory structure stub.
bool mfg_odbpp(Pcb *pcb, const char *dir);

// Generate all manufacturing outputs.
bool mfg_export_all(Pcb *pcb, const char *dir);
