#include "pdfout.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

bool pdf_write_simple(const char *path, const char *title, const char *text) {
    if (!path || !text) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    // minimal PDF
    const char *lines[64]; int n_lines = 0;
    const char *p = text;
    while (*p && n_lines < 63) { lines[n_lines++] = p; while (*p && *p != '\n') p++; if (*p) p++; }
    if (n_lines == 0) lines[n_lines++] = " ";

    fprintf(f, "%%PDF-1.4\n1 0 obj<</Type/Catalog/Pages 2 0 R>>endobj\n");
    fprintf(f, "2 0 obj<</Type/Pages/Kids[3 0 R]/Count 1>>endobj\n");
    fprintf(f, "3 0 obj<</Type/Page/Parent 2 0 R/MediaBox[0 0 612 792]/Contents 4 0 R>>endobj\n");
    fprintf(f, "4 0 obj<</Length 5 0 R>>stream\nBT\n/F1 12 Tf\n");
    fprintf(f, "1 0 0 1 50 720 Tm\n(%s) Tj\n", title ? title : "Document");
    fprintf(f, "/F1 10 Tf\n");
    for (int i = 0; i < n_lines; i++) {
        // escape PDF special chars
        char clean[256]; int ci = 0;
        for (const char *c = lines[i]; *c && ci < 254; c++) {
            if (*c == '(' || *c == ')' || *c == '\\') { clean[ci++] = '\\'; clean[ci++] = *c; }
            else clean[ci++] = *c;
        }
        clean[ci] = 0;
        fprintf(f, "1 0 0 1 50 %d Tm\n(%s) Tj\n", 700 - i * 14, clean);
    }
    fprintf(f, "ET\nendstream\nendobj\n");
    int slen = (int)ftell(f) - 200;
    fprintf(f, "5 0 obj %d endobj\n", slen > 0 ? slen : 100);
    fprintf(f, "xref\n0 6\n0000000000 65535 f \n0000000009 00000 n \n0000000058 00000 n \n0000000115 00000 n \n0000000210 00000 n \n%010d 00000 n \n", 400);
    fprintf(f, "trailer<</Size 6/Root 1 0 R>>\nstartxref\n%d\n%%%%EOF\n", 500);
    fclose(f); return true;
}

bool pdf_export_drawing(Pcb *pcb, const char *path) {
    if (!pcb || !path) return false;
    char buf[2048];
    snprintf(buf, sizeof(buf),
        "PCB Drawing: %s\n"
        "========================\n"
        "Dimensions: %.1f x %.1f mm\n"
        "Tracks: %d  Footprints: %d  Vias: %d\n"
        "Layers: 2 (top + bottom)\n"
        "Material: FR-4 1.6mm\n"
        "Solder mask: Green LPI\n"
        "Silkscreen: White\n",
        pcb->name ? pcb->name : "board",
        pcb->bmax.x - pcb->bmin.x, pcb->bmax.y - pcb->bmin.y,
        pcb->ntracks, pcb->nfps, pcb->nvias);
    return pdf_write_simple(path, pcb->name ? pcb->name : "Drawing", buf);
}

bool pdf_export_schematic(Schematic *s, const char *path) {
    if (!s || !path) return false;
    char buf[2048];
    snprintf(buf, sizeof(buf),
        "Schematic: %s\n"
        "========================\n"
        "Components: %d  Wires: %d  Labels: %d\n",
        s->name ? s->name : "?",
        s->ninsts, s->nwires, s->nlabels);
    return pdf_write_simple(path, s->name ? s->name : "Schematic", buf);
}

bool dxf_export_pcb(Pcb *pcb, const char *path) {
    if (!pcb || !path) return false;
    FILE *f = fopen(path, "w"); if (!f) return false;
    fprintf(f, "0\nSECTION\n2\nENTITIES\n");
    // board outline as LWPOLYLINE
    if (pcb->noutline >= 3) {
        fprintf(f, "0\nLWPOLYLINE\n8\n0\n90\n%d\n70\n1\n", pcb->noutline);
        for (int i = 0; i < pcb->noutline; i++)
            fprintf(f, "10\n%.3f\n20\n%.3f\n", pcb->outline[i].x, pcb->outline[i].y);
    }
    // tracks as LINEs
    for (int i = 0; i < pcb->ntracks; i++) {
        fprintf(f, "0\nLINE\n8\n1\n");
        fprintf(f, "10\n%.3f\n20\n%.3f\n11\n%.3f\n21\n%.3f\n",
            pcb->tracks[i].a.x, pcb->tracks[i].a.y,
            pcb->tracks[i].b.x, pcb->tracks[i].b.y);
    }
    // pads as CIRCLEs
    for (int i = 0; i < pcb->nfps; i++) {
        fprintf(f, "0\nCIRCLE\n8\n2\n");
        fprintf(f, "10\n%.3f\n20\n%.3f\n40\n1.5\n", pcb->fps[i].pos.x, pcb->fps[i].pos.y);
    }
    fprintf(f, "0\nENDSEC\n0\nEOF\n");
    fclose(f); return true;
}
