#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include "functions.h"

enum {
    SIZE = 100
};

int IsCorrectName(char* name) { 
    return !(name == NULL || name == "" || strcmp(name, "./") == 0 || strcmp(name, "../") == 0);
}

void ConnectTwoStrs(char* str1, char* str2, char spec_symbol, char* connected_str) {
    size_t len_str = strlen(str1) + strlen(str2) + 2; // 0 and /
    snprintf(connected_str, len_str, "%s%c%s", str1, spec_symbol, str2);
    printf("%s %s\n", str1, str2);
}

char* GetShortName(char* name) {
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

void Concat(const char* str1, const char* str2, char* res) {
    size_t res_len = strlen(str1) + strlen(str2) + 1;
    snprintf(res, res_len, "%s%s", str1, str2);
}

void PrintFile(FILE* file) {
    char arr[SIZE];

    while (!feof(file)) { 
        int bytes_read = fread(arr, 1, SIZE, file);
        int bytes_write = 0;

        while (bytes_write < bytes_read) {
            bytes_write += fwrite(arr + bytes_write, 1, bytes_read, stdout);
            printf("\n");
        }
        fflush(stdout);
    }
}    

static char ftypelet(mode_t bits) {
    if (S_ISREG(bits))
    return '-';
    if (S_ISDIR(bits))
    return 'd';

    if (S_ISBLK(bits))
    return 'b';
    if (S_ISCHR(bits))
    return 'c';
    if (S_ISLNK(bits))
    return 'l';
    if (S_ISFIFO(bits))
    return 'p';
    if (S_ISSOCK(bits))
    return 's';

    return '?';
}

void strmode(mode_t mode, char *str) {
    str[0] = ftypelet(mode);
    str[1] = mode & S_IRUSR ? 'r' : '-';
    str[2] = mode & S_IWUSR ? 'w' : '-';
    str[3] = (mode & S_ISUID
            ? (mode & S_IXUSR ? 's' : 'S')
            : (mode & S_IXUSR ? 'x' : '-'));
    str[4] = mode & S_IRGRP ? 'r' : '-';
    str[5] = mode & S_IWGRP ? 'w' : '-';
    str[6] = (mode & S_ISGID
            ? (mode & S_IXGRP ? 's' : 'S')
            : (mode & S_IXGRP ? 'x' : '-'));
    str[7] = mode & S_IROTH ? 'r' : '-';
    str[8] = mode & S_IWOTH ? 'w' : '-';
    str[9] = (mode & __S_ISVTX
            ? (mode & S_IXOTH ? 't' : 'T')
            : (mode & S_IXOTH ? 'x' : '-'));
    str[10] = ' ';
    str[11] = '\0';
} 

static int str_is_num(const char* str, size_t len) {
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

static int convert_oct_to_dec(int num_oct) {
    int num_dec = 0;
    int mult = 1;

    while (num_oct != 0) {
        int digit = num_oct % 10;
        if (digit < 0 || digit >= 8) {
            return -1;
        }
        num_dec += digit * mult;
        mult *= 8;
        num_oct /= 10;
    }
    return num_dec;
}

mode_t ParseMode(const char* str) {
    if (!str_is_num(str, strlen(str))) {
        return -1;
    }
    mode_t mode_oct = atoi(str);
    return convert_oct_to_dec(mode_oct);
}
