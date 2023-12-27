#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>

/* add O_NONBLOCK to specific fd */
void setNonBlocking(int fd) {
    int flags;
    flags = fcntl(fd, F_GETFL);
    if (flags < 0){
        perror("fcntl F_GETFL failed");
        exit(EXIT_FAILURE);
    }
    flags = flags | O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0){
        perror("fcntl F_SETFL failed");
        exit(EXIT_FAILURE);
    }
}

/* print remote client ip and port info */
void printClient(int fd){
    struct sockaddr_in remote_addr;
    int sockaddr_len = sizeof(struct sockaddr);
    char ip_addr[20];
    getpeername(fd, (struct sockaddr*)&remote_addr, &sockaddr_len);
    inet_ntop(AF_INET, &(remote_addr.sin_addr), ip_addr, 20);
    printf(" %s:%d\n", ip_addr, ntohs(remote_addr.sin_port));
}

/* accept socket connection and print remote info */
int acceptConn(int listen_fd) {
    int sockaddr_len = sizeof(struct sockaddr);
    struct sockaddr_in remote_addr;

    /* accept() - accepts an incoming connection and creates a new socket for communication. */
    int conn_fd = accept(listen_fd, (struct sockaddr *)&remote_addr, &sockaddr_len);
    if (conn_fd == -1) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Accept a connection from");
    printClient(conn_fd);

    return conn_fd;
}

/* make the caller listened on specific tcp port, and return listen fd */
int listenPort(int port) {
    int sock_fd;
    int ret;
    int optval = 1;
    int sockaddr_len = sizeof(struct sockaddr);
    struct sockaddr_in local_addr;

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    memset(local_addr.sin_zero, '\0', 8);

    /* socket() - creates a new socket. */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (ret == -1) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    /* bind() - binds a socket to a specific address and port. */
    ret = bind(sock_fd, (struct sockaddr *) &local_addr, sockaddr_len);
    if (ret == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    /* listen() - puts a socket in a listening state, waiting for incoming connections. */
    ret = listen(sock_fd, 10);
    if (ret == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    return sock_fd;
}
