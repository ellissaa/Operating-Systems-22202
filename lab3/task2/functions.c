#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"
#include "utility.h"

enum {
    SIZE = 4096 + 255
};

void n_mkdir(const char* name, mode_t mode) { // создание директории
    if (mkdir(name, mode) == -1) {
        perror("n_mkdir failed.");
        return;
    }
    chmod(name, mode);
} 

void n_ls(const char* name) { // вывести содержимое каталога, указанного в аргументе
    DIR* stream = opendir(name);
    if (stream == NULL) {
        perror("n_ls failed.");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(stream)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(stream);
} 

void n_rmdir_rec(const char* name) { // удалить каталог, указанный в аргументе
    DIR* stream = opendir(name);
    if (stream == NULL) {
        perror("n_rmdir_rec failed");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(stream)) != NULL) {
        if (IsCorrectName(entry->d_name)) {
            continue;
        }

        char entry_name[strlen(name) + strlen(entry->d_name) + 2];
        if (name[strlen(name) - 1] == '/') {
            Concat(name, entry->d_name, entry_name);
        } else {
            ConnectTwoStrs(name, entry->d_name, '/', entry_name);
            break;
        }

        struct stat entry_info;
        stat(entry_name, &entry_info);

        if (S_ISDIR(entry_info.st_mode)) {
            n_rmdir_rec(entry_name);
        } else {
            unlink(entry_name);
        }
    }

    closedir(stream);
    if (rmdir(name) == -1) {
        perror("n_rmdir_rec failed");
    }
} 

void n_createfile(const char* filename, mode_t mode) {  // создать файл, указанный в аргументе
    if (open(filename, O_RDONLY | O_CREAT | O_EXCL, mode) == -1) {
        perror("n_createfile failed.");
        return;
    }
    chmod(filename, mode);
}

void n_cat(const char* filename) { // вывести содержимое файла, указанного в аргументе
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("n_cat failed.");
        return;
    }
    PrintFile(file);
    fclose(file);
}

void n_unlink(const char* filename) { // удаление файла (жесткой ссылки)
    if (unlink(filename) == -1) {
        perror("n_unlink failed.");
        return;
    }
}

void n_symlink(const char* target, const char* link) { // создать символьную ссылку на файл, указанный в аргументе
    if (symlink(target, link) == -1) {
        perror("n_unlink failed.");
        return;
    }
}

void n_read_symlink(const char* link) { // вывести содержимое символьной ссылки, указанный в аргументе
    char arr[SIZE];
    int bytes_read = readlink(link, arr, sizeof(arr) - 1);

    if (bytes_read == -1) {
        perror("n_read_symlink failed.");
        return;
    }
    arr[bytes_read] = 0;
    printf("%s\n", arr);
}

void n_link(const char* old, const char* new) { //  создать жесткую ссылку на файл, указанный в аргументе
    if (link(old, new) == -1) {
        perror("n_link failed.");
        return;
    }
}

void n_stat(const char* filename) { // вывести права доступа к файлу и количество жестких ссылок на него
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("n_stat failed.");
        return;
    }

    char mode_arr[12];
    strmode(file_stat.st_mode, mode_arr);
    printf("%s%ld\n", mode_arr, file_stat.st_nlink);
}

void n_chmod(const char* name, mode_t mode) { // изменить права доступа к файлу, указанному в аргументе
    if (chmod(name, mode) == -1) {
        perror("n_chmod failed.");
        return;
    }
}
