#include "importer.h"
#include "miniz.h"
#include <windows.h>

// ================= zip handling =================
static bool file_is_dir(const char *path) {
    DWORD a = GetFileAttributesA(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}

static bool make_dirs(const char *path) {
    // creates all dirs in path
    char *copy = str_dup(path);
    char *p = copy;
    while (*p) {
        if (*p == '/' || *p == '\\') {
            char sep = *p;
            *p = '\0';
            size_t seg = (size_t)(p - copy);
            if (seg > 1 && copy[seg - 1] != ':') CreateDirectoryA(copy, NULL);
            *p = sep;
        }
        p++;
    }
    if (copy[0] && copy[strlen(copy) - 1] != ':') CreateDirectoryA(copy, NULL);
    free(copy);
    return true;
}

static bool temp_dir(char *out, size_t n) {
    char t[MAX_PATH];
    DWORD l = GetTempPathA(MAX_PATH, t);
    if (l == 0 || l >= MAX_PATH) return false;
    snprintf(out, n, "%sllamacadeda_%08x\\", t, (unsigned)GetTickCount());
    CreateDirectoryA(out, NULL);
    return true;
}

static bool is_supported_import_ext(const char *ext) {
    static const char *exts[] = {
        "md", "csv", "docx", "txt", "step", "stp", "stl", "obj",
        "kicad_sch", "kicad_pcb", "kicad_sym", "kicad_mod", "kicad_pro",
        "sch", "brd", "lpp", "lp_sym", "lp_fp", "lp_cmp", "lp_dev", "lp_pkg", "lp",
        "net", "cir", "asc", "zip"
    };
    for (size_t i = 0; i < sizeof(exts) / sizeof(exts[0]); i++) {
        if (strcmp(ext, exts[i]) == 0) return true;
    }
    return false;
}

static char *sanitize_zip_name(const char *name) {
    // remove any ".." components and drive letters
    Buf b; buf_init(&b);
    ARRAY(char *) parts; arr_init(parts);
    int n = 0;
    char **toks = str_split(name, "/\\", &n);
    for (int i = 0; i < n; i++) {
        if (strcmp(toks[i], ".") == 0 || strcmp(toks[i], "..") == 0 || strchr(toks[i], ':')) continue;
        if (strlen(toks[i]) > 0) arr_push(parts, toks[i]);
    }
    for (int i = 0; i < parts.len; i++) {
        if (i) buf_append_str(&b, "/");
        buf_append_str(&b, parts.v[i]);
    }
    char *r = str_dup(buf_cstr(&b));
    buf_free(&b);
    str_free_split(toks, n);
    arr_free(parts);
    return r;
}

static void import_result_add(ImportResult *r, ImportResult *sub) {
    r->ncomponents += sub->ncomponents;
    r->nsymbols += sub->nsymbols;
    r->nfootprints += sub->nfootprints;
    r->ncad += sub->ncad;
    r->nsch += sub->nsch;
    r->npcb += sub->npcb;
    r->ndocs += sub->ndocs;
}

static void remove_dir_tree(const char *path) {
    char search[MAX_PATH];
    snprintf(search, sizeof(search), "%s*", path);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(search, &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0) continue;
        char full[MAX_PATH];
        snprintf(full, sizeof(full), "%s%s", path, fd.cFileName);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            char sub[MAX_PATH];
            snprintf(sub, sizeof(sub), "%s\\", full);
            remove_dir_tree(sub);
            RemoveDirectoryA(full);
        } else {
            DeleteFileA(full);
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

ImportResult importer_import_zip(Project *p, const char *zip_path) {
    ImportResult r;
    import_result_init(&r);
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, zip_path, 0)) {
        r.ok = 0;
        snprintf(r.message, sizeof(r.message), "Cannot open zip: %s", zip_path);
        return r;
    }
    char tmp[MAX_PATH];
    if (!temp_dir(tmp, sizeof(tmp))) {
        mz_zip_reader_end(&zip);
        r.ok = 0;
        snprintf(r.message, sizeof(r.message), "Cannot create temp dir");
        return r;
    }
    int n = (int)mz_zip_reader_get_num_files(&zip);
    int imported = 0;
    for (int i = 0; i < n; i++) {
        char name[1024];
        mz_zip_reader_get_filename(&zip, (mz_uint)i, name, sizeof(name));
        if (!name[0]) continue;
        if (mz_zip_reader_is_file_a_directory(&zip, (mz_uint)i)) continue;
        const char *ext = path_ext(name);
        if (!is_supported_import_ext(ext)) continue;
        size_t size = 0;
        void *data = mz_zip_reader_extract_to_heap(&zip, (mz_uint)i, &size, 0);
        if (!data) continue;
        char *safe = sanitize_zip_name(name);
        char full[MAX_PATH];
        snprintf(full, sizeof(full), "%s%s", tmp, safe);
        free(safe);
        // create parent dirs
        char *dir = path_dirname(full);
        if (strcmp(dir, ".") != 0) make_dirs(dir);
        free(dir);
        if (file_write(full, data, size)) {
            ImportResult sub = importer_import_file(p, full);
            import_result_add(&r, &sub);
            if (sub.ok) imported++;
        }
        free(data);
    }
    mz_zip_reader_end(&zip);
    remove_dir_tree(tmp);
    RemoveDirectoryA(tmp);
    r.ok = 1;
    snprintf(r.message, sizeof(r.message), "Imported %d file(s) from zip", imported);
    return r;
}
