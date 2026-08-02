#pragma once
// energy.h - Farm energy tracking, cost estimation, failure analytics.
#include <stdbool.h>
#include "../slicer/farm.h"

// Estimate energy used (kWh) for a printer based on hours run.
// avg_power_w: typical power draw (e.g. 150W for FDM, 300W for enclosure)
float energy_estimate_kwh(float hours, float avg_power_w);

// Estimate cost for the farm over a period.
float energy_cost_total(Farm *f, float cost_per_kwh, float avg_power_w);

// Failure analytics: count failures by type, return report text.
char *farm_failure_report(Farm *f);

// Estimate purchase needs based on filament usage rate.
char *farm_purchase_forecast(Farm *f, float hours_per_week);
