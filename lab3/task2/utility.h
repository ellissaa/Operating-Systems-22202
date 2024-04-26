#ifndef ERROR_PRINTERS_H_INCLUDED
#define ERROR_PRINTERS_H_INCLUDED

#include <fcntl.h>

int IsCorrectName(const char* name);

void ConnectTwoStrs(const char* str1, const char* str2, char spec_symbol, char* connected_str);

char* GetShortName(char* name);   

void Concat(const char* str1, const char* str2, char* res);

void PrintFile(FILE* file);

void strmode(mode_t mode, char *str);

mode_t ParseMode(const char* str);

#endif