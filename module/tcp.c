#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "stream.h"
#include "tcp.h"


/* ------------------------------------------------------------------- */
/* Auxiliary                                                           */
/* ------------------------------------------------------------------- */


int l_tcp_new(lua_State* L)
{
    uv_tcp_t* handle = malloc(sizeof(uv_tcp_t));
    lua_pushlightuserdata(L, (void*)handle);

    return 1;
}

int l_tcp_delete(lua_State* L)
{
    uv_tcp_t* handle = lua_touserdata(L, 1);
    free(handle);

    return 0;
}


/* ------------------------------------------------------------------- */
/* API                                                                 */
/* ------------------------------------------------------------------- */


/* int uv_tcp_init(uv_loop_t* loop, uv_tcp_t* handle) */
int l_tcp_init(lua_State* L)
{
    uv_tcp_t *handle = (uv_tcp_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_tcp_init(uv_default_loop(), handle));

    return 1;
}


/* int uv_tcp_init_ex(uv_loop_t* loop, uv_tcp_t* handle, unsigned int flags) */
int l_tcp_init_ex(lua_State* L)
{
    uv_tcp_t *handle = (uv_tcp_t*)lua_touserdata(L, 1);
    unsigned int flags = lua_tonumber(L, 2);

    lua_pushinteger(L, uv_tcp_init_ex(uv_default_loop(), handle, flags));
    return 1;
}


/* int uv_tcp_open(uv_tcp_t* handle, uv_os_sock_t sock) */
int l_tcp_open(lua_State* L)
{
    uv_tcp_t *handle = (uv_tcp_t*)lua_touserdata(L, 1);

    /* TODO: initialize somehow this socket (perhaps some Lua table?) */
    uv_os_sock_t sock;

    lua_pushinteger(L, uv_tcp_open(handle, sock));
    return 1;
}


/* int uv_tcp_nodelay(uv_tcp_t* handle, int enable) */
int l_tcp_nodelay(lua_State* L)
{
    uv_tcp_t *handle = (uv_tcp_t*)lua_touserdata(L, 1);
    int enable = lua_tonumber(L, 2);

    lua_pushinteger(L, uv_tcp_nodelay(handle, enable));
    return 1;
}


/* int uv_tcp_keepalive(uv_tcp_t* handle, int enable, unsigned int delay) */
int l_tcp_keepalive(lua_State* L)
{
    uv_tcp_t *handle = (uv_tcp_t*)lua_touserdata(L, 1);
    int enable = lua_tonumber(L, 2);
    unsigned int delay = lua_tonumber(L, 3);

    lua_pushinteger(L, uv_tcp_keepalive(handle, enable, delay));
    return 1;
}


/* int uv_tcp_simultaneous_accepts(uv_tcp_t* handle, int enable) */
int l_tcp_simultaneous_accepts(lua_State* L)
{
    uv_tcp_t *handle = (uv_tcp_t*)lua_touserdata(L, 1);
    int enable = lua_tonumber(L, 2);

    lua_pushinteger(L, uv_tcp_simultaneous_accepts(handle, enable));
    return 1;
}


/* int uv_tcp_bind(uv_tcp_t* handle, const struct sockaddr* addr, unsigned int flags) */
int l_tcp_bind(lua_State* L)
{
    uv_tcp_t* handle = (uv_tcp_t*)lua_touserdata(L, 1);

    /* TODO: make sure this is passed properly */
    const struct sockaddr *addr = (struct sockaddr*)lua_touserdata(L, 2);
    unsigned int flags = lua_tonumber(L, 3);

    lua_pushinteger(L, uv_tcp_bind(handle, addr, flags));
    return 1;
}


/* int uv_tcp_getsockname(const uv_tcp_t* handle, struct sockaddr* name, int* namelen) */
int l_tcp_getsockname(lua_State* L)
{

}


/* int uv_tcp_connect(uv_connect_t* req, uv_tcp_t* handle, const struct sockaddr* addr, uv_connect_cb cb) */
int l_tcp_connect(lua_State* L)
{
    uv_connect_t *req = (uv_connect_t*)lua_touserdata(L, 1);
    uv_tcp_t *handle = (uv_tcp_t*)lua_touserdata(L, 2);
    const struct sockaddr *addr = (struct sockaddr*)lua_touserdata(L, 3);
    lua_pushvalue(L, 4);
    int cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    l_connect_d *data = req->data;
    data->callback = cb_ref;

    lua_pushinteger(L, uv_tcp_connect(req, handle, addr, l_connect_cb));
    return 1;
}
