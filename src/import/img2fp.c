#include "img2fp.h"
#include "../core/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool img_to_footprint(const char *img_path, const char *out_path, float pixel_size_mm, const char *layer) {
    if (!img_path || !out_path || pixel_size_mm <= 0) return false;
    FILE *fin = fopen(img_path, "rb");
    if (!fin) return false;

    char magic[3] = {0};
    if (!fgets(magic, sizeof(magic), fin)) { fclose(fin); return false; }
    if (magic[0] != 'P') { fclose(fin); return false; }

    // skip comments
    int c;
    while ((c = fgetc(fin)) == '#') { while ((c = fgetc(fin)) != '\n' && c != EOF); }

    int w = 0, h = 0, maxval = 255;
    ungetc(c, fin);
    fscanf(fin, "%d %d", &w, &h);
    if (magic[1] == '5') fscanf(fin, "%d", &maxval);
    fgetc(fin); // consume newline

    if (w <= 0 || h <= 0 || w > 4096 || h > 4096) { fclose(fin); return false; }

    FILE *fout = fopen(out_path, "w");
    if (!fout) { fclose(fin); return false; }

    fprintf(fout, "(module Image (layer %s) (tedit 0)\n", layer ? layer : "F.Cu");
    fprintf(fout, "  (fp_text reference REF** (at 0 0) (layer F.SilkS) (effects (font (size 1 1) (thickness 0.15)))))\n");

    int pad_num = 0;
    if (magic[1] == '4') {
        // PBM binary: 1 bit per pixel, packed
        int row_bytes = (w + 7) / 8;
        unsigned char *row = (unsigned char *)malloc((size_t)row_bytes);
        for (int y = 0; y < h; y++) {
            if (fread(row, 1, (size_t)row_bytes, fin) != (size_t)row_bytes) break;
            for (int x = 0; x < w; x++) {
                if (row[x / 8] & (0x80 >> (x % 8))) continue; // white pixel = no pad
                float px = (x - w / 2) * pixel_size_mm;
                float py = ((h - 1 - y) - h / 2) * pixel_size_mm;
                fprintf(fout, "  (pad %d smd rect (at %.3f %.3f) (size %.3f %.3f) (layers %s F.Paste F.Mask))\n",
                    ++pad_num, px, py, pixel_size_mm, pixel_size_mm, layer ? layer : "F.Cu");
            }
        }
        free(row);
    } else if (magic[1] == '5') {
        // PGM grayscale: 1 byte per pixel
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                unsigned char val = (unsigned char)fgetc(fin);
                if (val > maxval / 2) continue; // light = no pad
                float px = (x - w / 2) * pixel_size_mm;
                float py = ((h - 1 - y) - h / 2) * pixel_size_mm;
                fprintf(fout, "  (pad %d smd rect (at %.3f %.3f) (size %.3f %.3f) (layers %s F.Paste F.Mask))\n",
                    ++pad_num, px, py, pixel_size_mm, pixel_size_mm, layer ? layer : "F.Cu");
            }
        }
    }

    fprintf(fout, ")\n");
    fclose(fout);
    fclose(fin);
    return true;
}
