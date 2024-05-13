#define _GNU_SOURCE

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

enum {
    PAGE_SIZE = 4096,
    SIZE = 4096 * 4,
    BUFF_SIZE = 4096 * 2048 * 200,
    ITER = 20
};

void StackOverflow() {
    char array[SIZE];
    printf("%p\n", array);
    sleep(1);
    StackOverflow();
}

void HeapAlloc() {
    // char *pointers[ITER];
    // for (int i = 0; i < ITER; i++) {
    //     char *buffer = malloc(BUFF_SIZE);
    //     pointers[i] = buffer;
    //     printf("%p\n", buffer);
    //     sleep(1);   
    // }

    // for (int i = 0; i < ITER; i++) {
    //     free(pointers[i]);
    // }
    while(1) {
        char *buffer = malloc(BUFF_SIZE);
        // pointers[i] = buffer;
        printf("%p\n", buffer);
        sleep(1);
    }
    sleep(10);
}

void WriteOnly() {
    char *new_region = (char*) mmap(NULL, 10 * PAGE_SIZE, PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_region == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }

    printf("%p\n", new_region);
    new_region[10] = 'a';
    printf("%c\n", new_region[10]);
}

void ReadOnly() {
    char *new_region = (char*) mmap(NULL, 10 * PAGE_SIZE, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_region == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }

    printf("%p\n", new_region);
    new_region[10] = 'a';
}

void MapUnmap() {
    char *new_region = (char*) mmap(NULL, 10 * PAGE_SIZE, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_region == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }

    printf("%p\n", new_region);
    sleep(20);
    munmap(new_region + 4 * PAGE_SIZE, 3 * PAGE_SIZE);
    sleep(20);
}

void Handler(int sig) {
    printf("Segfault!!!\n");
    exit(-1);
}

int main(int argc, char **argv) {
    struct sigaction action;
    action.sa_handler = Handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGSEGV, &action, NULL);

    pid_t pid = getpid();
    printf("pid: %i\n", pid);
    sleep(10);

    // StackOverflow();
    HeapAlloc();
    // WriteOnly();
    // ReadOnly();
    // MapUnmap();
}