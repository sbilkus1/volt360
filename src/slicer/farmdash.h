#pragma once
// farmdash.h - Farm analytics: production efficiency, machine utilization.
#include <stdbool.h>
#include "../slicer/farm.h"

// Machine utilization report (percentage of time busy vs total).
char *farm_utilization_report(Farm *f);

// Production efficiency: units per hour, success rate.
char *farm_efficiency_report(Farm *f);

// Downtime analysis: idle time by printer.
char *farm_downtime_report(Farm *f);

// Eye diagram text report for signal integrity.
char *eye_diagram_report(float bitrate_gbps, float rise_time_ps, float jitter_ps, float noise_mV);
