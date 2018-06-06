#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

typedef struct {
    lua_State* L;
    int cb;
} l_timer_d;

int l_timer_new(lua_State* L);
int l_timer_delete(lua_State* L);

/* int uv_timer_init(uv_loop_t* loop, uv_timer_t* handle); */
int l_timer_init(lua_State* L);
/* int uv_timer_start(uv_timer_t* handle, uv_timer_cb cb, uint64_t timeout, uint64_t repeat); */
int l_timer_start(lua_State* L);
/* int uv_timer_stop(uv_timer_t* handle); */
int l_timer_stop(lua_State* L);
/* int uv_timer_again(uv_timer_t* handle); */
int l_timer_again(lua_State* L);
/* void uv_timer_set_repeat(uv_timer_t* handle, uint64_t repeat); */
int l_timer_set_repeat(lua_State* L);
/* uint64_t uv_timer_get_repeat(const uv_timer_t* handle); */
int l_timer_get_repeat(lua_State* L);


/* void uv_timer_cb(uv_timer_t* handle); */
void l_timer_cb(uv_timer_t* handle);
