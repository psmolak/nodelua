#include <stdio.h>
#include <stdlib.h>

#include <uv.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "handle.h"
#include "stream.h"


/* ------------------------------------------------------------------- */
/* API                                                                 */
/* ------------------------------------------------------------------- */


/* int uv_shutdown(uv_shutdown_t* req, uv_stream_t* handle, uv_shutdown_cb cb) */
int l_shutdown(lua_State* L)
{
    uv_shutdown_t* req = (uv_shutdown_t*)lua_touserdata(L, 1);
    uv_stream_t* handle = (uv_stream_t*)lua_touserdata(L, 2);
    lua_pushvalue(L, 3);
    int cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    /* cb_ref should be assigned to uv_shutdown_t request, not uv_stream_t */
    l_shutdown_d *data = (l_shutdown_d*)req->data;
    data->callback = cb_ref;

    lua_pushinteger(L, uv_shutdown(req, handle, l_shutdown_cb));
    return 1;
}


/* int uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb); */
int l_listen(lua_State* L)
{
    uv_stream_t* stream = (uv_stream_t*)lua_touserdata(L, 1);
    int backlog = lua_tointeger(L, 2);
    lua_pushvalue(L, 3);
    int cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    l_stream_d *data = (l_stream_d*)stream->data;
    data->on_connection = cb_ref;
    
    lua_pushinteger(L, uv_listen(stream, backlog, l_connection_cb));
    return 1;
}


/* int uv_accept(uv_stream_t* server, uv_stream_t* client); */
int l_accept(lua_State* L)
{
    uv_stream_t* server = (uv_stream_t*)lua_touserdata(L, 1);
    uv_stream_t* client = (uv_stream_t*)lua_touserdata(L, 2);

    lua_pushinteger(L, uv_accept(server, client));
    return 1;
}


/* int uv_read_start(uv_stream_t* stream, uv_alloc_cb alloc_cb, uv_read_cb read_cb); */
int l_read_start(lua_State* L)
{
    uv_stream_t* stream = (uv_stream_t*)lua_touserdata(L, 1);
    lua_pushvalue(L, 2);
    int on_alloc_cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pushvalue(L, 3);
    int on_read_cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    l_stream_d *data = (l_stream_d*)stream->data;
    data->on_alloc = on_alloc_cb_ref;
    data->on_read = on_read_cb_ref;

    lua_pushinteger(L, uv_read_start(stream, l_alloc_cb, l_read_cb));
    return 1;
}


/* int uv_read_stop(uv_stream_t* stream); */
int l_read_stop(lua_State* L)
{
    uv_stream_t* stream = (uv_stream_t*)lua_touserdata(L, 1);
    lua_pushvalue(L, uv_read_stop(stream));

    l_stream_d *data = stream->data;
    int on_read = data->on_read;

    /* apparently this should be unreferenced here */
    luaL_unref(L, LUA_REGISTRYINDEX, on_read);

    return 1;
}


/* int uv_write(uv_write_t* req, 
 *              uv_stream_t* handle,
 *              const uv_buf_t bufs[],
 *              unsigned int nbuffs,
 *              uv_write_cb cb); */
int l_write(lua_State* L)
{
    uv_write_t* req = (uv_write_t*)lua_touserdata(L, 1);
    uv_stream_t* handle = (uv_stream_t*)lua_touserdata(L, 2);

    /* this should be Lua table of uv_buf_t's */
    const uv_buf_t *bufs = (uv_buf_t*)lua_touserdata(L, 3);
    unsigned int nbuffs = lua_tointeger(L, 4);

    lua_pushvalue(L, 5);
    int write_cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    l_write_d *data = (l_write_d*)req->data;
    data->callback = write_cb_ref;

    lua_pushinteger(L, uv_write(req, handle, bufs, nbuffs, l_write_cb));
    return 1;
}


/* int uv_write2(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs, uv_stream_t* send_handle, uv_write_cb cb) */
int l_write2(lua_State* L)
{

}


/* int uv_try_write(uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs) */
int l_try_write(lua_State *L)
{

}


/* int uv_is_readable(const uv_stream_t* handle) */
int l_is_readable(lua_State *L)
{

}


/* int uv_is_writable(const uv_stream_t* handle) */
int l_is_writable(lua_State *L)
{

}


/* int uv_stream_set_blocking(uv_stream_t* handle, int blocking) */
int l_stream_set_blocking(lua_State *L)
{

}


/* size_t uv_stream_get_write_queue_size(const uv_stream_t* stream) */
int l_stream_get_write_queue_size(lua_State *L)
{

}


/* ------------------------------------------------------------------- */
/* Callbacks                                                           */
/* ------------------------------------------------------------------- */


/* Callback called when data was read on a stream */
void l_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    l_stream_d *data = (l_stream_d*)stream->data;
    lua_rawgeti(data->L, LUA_REGISTRYINDEX, data->on_read);
    lua_pushinteger(data->L, nread);

    /* Now it should push those as table of strings or userdata */
    lua_pushlightuserdata(data->L, (void*)buf);
    lua_call(data->L, 2, 0);
}


/* Callback called after data was written on a stream */
void l_write_cb(uv_write_t* req, int status)
{
    l_write_d *data = (l_write_d*)req->data;
    lua_rawgeti(data->L, LUA_REGISTRYINDEX, data->callback);
    lua_pushinteger(data->L, status);
    lua_call(data->L, 1, 0);
}

/* Callback called after a connection started by uv_connect() is done. */
void l_connect_cb(uv_connect_t* req, int status)
{
    l_connect_d *data = (l_connect_d*)req->data;
    lua_rawgeti(data->L, LUA_REGISTRYINDEX, data->callback);
    lua_pushinteger(data->L, status);
    lua_call(data->L, 1, 0);
}

/* Callback called after a shutdown request has been completed */
void l_shutdown_cb(uv_shutdown_t* req, int status)
{
    l_shutdown_d *data = (l_shutdown_d*)req->data;
    lua_rawgeti(data->L, LUA_REGISTRYINDEX, data->callback);
    lua_pushinteger(data->L, status);
    lua_call(data->L, 1, 0);
}

/* Callback called when a stream server has received an incoming connection */
void l_connection_cb(uv_stream_t* server, int status)
{
    l_stream_d *data = (l_stream_d*)server->data;
    lua_rawgeti(data->L, LUA_REGISTRYINDEX, data->on_connection);
    lua_pushinteger(data->L, status);
    lua_call(data->L, 1, 0);
}
