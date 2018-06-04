#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "module/handle.h"
#include "module/stream.h"
#include "module/timer.h"

struct nodelua_s {
    lua_State *L;
    uv_loop_t* loop;
};

typedef struct nodelua_s nodelua_t;

void nodelua_init(nodelua_t* node);
void nodelua_openlibs(lua_State* L);
void nodelua_run(nodelua_t* node, const char* file_name);

void nodelua_init(nodelua_t* node)
{
    node->L = luaL_newstate();
    luaL_openlibs(node->L);
    nodelua_openlibs(node->L);
    node->loop = uv_default_loop();
}

void nodelua_openlibs(lua_State* L)
{
    const struct luaL_Reg functions[] = {
        { "timer_new", l_timer_new },
        { "timer_delete", l_timer_delete },
        { "timer_init", l_timer_init },
        { "timer_start", l_timer_start },
        { "timer_stop", l_timer_stop },
        { NULL, NULL }
    };

    luaL_newlib(L, functions);
    lua_setglobal(L, "uv");
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
