#pragma once
/* plugin_market.h - plugin marketplace with community repository */
#include <stdbool.h>
#include "raylib.h"

typedef struct {
    char name[64];
    char author[32];
    char version[16];
    char desc[256];
    char category[32];
    char url[256];
    char icon[4];
    int downloads;
    float rating;
    int installed;
} PluginEntry;

typedef struct {
    PluginEntry *entries;
    int nentries;
    int loading;
    char repo_url[256];
} PluginMarket;

void plugin_market_init(PluginMarket *pm);
void plugin_market_fetch(PluginMarket *pm, const char *repo_url);
void plugin_market_free(PluginMarket *pm);
int plugin_market_search(PluginMarket *pm, const char *query, PluginEntry *results, int max_results);
bool plugin_market_install(PluginMarket *pm, int index, const char *plugins_dir);
bool plugin_market_uninstall(int index, const char *plugins_dir);
char *plugin_market_status(const PluginMarket *pm);
void plugin_market_render(const PluginMarket *pm, Rectangle viewport);
