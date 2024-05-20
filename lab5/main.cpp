#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int global = 10;

int main(int argc, char ** argv) {
    int local = 5;
    printf("Local\t %i\t %p\n", local, &local);
    printf("Global\t %i\t %p\n\n", global, &global);
    
    sleep(3);
    pid_t pid = getpid();
    printf("Main pid\t %i\n\n", pid);

    pid_t fork_pid = fork();
    if (fork_pid == -1) {
        perror("fork");
        return -1;
    }

    if (fork_pid == 0) {
        printf("In child\n");

        pid = getpid();
        printf("Child pid\t %i\n", pid);
        // может отличаться от initial pid, если исходный процесс уже завершился
        pid_t parent_pid = getppid();
        printf("Parent pid\t %i\n\n", parent_pid);

        printf("Local\t %i\t %p\n", local, &local);
        printf("Global\t %i\t %p\n\n", global, &global);

        local *= 10;
        global *= 10;

        printf("After change:\n");
        printf("Local\t %i\t %p\n", local, &local);
        printf("Global\t %i\t %p\n\n", global, &global);

        sleep(10);
        printf("Child exiting...\n\n");
        _exit(5);
    } else {
        sleep(1);

        printf("In parent\n");
        printf("Local\t %i\t %p\n", local, &local);
        printf("Global\t %i\t %p\n\n", global, &global);

        sleep(30);
        int status;
        wait(&status); // дожидается изменения состояния любого дочернего процесса
        if (WIFEXITED(status)) {
            printf("Terminated normally; exit status: %i\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)) {
            printf("Terminated by signal; signal number: %i\n", WTERMSIG(status));
        }
    }
}