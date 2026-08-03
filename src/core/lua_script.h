#pragma once

#include <stdbool.h>

typedef struct {
    void *L;
    int initialized;
} LuaEngine;

bool lua_init(LuaEngine *eng);
void lua_close(LuaEngine *eng);
bool lua_run_file(LuaEngine *eng, const char *path);
bool lua_run_string(LuaEngine *eng, const char *code);
char *lua_get_output(LuaEngine *eng);

void lua_register_api(LuaEngine *eng);
