#include <stdio.h>
#include <stdlib.h>

#include <uv.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "module.h"

struct nodelua_s {
    lua_State *L;
    uv_loop_t* loop;
};

typedef struct nodelua_s nodelua_t;

void nodelua_init(nodelua_t* node)
{
    node->L = luaL_newstate();

    luaL_openlibs(node->L);
    lua_pushcfunction(node->L, l_uv_timer_init);
    lua_setglobal(node->L, "l_uv_timer_init");
    lua_pushcfunction(node->L, l_uv_timer_start);
    lua_setglobal(node->L, "l_uv_timer_start");

    node->loop = uv_default_loop();
}

void nodelua_run(nodelua_t* node, const char* file_name)
{
    if (luaL_loadfile(node->L, file_name) || lua_pcall(node->L, 0, 0, 0)) {
        fprintf(stderr, "Error occured while reading script file: %s\n", file_name);
    }

    uv_run(node->loop, UV_RUN_DEFAULT);
}

int main(void)
{
    nodelua_t node;
    nodelua_init(&node);
    nodelua_run(&node, "script.lua");

    return 0;
}
