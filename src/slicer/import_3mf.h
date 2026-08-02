#pragma once
/* import_3mf.h - 3MF file importer (ZIP-based with XML parsing).
 * Extracts mesh data from /3D/3dmodel.model inside the 3MF archive.
 */
#include <stdbool.h>
#include "../model/project.h"

/* Import a .3mf file into a CadMesh struct.
 * Returns true on success, false on failure. */
bool import_3mf(const char *path, CadMesh *out);
