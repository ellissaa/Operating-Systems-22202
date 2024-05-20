#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LISTEN_BACKLOG 50
#define ARRAY_SIZE 1024

static void handle_error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

static void echo_requests(int comm_sock_fd) {
    char array[ARRAY_SIZE];

    while (1) {
        ssize_t recieved = recv(comm_sock_fd, array, ARRAY_SIZE, 0);
        if (recieved == -1) {
            handle_error("recv");
        }

        ssize_t sent = send(comm_sock_fd, array, recieved, 0);
        if (sent == -1) {
            handle_error("send");
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int listening_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_sock_fd == -1) {
        handle_error("socket");
    }

    int port = atoi(argv[1]);

    struct sockaddr_in serv_address;
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);
    serv_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listening_sock_fd, (struct sockaddr *) &serv_address, sizeof(serv_address)) == -1) {
        handle_error("bind");
    }

    if (listen(listening_sock_fd, LISTEN_BACKLOG) == -1) {
        handle_error("listen");
    }

    char hostname[50];
    char service[10];
    getnameinfo((struct sockaddr *) &serv_address, sizeof(serv_address),
        hostname, sizeof(hostname), service, sizeof(service), 0);
    printf("Server %s %s\n", hostname, service);

    while (1) {
        struct sockaddr_in comm_sock_addr;
        socklen_t addr_len = sizeof(comm_sock_addr);
        int comm_sock_fd = accept(listening_sock_fd,
            (struct sockaddr *) &comm_sock_addr, &addr_len);
        if (comm_sock_fd == -1) {
            handle_error("accept");
        }

        pid_t pid = fork();
        if (pid == -1) {
            handle_error("fork");
        }
        if (pid == 0) {
            echo_requests(comm_sock_fd);
        }
    }
}
