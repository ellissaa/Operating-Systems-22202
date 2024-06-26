#define _POSIX_C_SOURCE 201112L

#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ARRAY_SIZE 1024

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

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
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
    struct sockaddr *server_address = server_addr_info->ai_addr;

    char send_array[ARRAY_SIZE];
    char recv_array[ARRAY_SIZE];
    int message_count = 0;

    while (1) {
        socklen_t addr_len = server_addr_info->ai_addrlen;
        snprintf(send_array, ARRAY_SIZE,
            "Message %i from process %i", message_count, pid);

        int sent = sendto(sock_fd, send_array, ARRAY_SIZE, 0, server_address, addr_len);
        if (sent == -1) {
            handle_error("sendto");
        }

        int received = recvfrom(sock_fd, recv_array, ARRAY_SIZE, 0, server_address, &addr_len);
        if (received == -1) {
            handle_error("recvfrom");
        }
        printf("%s\n", recv_array);

        message_count++;
        sleep(1);
    }
}
