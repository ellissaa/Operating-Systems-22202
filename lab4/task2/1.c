#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    __pid_t pid = getpid();
    printf("%i\n", pid);
    sleep(1);

    if(execv(argv[0], argv) == -1) {
        perror("execve error");
        return -1;
    }

    printf("hello world!\n");
}
