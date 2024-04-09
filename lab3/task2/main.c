#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "utility.h"

static char* GetArgs2(int argc, char** argv) {
    if (argc < 3) {
        printf("Too few arguments. Expected 2.\n");
        exit(-1);
    }
    return argv[2];
}

int main(int argc, char** argv) {
    char* program_name = argv[0];
    program_name = GetDirName(program_name);

    if (argc < 2) { 
        printf("Too few arguments. Expected 2.\n");
        return -1;
    }
    char* arg1 = argv[1];    

    if      (strcmp(program_name, "n_mkdir") == 0)      n_mkdir(arg1, DIR_MODE);
    else if (strcmp(program_name, "n_ls") == 0)         n_ls(arg1);
    else if (strcmp(program_name, "n_rmdir_rec") == 0)  n_rmdir_rec(arg1);
    else if (strcmp(program_name, "n_createfile") == 0)   n_createfile(arg1, FILE_MODE);
    else if (strcmp(program_name, "n_cat") == 0)        n_cat(arg1);
    else if (strcmp(program_name, "n_unlink") == 0)     n_unlink(arg1);
    else if (strcmp(program_name, "n_read_symlink") == 0) n_read_symlink(arg1);
    else if (strcmp(program_name, "n_stat") == 0)       n_stat(arg1);

    else if (strcmp(program_name, "n_symlink") == 0) {
        char* arg2 = GetArgs2(argc, argv);
        n_symlink(arg1, arg2);
    }
    else if (strcmp(program_name, "n_link") == 0) {
        char* arg2 = GetArgs2(argc, argv);
        n_link(arg1, arg2);
    }
    else if (strcmp(program_name, "n_chmod") == 0) {
        char* arg2 = GetArgs2(argc, argv);
        mode_t mode = ParseMode(arg2);
        if (mode == -1) {
            printf("Invalid mode.\n");
            return -1;
        }
        n_chmod(arg1, mode);
    }

    else {
        printf("Unknown command\n");
        return -1;
    }    
}
