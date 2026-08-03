#pragma once
#include "../model/project.h"

bool v360_save(const Project *p, const char *path);
bool v360_load(const char *path, Project *out);
bool v360_export_step(const Project *p, const char *step_path);
char *v360_bundle_info(const char *path);
