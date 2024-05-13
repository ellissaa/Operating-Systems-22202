#include "functions.h"
#include <stdio.h>

enum {
    SIZE = 100
};

void PrintFile(FILE* file) {
    char arr[SIZE];

    while (!feof(file)) {
        int bytes_read = fread(arr, 1, SIZE, file);
        int bytes_write = 0;

        while (bytes_write < bytes_read) {
            bytes_write += fwrite(arr + bytes_write, 1, bytes_read - bytes_write, stdout);
        }
    }
    fflush(stdout);
}

void PrintArray(char* array, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c", array[i]);
    }
    printf("\n");
}
