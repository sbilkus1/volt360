#include "../src/core/json.h"
#include "../src/core/util.h"
#include "../src/model/project.h"
#include <stdio.h>

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    size_t len = 0;
    char *data = file_read("build\\test_project\\project.json", &len);
    if (!data) { printf("no data\n"); return 1; }
    printf("read %zu bytes\n", len);
    JsonValue *root = json_parse(data, len);
    if (!root) { printf("parse returned NULL\n"); free(data); return 1; }
    printf("root type=%d\n", (int)root->type);
    printf("symbols=%d schematics=%d pcbs=%d cad=%d docs=%d\n",
        json_arr_len(json_get_arr(root, "symbols")),
        json_arr_len(json_get_arr(root, "schematics")),
        json_arr_len(json_get_arr(root, "pcbs")),
        json_arr_len(json_get_arr(root, "cad_models")),
        json_arr_len(json_get_arr(root, "docs")));
    json_free(root);
    free(data);

    printf("calling project_load\n");
    Project p2; memset(&p2, 0, sizeof(p2));
    bool loaded = project_load(&p2, "build\\test_project");
    printf("loaded=%d symbols=%d schematics=%d pcbs=%d cad=%d docs=%d\n",
        loaded ? 1 : 0, p2.symbols.len, p2.schematics.len, p2.pcbs.len,
        p2.cad_models.len, p2.docs.len);
    project_free(&p2);
    printf("done\n");
    return 0;
}
