#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


int l_tcp_new(lua_State* L);
int l_tcp_delete(lua_State* L);

/* int uv_tcp_init(uv_loop_t* loop, uv_tcp_t* handle) */
int l_tcp_init(lua_State* L);
/* int uv_tcp_init_ex(uv_loop_t* loop, uv_tcp_t* handle, unsigned int flags) */
int l_tcp_init_ex(lua_State* L);
/* int uv_tcp_open(uv_tcp_t* handle, uv_os_sock_t sock) */
int l_tcp_open(lua_State* L);
/* int uv_tcp_nodelay(uv_tcp_t* handle, int enable) */
int l_tcp_nodelay(lua_State* L);
/* int uv_tcp_keepalive(uv_tcp_t* handle, int enable, unsigned int delay) */
int l_tcp_keepalive(lua_State* L);
/* int uv_tcp_simultaneous_accepts(uv_tcp_t* handle, int enable) */
int l_tcp_simultaneous_accepts(lua_State* L);
/* int uv_tcp_bind(uv_tcp_t* handle, const struct sockaddr* addr, unsigned int flags) */
int l_tcp_bind(lua_State* L);
/* int uv_tcp_getsockname(const uv_tcp_t* handle, struct sockaddr* name, int* namelen) */
int l_tcp_getsockname(lua_State* L);
/* int uv_tcp_getpeername(const uv_tcp_t* handle, struct sockaddr* name, int* namelen) */
int l_tcp_getpeername(lua_State* L);
/* int uv_tcp_connect(uv_connect_t* req, uv_tcp_t* handle, const struct sockaddr* addr, uv_connect_cb cb) */
int l_tcp_connect(lua_State* L);
