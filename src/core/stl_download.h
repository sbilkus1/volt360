#pragma once
#include "../model/project.h"

typedef enum { SOURCE_THINGIVERSE, SOURCE_PRINTABLES, SOURCE_URL } StlSource;

bool stl_download_import(const char *url, CadMesh *out);
char *stl_detect_source(const char *url);
bool stl_batch_import(const char **urls, int count, CadMesh *out_meshes);
