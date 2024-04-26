#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int global;
int global_initialized = 11;
const int global_const = 1;

enum {
    SIZE = 100
};

void func1() {
    int local = 11;
    static int local_static = 12;
    const int local_const = 2;

    printf("local: %lx\nlocal static: %lx\nlocal const: %lx\n", (size_t) &local, 
        (size_t) &local_static, (size_t) &local_const);
}

size_t addr() {
    int local = 13;
    return (size_t) &local;
}

void func2() {
    char* array = malloc(SIZE);
    snprintf(array, sizeof("hello world!"), "hello world!");
    
    printf("BEFORE:\n");
    PrintArray(array, SIZE);
    
    free(array);
    printf("AFTER:\n");
    PrintArray(array, SIZE);

    char* array2 = malloc(SIZE);
    snprintf(array2, sizeof("hello world!!"), "hello world!!");
    printf("BEFORE 2:\n");
    PrintArray(array2, SIZE);

    // array2 += SIZE / 2;
    // free(array2);
    // printf("AFTER 2:\n");
    // PrintArray(array2, SIZE);
}

void func3() {
    char* var_value = getenv("VAR");
    if (var_value != NULL) {
        printf("%s\n", var_value);
    }

    if (setenv("VAR", "hello", 1) == -1) {
        perror("setenv");
        return;
    }

    var_value = getenv("VAR");
    if (var_value != NULL) {
        printf("%s\n", var_value);
    }
}

int main() {
    FILE* maps = fopen("/proc/self/maps", "r");
    if (maps == NULL) {
        perror("Can't open file");
        return -1;
    }

    PrintFile(maps);
    fclose(maps);

    printf("FUNC1 CALLED\n");
    func1();

    printf("global: %lx\nglobal initialized: %lx\nglobal const: %lx\n", (size_t) &global, 
            (size_t) &global_initialized, (size_t) &global_const); 

    printf("LOCAL ADDR: %lx\n", addr());

    func2();
    func3();
}