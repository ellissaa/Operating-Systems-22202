#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void write_sequence(int write_fd) {
    printf("Write started\n");
    unsigned int number = 0;
    while (1) {
        write(write_fd, &number, sizeof(number));
        number++;
        sleep(1);
    }
}

void read_sequence(int read_fd) {
    printf("Read started\n");
    unsigned int expected_number = 0;
    while (1) {
        unsigned int number;
        read(read_fd, &number, sizeof(number));
        if (number != expected_number) {
            printf("Fail\n");
        }
        else {
            printf("OK\n");
        }
        expected_number++;
        sleep(1);
    }
}

int main(int argc, char **argv) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return(-1);
    }

    printf("Read fd : %i write fd : %i\n", pipefd[0], pipefd[1]);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return(-1);
    }

    if (pid == 0) {
        close(pipefd[1]); 
        read_sequence(pipefd[0]);
    } else {
        close(pipefd[0]);
        write_sequence(pipefd[1]);
    }
}
