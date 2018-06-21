#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "filesystem.h"


/* ------------------------------------------------------------------- */
/* Auxiliary                                                           */
/* ------------------------------------------------------------------- */


int l_fs_req_new(lua_State* L)
{
    uv_fs_t* req = malloc(sizeof(uv_fs_t));
    if (req == NULL) {
        luaL_error(L, "Failed to allocate memory for fs request");
    }
    l_fs_data* data = malloc(sizeof(l_fs_data));
    if (data == NULL) {
        free(req);
        luaL_error(L, "Failed to allocate memory for fs data");
    }
    req->data = data;
    data->L = L;
    data->callback = LUA_NOREF;

    lua_pushlightuserdata(L, (void*)req);
    return 1;
}


int l_fs_req_delete(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    free(req->data);
    free(req);
    return 0;
}


/* ------------------------------------------------------------------- */
/* API                                                                 */
/* ------------------------------------------------------------------- */


/* void uv_fs_req_cleanup(uv_fs_t* req) */
int l_fs_req_cleanup(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    if (data->callback != LUA_NOREF) {
        luaL_unref(L, LUA_REGISTRYINDEX, data->callback);
    }
    uv_fs_req_cleanup(req);
    return 0;
}


/* int uv_fs_close(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb) */
int l_fs_close(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    uv_file file = (uv_file)lua_tointeger(L, 2);
    lua_pushvalue(L, 3);
    int callback = luaL_ref(L, LUA_REGISTRYINDEX);
    l_fs_data* data = (l_fs_data*)req->data;
    data->callback = callback;

    lua_pushinteger(L, uv_fs_close(uv_default_loop(), req, file, l_fs_callback));
    return 1;
}


/* int uv_fs_open(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, int mode, uv_fs_cb cb) */
int l_fs_open(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    const char* path = lua_tostring(L, 2);
    int flags = lua_tointeger(L, 3);
    int mode = lua_tointeger(L, 4);
    lua_pushvalue(L, 5);
    int callback = luaL_ref(L, LUA_REGISTRYINDEX);
    l_fs_data* data = req->data;
    data->callback = callback;

    lua_pushinteger(L, uv_fs_open(uv_default_loop(), req, path, flags, mode , l_fs_callback));
    return 1;
}


/* int uv_fs_read(uv_loop_t* loop,
 *                uv_fs_t* req,
 *                uv_file file,
 *                const uv_buf_t bufs[],
 *                unsigned int nbufs,
 *                int64_t offset,
 *                uv_fs_cb cb) */
int l_fs_read(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    uv_file file = (uv_file)lua_tointeger(L, 2);

    /* For now, just malloc one buffer with 64k bytes and ignore arguments */
    data->bufs = malloc(sizeof(uv_buf_t));
    data->bufs[0].base = malloc(sizeof(char) * 65536);
    data->bufs[0].len = 65536;
    data->nbufs = 1;

    unsigned int nbufs = lua_tointeger(L, 4);
    int64_t offset = lua_tointeger(L, 5);
    lua_pushvalue(L, 6);
    int callback = luaL_ref(L, LUA_REGISTRYINDEX);
    data->callback = callback;

    lua_pushinteger(L, uv_fs_read(uv_default_loop(),
                                  req,
                                  file,
                                  data->bufs,
                                  1,
                                  offset,
                                  l_fs_callback));
    return 1;
}


/* int uv_fs_unlink(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_unlink(lua_State* L)
{
   uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1); 
   l_fs_data* data = (l_fs_data*)req->data;
   const char* path = lua_tostring(L, 2);
   lua_pushvalue(L, 3);
   int callback = luaL_ref(L, LUA_REGISTRYINDEX);
   data->callback = callback;

   lua_pushinteger(L, uv_fs_unlink(uv_default_loop(), req, path, l_fs_callback));
   return 0;
}

/* int uv_fs_write(uv_loop_t* loop,
 *                 uv_fs_t* req,
 *                 uv_file file,
 *                 const uv_buf_t bufs[],
 *                 unsigned int nbufs,
 *                 int64_t offset,
 *                 uv_fs_cb cb) */
int l_fs_write(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    uv_file file = (uv_file)lua_tointeger(L, 2);

    unsigned int nbufs = lua_tointeger(L, 4);
    uv_buf_t *bufs = malloc(sizeof(uv_buf_t) * nbufs);

    data->bufs = bufs;
    data->nbufs = nbufs;

    for (int i = 0; i < nbufs; i++) {
        lua_geti(L, 3, i + 1);

        size_t len;
        const char *buf = lua_tolstring(L, -1, &len);

        bufs[i].len = len;
        bufs[i].base = malloc(sizeof(char) * len);
        memcpy((void*)bufs[i].base, (void*)buf, len);

        lua_pop(L, 1);
    }

    int64_t offset = lua_tointeger(L, 5);
    lua_pushvalue(L, 6);
    data->callback = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_fs_write(uv_default_loop(), req, file, bufs, nbufs, offset, l_fs_callback));
    return 1;
}

/* int uv_fs_mkdir(uv_loop_t* loop,
 *                 uv_fs_t* req,
 *                 const char* path,
 *                 int mode,
 *                 uv_fs_cb cb) */
int l_fs_mkdir(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    const char* path = lua_tostring(L, 2);
    int mode = lua_tointeger(L, 3);
    lua_pushvalue(L, 4);
    data->callback = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_fs_mkdir(uv_default_loop(), req, path, mode, l_fs_callback));
    return 1;
}

/* int uv_fs_mkdtemp(uv_loop_t* loop, uv_fs_t* req, const char* tpl, uv_fs_cb cb) */
int l_fs_mkdtemp(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    const char* tpl = lua_tostring(L, 2);
    lua_pushvalue(L, 3);
    data->callback = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_fs_mkdtemp(uv_default_loop(), req, tpl, l_fs_callback));
    return 1;
}

/* int uv_fs_rmdir(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_rmdir(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    const char* path = lua_tostring(L, 2);
    lua_pushvalue(L, 3);
    data->callback = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_fs_rmdir(uv_default_loop(), req, path, l_fs_callback));
    return 1;
}

/* int uv_fs_scandir(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, uv_fs_cb cb) */
int l_fs_scandir(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    const char* path = lua_tostring(L, 2);
    int flags = lua_tointeger(L, 3);
    lua_pushvalue(L, 4);
    data->callback = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_fs_scandir(uv_default_loop(), req, path, flags, l_fs_callback));
    return 1;
}

static const char* dirent_to_str(uv_dirent_t ent);

/* int uv_fs_scandir_next(uv_fs_t* req, uv_dirent_t* ent) */
int l_fs_scandir_next(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    uv_dirent_t ent;
    int err = uv_fs_scandir_next(req, &ent);
    lua_pushinteger(L, err);
    if (err == UV_EOF) {
        lua_pushnil(L);
    }
    else {
        lua_newtable(L);
        lua_pushstring(L, dirent_to_str(ent));
        lua_setfield(L, -2, "type");
        lua_pushstring(L, ent.name);
        lua_setfield(L, -2, "name");
    }

    return 2;
}

static const char* dirent_to_str(uv_dirent_t ent)
{
    switch(ent.type) {
        case UV_DIRENT_UNKNOWN:
            return "UV_DIRENT_UNKNOWN";
            break;

        case UV_DIRENT_FILE:
            return "UV_DIRENT_FILE";
            break;

        case UV_DIRENT_DIR:
            return "UV_DIRENT_DIR";
            break;

        case UV_DIRENT_LINK:
            return "UV_DIRENT_LINK";
            break;

        case UV_DIRENT_FIFO:
            return "UV_DIRENT_FIFO";
            break;

        case UV_DIRENT_SOCKET:
            return "UV_DIRENT_SOCKET";
            break;

        case UV_DIRENT_CHAR:
            return "UV_DIRENT_CHAR";
            break;

        case UV_DIRENT_BLOCK:
            return "UV_DIRENT_BLOCK";
            break;
    }
}

/* int uv_fs_stat(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_stat(lua_State* L);
/* int uv_fs_fstat(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb) */
int l_fs_fstat(lua_State* L);
/* int uv_fs_lstat(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_lstat(lua_State* L);

/* int uv_fs_rename(uv_loop_t* loop,
 *                  uv_fs_t* req,
 *                  const char* path,
 *                  const char* new_path,
 *                  uv_fs_cb cb) */
int l_fs_rename(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    const char* path = lua_tostring(L, 2);
    const char* new_path = lua_tostring(L, 3);
    lua_pushvalue(L, 4);
    data->callback = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_fs_rename(uv_default_loop(), req, path, new_path, l_fs_callback));
    return 1;
}

/* int uv_fs_fsync(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb) */
int l_fs_fsync(lua_State* L);
/* int uv_fs_fdatasync(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb) */
int l_fs_fdatasync(lua_State* L);
/* int uv_fs_ftruncate(uv_loop_t* loop, uv_fs_t* req, uv_file file, int64_t offset, uv_fs_cb cb) */
int l_fs_ftruncate(lua_State* L);

/* int uv_fs_copyfile(uv_loop_t* loop,
 *                    uv_fs_t* req,
 *                    const char* path,
 *                    const char* new_path,
 *                    int flags,
 *                    uv_fs_cb cb) */
int l_fs_copyfile(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    const char* path = lua_tostring(L, 2);
    const char* new_path = lua_tostring(L, 3);
    int flags = lua_tointeger(L, 4);
    lua_pushvalue(L, 5);
    data->callback = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_fs_copyfile(uv_default_loop(), req, path, new_path, flags, l_fs_callback));
    return 1;
}

/* int uv_fs_sendfile(uv_loop_t* loop, uv_fs_t* req, uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length, uv_fs_cb cb) */
int l_fs_sendfile(lua_State* L);
/* nt uv_fs_access(uv_loop_t* loop, uv_fs_t* req, const char* path, int mode, uv_fs_cb cb) */
int l_fs_access(lua_State* L);

/* int uv_fs_chmod(uv_loop_t* loop, uv_fs_t* req, const char* path, int mode, uv_fs_cb cb) */
int l_fs_chmod(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    const char* path = lua_tostring(L, 2);
    int mode = lua_tointeger(L, 3);
    lua_pushvalue(L, 4);
    data->callback = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_fs_chmod(uv_default_loop(), req, path, mode, l_fs_callback));
    return 1;
}

/* int uv_fs_fchmod(uv_loop_t* loop, uv_fs_t* req, uv_file file, int mode, uv_fs_cb cb) */
int l_fs_fchmod(lua_State* L)
{
    uv_fs_t* req = (uv_fs_t*)lua_touserdata(L, 1);
    l_fs_data* data = (l_fs_data*)req->data;
    uv_file file = lua_tointeger(L, 2);
    int mode = lua_tointeger(L, 3);
    lua_pushvalue(L, 4);
    data->callback = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_pushinteger(L, uv_fs_fchmod(uv_default_loop(), req, file, mode, l_fs_callback));
    return 1;
}

/* int uv_fs_utime(uv_loop_t* loop, uv_fs_t* req, const char* path, double atime, double mtime, uv_fs_cb cb) */
int l_fs_utime(lua_State* L);
/* int uv_fs_futime(uv_loop_t* loop, uv_fs_t* req, uv_file file, double atime, double mtime, uv_fs_cb cb) */
int l_fs_futime(lua_State* L);
/* int uv_fs_link(uv_loop_t* loop, uv_fs_t* req, const char* path, const char* new_path, uv_fs_cb cb) */
int l_fs_link(lua_State* L);
/* int uv_fs_symlink(uv_loop_t* loop, uv_fs_t* req, const char* path, const char* new_path, int flags, uv_fs_cb cb) */
int l_fs_symlink(lua_State* L);
/* int uv_fs_readlink(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_readlink(lua_State* L);
/* int uv_fs_realpath(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_realpath(lua_State* L);
/* int uv_fs_chown(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_uid_t uid, uv_gid_t gid, uv_fs_cb cb) */
int l_fs_chown(lua_State* L);
/* int uv_fs_fchown(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_uid_t uid, uv_gid_t gid, uv_fs_cb cb) */
int l_fs_fchown(lua_State* L);

/* uv_fs_type uv_fs_get_type(const uv_fs_t* req) */
int l_fs_get_type(lua_State* L);
/* ssize_t uv_fs_get_result(const uv_fs_t* req) */
int l_fs_get_result(lua_State* L);
/* void* uv_fs_get_ptr(const uv_fs_t* req) */
int l_fs_get_ptr(lua_State* L);
/* const char* uv_fs_get_path(const uv_fs_t* req) */
int l_fs_get_path(lua_State* L);
/* uv_stat_t* uv_fs_get_statbuf(uv_fs_t* req) */
int l_fs_get_statbuf(lua_State* L);


/* ------------------------------------------------------------------- */
/* Callbacks                                                           */
/* ------------------------------------------------------------------- */


void l_fs_callback(uv_fs_t* req)
{
    l_fs_data* data = (l_fs_data*)req->data;
    lua_rawgeti(data->L, LUA_REGISTRYINDEX, data->callback);
    lua_pushlightuserdata(data->L, (void*)req);

    int args = 1;
    switch (req->fs_type) {
        case UV_FS_READ:
            lua_pushlstring(data->L, data->bufs[0].base, data->bufs[0].len);
            lua_pushinteger(data->L, req->result);
            args += 2;
            break;

        case UV_FS_CHOWN:
        case UV_FS_COPYFILE:
        case UV_FS_RENAME:
        case UV_FS_SCANDIR:
        case UV_FS_RMDIR:
        case UV_FS_MKDTEMP:
        case UV_FS_MKDIR:
        case UV_FS_UNLINK:
        case UV_FS_WRITE:
        case UV_FS_OPEN:
            lua_pushinteger(data->L, req->result);
            args++;
            break;
    }

    lua_call(data->L, args, 0);

    switch (req->fs_type) {
        case UV_FS_WRITE:
        case UV_FS_READ:
            for (size_t i = 0; i < data->nbufs; i++) {
                free(data->bufs[i].base);
            }
            free(data->bufs);
            break;
    }
}
