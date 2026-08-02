#pragma once
// freecadfeat.h - FEM, TechDraw, Robot, Python, Timing, OpenCASCADE-compat, Spreadsheet.
#include <stdbool.h>
#include "../model/project.h"

// === FEM WORKBENCH ===
// Run FEA on a mesh with loads and boundary conditions.
typedef struct { V3 pos; V3 force; } FEMLoad;
typedef struct { V3 pos; int fixed_x, fixed_y, fixed_z; } FEMBC;

char *fem_static_analysis(CadMesh *m, float E_GPa, float nu, FEMLoad *loads, int nloads, FEMBC *bcs, int nbcs);
char *fem_modal_analysis(CadMesh *m, float E_GPa, float nu, float density, int n_modes);

// === TECHDRAW ===
// Generate orthographic drawing views.
char *techdraw_front_view(CadMesh *m);
char *techdraw_top_view(CadMesh *m);
char *techdraw_right_view(CadMesh *m);
char *techdraw_all_views(CadMesh *m, const char *title);

// === ROBOT SIMULATION ===
// Forward kinematics for a simple 6-DOF serial robot.
typedef struct { float a, d, alpha, theta; } DHParam;
void robot_forward_kinematics(DHParam *dh, int n_joints, V3 *end_effector);
char *robot_kinematics_report(DHParam *dh, int n_joints, float joint_angles[6]);

// === PYTHON API ===
char *python_api_docs(void);

// === STATIC TIMING ANALYSIS ===
// Estimate signal propagation delay for PCB traces.
float timing_propagation_delay(float length_mm, float er);
float timing_setup_margin(float clock_period_ns, float data_delay_ns, float clock_skew_ns, float setup_time_ns);
float timing_hold_margin(float data_delay_ns, float clock_skew_ns, float hold_time_ns);
char *timing_report(Pcb *pcb, float clock_freq_mhz);

// === OPENCASCADE COMPATIBILITY ===
char *opencascade_compat_notes(void);

// === SPREADSHEET ===
char *spreadsheet_create_sample(void);
