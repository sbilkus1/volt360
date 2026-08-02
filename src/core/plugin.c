#include "plugin.h"
#include "util.h"

#ifdef _WIN32
#include <windows.h>
#define PLUGIN_EXT ".dll"
#else
#include <dlfcn.h>
#include <dirent.h>
#ifdef __APPLE__
#define PLUGIN_EXT ".dylib"
#else
#define PLUGIN_EXT ".so"
#endif
#endif

/* --- Cross-platform loader ------------------------------------------ */

void *plugin_load(const char *path)
{
#ifdef _WIN32
    HMODULE h = LoadLibraryA(path);
    if (!h) return NULL;
    return (void *)h;
#else
    return dlopen(path, RTLD_NOW);
#endif
}

void *plugin_get_symbol(void *handle, const char *name)
{
#ifdef _WIN32
    return (void *)GetProcAddress((HMODULE)handle, name);
#else
    return dlsym(handle, name);
#endif
}

void plugin_unload(void *handle)
{
    if (!handle) return;
#ifdef _WIN32
    FreeLibrary((HMODULE)handle);
#else
    dlclose(handle);
#endif
}

/* --- Helper: try to resolve the complete API from a loaded handle --- */

typedef PluginAPI *(*get_plugin_api_fn)(void);

static int plugin_resolve_api(PluginHandle *ph)
{
    get_plugin_api_fn get_api;
    PluginAPI *api_ptr;

    /* Primary path: single get_plugin_api symbol returns a vtable pointer */
    get_api = (get_plugin_api_fn)plugin_get_symbol(ph->handle, "get_plugin_api");
    if (get_api) {
        api_ptr = get_api();
        if (api_ptr) {
            ph->api = *api_ptr;
            return 1;
        }
    }

    /* Fallback: resolve individual named symbols */
    ph->api.init        = (int(*)(void))                 plugin_get_symbol(ph->handle, "plugin_init");
    ph->api.tick        = (int(*)(float))                plugin_get_symbol(ph->handle, "plugin_tick");
    ph->api.render      = (int(*)(void))                 plugin_get_symbol(ph->handle, "plugin_render");
    ph->api.get_name    = (const char *(*)(void))        plugin_get_symbol(ph->handle, "plugin_get_name");
    ph->api.get_version = (const char *(*)(void))        plugin_get_symbol(ph->handle, "plugin_get_version");
    ph->api.get_features= (int(*)(void))                 plugin_get_symbol(ph->handle, "plugin_get_features");
    ph->api.shutdown    = (void(*)(void))                plugin_get_symbol(ph->handle, "plugin_shutdown");

    /* At minimum we need a name to consider it valid */
    if (!ph->api.get_name) return 0;

    return 1;
}

/* --- Load a single plugin by path into the manager ------------------ */

static int plugin_load_one(PluginManager *pman, const char *path)
{
    PluginHandle ph;
    void *h;

    memset(&ph, 0, sizeof(ph));

    h = plugin_load(path);
    if (!h) return 0;

    ph.handle = h;

    if (!plugin_resolve_api(&ph)) {
        plugin_unload(h);
        return 0;
    }

    /* Populate info from API calls */
    if (ph.api.get_name)
        ph.info.name = str_dup(ph.api.get_name());
    if (ph.api.get_version)
        ph.info.version = str_dup(ph.api.get_version());

    /* Optional "plugin_get_info" symbol for description/author */
    {
        typedef const PluginInfo *(*get_info_fn)(void);
        get_info_fn gi = (get_info_fn)plugin_get_symbol(h, "plugin_get_info");
        if (gi) {
            const PluginInfo *ext = gi();
            if (ext) {
                if (ext->description) ph.info.description = str_dup(ext->description);
                if (ext->author)      ph.info.author      = str_dup(ext->author);
            }
        }
    }

    ph.active = 1;

    /* Append to manager dynamic array */
    {
        PluginHandle *tmp = (PluginHandle*)realloc(pman->plugins,
            (size_t)(pman->nplugins + 1) * sizeof(PluginHandle));
        if (!tmp) {
            plugin_unload(h);
            return 0;
        }
        pman->plugins = tmp;
        pman->plugins[pman->nplugins] = ph;
        pman->nplugins++;
    }

    return 1;
}

/* --- Scan directory and load all plugin files ----------------------- */

#ifdef _WIN32

int plugin_load_all(PluginManager *pman, const char *dir)
{
    char search_path[1024];
    WIN32_FIND_DATAA fd;
    HANDLE hFind;
    int loaded = 0;

    snprintf(search_path, sizeof(search_path), "%s\\*%s", dir, PLUGIN_EXT);

    hFind = FindFirstFileA(search_path, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return 0;

    do {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s\\%s", dir, fd.cFileName);
        if (plugin_load_one(pman, full_path))
            loaded++;
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
    return loaded;
}

#else /* _WIN32 */

int plugin_load_all(PluginManager *pman, const char *dir)
{
    DIR *d;
    struct dirent *ent;
    int loaded = 0;

    d = opendir(dir);
    if (!d) return 0;

    while ((ent = readdir(d)) != NULL) {
        const char *name = ent->d_name;
        size_t extlen = strlen(PLUGIN_EXT);
        size_t namelen = strlen(name);

        if (namelen <= extlen) continue;
        if (strcmp(name + namelen - extlen, PLUGIN_EXT) != 0) continue;

        {
            char *full_path = path_join(dir, name);
            if (full_path) {
                if (plugin_load_one(pman, full_path))
                    loaded++;
                free(full_path);
            }
        }
    }

    closedir(d);
    return loaded;
}

#endif /* _WIN32 */

/* --- Manager operations --------------------------------------------- */

void plugin_manager_tick_all(PluginManager *pman, float dt)
{
    int i;
    for (i = 0; i < pman->nplugins; i++) {
        if (pman->plugins[i].active && pman->plugins[i].api.tick)
            pman->plugins[i].api.tick(dt);
    }
}

void plugin_manager_render_all(PluginManager *pman)
{
    int i;
    for (i = 0; i < pman->nplugins; i++) {
        if (pman->plugins[i].active && pman->plugins[i].api.render)
            pman->plugins[i].api.render();
    }
}

void plugin_manager_shutdown_all(PluginManager *pman)
{
    int i;
    for (i = 0; i < pman->nplugins; i++) {
        PluginHandle *ph = &pman->plugins[i];
        if (ph->active && ph->api.shutdown)
            ph->api.shutdown();
        plugin_unload(ph->handle);
        free((void*)ph->info.name);
        free((void*)ph->info.version);
        free((void*)ph->info.description);
        free((void*)ph->info.author);
    }
    free(pman->plugins);
    pman->plugins = NULL;
    pman->nplugins = 0;
}
