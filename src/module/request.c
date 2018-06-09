#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "request.h"


/* ------------------------------------------------------------------- */
/* Auxiliary                                                           */
/* ------------------------------------------------------------------- */


int l_write_req_new(lua_State* L)
{
    uv_write_t* req = malloc(sizeof(uv_write_t));
    l_write_d* data = malloc(sizeof(l_write_d));
    req->data = data;
    data->L = L;

    lua_pushlightuserdata(L, (void*)req);
    return 1;
}


int l_write_req_delete(lua_State* L)
{
    uv_write_t* req = (uv_write_t*)lua_touserdata(L, 1);
    free(req->data);
    free(req);
    return 0;
}


int l_shutdown_req_new(lua_State* L)
{
    uv_shutdown_t* req = malloc(sizeof(uv_shutdown_t));
    l_shutdown_d* data = malloc(sizeof(l_shutdown_d));
    req->data = data;
    data->L = L;

    lua_pushlightuserdata(L, (void*)req);
    return 1;
}


int l_shutdown_req_delete(lua_State* L)
{
    uv_shutdown_t* req = (uv_shutdown_t*)lua_touserdata(L, 1);
    free(req->data);
    free(req);
    return 0;
}


int l_connect_req_new(lua_State* L)
{
    uv_connect_t* req = malloc(sizeof(uv_connect_t));
    l_connect_d* data = malloc(sizeof(l_connect_d));
    req->data = data;
    data->L = L;

    lua_pushlightuserdata(L, (void*)req);
    return 1;
}


int l_connect_req_delete(lua_State* L)
{
    uv_connect_t* req = (uv_connect_t*)lua_touserdata(L, 1);
    free(req->data);
    free(req);
    return 0;
}


/* ------------------------------------------------------------------- */
/* API                                                                 */
/* ------------------------------------------------------------------- */


/* int uv_cancel(uv_req_t* req) */
int l_req_cancel(lua_State* L)
{
    uv_req_t* req = (uv_req_t*)lua_touserdata(L, 1);
    lua_pushinteger(L, uv_cancel(req));
    return 1;
}


/* size_t uv_req_size(uv_req_type type) */
int l_req_size(lua_State* L)
{
    uv_req_type type = (uv_req_type)lua_tointeger(L, 1);
    lua_pushinteger(L, uv_req_size((uv_req_type)type));
    return 1;
}


/* void* uv_req_get_data(const uv_req_t* req) */
int l_req_get_data(lua_State* L)
{
    
}


/* void* uv_req_set_data(uv_req_t* req) */
int l_req_set_data(lua_State* L)
{

}


/* uv_req_type uv_req_get_type(const uv_req_t* req) */
int l_req_get_type(lua_State* L)
{

}


/* const char* uv_req_type_name(uv_req_type type) */
int l_req_type_name(lua_State* L)
{

}

