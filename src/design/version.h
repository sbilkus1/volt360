#pragma once
// version.h - Simple project version history tracking.
#include <stdbool.h>
#include "../model/project.h"

// Save a snapshot of the project with a commit message.
bool version_commit(Project *p, const char *message);

// List recent commits as text.
char *version_history(const char *project_path);

// Rollback to a previous version (load from snapshot).
bool version_rollback(Project *p, int version_index);

// Get number of saved versions.
int version_count(const char *project_path);
