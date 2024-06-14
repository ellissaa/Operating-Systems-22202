// #define _GNU_SOURCE

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#define PAGE_SIZE 4096

void write_region(unsigned int *region) {
    size_t region_size = PAGE_SIZE / sizeof(unsigned int);
    size_t index = 0;   
    unsigned int number = 0;
    while (1) {
        region[index] = number;
        number++;
        index++;
        if (index == region_size) index = 0;
        sleep(1);
    }
}

void read_region(unsigned int *region) {
    size_t region_size = PAGE_SIZE / sizeof(unsigned int);
    size_t index = 0;
    unsigned int expected_number = 0;
    while (1) {
        unsigned int number = region[index];
        if (number != expected_number)
            printf("Fail at %p\n", region + index);
        expected_number++;
        index++;
        if (index == region_size) index = 0;
        sleep(1);
    }
}

int main(int argc, char **argv) {
    unsigned int *region = (unsigned int *) mmap(NULL, PAGE_SIZE, PROT_WRITE | PROT_READ,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (region == MAP_FAILED) {
        perror("mmap");
        return(-1);
    }
    printf("New region : %p\n", region);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return(-1);
    }

    if (pid == 0) {
        read_region(region);
    } else {
        write_region(region);
    }
}
