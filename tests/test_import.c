// test_import.c - end-to-end import pipeline test (run via: build.bat test)
#include "../src/import/importer.h"
#include "../src/design/design.h"
#include "../src/design/gen_design.h"
#include "../src/design/joints.h"
#include "../src/design/co_design.h"
#include "../src/slicer/slicer.h"
#include "../src/slicer/gcode.h"
#include "../src/slicer/farm.h"
#include "../third_party/miniz/miniz.h"
#include <windows.h>
#include <math.h>

static int g_fail = 0;
static void check(const char *label, bool ok, const char *detail) {
    printf("[%s] %s %s\n", ok ? "PASS" : "FAIL", label, detail ? detail : "");
    if (!ok) g_fail++;
}

static CadMesh g_farm_mesh;
static int farm_slice_cb(int job, SliceResult *out) {
    (void)job;
    SliceSettings cfg; slice_settings_default(&cfg);
    return slice_mesh(&g_farm_mesh, &cfg, out);
}

static void make_dirs_for(const char *path) {
    char *copy = str_dup(path);
    for (char *p = copy; *p; p++) {
        if (*p == '\\' || *p == '/') { *p = '\0'; if (*copy) CreateDirectoryA(copy, NULL); *p = '\\'; }
    }
    CreateDirectoryA(copy, NULL);
    free(copy);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    Project p;
    project_init(&p, "test", "build\\test_project");
    make_dirs_for("build\\test_project\\");
    free(p.llm_model); p.llm_model = str_dup("llama3");
    free(p.llm_url); p.llm_url = str_dup("http://127.0.0.1:11434");

    printf("== importing examples/ ==\n");
    ImportResult r = importer_import_dir(&p, "examples");
    printf("  symbols=%d footprints=%d components=%d cad=%d schematics=%d pcbs=%d docs=%d\n",
           p.symbols.len, p.footprints.len, p.components.len,
           p.cad_models.len, p.schematics.len, p.pcbs.len, p.docs.len);
    check("dir import ok", r.ok, r.message);

    // individual checks
    check("kicad symbol imported", project_find_symbol(&p, "R") != NULL, "Device:R");
    check("kicad footprint imported", project_find_footprint(&p, "R_0603_1608Metric") != NULL, "R_0603_1608Metric");
    check("schematic imported (>=1)", p.schematics.len >= 1, "sample.kicad_sch");
    check("pcb imported (>=1)", p.pcbs.len >= 1, "sample.kicad_pcb");
    check("cad models (>=2: stl+obj)", p.cad_models.len >= 2, "sample.stl + sample.obj");
    check("netlist schematic (>=3)", p.schematics.len >= 3, "sample.net + sample.cir");
    check("docs (>=2: md+csv)", p.docs.len >= 2, "sample.md + sample.csv");

    if (p.schematics.len > 0) {
        Schematic *s0 = &p.schematics.v[0];
        printf("  schematic[0]=%s insts=%d wires=%d labels=%d\n",
               s0->name, s0->ninsts, s0->nwires, s0->nlabels);
    }
    if (p.pcbs.len > 0) {
        Pcb *b0 = &p.pcbs.v[0];
        printf("  pcb[0]=%s outline=%d fps=%d tracks=%d vias=%d\n",
               b0->name, b0->noutline, b0->nfps, b0->ntracks, b0->nvias);
        check("pcb outline closed (>=4)", b0->noutline >= 4, "board rect");
        check("pcb has 1 fp + 2 tracks + 1 via", b0->nfps >= 1 && b0->ntracks >= 2 && b0->nvias >= 1, "R1");
    }

    printf("\n== building zip in memory ==\n");
    char *tmp = getenv("TEMP");
    if (!tmp) tmp = ".";
    char zippath[1024];
    snprintf(zippath, sizeof(zippath), "%s\\llama_test_%d.zip", tmp, (int)GetTickCount());
    mz_zip_archive z; memset(&z, 0, sizeof(z));
    bool zw = mz_zip_writer_init_file(&z, zippath, 0);
    check("zip writer init", zw, zippath);
    size_t csvlen, schlen;
    char *csv = file_read("examples\\sample.csv", &csvlen);
    char *sch = file_read("examples\\sample.kicad_sch", &schlen);
    if (csv) mz_zip_writer_add_mem(&z, "nested/data.csv", csv, csvlen, MZ_BEST_COMPRESSION);
    if (sch) mz_zip_writer_add_mem(&z, "nested/sch/sample.kicad_sch", sch, schlen, MZ_BEST_COMPRESSION);
    bool zc = mz_zip_writer_finalize_archive(&z);
    mz_zip_writer_end(&z);
    free(csv); free(sch);
    check("zip written", zw && zc, zippath);

    int docs_before = p.docs.len, sch_before = p.schematics.len;
    ImportResult rz = importer_import_zip(&p, zippath);
    check("zip import ok", rz.ok, rz.message);
    check("zip doc extracted (csv->doc)", p.docs.len >= docs_before + 1, "nested/data.csv");
    check("zip schematic extracted", p.schematics.len >= sch_before + 1, "nested/sch/sample.kicad_sch");
    DeleteFileA(zippath);

    printf("\n== building minimal docx in memory ==\n");
    char docxpath[1024];
    snprintf(docxpath, sizeof(docxpath), "%s\\llama_test_%d.docx", tmp, (int)GetTickCount());
    memset(&z, 0, sizeof(z));
    zw = mz_zip_writer_init_file(&z, docxpath, 0);
    const char *docxml =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<w:document xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">"
        "<w:body>"
        "<w:p><w:r><w:t>LM358 Op Amp</w:t></w:r></w:p>"
        "<w:p><w:r><w:t>Supply 3V-32V</w:t></w:r></w:p>"
        "</w:body></w:document>";
    if (zw) {
        mz_zip_writer_add_mem(&z, "word/document.xml", docxml, strlen(docxml), MZ_BEST_COMPRESSION);
        mz_zip_writer_finalize_archive(&z);
        mz_zip_writer_end(&z);
        int before = p.docs.len;
        ImportResult rd = importer_import_file(&p, docxpath);
        check("docx import ok", rd.ok, rd.message);
        check("docx text parsed", p.docs.len >= before + 1, "LM358 in text");
        DeleteFileA(docxpath);
    }

    printf("\n== design module ==\n");
    check("material lib has built-ins", material_lib_count() >= 10, "Al/Steel/Ti/Cu/ABS/PLA/Nylon/Acrylic/FR4/Alumina");
    CadMaterial *mat = material_find("aluminum 6061");
    check("material find (case-insensitive)", mat != NULL, "Aluminum 6061");
    if (mat) { check("material density ~2.70", fabs(mat->density - 2.70f) < 0.01f, "g/cm3"); material_free(mat); }

    DesignFeature feats[3] = { 0 };
    feats[0].type = FEAT_BOX;      feats[0].w = 20; feats[0].h = 10; feats[0].d = 5; feats[0].segments = 4;
    feats[1].type = FEAT_CYLINDER; feats[1].w = 5;  feats[1].h = 20; feats[1].segments = 16;
    feats[2].type = FEAT_SPHERE;   feats[2].w = 6;  feats[2].segments = 16;
    CadMesh m;
    check("feature box mesh", feature_make_mesh(&feats[0], 0, &m) && m.nverts >= 24 && m.ntris >= 12, "box 20x10x5");
    mesh_free(&m);
    check("feature cylinder mesh", feature_make_mesh(&feats[1], 0, &m) && m.nverts > 32, "cyl r5 h20");
    mesh_free(&m);
    check("feature sphere mesh", feature_make_mesh(&feats[2], 0, &m) && m.nverts > 100, "sphere r6");
    mesh_free(&m);

    int cad_before = p.cad_models.len;
    design_build_assembly(&p, feats, 3);
    check("design build assembly (3 cad)", p.cad_models.len == cad_before + 3, "3 pushed into project");

    float vol = 0, mass = 0; V3 cg = { 0 };
    feature_make_mesh(&feats[0], 0, &m); // 20x10x5 box = 1000 mm3
    mesh_mass_props(&m, 2.70f, &vol, &mass, &cg);
    check("box volume ~1000 mm3", fabs(vol - 1000.0f) < 5.0f, "20*10*5");
    check("box mass ~2.70 g", fabs(mass - 2.70f) < 0.02f, "2.70 g/cm3 * 1 cm3");
    check("centroid ~origin", fabs(cg.x) < 0.1f && fabs(cg.y) < 0.1f && fabs(cg.z) < 0.1f, "centered");
    mesh_free(&m);

    char *bom = project_bom_text(&p);
    check("bom text non-empty", bom && bom[0], bom ? bom : "");
    free(bom);

    printf("\n== generative design (SIMP topopt) ==\n");
    GenDesign gd; memset(&gd, 0, sizeof(gd));
    gd.nx = 32; gd.ny = 20; gd.volfrac = 0.4f; gd.fix_left = 1;
    gd.load_xf = 1.0f; gd.load_yf = 0.5f; gd.iters = 30;
    bool gok = gen_design_run(&gd);
    check("gen design run", gok, "32x20 grid, volfrac 0.4");
    int solid = 0;
    if (gok) { for (int i = 0; i < gd.nx * gd.ny; i++) if (gd.dens[i] > 0.5) solid++; }
    check("gen design has material", solid > 0 && solid < gd.nx * gd.ny, "0 < solid < total");
    CadMesh gm;
    if (gok) {
        bool gmesh = gen_design_mesh(&gd, 2.0f, 5.0f, &gm);
        check("gen design mesh", gmesh && gm.nverts > 0, "voxel boxes");
        if (gmesh) mesh_free(&gm);
    }
    gen_design_free(&gd);

    printf("\n== assembly joints ==\n");
    int ai = assembly_demo_robot(&p);
    check("demo assembly built", ai >= 0 && p.assemblies.len == 1, "robot-demo");
    if (ai >= 0) {
        Assembly *as = &p.assemblies.v[ai];
        check("joint added", as->njoints >= 1 && as->joints[0].type == JOINT_REVOLUTE, "revolute");
        float before[16], after[16];
        joint_world(as, as->nitems - 1, before);
        as->clock = 0; as->joints[0].angle = 0;
        assembly_tick(as, 2.0f);
        joint_world(as, as->nitems - 1, after);
        bool moved = false;
        for (int k = 0; k < 16; k++) if (fabs(before[k] - after[k]) > 1e-4f) moved = true;
        check("joint motion changes transform", moved, "clock advances angle");
        check("clock advanced", fabs(as->clock - 2.0f) < 1e-3f, "dt=2s");
    }

    printf("\n== STL export round-trip ==\n");
    char stlpath[1024];
    snprintf(stlpath, sizeof(stlpath), "%s\\llama_test_%d.stl", tmp, (int)GetTickCount());
    feature_make_mesh(&feats[0], 0, &m);
    check("stl write", mesh_write_stl(stlpath, &m), stlpath);
    CadMesh rl;
    bool stlok = mesh_load_stl(stlpath, &rl);
    check("stl read back", stlok && rl.nverts > 0, "binary stl");
    if (stlok) mesh_free(&rl);
    DeleteFileA(stlpath);
    mesh_free(&m);

    printf("\n== slicer ==\n");
    DesignFeature sbox[1] = { 0 };
    sbox[0].type = FEAT_BOX; sbox[0].w = 20; sbox[0].h = 20; sbox[0].d = 20; sbox[0].segments = 4;
    CadMesh cm;
    check("slice input mesh", feature_make_mesh(&sbox[0], 0, &cm) && cm.ntris > 0, "20mm cube");
    SliceSettings sc; slice_settings_default(&sc);
    SliceResult sr;
    bool sok = slice_mesh(&cm, &sc, &sr);
    check("slice 20mm cube", sok, "default settings");
    if (sok) {
        check("layer count ~100", sr.nlayers >= 95 && sr.nlayers <= 105, "20mm / 0.2mm + 1");
        int perims = 0, infills = 0, solid = 0;
        for (int i = 0; i < sr.nlayers; i++)
            for (int j = 0; j < sr.layers[i].npaths; j++) {
                if (sr.layers[i].paths[j].kind == SLICE_PATH_PERIMETER) perims++;
                else { infills++; if (sr.layers[i].paths[j].solid) solid++; }
            }
        check("perimeter paths present", perims > 0, "outer + inset loops");
        check("infill paths present", infills > 0, "grid infill");
        check("solid top/bottom present", solid > 0, "solid_bottom/top layers");
        check("statistics sane", sr.total_dist > 0 && sr.filament_mm > 0 && sr.est_time_s > 0, "dist/filament/time");
        bool last_solid = false;
        for (int i = sr.nlayers - 1; i >= 0 && !last_solid; i--)
            if (sr.layers[i].npaths > 0)
                for (int j = 0; j < sr.layers[i].npaths; j++)
                    if (sr.layers[i].paths[j].kind == SLICE_PATH_INFILL && sr.layers[i].paths[j].solid) last_solid = true;
        check("top layer solid", last_solid, "solid_top_layers=3");

        V2 sq[4] = { {0,0},{10,0},{10,10},{0,10} };
        V2 of[16]; int on = 0;
        bool ook = poly_offset(sq, 4, 1.0f, of, &on, 16);
        check("poly_offset inset square", ook && on == 4, "10mm square -1mm");
        if (ook && on == 4) {
            float w = of[1].x - of[0].x;
            check("inset width ~8mm", fabs(w - 8.0f) < 0.2f, "10mm - 2*1mm");
        }

        char gpath[1024];
        snprintf(gpath, sizeof(gpath), "%s\\llama_test_%d.gcode", tmp, (int)GetTickCount());
        sc.tools = 4; sc.random_color = 1; sc.color_change_layer = 10;
        check("gcode emit", gcode_emit(&sr, &sc, gpath), gpath);
        size_t glen = 0;
        char *gdata = file_read(gpath, &glen);
        check("gcode non-empty", gdata && glen > 100, "");
        if (gdata) {
            check("gcode has temps", str_contains(gdata, "M109") && str_contains(gdata, "M190"), "M109/M190");
            check("gcode has AMS tool T0-3", str_contains(gdata, "T0") || str_contains(gdata, "T1") || str_contains(gdata, "T2") || str_contains(gdata, "T3"), "random tool");
            check("gcode has M600", str_contains(gdata, "M600"), "color change");
            check("gcode has layer markers", str_contains(gdata, ";LAYER:"), "");
            check("gcode has G28 home", str_contains(gdata, "G28"), "");
            free(gdata);
        }
        DeleteFileA(gpath);

        int tl = gcode_pick_tool(&sc);
        check("random tool in range", tl >= 0 && tl < 4, "tools=4");
        SliceSettings nsc; slice_settings_default(&nsc);
        check("no tool when disabled", gcode_pick_tool(&nsc) == -1, "tools=0/random_color=0");
        slice_result_free(&sr);
    }
    mesh_free(&cm);

    printf("\n== print farm ==\n");
    {
        DesignFeature fcube[1] = { 0 };
        fcube[0].type = FEAT_BOX; fcube[0].w = 20; fcube[0].h = 20; fcube[0].d = 20; fcube[0].segments = 4;
        memset(&g_farm_mesh, 0, sizeof(g_farm_mesh));
        check("farm slice mesh", feature_make_mesh(&fcube[0], 0, &g_farm_mesh) && g_farm_mesh.ntris > 0, "20mm cube");

        Farm fa;
        farm_init(&fa, "build\\farm");
        make_dirs_for("build\\farm\\");
        int p0 = farm_add_printer(&fa, "printer-A", 4, "PLA");
        int p1 = farm_add_printer(&fa, "printer-B", 0, NULL);
        check("farm printers added", p0 == 0 && p1 == 1, "A(4 slots PLA) + B(single)");
        int j0 = farm_add_job(&fa, "gear", 2, 5, "PLA", 1);
        int j1 = farm_add_job(&fa, "bracket", 1, 10, "ABS", 0);
        int j2 = farm_add_job(&fa, "holder", 1, 5, "PLA", 2);
        check("farm jobs added", j0 == 0 && j1 == 1 && j2 == 2, "3 jobs");
        int ja[3] = { j0, j1, j2 };
        int ord = farm_add_order(&fa, "production run", ja, 3);
        check("farm order added", ord == 0 && fa.orders[0].total_units == 4, "3 jobs -> 4 units");
        int sp0 = farm_add_spool(&fa, "PLA", "red", 100.0f);
        int sp1 = farm_add_spool(&fa, "ABS", "black", 50.0f);
        check("farm spools added", sp0 == 0 && sp1 == 1, "PLA 100m + ABS 50m");
        int fa0 = farm_add_file(&fa, "gear.stl", "build\\farm\\gear.stl", (const char *[]){"bracket", "robot"}, 2);
        check("farm file added", fa0 == 0, "tagged file");

        int scanned = farm_scan_store(&fa);
        check("farm scan store ran", scanned >= 0, "base_dir scan");

        int a0 = farm_assign_job(&fa, j1, -1);
        check("ABS job routed to printer-B", a0 == 1, "filament compatibility");
        int a1 = farm_assign_job(&fa, j0, -1);
        check("PLA job routed to printer-A", a1 == 0, "filament compatibility");
        int a2 = farm_assign_job(&fa, j2, -1);
        check("3rd job stays queued (all busy)", a2 == -1, "smart scheduler");

        int mapped = farm_map_ams(&fa);
        check("AMS mapping resolves PLA job", mapped == 1, "printer-B has no AMS slots");
        check("job0 pinned to color slot 1", fa.jobs[j0].maps_to_slot == 1, "color_index=1");

        SliceResult sr0;
        SliceSettings dcfg; slice_settings_default(&dcfg);
        if (slice_mesh(&g_farm_mesh, &dcfg, &sr0)) {
            int an = farm_detect_anomalies(&sr0, &dcfg);
            check("anomaly flags clean on cube", an == 0, "healthy part");
            slice_result_free(&sr0);
        }
        int anb = farm_detect_anomalies(NULL, &dcfg);
        check("anomaly flags on empty result", (anb & FARM_ANOM_FIRST_LAYER) && (anb & FARM_ANOM_SPAGHETTI), "no layers");

        // synthetic mid-print collapse (real spaghetti)
        SliceResult fake; memset(&fake, 0, sizeof(fake));
        fake.nlayers = 13;
        fake.layers = (SliceLayer *)calloc((size_t)fake.nlayers, sizeof(SliceLayer));
        for (int i = 0; i < fake.nlayers; i++) {
            SliceLayer *L = &fake.layers[i];
            L->npaths = 1;
            L->paths = (SlicePath *)calloc(1, sizeof(SlicePath));
            L->paths[0].kind = SLICE_PATH_PERIMETER;
            L->paths[0].npts = 2;
            L->paths[0].pts = (V2 *)malloc(2 * sizeof(V2));
            L->paths[0].pts[0] = v2(0, 0);
            L->paths[0].pts[1] = i < 5 ? v2(100, 0) : v2(1, 0);
        }
        int anf = farm_detect_anomalies(&fake, &dcfg);
        check("spaghetti detected on collapse", (anf & FARM_ANOM_SPAGHETTI) != 0, "sudden 40% drop");
        for (int i = 0; i < fake.nlayers; i++) { free(fake.layers[i].paths[0].pts); free(fake.layers[i].paths); }
        free(fake.layers);

        char bpath[1024];
        snprintf(bpath, sizeof(bpath), "build\\farm\\batch_%d.gcode", (int)GetTickCount());
        char *bout = farm_generate_batch(&fa, a1, farm_slice_cb, bpath);
        check("farm batch generated", bout != NULL, bpath);
        if (bout) {
            size_t blen = 0;
            char *bdata = file_read(bout, &blen);
            check("farm batch gcode non-empty", bdata && blen > 200, "");
            if (bdata) {
                check("farm batch job markers", str_contains(bdata, ";FARM_JOB:") && str_contains(bdata, ";FARM_JOB_END:"), "start/end");
                check("farm batch eject + release temp", str_contains(bdata, "M280") && str_contains(bdata, "M190 S40"), "auto-eject");
                check("farm batch mapped tool", str_contains(bdata, "T1"), "AMS slot 1");
                check("farm batch completion", str_contains(bdata, ";FARM_OK"), "");
                free(bdata);
            }
            free(bout);
        }
        check("job0 finished by batch", strcmp(fa.jobs[j0].status, "done") == 0, "bookkeeping");
        check("spool PLA deducted", fabs(fa.spools[sp0].remaining_m - (100.0f - (fa.jobs[j0].filament_mm / 1000.0f) * 2.0f)) < 0.01f, "2 units deducted");
        check("order progress tracked", fa.orders[0].done_units == 2, "2 units done");
        check("printer A hours advanced", fa.printers[p0].hours_running > 0, "est time accumulated");

        char *stats = farm_stats_text(&fa);
        check("farm stats report", stats && str_contains(stats, "jobs:") && str_contains(stats, "orders:"), "report text");
        free(stats);

        check("farm save", farm_save(&fa), "build\\farm\\farm.json");
        Farm fb; memset(&fb, 0, sizeof(fb));
        bool fld = farm_load(&fb, "build\\farm");
        check("farm load", fld, "");
        if (fld) {
            check("reload printer/job/spool counts", fb.n_printers == fa.n_printers && fb.n_jobs == fa.n_jobs && fb.n_spools == fa.n_spools, "counts");
            check("reload order restored", fb.n_orders == 1 && fb.orders[0].total_units == 4, "order");
            check("reload job status", strcmp(fb.jobs[j0].status, "done") == 0, "job0 done");
            farm_free(&fb);
        }
        farm_free(&fa);
        mesh_free(&g_farm_mesh);
    }

    printf("\n== co-design: PCB <-> CAD, CAD -> print ==\n");
    if (p.pcbs.len > 0) {
        Pcb *b0 = &p.pcbs.v[0];
        EnclosureParams e; co_enclosure_defaults(&e);
        CadMesh em;
        bool emok = pcb_enclosure_mesh(b0, &e, &em);
        check("pcb enclosure mesh", emok && em.nverts > 0 && em.ntris > 0, "floor+walls+standoffs+lid");
        if (emok) {
            check("enclosure dimensions sane",
                  (em.bmax.x - em.bmin.x) > 20.0f && (em.bmax.y - em.bmin.y) > 20.0f && (em.bmax.z - em.bmin.z) > 10.0f,
                  "28x28x~16mm shell");
            mesh_free(&em);
        }
        int ai = co_design_enclosure(&p, b0->id, &e);
        check("enclosure linked to pcb", ai >= 0, "assembly with PART_PCB + PART_CAD");
        if (ai >= 0) {
            Assembly *as = &p.assemblies.v[ai];
            check("assembly places pcb inside enclosure",
                  as->nitems == 2 && as->items[0].kind == PART_PCB && as->items[1].kind == PART_CAD,
                  "co-design link");
            check("pcb raised onto standoffs", fabs(as->items[0].pos.z - (e.floor_thickness + e.standoff_h + e.pcb_thickness * 0.5f)) < 1e-3f, "z = floor+standoff+pcb/2");
        }
        CadModel *enc = co_find_enclosure(&p, b0->id);
        check("enclosure cad model findable", enc != NULL && enc->mesh.valid, b0->name);
        if (enc) {
            SliceSettings cs; slice_settings_default(&cs);
            char epath[1024];
            snprintf(epath, sizeof(epath), "%s\\llama_test_%d.gcode", tmp, (int)GetTickCount());
            bool poke = co_design_print_cad(&p, enc->id, &cs, epath);
            check("print designed enclosure (CAD->slicer)", poke, epath);
            if (poke) {
                size_t elen = 0;
                char *edata = file_read(epath, &elen);
                check("enclosure gcode written", edata && elen > 100, "");
                if (edata) {
                    check("enclosure gcode temps", str_contains(edata, "M109"), "");
                    free(edata);
                }
                DeleteFileA(epath);
            }
            bool poke2 = co_design_print_enclosure(&p, b0->id, &cs, epath);
            check("print enclosure via pcb id", poke2, "");
            if (poke2) DeleteFileA(epath);

            DesignFeature fcube2[1] = { 0 };
            fcube2[0].type = FEAT_BOX; fcube2[0].w = 10; fcube2[0].h = 10; fcube2[0].d = 10; fcube2[0].segments = 4;
            CadMesh pmesh;
            if (feature_make_mesh(&fcube2[0], 0, &pmesh)) {
                check("gcode_slice_and_emit cube", gcode_slice_and_emit(&pmesh, &cs, epath), "");
                if (file_exists(epath)) DeleteFileA(epath);
                mesh_free(&pmesh);
            }
        }
        check("co-design note recorded", p.notes.len >= 1, "project note");
    }

    printf("\n== save + reload ==\n");
    bool saved = project_save(&p);
    check("project saved to build\\test_project\\project.json", saved, "");
    Project p2; memset(&p2, 0, sizeof(p2));
    bool loaded = project_load(&p2, "build\\test_project");
    check("project reloaded", loaded, "");
    if (loaded) {
        check("reload counts match (symbols)", p2.symbols.len == p.symbols.len, "");
        check("reload counts match (schematics)", p2.schematics.len == p.schematics.len, "");
        check("reload counts match (pcbs)", p2.pcbs.len == p.pcbs.len, "");
        check("reload counts match (cad)", p2.cad_models.len == p.cad_models.len, "");
        check("reload counts match (docs)", p2.docs.len == p.docs.len, "");
        project_free(&p2);
    }

    project_free(&p);
    printf("\n== RESULT: %s (failures=%d) ==\n", g_fail ? "FAIL" : "PASS", g_fail);
    return g_fail ? 1 : 0;
}
