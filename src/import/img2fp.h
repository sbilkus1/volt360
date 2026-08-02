#pragma once
// img2fp.h - Bitmap to KiCad footprint converter
#include <stdbool.h>

// Convert a PBM (P4 binary) or PGM (P5 grayscale) image to a KiCad footprint.
// Each dark pixel becomes a small rectangular pad.
// pixel_size_mm: size of each pixel pad in mm
// layer: "F.Cu" (front) or "B.Cu" (back) or "F.SilkS"
bool img_to_footprint(const char *img_path, const char *out_path, float pixel_size_mm, const char *layer);
