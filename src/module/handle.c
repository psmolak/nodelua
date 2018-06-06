#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "handle.h"


/* ------------------------------------------------------------------- */
/* API                                                                 */
/* ------------------------------------------------------------------- */


/* int uv_is_active(const uv_handle_t* handle) */
int l_is_active(lua_State* L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_is_active(handle));

    return 1;
}


/* int uv_is_closing(const uv_handle_t* handle) */
int l_is_closing(lua_State* L)
{
    uv_handle_t* handle =  (uv_handle_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_is_closing(handle));

    return 1;
}


/* void uv_close(uv_handle_t* handle, uv_close_cb close_cb) */
int l_close(lua_State *L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    l_handle_d *data = (l_handle_d*)handle->data;

    lua_pushvalue(L, 2);
    int on_close = luaL_ref(L, LUA_REGISTRYINDEX);
    data->on_close = on_close;
    uv_close(handle, l_close_cb);

    return 0;
}


/* void uv_ref(uv_handle_t* handle) */
int l_ref(lua_State* L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    uv_ref(handle);

    return 0;
}


/* void uv_unref(uv_handle_t* handle) */
int l_unref(lua_State *L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    uv_unref(handle);

    return 0;
}


/* int uv_has_ref(const uv_handle_t* handle) */
int l_has_ref(lua_State *L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_has_ref(handle));

    return 1;
}


/* size_t uv_handle_size(uv_handle_type type) */
int l_handle_size(lua_State *L)
{
    uv_handle_type type = (uv_handle_type)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_handle_size(type));

    return 1;
}


/* ------------------------------------------------------------------- */
/* Miscellenaous API                                                   */
/* ------------------------------------------------------------------- */

/* int uv_send_buffer_size(uv_handle_t* handle, int* value) */
int l_send_buffer_size(lua_State* L)
{

}


/* int uv_recv_buffer_size(uv_handle_t* handle, int*value) */
int l_recv_buffer_size(lua_State* L)
{

}


/* int uv_fileno(const uv_handle_t* handle, uv_os_fd_t* fd) */
int l_fileno(lua_State* L)
{

}


/* uv_loop_t* uv_handle_get_loop(const uv_handle_t* handle) */
int l_handle_get_loop(lua_State* L)
{

}


/* uv_handle_type uv_handle_get_type(const uv_handle_t* handle) */
int l_handle_get_type(lua_State* L)
{
    uv_handle_t *handle = (uv_handle_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, (int)uv_handle_get_type(handle));

    return 1;
}


/* ------------------------------------------------------------------- */
/* Callbacks                                                           */
/* ------------------------------------------------------------------- */


/* Type definition for callback passed to uv_read_start() and uv_udp_recv_start() */
void l_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    l_handle_d *data = (l_handle_d*)handle->data;
    lua_State *L = data->L;
    int on_alloc_ref = data->on_alloc;

    /* TODO: make sure Lua callback knows on what handle it operates */
    /* TODO: pass uv_buf_t* object and suggested_size to Lua callback somehow */
    lua_rawgeti(L, LUA_REGISTRYINDEX, on_alloc_ref);
    lua_call(L, 0, 0);

    buf->base = malloc(suggested_size);
    buf->len = suggested_size;

    /* Since this callback is fired many times through its handle lifetime */
    /* there is no need to unref Lua callback here */
}


/* Type definition for callback passed to uv_close() */
void l_close_cb(uv_handle_t* handle)
{
    l_handle_d *data = (l_handle_d*)handle->data;
    lua_State *L = data->L;
    int on_close_ref = data->on_close;
    int on_alloc_ref = data->on_alloc;

    /* TODO: make sure Lua callback knows on what handle it operates */
    lua_rawgeti(L, LUA_REGISTRYINDEX, on_close_ref);
    lua_call(L, 0, 0);

    /* It's save to unref Lua callback here, since every future
     * close call on this handle will accept fresh callback*/
    luaL_unref(L, LUA_REGISTRYINDEX, on_close_ref);
    luaL_unref(L, LUA_REGISTRYINDEX, on_alloc_ref);
}

