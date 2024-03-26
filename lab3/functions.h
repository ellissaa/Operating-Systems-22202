#ifndef FUNC_INCLUDED
#define FUNC_INCLUDED

int IsCorrectName(char* name);

char* ReverseStr(char* name);

char* ConnectTwoStrs(char* str1, char* str2, char spec_symbol);

char* GetDirName(char* name);

void ReverseFile(char* src_file, char* dst_dir, __mode_t mode);

#endif