#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>

#include <uv.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "request.h"

struct l_fs_data {
    lua_State* L;
    int callback;

    uv_buf_t* bufs;
    size_t nbufs;
};

typedef struct l_fs_data l_fs_data;

int l_fs_req_new(lua_State* L);
int l_fs_req_delete(lua_State* L);

/* void uv_fs_req_cleanup(uv_fs_t* req) */
int l_fs_req_cleanup(lua_State* L);
/* int uv_fs_close(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb) */
int l_fs_close(lua_State* L);
/* int uv_fs_open(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, int mode, uv_fs_cb cb) */
int l_fs_open(lua_State* L);
/* int uv_fs_read(uv_loop_t* loop, uv_fs_t* req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb) */
int l_fs_read(lua_State* L);
/* int uv_fs_unlink(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_unlink(lua_State* L);
/* int uv_fs_write(uv_loop_t* loop, uv_fs_t* req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb) */
int l_fs_write(lua_State* L);
/* int uv_fs_mkdir(uv_loop_t* loop, uv_fs_t* req, const char* path, int mode, uv_fs_cb cb) */
int l_fs_mkdir(lua_State* L);
/* int uv_fs_mkdtemp(uv_loop_t* loop, uv_fs_t* req, const char* tpl, uv_fs_cb cb) */
int l_fs_mkdtemp(lua_State* L);
/* int uv_fs_rmdir(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_rmdir(lua_State* L);
/* int uv_fs_scandir(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, uv_fs_cb cb) */
int l_fs_scandir(lua_State* L);
/* int uv_fs_scandir_next(uv_fs_t* req, uv_dirent_t* ent) */
int l_fs_scandir_next(lua_State* L);
/* int uv_fs_stat(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_stat(lua_State* L);
/* int uv_fs_fstat(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb) */
int l_fs_fstat(lua_State* L);
/* int uv_fs_lstat(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) */
int l_fs_lstat(lua_State* L);
/* int uv_fs_rename(uv_loop_t* loop, uv_fs_t* req, const char* path, const char* new_path, uv_fs_cb cb) */
int l_fs_rename(lua_State* L);
/* int uv_fs_fsync(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb) */
int l_fs_fsync(lua_State* L);
/* int uv_fs_fdatasync(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_fs_cb cb) */
int l_fs_fdatasync(lua_State* L);
/* int uv_fs_ftruncate(uv_loop_t* loop, uv_fs_t* req, uv_file file, int64_t offset, uv_fs_cb cb) */
int l_fs_ftruncate(lua_State* L);
/* int uv_fs_copyfile(uv_loop_t* loop, uv_fs_t* req, const char* path, const char* new_path, int flags, uv_fs_cb cb) */
int l_fs_copyfile(lua_State* L);
/* int uv_fs_sendfile(uv_loop_t* loop, uv_fs_t* req, uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length, uv_fs_cb cb) */
int l_fs_sendfile(lua_State* L);
/* nt uv_fs_access(uv_loop_t* loop, uv_fs_t* req, const char* path, int mode, uv_fs_cb cb) */
int l_fs_access(lua_State* L);
/* int uv_fs_chmod(uv_loop_t* loop, uv_fs_t* req, const char* path, int mode, uv_fs_cb cb) */
int l_fs_chmod(lua_State* L);
/* int uv_fs_fchmod(uv_loop_t* loop, uv_fs_t* req, uv_file file, int mode, uv_fs_cb cb) */
int l_fs_fchmod(lua_State* L);
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

void l_fs_callback(uv_fs_t* req);

#endif
