#pragma once
// mesh_import.h - Additional mesh format importers: 3MF, PLY, AMF, GLTF.
#include <stdbool.h>
#include "../model/project.h"

// Import 3MF file (ZIP container with 3D model). Returns number of meshes imported.
int mesh_import_3mf(const char *path, Project *p);

// Import PLY file (Stanford polygon format).
bool mesh_import_ply(const char *path, CadMesh *out);

// Import AMF file (Additive Manufacturing Format, XML-based).
bool mesh_import_amf(const char *path, CadMesh *out);
