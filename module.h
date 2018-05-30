#include <stdio.h>
#include <stdlib.h>

#include <uv.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/* ------------------------------------------------------ */
/* uv_handle_t                                            */
/* ------------------------------------------------------ */

struct l_uv_handle_data {
    int on_alloc_ref;
    int on_close_ref;
    lua_State* L;
};

int l_uv_is_active(lua_State* L);
int l_uv_is_closing(lua_State* L);
int l_uv_close(lua_State *L);
int l_uv_ref(lua_State* L);
int l_uv_unref(lua_State *L);
int l_uv_has_ref(lua_State *L);
int l_uv_handle_size(lua_State *L);

int l_uv_send_buffer_size(lua_State* L);
int l_uv_handle_get_type(lua_State *L);

void l_uv_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void l_uv_close_cb(uv_handle_t* handle);

/* ------------------------------------------------------ */
/* uv_stream_t                                            */
/* ------------------------------------------------------ */

struct l_uv_stream_data {
    int connection_cb_ref;
    int on_alloc_cb_ref;
    int read_cb_ref;
    lua_State *L;
};

/* int uv_shutdown(uv_shutdown_t* req, uv_stream_t* handle, uv_shutdown_cb cb) */
int l_uv_shutdown(lua_State* L);

/* int uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb); */
int l_uv_listen(lua_State* L);

/* int uv_accept(uv_stream_t* server, uv_stream_t* client); */
int l_uv_accept(lua_State* L);

/* int uv_read_start(uv_stream_t* stream, uv_alloc_cb alloc_cb, uv_read_cb read_cb); */
int l_uv_read_start(lua_State* L);

/* int uv_read_stop(uv_stream_t* stream); */
int l_uv_read_stop(lua_State* L);

/* int uv_write(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbuffs, uv_write_cb cb); */
int l_uv_write(lua_State* L);

void l_uv_read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
void l_uv_write_cb(uv_write_t* req, int status);
void l_uv_connect_cb(uv_connect_t* req, int status);
void l_uv_shutdown_cb(uv_shutdown_t* req, int status);
void l_uv_connection_cb(uv_stream_t* server, int status);

/* ------------------------------------------------------ */
/* uv_timer_t                                             */
/* ------------------------------------------------------ */

struct l_uv_timer_data {
    lua_State* L;
    int cb_ref;
};

/* int uv_timer_init(uv_loop_t* loop, uv_timer_t* handle); */
int l_uv_timer_init(lua_State* L);

/* int uv_timer_start(uv_timer_t* handle, uv_timer_cb cb, uint64_t timeout, uint64_t repeat); */
int l_uv_timer_start(lua_State* L);

/* int uv_timer_stop(uv_timer_t* handle); */
int l_uv_timer_stop(lua_State* L);

/* int uv_timer_again(uv_timer_t* handle); */
int l_uv_timer_again(lua_State* L);

/* void uv_timer_set_repeat(uv_timer_t* handle, uint64_t repeat); */
int l_uv_timer_set_repeat(lua_State* L);

/* uint64_t uv_timer_get_repeat(const uv_timer_t* handle); */
int l_uv_timer_get_repeat(const uv_timer_t* handle);

/* void uv_timer_cb(uv_timer_t* handle); */
void l_uv_timer_cb(uv_timer_t* handle);

