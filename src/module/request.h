#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


struct l_request_d {
    lua_State* L;
    int callback;
};

struct l_write_d {
    /* l_request_d */
    lua_State* L;
    int callback;

    uv_buf_t* bufs;
    size_t nbufs;
};

typedef struct l_request_d l_request_d;
typedef struct l_request_d l_shutdown_d;
typedef struct l_request_d l_connect_d;
typedef struct l_request_d l_connection_d;
typedef struct l_write_d l_write_d;

int l_write_req_new(lua_State* L);
int l_write_req_delete(lua_State* L);

int l_shutdown_req_new(lua_State* L);
int l_shutdown_req_delete(lua_State* L);

int l_connect_req_new(lua_State* L);
int l_connect_req_delete(lua_State* L);

/* int uv_cancel(uv_req_t* req) */
int l_req_cancel(lua_State* L);
/* size_t uv_req_size(uv_req_type type) */
int l_req_size(lua_State* L);
/* void* uv_req_get_data(const uv_req_t* req) */
int l_req_get_data(lua_State* L);
/* void* uv_req_set_data(uv_req_t* req) */
int l_req_set_data(lua_State* L);
/* uv_req_type uv_req_get_type(const uv_req_t* req) */
int l_req_get_type(lua_State* L);
/* const char* uv_req_type_name(uv_req_type type) */
int l_req_type_name(lua_State* L);

