#pragma once
/* plugin.h - DLL plugin system for Volt360 */

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- Plugin API vtable ----------------------------------------------- */

typedef struct PluginAPI {
    int   (*init)(void);
    int   (*tick)(float dt);
    int   (*render)(void);
    const char *(*get_name)(void);
    const char *(*get_version)(void);
    int   (*get_features)(void);
    void  (*shutdown)(void);
} PluginAPI;

/* --- Plugin info ---------------------------------------------------- */

typedef struct PluginInfo {
    const char *name;
    const char *version;
    const char *description;
    const char *author;
} PluginInfo;

/* --- Plugin handle -------------------------------------------------- */

typedef struct PluginHandle {
    void      *handle;
    PluginAPI  api;
    PluginInfo info;
    int        active;
} PluginHandle;

/* --- Plugin manager ------------------------------------------------- */

typedef struct PluginManager {
    PluginHandle *plugins;
    int           nplugins;
} PluginManager;

/* --- Cross-platform loader API -------------------------------------- */

/* Load a plugin DLL/SO/DYLIB by path. Returns NULL on failure. */
void *plugin_load(const char *path);

/* Retrieve a symbol from a loaded plugin. Returns NULL on failure. */
void *plugin_get_symbol(void *handle, const char *name);

/* Unload a plugin handle. */
void plugin_unload(void *handle);

/* Scan a directory for .dll/.so/.dylib files and load them all.
   Returns number of plugins loaded, appends to pman. */
int plugin_load_all(PluginManager *pman, const char *dir);

/* Tick all active plugins. */
void plugin_manager_tick_all(PluginManager *pman, float dt);

/* Render all active plugins. */
void plugin_manager_render_all(PluginManager *pman);

/* Shutdown and unload all plugins. */
void plugin_manager_shutdown_all(PluginManager *pman);

#ifdef __cplusplus
}
#endif
