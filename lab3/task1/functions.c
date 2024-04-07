#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "functions.h"

enum {
    SIZE = 50
};

int IsCorrectName(char* name) { 
    return !(name == NULL || name == "" || strcmp(name, ".") == 0 || strcmp(name, "..") == 0);
}

void ReverseStr(char* name, char* reversed_str) {
    for (size_t i = 0; i < strlen(name); i++) {
        reversed_str[i] = name[strlen(name) - 1 - i];
    } 
    reversed_str[strlen(name)] = 0;
}

void ConnectTwoStrs(char* str1, char* str2, char spec_symbol, char* connected_str) {
    size_t len_str = strlen(str1) + strlen(str2) + 2; // 0 and /
    snprintf(connected_str, len_str, "%s%c%s", str1, spec_symbol, str2);
}

char* GetDirName(char* name) {
    char* dir_name = strrchr(name, '/');
    if (dir_name == NULL) {
        return name;
    }
    return dir_name + 1;
}

static void ReverseArr(char* arr, int length) {
    for (int i = 0, j = length - 1; i < j; i++, j--) {
        char tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

static void ReverseContent(FILE* src, FILE* dst) {
    char content[SIZE]; 
    
    fseek(src, 0, SEEK_END);
    long int length = ftell(src);
    int ptr = length / SIZE; // читаем по "блокам"
    int remainder = length - ptr * SIZE;

    for (int i = 1; i <= ptr; i++) {
        fseek(src, -(SIZE * i), SEEK_END);
        fread(content, 1, SIZE, src);
        ReverseArr(content, SIZE);
        fwrite(content, 1, SIZE, dst);
    }
    fseek(src, 0, SEEK_SET);
    fread(content, 1, remainder, src);
    ReverseArr(content, remainder);
    fwrite(content, 1, remainder, dst);
}

void ReverseFile(char* src_file, char* dst_dir, __mode_t mode) {
    char* short_file_name = GetDirName(src_file);
    if (!IsCorrectName(short_file_name)) {
        printf("The name of file isn't correct.\n");
        return;
    }
    char reverse_name[strlen(short_file_name)];
    ReverseStr(short_file_name, reverse_name);
    char dst_name[strlen(dst_dir) + strlen(reverse_name) + 2];
    ConnectTwoStrs(dst_dir, reverse_name, '/', dst_name);

    FILE* file = fopen(src_file, "rb");
    if (file == NULL) {
        printf("File isn't open.\n");
        return;
    }

    FILE* reverse_file = fopen(dst_name, "wb");
    ReverseContent(file, reverse_file);

    fclose(file);
    fclose(reverse_file);
    chmod(dst_name, mode);
}
