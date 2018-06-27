#ifndef HANDLE_H
#define HANDLE_H

#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef struct {
    lua_State* L;
    int on_alloc;
    int on_close;
} l_handle_d;

/* int uv_is_active(const uv_handle_t* handle) */
int l_is_active(lua_State* L);
/* int uv_is_closing(const uv_handle_t* handle) */
int l_is_closing(lua_State* L);
/* void uv_close(uv_handle_t* handle, uv_close_cb close_cb) */
int l_close(lua_State *L);
/* void uv_ref(uv_handle_t* handle) */
int l_ref(lua_State* L);
/* void uv_unref(uv_handle_t* handle) */
int l_unref(lua_State *L);
/* int uv_has_ref(const uv_handle_t* handle) */
int l_has_ref(lua_State *L);
/* size_t uv_handle_size(uv_handle_type type) */
int l_handle_size(lua_State *L);


/* int uv_send_buffer_size(uv_handle_t* handle, int* value) */
int l_send_buffer_size(lua_State* L);
/* int uv_recv_buffer_size(uv_handle_t* handle, int*value) */
int l_recv_buffer_size(lua_State* L);
/* int uv_fileno(const uv_handle_t* handle, uv_os_fd_t* fd) */
int l_fileno(lua_State* L);
/* uv_loop_t* uv_handle_get_loop(const uv_handle_t* handle) */
int l_handle_get_loop(lua_State* L);
/* uv_handle_type uv_handle_get_type(const uv_handle_t* handle) */
int l_handle_get_type(lua_State *L);


void l_alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void l_close_cb(uv_handle_t* handle);

#endif
