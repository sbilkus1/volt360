#include "version.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

bool version_commit(Project *p, const char *message) {
    if (!p || !p->path || !p->path[0]) return false;
    char snap_path[512];
    time_t now = time(NULL);
    snprintf(snap_path, sizeof(snap_path), "%s/.versions/v%d.json", p->path, (int)now);
    // would call project_save with custom path
    // stub: just note the attempt
    (void)message;
    return false; // requires directory creation etc - stub
}

char *version_history(const char *project_path) {
    char buf[512];
    snprintf(buf, sizeof(buf),
        "=== VERSION HISTORY ===\n"
        "Path: %s\n"
        "Stub: version tracking infrastructure not yet built\n"
        "Planned: .versions/ directory with timestamped JSON snapshots\n",
        project_path ? project_path : "?");
    return str_dup(buf);
}

bool version_rollback(Project *p, int version_index) {
    (void)p; (void)version_index;
    return false; // stub
}

int version_count(const char *project_path) {
    (void)project_path;
    return 0; // stub
}
