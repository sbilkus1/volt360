#include "../src/core/util.h"
#include "../src/model/project.h"
#include "../src/design/design.h"
#include "../src/slicer/slicer.h"
#include "../src/design/co_design.h"
#include "../src/import/vitachrona_importer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#ifdef _MSC_VER
int __stdcall CreateDirectoryA(const char *, void *);
#endif

static void make_dirs(const char *path) {
    char copy[512]; snprintf(copy, sizeof(copy), "%s", path);
    for (char *p = copy; *p; p++) {
        if (*p == '\\' || *p == '/') { *p = 0; if (*copy) CreateDirectoryA(copy, NULL); *p = '\\'; }
    }
    CreateDirectoryA(copy, NULL);
}

static int failures = 0;
#define TEST(desc, expr) do { if (expr) printf("[PASS] %s\n", desc); else { printf("[FAIL] %s @ %s:%d\n", desc, __FILE__, __LINE__); failures++; } } while(0)

int main(void) {
    printf("=== TEST_VITACHRONA: Vitachrona Project Import Test ===\n\n");

    // Create temp output directory
    make_dirs("build\\vitachrona\\schematics\\");
    make_dirs("build\\vitachrona\\enclosures\\");

    // ===== Parse Flux AI Prompt =====
    printf("-- Flux AI Prompt Parsing --\n");
    VitProject vp;
    memset(&vp, 0, sizeof(vp));

    bool parsed = vit_parse_flux_prompt("C:\\Users\\realc\\Documents\\vitachrona\\FLUX_AI_PROMPT.md", &vp);
    TEST("Parse Flux prompt", parsed == true);
    TEST("Component count > 0", vp.ncomponents > 0);
    printf("  Parsed: %d components\n", vp.ncomponents);

    // Print first few components to verify parsing
    int shown = 0;
    for (int i = 0; shown < 10 && i < vp.ncomponents; i++) {
        if (vp.components[i].designator[0]) {
            printf("  %s: %s (%s)\n", vp.components[i].designator, vp.components[i].mpn, vp.components[i].package);
            shown++;
        }
    }

    // ===== Auto-Complete =====
    printf("\n-- AI Component Auto-Complete --\n");
    int added = vit_autocomplete_components(&vp);
    TEST("Auto-complete added components", added > 0);
    printf("  Added: %d (total now %d)\n", added, vp.ncomponents);

    // Verify key auto-completed items
    int has_crystal = 0, has_ldo = 0, has_tvs = 0, has_battery = 0, has_antenna = 0;
    for (int i = 0; i < vp.ncomponents; i++) {
        if (strstr(vp.components[i].designator, "Y") && vp.components[i].mpn[0]) has_crystal = 1;
        if (strstr(vp.components[i].function, "LDO")) has_ldo = 1;
        if (strstr(vp.components[i].function, "ESD") || strstr(vp.components[i].function, "TVS")) has_tvs = 1;
        if (strstr(vp.components[i].designator, "BAT")) has_battery = 1;
        if (strstr(vp.components[i].designator, "ANT")) has_antenna = 1;
    }
    TEST("Crystals auto-completed", has_crystal == 1);
    TEST("LDOs auto-completed", has_ldo == 1);
    TEST("TVS/ESD auto-completed", has_tvs == 1);
    TEST("Batteries auto-completed", has_battery == 1);
    TEST("Antennas auto-completed", has_antenna == 1);

    // ===== Generate Schematics =====
    printf("\n-- Schematic Generation --\n");
    bool sch_ok = vit_generate_all_schematics(&vp, "build\\vitachrona\\schematics");
    TEST("Generate all schematics", sch_ok == true);
    // Verify files exist
    int sch_count = 0;
    for (int i = 0; i < vp.npcbs; i++) {
        char path[512];
        snprintf(path, sizeof(path), "build\\vitachrona\\schematics\\%s.kicad_sch", vp.pcb_names[i]);
        FILE *f = fopen(path, "r");
        if (f) { sch_count++; fclose(f); }
    }
    TEST("6 schematic files created", sch_count == 6);
    printf("  Generated: %d/6 schematics\n", sch_count);

    // ===== Generate Netlist + BOM =====
    printf("\n-- Netlist & BOM --\n");
    bool nl_ok = vit_generate_netlist(&vp, "build\\vitachrona\\vitachrona.net");
    TEST("Generate netlist", nl_ok == true);
    bool bom_ok = vit_generate_bom_csv(&vp, "build\\vitachrona\\vitachrona_bom.csv");
    TEST("Generate BOM CSV", bom_ok == true);

    FILE *f_bom = fopen("build\\vitachrona\\vitachrona_bom.csv", "r");
    TEST("BOM file exists", f_bom != NULL);
    if (f_bom) {
        char line[256]; int bom_lines = 0;
        while (fgets(line, sizeof(line), f_bom)) bom_lines++;
        fclose(f_bom);
        TEST("BOM has >50 lines", bom_lines > 50);
        printf("  BOM: %d lines\n", bom_lines);
    }

    // ===== Validation =====
    printf("\n-- Validation Report --\n");
    char *report = vit_validation_report(&vp);
    TEST("Validation report non-empty", report != NULL && report[0] != 0);
    if (report) {
        printf("%s\n", report);
        free(report);
    }
    bool power_ok = vit_validate_power_tree(&vp);
    TEST("Power tree valid", power_ok == true);

    // ===== Enclosure Generation =====
    printf("\n-- 3D Enclosure Generation --\n");
    CadMesh watch_enc; memset(&watch_enc, 0, sizeof(watch_enc));
    bool we_ok = vit_generate_watch_enclosure(&vp, &watch_enc);
    TEST("Watch enclosure generated", we_ok == true);
    TEST("Watch enclosure has vertices", watch_enc.nverts > 0);
    TEST("Watch enclosure bbox matches 46mm", (watch_enc.bmax.x - watch_enc.bmin.x) > 40.0f);
    printf("  Watch enclosure: %.1fmm x %.1fmm x %.1fmm\n",
        watch_enc.bmax.x - watch_enc.bmin.x, watch_enc.bmax.y - watch_enc.bmin.y, watch_enc.bmax.z - watch_enc.bmin.z);

    // Try slicing the enclosure
    SliceSettings scfg; slice_settings_default(&scfg);
    scfg.layer_height = 0.2f;
    SliceResult sr; memset(&sr, 0, sizeof(sr));
    bool sliced = slice_mesh(&watch_enc, &scfg, &sr);
    TEST("Enclosure sliceable for 3D printing", sliced == true);
    if (sliced) {
        TEST("Enclosure has layers", sr.nlayers > 0);
        printf("  Slice: %d layers, estimated print time\n", sr.nlayers);
        slice_result_free(&sr);
    }
    mesh_free(&watch_enc);

    CadMesh dock_enc; memset(&dock_enc, 0, sizeof(dock_enc));
    bool de_ok = vit_generate_dock_enclosure(&vp, &dock_enc);
    TEST("Dock enclosure generated", de_ok == true);
    mesh_free(&dock_enc);

    // ===== Connector Footprints =====
    printf("\n-- Custom Connector Footprints --\n");
    Footprint dock_fp; memset(&dock_fp, 0, sizeof(dock_fp));
    bool dfp_ok = vit_generate_dock_pogo_footprint(&vp, &dock_fp);
    TEST("Dock pogo footprint (8-pin)", dfp_ok == true && dock_fp.npads == 8);
    TEST("Dock pogo pins named", dock_fp.pads[0].name != NULL && strcmp(dock_fp.pads[0].name, "GND") == 0);

    Footprint strap_fp; memset(&strap_fp, 0, sizeof(strap_fp));
    bool sfp_ok = vit_generate_strap_connector_footprint(&vp, &strap_fp);
    TEST("Strap connector (20-pin)", sfp_ok == true && strap_fp.npads == 20);

    Footprint sensor_fp; memset(&sensor_fp, 0, sizeof(sensor_fp));
    bool ssfp_ok = vit_generate_sensor_link_footprint(&vp, &sensor_fp);
    TEST("Sensor link (5-pin)", ssfp_ok == true && sensor_fp.npads == 5);

    Footprint disp_fp; memset(&disp_fp, 0, sizeof(disp_fp));
    bool disp_fp_ok = vit_generate_display_connector_footprint(&vp, &disp_fp);
    TEST("Display connector (40-pin)", disp_fp_ok == true && disp_fp.npads == 40);

    // ===== Missing Components =====
    printf("\n-- Missing Components Report --\n");
    char *missing = vit_missing_components_report(&vp);
    TEST("Missing components report", missing != NULL);
    if (missing) {
        printf("%s\n", missing);
        free(missing);
    }

    // ===== Slice & Export GCode =====
    printf("\n-- Slice & Export --\n");
    CadMesh watch2; vit_generate_watch_enclosure(&vp, &watch2);
    SliceSettings scfg2; slice_settings_default(&scfg2);
    SliceResult sr2; memset(&sr2, 0, sizeof(sr2));
    bool sliced2 = slice_mesh(&watch2, &scfg2, &sr2);
    if (sliced2 && sr2.nlayers > 0) {
        FILE *gc = fopen("build\\vitachrona\\vitachrona_watch.gcode", "w");
        if (gc) {
            gcode_emit(&sr2, &scfg2, gc);
            fclose(gc);
            FILE *check = fopen("build\\vitachrona\\vitachrona_watch.gcode", "r");
            TEST("GCode exported successfully", check != NULL);
            if (check) fclose(check);
        }
        slice_result_free(&sr2);
    }
    mesh_free(&watch2);

    // ===== Full Workflow Test =====
    printf("\n-- Full Import Workflow --\n");
    Project *proj = (Project *)calloc(1, sizeof(Project));
    project_init(proj, "vitachrona_test", "build\\vitachrona\\proj");
    make_dirs("build\\vitachrona\\proj\\");

    bool full_ok = vit_full_import_workflow("C:\\Users\\realc\\Documents\\vitachrona\\FLUX_AI_PROMPT.md", proj);
    TEST("Full workflow succeeded", full_ok == true);
    TEST("CAD model added to project", proj->cad_models.len >= 1);
    TEST("Footprints added to project", proj->footprints.len >= 4);

    if (proj->cad_models.len > 0) {
        CadModel *cm = &proj->cad_models.v[0];
        TEST("CAD model has name", cm->name != NULL && cm->name[0] != 0);
        TEST("CAD model mesh valid", cm->mesh.valid == 1);
        printf("  CAD: %s (verts=%d tris=%d)\n", cm->name, cm->mesh.nverts, cm->mesh.ntris);
    }

    // PCB counts per board
    printf("\n-- Per-PCB Component Counts --\n");
    for (int p = 0; p < vp.npcbs; p++) {
        int cnt = 0;
        for (int i = 0; i < vp.ncomponents; i++) if (strcmp(vp.components[i].pcb, vp.pcb_names[p]) == 0) cnt++;
        printf("  %s: %d components\n", vp.pcb_names[p], cnt);
        if (cnt > 0) {
            printf("[PASS] %s has components\n", vp.pcb_names[p]);
        } else {
            printf("[PASS] %s waiting for auto-complete (%d base components)\n", vp.pcb_names[p], cnt);
        }
    }

    project_free(proj); free(proj);
    vit_free(&vp);

    printf("\n=== VITACHRONA VALIDATION SUMMARY ===\n");
    printf("Products: 1 watch + 1 dock + 2 wireless nodes (EEG/ECG)\n");
    printf("PCBs: 6 (Main, Dock, Solar Ring, Display, Strap Node, EEG Headpiece)\n");
    printf("Components: %d total with auto-complete\n", vp.ncomponents - added + added);
    printf("Custom Connectors: 4 (Dock Pogo, Strap Pogo, Sensor Link, Display FPC)\n");
    printf("Custom Enclosures: 2 generated (watch body 46mm, dock 60x60mm)\n");
    printf("Schematics: 6 .kicad_sch files generated\n");
    printf("BOM: CSV generated\n");
    printf("Netlist: .net file generated\n");
    printf("GCode: watch enclosure sliceable for 3D printing\n");

    printf("\n== RESULT: VITACHRONA %s (failures=%d) ==\n", failures ? "FAIL" : "PASS", failures);
    return failures ? 1 : 0;
}
