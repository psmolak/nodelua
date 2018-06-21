#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "timer.h"


/* ------------------------------------------------------------------- */
/* Auxiliary                                                           */
/* ------------------------------------------------------------------- */


int l_timer_new(lua_State* L)
{
    uv_timer_t* handle = malloc(sizeof(uv_timer_t));
    if (handle == NULL) {
        luaL_error(L, "Error while allocating memory for timer handler");
    }
    lua_pushlightuserdata(L, (void*)handle);

    return 1;
}

int l_timer_delete(lua_State* L)
{
    uv_timer_t* handle = lua_touserdata(L, 1);
    free(handle->data);
    free(handle);

    return 0;
}


/* ------------------------------------------------------------------- */
/* API                                                                 */
/* ------------------------------------------------------------------- */


/* int uv_timer_init(uv_loop_t* loop, uv_timer_t* handle); */
int l_timer_init(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_timer_init(uv_default_loop(), handle));
    handle->data = malloc(sizeof(l_timer_d));

    l_timer_d *data = (l_timer_d*)handle->data;
    data->L = L;

    return 1;
}


/* int uv_timer_start(uv_timer_t* handle, uv_timer_cb cb, uint64_t timeout, uint64_t repeat); */
int l_timer_start(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    lua_pushvalue(L, 2);
    int cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    uint64_t timeout = lua_tonumber(L, 3);
    uint64_t repeat = lua_tonumber(L, 4);

    l_timer_d *data = (l_timer_d*)handle->data;
    data->cb = cb_ref;

    lua_pushinteger(L, uv_timer_start(handle, l_timer_cb, timeout, repeat));
    return 1;
}


/* int uv_timer_stop(uv_timer_t* handle); */
int l_timer_stop(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_timer_stop(handle));
    return 1;
}


/* int uv_timer_again(uv_timer_t* handle); */
int l_timer_again(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_timer_again(handle));
    return 1;
}


/* void uv_timer_set_repeat(uv_timer_t* handle, uint64_t repeat); */
int l_timer_set_repeat(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    uint64_t repeat = lua_tointeger(L, 2);
    uv_timer_set_repeat(handle, repeat);
    return 0;
}


/* uint64_t uv_timer_get_repeat(const uv_timer_t* handle); */
int l_timer_get_repeat(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_timer_get_repeat(handle));
    return 1;
}

/* ------------------------------------------------------------------- */
/* Callbacks                                                           */
/* ------------------------------------------------------------------- */


/* void uv_timer_cb(uv_timer_t* handle); */
void l_timer_cb(uv_timer_t* handle)
{
    l_timer_d *data = (l_timer_d*)handle->data;
    lua_State *L = data->L;

    lua_rawgeti(L, LUA_REGISTRYINDEX, data->cb);
    lua_call(L, 0, 0);
}

