#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef struct l_stream_d {
    lua_State* L;
    int on_alloc;
    int on_close;
    int on_connection;
    int on_read;
} l_stream_d;

typedef struct {
    lua_State* L;
    int callback;
    uv_buf_t* bufs;
    size_t nbufs;
} l_write_d;

struct l_request_d {
    lua_State* L;
    int callback;
};

typedef struct l_request_d l_shutdown_d;
typedef struct l_request_d l_connect_d;
typedef struct l_request_d l_connection_d;


/* int uv_shutdown(uv_shutdown_t* req, uv_stream_t* handle, uv_shutdown_cb cb) */
int l_shutdown(lua_State* L);
/* int uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb); */
int l_listen(lua_State* L);
/* int uv_accept(uv_stream_t* server, uv_stream_t* client); */
int l_accept(lua_State* L);
/* int uv_read_start(uv_stream_t* stream, uv_alloc_cb alloc_cb, uv_read_cb read_cb); */
int l_read_start(lua_State* L);
/* int uv_read_stop(uv_stream_t* stream); */
int l_read_stop(lua_State* L);
/* int uv_write(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbuffs, uv_write_cb cb); */
int l_write(lua_State* L);
/* int uv_write2(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs, uv_stream_t* send_handle, uv_write_cb cb) */
int l_write2(lua_State* L);
/* int uv_try_write(uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs) */
int l_try_write(lua_State *L);
/* int uv_is_readable(const uv_stream_t* handle) */
int l_is_readable(lua_State *L);
/* int uv_is_writable(const uv_stream_t* handle) */
int l_is_writable(lua_State *L);
/* int uv_stream_set_blocking(uv_stream_t* handle, int blocking) */
int l_stream_set_blocking(lua_State *L);
/* size_t uv_stream_get_write_queue_size(const uv_stream_t* stream) */
int l_stream_get_write_queue_size(lua_State *L);


void l_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
void l_write_cb(uv_write_t* req, int status);
void l_connect_cb(uv_connect_t* req, int status);
void l_shutdown_cb(uv_shutdown_t* req, int status);
void l_connection_cb(uv_stream_t* server, int status);

