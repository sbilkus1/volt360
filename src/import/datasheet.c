#include "importer.h"
#include "../core/xml.h"
#include "miniz.h"

// ================= Markdown -> text =================
bool importer_md_to_text(const char *path, char **out_text) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    Buf b; buf_init(&b);
    const char *p = data;
    while (p && *p && p < data + len) {
        char line[4096];
        int i = 0;
        while (*p && *p != '\n' && i < 4094 && p < data + len) line[i++] = *p++;
        if (*p == '\n') p++;
        line[i] = '\0';
        // strip markdown syntax
        char *s = line;
        while (*s == ' ' || *s == '\t') s++;
        if (*s == '#') while (*s == '#') s++;
        else if (strncmp(s, "```", 3) == 0) { buf_append_str(&b, "\n"); continue; }
        else if (*s == '>') s++;
        else if (*s == '-' || *s == '*' || *s == '+') {
            if (*(s + 1) == ' ') s += 2;
        }
        str_trim(s);
        // inline cleanup: strip *bold*, _em_, `code`, [txt](url)
        char *clean = str_replace_all(s, "**", "");
        char *t2 = str_replace_all(clean, "`", "");
        free(clean);
        char *t3 = str_replace_all(t2, "~~", "");
        free(t2);
        char *t4 = str_replace_all(t3, "|", " | ");
        free(t3);
        buf_append_str(&b, t4);
        free(t4);
        buf_append_str(&b, "\n");
    }
    free(data);
    char *out = str_dup(buf_cstr(&b));
    buf_free(&b);
    *out_text = out;
    return out[0] != '\0';
}

// ================= CSV -> text =================
typedef struct { ARRAY(char *) cells; } CsvRow;
typedef struct { CsvRow *v; int len; int cap; } CsvRowList;
static void csv_parse(const char *data, CsvRowList *rows) {
    CsvRow row; row.cells.v = NULL; row.cells.len = 0; row.cells.cap = 0;
    Buf cell; buf_init(&cell);
    bool in_quotes = false;
    const char *p = data;
    while (*p) {
        char c = *p;
        if (in_quotes) {
            if (c == '"') {
                if (p[1] == '"') { buf_append(&cell, "\"", 1); p++; }
                else in_quotes = false;
            } else buf_append(&cell, &c, 1);
            p++;
            continue;
        }
        if (c == '"' && cell.len == 0) { in_quotes = true; p++; continue; }
        if (c == ',' || c == ';' || c == '\t') {
            char *s = str_dup(buf_cstr(&cell));
            buf_free(&cell); buf_init(&cell);
            arr_push(row.cells, s);
            p++;
            continue;
        }
        if (c == '\n' || c == '\r') {
            if (*p == '\r' && p[1] == '\n') p++;
            p++;
            char *s = str_dup(buf_cstr(&cell));
            buf_free(&cell); buf_init(&cell);
            arr_push(row.cells, s);
            arr_push((*rows), row);
            row.cells.v = NULL; row.cells.len = 0; row.cells.cap = 0;
            continue;
        }
        buf_append(&cell, &c, 1);
        p++;
    }
    if (cell.len > 0 || row.cells.len > 0) {
        char *s = str_dup(buf_cstr(&cell));
        buf_free(&cell); buf_init(&cell);
        arr_push(row.cells, s);
        arr_push((*rows), row);
    }
    buf_free(&cell);
}

bool importer_csv_to_text(const char *path, char **out_text) {
    size_t len;
    char *data = file_read(path, &len);
    if (!data) return false;
    ARRAY(CsvRow) rows; arr_init(rows);
    csv_parse(data, (CsvRowList *)&rows);
    free(data);
    Buf b; buf_init(&b);
    for (int r = 0; r < rows.len; r++) {
        for (int c = 0; c < rows.v[r].cells.len; c++) {
            if (c) buf_append_str(&b, " | ");
            char *trimmed = str_dup(rows.v[r].cells.v[c]);
            str_trim(trimmed);
            buf_append_str(&b, trimmed);
            free(trimmed);
        }
        buf_append_str(&b, "\n");
    }
    // free
    for (int r = 0; r < rows.len; r++) str_free_split(rows.v[r].cells.v, rows.v[r].cells.len);
    arr_free(rows);
    char *out = str_dup(buf_cstr(&b));
    buf_free(&b);
    *out_text = out;
    return out[0] != '\0';
}

// ================= DOCX -> text =================
bool importer_docx_to_text(const char *path, char **out_text) {
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, path, 0)) return false;
    size_t xml_len = 0;
    void *xml_data = NULL;
    int n = (int)mz_zip_reader_get_num_files(&zip);
    for (int i = 0; i < n; i++) {
        char name[512];
        mz_zip_reader_get_filename(&zip, (mz_uint)i, name, sizeof(name));
        if (strcmp(name, "word/document.xml") == 0) {
            xml_data = mz_zip_reader_extract_to_heap(&zip, (mz_uint)i, &xml_len, 0);
            break;
        }
    }
    if (!xml_data) { mz_zip_reader_end(&zip); return false; }
    XmlNode *root = xml_parse((const char *)xml_data, xml_len);
    free(xml_data);
    mz_zip_reader_end(&zip);
    if (!root) return false;
    Buf b; buf_init(&b);
    // walk: w:p -> newline, w:t -> text, w:tab -> tab
    ARRAY(XmlNode *) stack; arr_init(stack);
    arr_push(stack, root);
    while (stack.len) {
        XmlNode *node = arr_last(stack);
        arr_remove_at(stack, stack.len - 1);
        if (xml_name_is(node, "p")) buf_append_str(&b, "\n");
        if (xml_name_is(node, "t") && node->text) { buf_append_str(&b, node->text); }
        if (xml_name_is(node, "tab")) buf_append_str(&b, "\t");
        // push children reversed
        for (int i = node->children.len - 1; i >= 0; i--) arr_push(stack, node->children.v[i]);
    }
    arr_free(stack);
    xml_free(root);
    char *out = str_dup(buf_cstr(&b));
    buf_free(&b);
    *out_text = out;
    return out[0] != '\0';
}

// ================= generic =================
char *importer_text_from_file(const char *path) {
    const char *ext = path_ext(path);
    char *out = NULL;
    if (strcmp(ext, "md") == 0 || strcmp(ext, "markdown") == 0 || strcmp(ext, "txt") == 0) {
        importer_md_to_text(path, &out);
    } else if (strcmp(ext, "csv") == 0) {
        importer_csv_to_text(path, &out);
    } else if (strcmp(ext, "docx") == 0) {
        importer_docx_to_text(path, &out);
    } else {
        size_t len;
        char *data = file_read(path, &len);
        if (data) { out = data; }
    }
    if (!out) out = str_dup("");
    return out;
}
