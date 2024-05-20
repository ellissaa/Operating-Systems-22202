#define _POSIX_C_SOURCE 201112L

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define ARR_SIZE 1024

static void handle_error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <port>", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = getpid();

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        handle_error("socket");
    }

    char *serv_hostname = argv[1];
    char *serv_port = argv[2];

    struct addrinfo *server_addr_info;
    int ret_code = getaddrinfo(serv_hostname, serv_port, NULL, &server_addr_info);
    if (ret_code != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret_code));
        exit(EXIT_FAILURE);
    }
    struct sockaddr *serv_address = server_addr_info->ai_addr;

    if (connect(sock_fd, serv_address, server_addr_info->ai_addrlen) == -1) {
        handle_error("connect");
    }

    char send_array[ARR_SIZE];
    char recv_array[ARR_SIZE];
    int message_count = 0;

    while (1) {
        snprintf(send_array, ARR_SIZE,
            "Message %i from process %i", message_count, pid);

        int sent = send(sock_fd, send_array, ARR_SIZE, 0);
        if (sent == -1) {
            handle_error("send");
        }

        int received = recv(sock_fd, recv_array, ARR_SIZE, 0);
        if (received == -1) {
            handle_error("recv");
        }
        printf("%s\n", recv_array);

        message_count++;
        sleep(1);
    }
}
