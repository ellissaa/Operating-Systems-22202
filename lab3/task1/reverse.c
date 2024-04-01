#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include "functions.h"

void ReverseDir(char* orig_dir_name, char* dst_dir, __mode_t mode) {
    DIR* dirstream = opendir(orig_dir_name);
    if (dirstream == NULL) {
        printf("Can't open directory witn name %s\n", orig_dir_name);
        return;
    }

    if (orig_dir_name[strlen(orig_dir_name) - 1] == '/') {
        orig_dir_name[strlen(orig_dir_name) - 1] = 0;
    }
    
    char* dir_name = GetDirName(orig_dir_name);
    if (!IsCorrectName(dir_name)) {
        printf("The name of directory isn't correct.\n");
        return;
    }

    char* reverse_dir = ReverseStr(dir_name);
    char* next_dst_dir = ConnectTwoStrs(dst_dir, reverse_dir, '/'); 

    mkdir(next_dst_dir, mode);

    struct dirent* content; // чтобы смотреть, что лежит в директории
    while ((content = readdir(dirstream)) != NULL) {
        if (!IsCorrectName(content->d_name)) {
            continue;
        }

        char* full_cont_name = ConnectTwoStrs(orig_dir_name, content->d_name, '/'); // полное имя файла директории
        struct stat cont_inform;
        stat(full_cont_name, &cont_inform);

        if (S_ISDIR(cont_inform.st_mode)) {
            ReverseDir(full_cont_name, next_dst_dir, cont_inform.st_mode);
        }
        else if (S_ISREG(cont_inform.st_mode)) {
            ReverseFile(full_cont_name, next_dst_dir, cont_inform.st_mode);
        }
        free(full_cont_name);
    }
    free(reverse_dir);
    free(next_dst_dir);
    closedir(dirstream);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Wrong name of directory.\n");
        return -1;
    }

    struct stat dir_inform;
    stat(argv[1], &dir_inform); // man 2 stat
    ReverseDir(argv[1], ".", dir_inform.st_mode);
}