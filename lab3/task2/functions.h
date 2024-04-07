#ifndef N_FUNCTIONS_H
#define N_FUNCTIONS_H

#define DIR_MODE 0755
#define FILE_MODE 644

#include <fcntl.h>

void n_mkdir(const char* pathname, mode_t mode);

void n_ls(const char* pathname);

void n_rmdir_rec(const char* pathname);

void n_createfile(const char* filename, mode_t mode);

void n_cat(const char* filename);

void n_unlink(const char* filename); // удаление файла (жесткой ссылки)

void n_readlink(const char* link);

void n_symlink(const char* target, const char* link);

void n_link(const char* oldpath, const char* newpath);

void n_stat(const char* filename);

void n_chmod(const char* pathname, mode_t mode);

#endif