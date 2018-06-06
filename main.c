#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "module/handle.h"
#include "module/stream.h"
#include "module/timer.h"
#include "module/tcp.h"


struct nodelua_s {
    lua_State *L;
    uv_loop_t* loop;
};

typedef struct nodelua_s nodelua_t;

void nodelua_init(nodelua_t* node);
void nodelua_openlibs(lua_State* L);
void nodelua_run(nodelua_t* node, const char* file_name);

void nodelua_init(nodelua_t* node)
{
    node->L = luaL_newstate();
    luaL_openlibs(node->L);
    nodelua_openlibs(node->L);
    node->loop = uv_default_loop();
}

void nodelua_openlibs(lua_State* L)
{
    const struct luaL_Reg functions[] = {

        /* uv_handle_t */
        { "is_active", l_is_active },
        { "is_closing", l_is_closing },
        { "close", l_close },
        { "ref", l_ref },
        { "unref", l_unref },
        { "has_ref", l_has_ref },
        { "handle_size", l_handle_size },
        /* { "send_buffer_size", l_send_buffer_size }, */
        /* { "recv_buffer_size", l_recv_buffer_size }, */
        /* { "fileno", l_fileno }, */
        /* { "handle_get_loop", l_handle_get_loop }, */
        { "handle_get_type", l_handle_get_type },

        /* uv_stream_t */
        { "shutdown", l_shutdown },
        { "listen", l_listen },
        { "accept", l_accept },
        { "read_start", l_read_start },
        { "read_stop", l_read_stop },
        { "write", l_write },
        /* { "write2", l_write2 }, */
        /* { "try_write", l_try_write }, */
        { "is_readable", l_is_readable },
        { "is_writable", l_is_writable },
        { "stream_set_blocking", l_stream_set_blocking },
        { "stream_get_write_queue_size", l_stream_get_write_queue_size },

        /* uv_tcp_t */
        { "tcp_new", l_tcp_new },
        { "tcp_delete", l_tcp_delete },
        { "tcp_init", l_tcp_init },
        { "tcp_init_ex", l_tcp_init_ex },
        { "tcp_open", l_tcp_open },
        { "tcp_nodelay", l_tcp_nodelay },
        { "tcp_keepalive", l_tcp_keepalive },
        { "tcp_simultaneous_accepts", l_tcp_simultaneous_accepts },
        { "tcp_bind", l_tcp_bind },
        /* { "tcp_getsockname", l_tcp_getsockname }, */
        /* { "tcp_getpeername", l_tcp_getpeername }, */
        { "tcp_connect", l_tcp_connect },

        /* uv_timer_t */
        { "timer_new", l_timer_new },
        { "timer_delete", l_timer_delete },
        { "timer_init", l_timer_init },
        { "timer_start", l_timer_start },
        { "timer_stop", l_timer_stop },
        /* { "timer_again", l_timer_again }, */
        /* { "timer_set_repeat", l_timer_set_repeat }, */
        /* { "timer_get_repeat", l_timer_get_repeat }, */

        { NULL, NULL }
    };

    luaL_newlib(L, functions);
    lua_setglobal(L, "uv");
}

void nodelua_run(nodelua_t* node, const char* file_name)
{
    if (luaL_loadfile(node->L, file_name) || lua_pcall(node->L, 0, 0, 0)) {
        fprintf(stderr, "Error occured while reading script file: %s\n", file_name);
    }

    uv_run(node->loop, UV_RUN_DEFAULT);
}

int main(void)
{
    nodelua_t node;
    nodelua_init(&node);
    nodelua_run(&node, "script.lua");

    return 0;
}
