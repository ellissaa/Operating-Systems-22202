#ifndef ERROR_PRINTERS_H_INCLUDED
#define ERROR_PRINTERS_H_INCLUDED

#include <fcntl.h>
#include <cstdio>

int IsCorrectName(const char* name);

char* ConnectTwoStrs(const char* str1, const char* str2, char symb);

char* concat(const char* str1, const char* str2);

char* GetDirName(char* path);

void PrintFile(FILE* file);

void StrMode(mode_t mode, char *str);

mode_t ParseMode(const char* str);

#endif