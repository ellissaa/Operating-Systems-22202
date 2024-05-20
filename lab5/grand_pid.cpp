#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
 
int main(int argc, char ** argv) { 
    printf("Main process pid: %d\n", getpid()); 
    sleep(5); 
    int child_pid = fork(); 

    if (child_pid == 0) { 
        int another_child_pid = fork(); 
        // printf("Father pid: %d, child pid: %d", getppid(), getpid());

        if (another_child_pid == 0) { 
            printf("Grand child pid: %d, parent pid is %d\n", getpid(), getppid()); 
            sleep(2); 
            printf("Grand child pid: %d, parent pid is %d\n", getpid(), getppid());
            printf("Grand child exiting...\n"); 
            _exit(5); 
        } else { 
            sleep(1);
            printf("Zombie parent pid: %d\n", getpid()); 
            _exit(5); 
        } 
    } else if (child_pid != 0) { 
        printf("Grand father pid: %d\n", getpid()); 
        sleep(20); 
        int status;
        wait(&status); // дожидается изменения состояния любого дочернего процесса
        if (WIFEXITED(status)) {
            printf("Terminated normally; exit status: %i\n", WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status)) {
            printf("Terminated by signal; signal number: %i\n", WTERMSIG(status));
        }
    } else { 
        perror("fork"); 
    } 
    
}
