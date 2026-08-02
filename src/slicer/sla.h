#pragma once
// sla.h - SLA/SLS/Metal additive manufacturing parameters.
#include <stdbool.h>

// SLA resin curing parameters.
float sla_cure_time_s(float layer_thickness_um, float exposure_power_mW);

// SLS powder parameters.
float sls_energy_density(float laser_power_W, float scan_speed_mm_s, float hatch_spacing_mm);

// Metal PBF (powder bed fusion) volumetric energy density.
float metal_pbf_energy(float laser_power_W, float scan_speed_mm_s, float layer_thickness_um, float hatch_mm);

// Support volume estimate for SLA (percentage of model volume).
float sla_support_volume_pct(float overhang_angle_deg);

// Full additive report.
char *additive_report(const char *tech, float layer_um, float power);
