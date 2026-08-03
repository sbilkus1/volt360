#include "v360_bundle.h"
#include "json.h"
#include "../core/util.h"
#include <miniz.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void serialise_project_meta(const Project *p, JsonW *jw)
{
    jw_obj(jw);
    jw_key(jw, "id");   jw_str(jw, "project");
    jw_key(jw, "name"); jw_str(jw, p->name ? p->name : "");
    jw_key(jw, "version"); jw_int(jw, 1);
}

static void serialise_schematic(const Schematic *s, JsonW *jw)
{
    jw_obj(jw);
    jw_key(jw, "id");     jw_str(jw, s->id ? s->id : "");
    jw_key(jw, "name");   jw_str(jw, s->name ? s->name : "");
    jw_key(jw, "origin"); jw_obj(jw);
    jw_key(jw, "x"); jw_num(jw, s->origin.x);
    jw_key(jw, "y"); jw_num(jw, s->origin.y);
    jw_obj(jw);
    jw_key(jw, "inst_count"); jw_int(jw, s->ninsts);
    jw_key(jw, "wire_count"); jw_int(jw, s->nwires);
    jw_key(jw, "label_count"); jw_int(jw, s->nlabels);
    jw_key(jw, "text_count"); jw_int(jw, s->ntexts);
}

static void serialise_pcb(const Pcb *p, JsonW *jw)
{
    jw_obj(jw);
    jw_key(jw, "id");        jw_str(jw, p->id ? p->id : "");
    jw_key(jw, "name");      jw_str(jw, p->name ? p->name : "");
    jw_key(jw, "thickness"); jw_num(jw, p->thickness);
    jw_key(jw, "outline");   jw_arr(jw);
    int i;
    for (i = 0; i < p->noutline; i++) {
        jw_arr_item_obj(jw);
        jw_key(jw, "x"); jw_num(jw, p->outline[i].x);
        jw_key(jw, "y"); jw_num(jw, p->outline[i].y);
        jw_obj(jw);
    }
    jw_arr(jw);
    jw_key(jw, "fp_count"); jw_int(jw, p->nfps);
    jw_key(jw, "track_count"); jw_int(jw, p->ntracks);
    jw_key(jw, "via_count"); jw_int(jw, p->nvias);
}

static void serialise_bom(const Project *p, JsonW *jw)
{
    int i;
    jw_arr(jw);
    for (i = 0; i < arr_count(p->components); i++) {
        const Component *c = &arr_get(p->components, i);
        jw_arr_item_obj(jw);
        jw_key(jw, "id");           jw_str(jw, c->id ? c->id : "");
        jw_key(jw, "name");         jw_str(jw, c->name ? c->name : "");
        jw_key(jw, "manufacturer"); jw_str(jw, c->manufacturer ? c->manufacturer : "");
        jw_key(jw, "package");      jw_str(jw, c->package ? c->package : "");
        jw_key(jw, "footprint");    jw_str(jw, c->footprint ? c->footprint : "");
        jw_obj(jw);
    }
    jw_arr(jw);
}

static void serialise_mesh_json(const CadMesh *m, JsonW *jw)
{
    int i;
    jw_obj(jw);
    jw_key(jw, "nverts"); jw_int(jw, m->nverts);
    jw_key(jw, "ntris");  jw_int(jw, m->ntris);

    jw_key(jw, "verts");
    jw_arr(jw);
    for (i = 0; i < m->nverts * 3; i++) {
        jw_arr_item_num(jw, (double)m->pos[i]);
    }
    jw_arr(jw);

    jw_key(jw, "normals");
    jw_arr(jw);
    if (m->nrm) {
        for (i = 0; i < m->nverts * 3; i++) {
            jw_arr_item_num(jw, (double)m->nrm[i]);
        }
    }
    jw_arr(jw);

    jw_key(jw, "tris");
    jw_arr(jw);
    for (i = 0; i < m->ntris * 3; i++) {
        jw_arr_item_num(jw, (double)m->idx[i]);
    }
    jw_arr(jw);

    jw_key(jw, "bmin"); jw_obj(jw);
    jw_key(jw, "x"); jw_num(jw, m->bmin.x);
    jw_key(jw, "y"); jw_num(jw, m->bmin.y);
    jw_key(jw, "z"); jw_num(jw, m->bmin.z);
    jw_obj(jw);
    jw_key(jw, "bmax"); jw_obj(jw);
    jw_key(jw, "x"); jw_num(jw, m->bmax.x);
    jw_key(jw, "y"); jw_num(jw, m->bmax.y);
    jw_key(jw, "z"); jw_num(jw, m->bmax.z);
    jw_obj(jw);
    jw_key(jw, "valid"); jw_bool(jw, m->valid != 0);
}

bool v360_save(const Project *p, const char *path)
{
    mz_zip_archive zip;
    JsonW jw;
    const char *cstr;
    int i;

    if (!p || !path) return false;

    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_writer_init_file(&zip, path, 0)) return false;

    /* project.json */
    jw_init(&jw); jw_begin(&jw);
    serialise_project_meta(p, &jw);
    jw_end(&jw);
    cstr = jw_cstr(&jw);
    mz_zip_writer_add_mem(&zip, "project.json", cstr, strlen(cstr), MZ_DEFAULT_LEVEL);
    jw_free(&jw);

    /* schematic.json */
    if (arr_count(p->schematics) > 0) {
        const Schematic *s = &arr_get(p->schematics, 0);
        jw_init(&jw); jw_begin(&jw);
        serialise_schematic(s, &jw);
        jw_end(&jw);
        cstr = jw_cstr(&jw);
        mz_zip_writer_add_mem(&zip, "schematic.json", cstr, strlen(cstr), MZ_DEFAULT_LEVEL);
        jw_free(&jw);
    }

    /* pcb json files */
    for (i = 0; i < arr_count(p->pcbs); i++) {
        const Pcb *pcb = &arr_get(p->pcbs, i);
        char pcb_name[512];
        if (pcb->name && pcb->name[0]) {
            snprintf(pcb_name, sizeof(pcb_name), "pcb_%s.json", pcb->name);
        } else {
            snprintf(pcb_name, sizeof(pcb_name), "pcb_%d.json", i);
        }
        jw_init(&jw); jw_begin(&jw);
        serialise_pcb(pcb, &jw);
        jw_end(&jw);
        cstr = jw_cstr(&jw);
        mz_zip_writer_add_mem(&zip, pcb_name, cstr, strlen(cstr), MZ_DEFAULT_LEVEL);
        jw_free(&jw);
    }

    /* bom.json */
    if (arr_count(p->components) > 0) {
        jw_init(&jw); jw_begin(&jw);
        serialise_bom(p, &jw);
        jw_end(&jw);
        cstr = jw_cstr(&jw);
        mz_zip_writer_add_mem(&zip, "bom.json", cstr, strlen(cstr), MZ_DEFAULT_LEVEL);
        jw_free(&jw);
    }

    /* meshes/ */
    for (i = 0; i < arr_count(p->cad_models); i++) {
        const CadModel *cm = &arr_get(p->cad_models, i);
        if (cm->mesh.valid && cm->mesh.nverts > 0) {
            char mesh_path[512];
            const char *mesh_name = cm->name ? cm->name : cm->id;
            if (!mesh_name) mesh_name = "unknown";
            snprintf(mesh_path, sizeof(mesh_path), "meshes/%s.json", mesh_name);
            jw_init(&jw); jw_begin(&jw);
            serialise_mesh_json(&cm->mesh, &jw);
            jw_end(&jw);
            cstr = jw_cstr(&jw);
            mz_zip_writer_add_mem(&zip, mesh_path, cstr, strlen(cstr), MZ_DEFAULT_LEVEL);
            jw_free(&jw);
        }
    }

    /* thumb.png – placeholder 1x1 PNG */
    {
        unsigned char png_1x1[] = {
            0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A,0x00,0x00,0x00,0x0D,
            0x49,0x48,0x44,0x52,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,
            0x08,0x02,0x00,0x00,0x00,0x90,0x77,0x53,0xDE,0x00,0x00,0x00,
            0x0C,0x49,0x44,0x41,0x54,0x08,0xD7,0x63,0x60,0x60,0x60,0x00,
            0x00,0x00,0x04,0x00,0x01,0x27,0x34,0x27,0x0A,0x00,0x00,0x00,
            0x00,0x49,0x45,0x4E,0x44,0xAE,0x42,0x60,0x82
        };
        mz_zip_writer_add_mem(&zip, "thumb.png", png_1x1, sizeof(png_1x1), MZ_DEFAULT_LEVEL);
    }

    mz_zip_writer_finalize_archive(&zip);
    mz_zip_writer_end(&zip);
    return true;
}

static CadMesh deserialise_mesh_from_json(JsonValue *jv)
{
    CadMesh m;
    memset(&m, 0, sizeof(m));

    double nverts_d = json_get_num(jv, "nverts");
    double ntris_d  = json_get_num(jv, "ntris");
    m.nverts = (int)nverts_d;
    m.ntris  = (int)ntris_d;

    if (m.nverts <= 0 || m.ntris <= 0) return m;

    m.pos = (float *)calloc((size_t)m.nverts * 3, sizeof(float));
    m.nrm = (float *)calloc((size_t)m.nverts * 3, sizeof(float));
    m.idx = (int *)calloc((size_t)m.ntris * 3, sizeof(int));

    /* verts */
    JsonValue *varr = json_get_arr(jv, "verts");
    if (varr) {
        int vlen = json_arr_len(varr);
        int i;
        for (i = 0; i < vlen && i < m.nverts * 3; i++) {
            m.pos[i] = (float)json_num_value(json_arr_get(varr, i));
        }
    }

    /* normals */
    JsonValue *narr = json_get_arr(jv, "normals");
    if (narr) {
        int nlen = json_arr_len(narr);
        int i;
        for (i = 0; i < nlen && i < m.nverts * 3; i++) {
            m.nrm[i] = (float)json_num_value(json_arr_get(narr, i));
        }
    }

    /* tris */
    JsonValue *tarr = json_get_arr(jv, "tris");
    if (tarr) {
        int tlen = json_arr_len(tarr);
        int i;
        for (i = 0; i < tlen && i < m.ntris * 3; i++) {
            m.idx[i] = (int)json_num_value(json_arr_get(tarr, i));
        }
    }

    /* bbox */
    JsonValue *bminj = json_get_obj(jv, "bmin");
    JsonValue *bmaxj = json_get_obj(jv, "bmax");
    if (bminj) { m.bmin.x = (float)json_get_num(bminj, "x"); m.bmin.y = (float)json_get_num(bminj, "y"); m.bmin.z = (float)json_get_num(bminj, "z"); }
    if (bmaxj) { m.bmax.x = (float)json_get_num(bmaxj, "x"); m.bmax.y = (float)json_get_num(bmaxj, "y"); m.bmax.z = (float)json_get_num(bmaxj, "z"); }

    m.valid = json_get_bool(jv, "valid") ? 1 : 0;
    return m;
}

bool v360_load(const char *path, Project *out)
{
    mz_zip_archive zip;
    size_t size;
    char *data;
    JsonValue *root;

    if (!path || !out) return false;

    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, path, 0)) return false;

    /* project.json */
    data = (char *)mz_zip_reader_extract_file_to_heap(&zip, "project.json", &size, 0);
    if (data) {
        root = json_parse(data, size);
        if (root) {
            const char *name = json_get_str(root, "name");
            if (name) {
                free(out->name); out->name = str_dup(name);
            }
            json_free(root);
        }
        mz_free(data);
    }

    /* schematic.json */
    data = (char *)mz_zip_reader_extract_file_to_heap(&zip, "schematic.json", &size, 0);
    if (data) {
        root = json_parse(data, size);
        if (root) {
            Schematic sch;
            memset(&sch, 0, sizeof(sch));
            const char *s_id = json_get_str(root, "id");
            const char *s_name = json_get_str(root, "name");
            sch.id = s_id ? str_dup(s_id) : NULL;
            sch.name = s_name ? str_dup(s_name) : NULL;
            JsonValue *orig = json_get_obj(root, "origin");
            if (orig) {
                sch.origin.x = (float)json_get_num(orig, "x");
                sch.origin.y = (float)json_get_num(orig, "y");
            }
            arr_push(out->schematics, sch);
            json_free(root);
        }
        mz_free(data);
    }

    /* pcb files – iterate archive entries with prefix "pcb_" */
    {
        mz_uint num_files = mz_zip_reader_get_num_files(&zip);
        mz_uint i;
        for (i = 0; i < num_files; i++) {
            mz_zip_archive_file_stat file_stat;
            if (!mz_zip_reader_file_stat(&zip, i, &file_stat)) continue;
            if (!str_starts_with(file_stat.m_filename, "pcb_")) continue;
            if (!str_ends_with(file_stat.m_filename, ".json")) continue;

            data = (char *)mz_zip_reader_extract_file_to_heap(&zip, file_stat.m_filename, &size, 0);
            if (!data) continue;
            root = json_parse(data, size);
            if (root) {
                Pcb pcb;
                memset(&pcb, 0, sizeof(pcb));
                const char *pid = json_get_str(root, "id");
                const char *pname = json_get_str(root, "name");
                pcb.id = pid ? str_dup(pid) : NULL;
                pcb.name = pname ? str_dup(pname) : NULL;
                pcb.thickness = (float)json_get_num(root, "thickness");
                arr_push(out->pcbs, pcb);
                json_free(root);
            }
            mz_free(data);
        }
    }

    /* bom.json */
    data = (char *)mz_zip_reader_extract_file_to_heap(&zip, "bom.json", &size, 0);
    if (data) {
        root = json_parse(data, size);
        if (root && root->type == J_ARR) {
            int i;
            for (i = 0; i < json_arr_len(root); i++) {
                JsonValue *item = json_arr_get(root, i);
                if (item && item->type == J_OBJ) {
                    Component comp;
                    memset(&comp, 0, sizeof(comp));
                    const char *c_id = json_get_str(item, "id");
                    const char *c_name = json_get_str(item, "name");
                    const char *c_mfr = json_get_str(item, "manufacturer");
                    const char *c_pkg = json_get_str(item, "package");
                    const char *c_fp  = json_get_str(item, "footprint");
                    comp.id = c_id ? str_dup(c_id) : NULL;
                    comp.name = c_name ? str_dup(c_name) : NULL;
                    comp.manufacturer = c_mfr ? str_dup(c_mfr) : NULL;
                    comp.package = c_pkg ? str_dup(c_pkg) : NULL;
                    comp.footprint = c_fp ? str_dup(c_fp) : NULL;
                    arr_push(out->components, comp);
                }
            }
            json_free(root);
        }
        mz_free(data);
    }

    /* meshes/ */
    {
        mz_uint num_files = mz_zip_reader_get_num_files(&zip);
        mz_uint i;
        for (i = 0; i < num_files; i++) {
            mz_zip_archive_file_stat file_stat;
            if (!mz_zip_reader_file_stat(&zip, i, &file_stat)) continue;
            if (!str_starts_with(file_stat.m_filename, "meshes/")) continue;

            data = (char *)mz_zip_reader_extract_file_to_heap(&zip, file_stat.m_filename, &size, 0);
            if (!data) continue;
            root = json_parse(data, size);
            if (root) {
                CadModel cm;
                memset(&cm, 0, sizeof(cm));
                /* derive name from path: meshes/foo.json -> foo */
                {
                    const char *slash = strrchr(file_stat.m_filename, '/');
                    if (!slash) slash = strrchr(file_stat.m_filename, '\\');
                    const char *name_start = slash ? slash + 1 : file_stat.m_filename;
                    char *name = str_dup_len(name_start, strlen(name_start));
                    char *dot = strrchr(name, '.');
                    if (dot) *dot = '\0';
                    cm.name = name;
                }
                cm.id = str_dup(cm.name);
                cm.mesh = deserialise_mesh_from_json(root);
                arr_push(out->cad_models, cm);
                json_free(root);
            }
            mz_free(data);
        }
    }

    mz_zip_reader_end(&zip);
    return true;
}

bool v360_export_step(const Project *p, const char *step_path)
{
    int i;
    Buf step;
    if (!p || !step_path) return false;

    buf_init(&step);

    buf_append_str(&step, "ISO-10303-21;\n");
    buf_append_str(&step, "HEADER;\n");
    buf_append_str(&step, "FILE_DESCRIPTION(('Volt360 Export'),'2;1');\n");
    if (p->name) {
        buf_appendf(&step, "FILE_NAME('%s','%s',('Volt360'),(''),'Volt360','');\n",
                    p->name, p->name);
    } else {
        buf_append_str(&step, "FILE_NAME('export','export',('Volt360'),(''),'Volt360','');\n");
    }
    buf_append_str(&step, "FILE_SCHEMA(('AUTOMOTIVE_DESIGN_CC2 { 1 0 10303 214 3 1 1 }'));\n");
    buf_append_str(&step, "ENDSEC;\n");
    buf_append_str(&step, "DATA;\n");

    for (i = 0; i < arr_count(p->cad_models); i++) {
        const CadModel *cm = &arr_get(p->cad_models, i);
        if (!cm->mesh.valid) continue;
        const char *name = cm->name ? cm->name : "part";
        int entity_id = i + 1;
        buf_appendf(&step, "#%d=PRODUCT('%s','%s',$,(#%d));\n",
                    entity_id, name, name, entity_id + 1000);
        buf_appendf(&step, "#%d=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE('','',#%d,$);\n",
                    entity_id + 500, entity_id);
        buf_appendf(&step, "#%d=PRODUCT_DEFINITION('design',$,#%d,#%d);\n",
                    entity_id + 800, entity_id + 500, entity_id + 600);
        buf_appendf(&step, "#%d=PRODUCT_DEFINITION_SHAPE('',$,#%d);\n",
                    entity_id + 1000, entity_id + 800);
        buf_appendf(&step, "#%d=SHAPE_REPRESENTATION('',(#%d),#%d);\n",
                    entity_id + 1200, entity_id + 1400, entity_id + 1300);
        buf_appendf(&step, "#%d=MANIFOLD_SURFACE_SHAPE_REPRESENTATION('%s_brep',(#%d),#%d);\n",
                    entity_id + 1400, name, entity_id + 1500, entity_id + 1300);
        int vert_count = cm->mesh.nverts;
        int tri_count = cm->mesh.ntris;
        int base_v = entity_id + 2000;
        buf_appendf(&step, "#%d=FACETED_BREP_SHAPE_REPRESENTATION('%s',(#%d),#%d);\n",
                    entity_id + 1500, name, base_v, entity_id + 1300);
        buf_appendf(&step, "#%d=FACETED_BREP('',$,#%d);\n",
                    base_v, base_v + 1);
        buf_appendf(&step, "#%d=CLOSED_SHELL('',(#%d));\n",
                    base_v + 1, base_v + 2);
        buf_appendf(&step, "#%d=ADVANCED_FACE('',(#%d),$,%cT%c);\n",
                    base_v + 2, base_v + 3, 'F', ' ');
        buf_appendf(&step, "#%d=SURFACE_OF_LINEAR_EXTRUSION('',$,#%d,$,$,$);\n",
                    base_v + 3, base_v + 4);
        buf_appendf(&step, "#%d=LINE('',#%d,$);\n",
                    base_v + 4, base_v + 5);
        buf_appendf(&step, "#%d=CARTESIAN_POINT('',(%.4f,%.4f,%.4f));\n",
                    base_v + 5, cm->mesh.bmin.x, cm->mesh.bmin.y, cm->mesh.bmin.z);
        buf_appendf(&step, "#%d=(NAMED_UNIT(*)%cSI_UNIT($,%cSOLID_MODEL.%cLENGTH_UNIT.));\n",
                    entity_id + 600, '&', '.', '.');
        buf_appendf(&step, "#%d=(GEOMETRIC_REPRESENTATION_CONTEXT(3)%cGLOBAL_UNCERTAINTY_ASSIGNED_CONTEXT((#%d))%cGLOBAL_UNIT_ASSIGNED_CONTEXT((#%d,#%d,#%d))%cREPRESENTATION_CONTEXT('',''));\n",
                    entity_id + 1300, '&', entity_id + 10000, '&',
                    entity_id + 600, entity_id + 601, entity_id + 602, '&');
        buf_appendf(&step, "#%d=(LENGTH_UNIT()%cNAMED_UNIT(*)%cSI_UNIT(%cMILLI.,%cMETRE.));\n",
                    entity_id + 600, '&', '&', '.', '.');
        buf_appendf(&step, "#%d=(NAMED_UNIT(*)%cPLANE_ANGLE_UNIT()%cSI_UNIT($,%cRADIAN.));\n",
                    entity_id + 601, '&', '&', '.');
        buf_appendf(&step, "#%d=(NAMED_UNIT(*)%cSOLID_ANGLE_UNIT()%cSI_UNIT($,%cSTERADIAN.));\n",
                    entity_id + 602, '&', '&', '.');
        buf_appendf(&step, "#%d=UNCERTAINTY_MEASURE_WITH_UNIT(LENGTH_MEASURE(0.01),#%d,'','');\n",
                    entity_id + 10000, entity_id + 600);
    }

    buf_append_str(&step, "ENDSEC;\n");
    buf_append_str(&step, "END-ISO-10303-21;\n");

    bool ok = file_write(step_path, step.data, step.len);
    buf_free(&step);
    return ok;
}

char *v360_bundle_info(const char *path)
{
    mz_zip_archive zip;
    mz_uint num_files, i;
    JsonW jw;
    int file_count = 0;
    size_t total_size = 0;
    char *project_name = NULL;
    char *project_version = NULL;
    char buffer[512];

    if (!path) return str_dup("{}");

    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, path, 0)) {
        return str_dup("{\"error\":\"cannot open bundle\"}");
    }

    /* read project.json for name/version */
    {
        size_t size;
        char *data = (char *)mz_zip_reader_extract_file_to_heap(&zip, "project.json", &size, 0);
        if (data) {
            JsonValue *root = json_parse(data, size);
            if (root) {
                const char *n = json_get_str(root, "name");
                if (n) project_name = str_dup(n);
                double ver = json_get_num(root, "version");
                if (ver > 0.0) {
                    snprintf(buffer, sizeof(buffer), "%.0f", ver);
                    project_version = str_dup(buffer);
                }
                json_free(root);
            }
            mz_free(data);
        }
    }

    num_files = mz_zip_reader_get_num_files(&zip);
    for (i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat stat;
        if (mz_zip_reader_file_stat(&zip, i, &stat)) {
            file_count++;
            total_size += (size_t)stat.m_uncomp_size;
        }
    }

    mz_zip_reader_end(&zip);

    jw_init(&jw); jw_begin(&jw);
    jw_key(&jw, "name");    jw_str(&jw, project_name ? project_name : "unknown");
    jw_key(&jw, "version"); jw_str(&jw, project_version ? project_version : "1");
    jw_key(&jw, "file_count"); jw_int(&jw, file_count);
    jw_key(&jw, "total_size"); jw_int(&jw, (long long)total_size);
    jw_end(&jw);

    free(project_name);
    free(project_version);

    char *result = str_dup(jw_cstr(&jw));
    jw_free(&jw);
    return result;
}
