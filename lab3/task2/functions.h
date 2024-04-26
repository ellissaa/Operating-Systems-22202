#ifndef N_FUNCTIONS_H
#define N_FUNCTIONS_H

#define DIR_MODE 0755 // дефолтные значения для прав
#define FILE_MODE 644

#include <fcntl.h>

void n_mkdir(const char* name, mode_t mode); // создание директории

void n_ls(const char* name); // вывести содержимое каталога, указанного в аргументе

void n_rmdir_rec(const char* name); // удалить каталог, указанный в аргументе

void n_createfile(const char* filename, mode_t mode); // создать файл, указанный в аргументе

void n_cat(const char* filename); // вывести содержимое файла, указанного в аргументе

void n_unlink(const char* filename); // удаление файла (жесткой ссылки)

void n_symlink(const char* target, const char* link); // создать символьную ссылку на файл, указанный в аргументе

void n_read_symlink(const char* link); // вывести содержимое символьной ссылки, указанный в аргументе

void n_link(const char* oldpath, const char* newpath); //  создать жесткую ссылку на файл, указанный в аргументе

void n_stat(const char* filename); // вывести права доступа к файлу и количество жестких ссылок на него

void n_chmod(const char* name, mode_t mode); // изменить права доступа к файлу, указанному в аргументе

#endif