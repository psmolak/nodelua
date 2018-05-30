#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include <uv.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "module.h"

/* ------------------------------------------------------ */
/* uv_handle_t                                            */
/* ------------------------------------------------------ */

/* int uv_is_active(const uv_handle_t* handle) */
int l_uv_is_active(lua_State* L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_is_active(handle));

    return 1;
}

/* int uv_is_closing(const uv_handle_t* handle) */
int l_uv_is_closing(lua_State* L)
{
    uv_handle_t* handle =  (uv_handle_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_is_closing(handle));

    return 1;
}

/* void uv_close(uv_handle_t* handle, uv_close_cb close_cb) */
int l_uv_close(lua_State *L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    uv_close(handle, l_uv_close_cb);

    return 0;
}

/* void uv_ref(uv_handle_t* handle) */
int l_uv_ref(lua_State* L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    uv_ref(handle);

    return 0;
}

/* void uv_unref(uv_handle_t* handle) */
int l_uv_unref(lua_State *L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    uv_unref(handle);

    return 0;
}

/* int uv_has_ref(const uv_handle_t* handle) */
int l_uv_has_ref(lua_State *L)
{
    uv_handle_t* handle = (uv_handle_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_has_ref(handle));

    return 1;
}

/* size_t uv_handle_size(uv_handle_type type) */
int l_uv_handle_size(lua_State *L)
{
    uv_handle_type type = (uv_handle_type)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_handle_size(type));

    return 1;
}

/* uv_handle_type uv_handle_get_type(const uv_handle_t* handle); */
int l_uv_handle_get_type(lua_State* L)
{
    uv_handle_t *handle = (uv_handle_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, (int)uv_handle_get_type(handle));

    return 1;
}

/* int uv_send_buffer_size(uv_handle_t* handle, int* value) */
int l_uv_send_buffer_size(lua_State* L)
{

}

void l_uv_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{

}

void l_uv_close_cb(uv_handle_t* handle)
{
    struct l_uv_handle_data *data = (struct l_uv_handle_data*)handle->data;
    lua_State *L = data->L;
    int callback_ref = data->on_close_ref;

    /* assumes lua callback somehow knows what handler called it */
    lua_rawgeti(L, LUA_REGISTRYINDEX, callback_ref);
    lua_pcall(L, 0, 0, 0);
    luaL_unref(L, LUA_REGISTRYINDEX, callback_ref);
}

/* ------------------------------------------------------ */
/* uv_stream_t                                            */
/* ------------------------------------------------------ */

/* int uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb); */
int l_uv_listen(lua_State* L)
{
    uv_stream_t* stream = (uv_stream_t*)lua_touserdata(L, 1);
    int backlog = lua_tointeger(L, 2);
    lua_pushvalue(L, 3);
    int cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    struct l_uv_stream_data *data = (struct l_uv_stream_data*)stream->data;
    data->connection_cb_ref = cb_ref;

    lua_pushinteger(L, uv_listen(stream, backlog, l_uv_connection_cb));
    return 1;
}

/* int uv_accept(uv_stream_t* server, uv_stream_t* client); */
int l_uv_accept(lua_State* L)
{
    uv_stream_t* server = (uv_stream_t*)lua_touserdata(L, 1);
    uv_stream_t* client = (uv_stream_t*)lua_touserdata(L, 2);

    lua_pushinteger(L, uv_accept(server, client));
    return 1;
}

/* int uv_read_start(uv_stream_t* stream, uv_alloc_cb alloc_cb, uv_read_cb read_cb); */
int l_uv_read_start(lua_State* L)
{
    uv_stream_t* stream = (uv_stream_t*)lua_touserdata(L, 1);
    lua_pushvalue(L, 2);
    int on_alloc_cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_pushvalue(L, 3);
    int read_cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    struct l_uv_stream_data *data = (struct l_uv_stream_data*)stream->data;
    data->on_alloc_cb_ref = on_alloc_cb_ref;
    data->read_cb_ref = read_cb_ref;

    lua_pushinteger(L, uv_read_start(stream, l_uv_alloc_cb, l_uv_read_cb));
    return 1;
}

/* int uv_read_stop(uv_stream_t* stream); */
int l_uv_read_stop(lua_State* L)
{

}

/* int uv_write(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbuffs, uv_write_cb cb); */
int l_uv_write(lua_State* L)
{
    uv_write_t* req = (uv_write_t*)lua_touserdata(L, 1);
    uv_stream_t* handle = (uv_stream_t*)lua_touserdata(L, 2);
    const uv_buf_t *bufs = (uv_buf_t*)lua_touserdata(L, 3);
    unsigned int nbuffs = lua_tointeger(L, 4);
    lua_pushvalue(L, 5);
    int write_cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_write(req, handle, bufs, nbuffs, l_uv_write_cb));

    return 1;
}

void l_uv_connection_cb(uv_stream_t* server, int status)
{
    struct l_uv_stream_data *data = (struct l_uv_stream_data*)server->data;
    lua_rawgeti(data->L, LUA_REGISTRYINDEX, data->connection_cb_ref);
    lua_pushinteger(data->L, status);
    lua_call(data->L, 1, 0);
}

void l_uv_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    struct l_uv_stream_data *data = (struct l_uv_stream_data*)stream->data;
    lua_rawgeti(data->L, LUA_REGISTRYINDEX, data->read_cb_ref);
    lua_pushinteger(data->L, nread);

    /* Now i should push those as table of strings or userdata */
    lua_pushlightuserdata(data->L, (void*)buf);
    lua_call(data->L, 2, 0);
}

void l_uv_write_cb(uv_write_t* req, int status) {}

/* ------------------------------------------------------ */
/* uv_timer_t                                             */
/* ------------------------------------------------------ */

/* int uv_timer_init(uv_loop_t* loop, uv_timer_t* handle); */
int l_uv_timer_init(lua_State* L)
{
    uv_timer_t* handle = lua_newuserdata(L, sizeof(uv_timer_t) + sizeof(struct l_uv_timer_data));
    lua_pushinteger(L, uv_timer_init(uv_default_loop(), handle));
    handle->data = (void*)((char*)handle + sizeof(uv_timer_t));

    struct l_uv_timer_data *data = (struct l_uv_timer_data*)handle->data;
    data->L = L;

    return 2;
}

/* int uv_timer_start(uv_timer_t* handle, uv_timer_cb cb, uint64_t timeout, uint64_t repeat); */
int l_uv_timer_start(lua_State* L)
{
    uv_timer_t* handle = (uv_timer_t*)lua_touserdata(L, 1);
    lua_pushvalue(L, 2);
    int cb_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    uint64_t timeout = lua_tonumber(L, 3);
    uint64_t repeat = lua_tonumber(L, 4);

    struct l_uv_timer_data *data = (struct l_uv_timer_data*)handle->data;
    data->cb_ref = cb_ref;

    lua_pushinteger(L, uv_timer_start(handle, l_uv_timer_cb, timeout, repeat));

    return 1;
}

/* void uv_timer_cb(uv_timer_t* handle); */
void l_uv_timer_cb(uv_timer_t* handle)
{
    struct l_uv_timer_data *data = (struct l_uv_timer_data*)handle->data;
    lua_rawgeti(data->L, LUA_REGISTRYINDEX, data->cb_ref);
    lua_call(data->L, 0, 0);
}

