#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "functions.h"

int IsCorrectName(char* name) { 
    return !(name == NULL || name == "" || strcmp(name, ".") == 0 || strcmp(name, "..") == 0);
}

char* ReverseStr(char* name) {
    size_t len_str = strlen(name);
    char* reversed_str = malloc((len_str + 1) * sizeof(char));

    for (size_t i = 0; i < len_str; i++) {
        reversed_str[i] = name[len_str - 1 - i];
    } 
    reversed_str[len_str] = 0;
    return reversed_str;
}

char* ConnectTwoStrs(char* str1, char* str2, char spec_symbol) {
    size_t len_str = strlen(str1) + strlen(str2) + 2; // 0 and /
    char* connected_str = malloc((len_str) * sizeof(char));

    snprintf(connected_str, len_str, "%s%c%s", str1, spec_symbol, str2);
    return connected_str;
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
    int size = 50;
    char* content = malloc(size * sizeof(*content)); 
    
    fseek(src, 0, SEEK_END);
    long int length = ftell(src);
    int ptr = length / size; // читаем по "блокам"
    int remainder = length - ptr * size;

    for (int i = 1; i <= ptr; i++) {
        fseek(src, -(size * i), SEEK_END);
        fread(content, 1, size, src);
        ReverseArr(content, size);
        fwrite(content, 1, size, dst);
    }
    fseek(src, 0, SEEK_SET);
    fread(content, 1, remainder, src);
    ReverseArr(content, remainder);
    fwrite(content, 1, remainder, dst);

    free(content);
}

void ReverseFile(char* src_file, char* dst_dir, __mode_t mode) {
    char* short_file_name = GetDirName(src_file);
    if (!IsCorrectName(short_file_name)) {
        printf("The name of file isn't correct.\n");
        return;
    }
    char* reverse_name = ReverseStr(short_file_name);
    char* dst_name = ConnectTwoStrs(dst_dir, reverse_name, '/');

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
    free(reverse_name);
    free(dst_name);
}
