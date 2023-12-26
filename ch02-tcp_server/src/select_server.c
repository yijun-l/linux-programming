#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>

#define PORT 8891
#define BUFFER_SIZE 1024


int main(int argc, char* argv[])
{
    int sock_fd, acpt_fd, nfds;
    int ret;
    int optval = 1;
    int sockaddr_len = sizeof(struct sockaddr);
    struct sockaddr_in local_addr, remote_addr;
    fd_set fds, retfds;
    char *in_buffer, *out_buffer;

    FD_ZERO(&fds);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    memset(local_addr.sin_zero, '\0', 8);

    /* socket() - creates a new socket. */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
	perror("socket creation failed");
	exit(EXIT_FAILURE);
    }


    ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (ret == -1)
    {
	perror("setsockopt failed");
	exit(EXIT_FAILURE);
    }

    /* bind() - binds a socket to a specific address and port. */
    ret = bind(sock_fd, (struct sockaddr*)&local_addr, sockaddr_len);
    if (ret == -1)
    {
	perror("bind failed");
	exit(EXIT_FAILURE);
    }

    /* listen() - puts a socket in a listening state, waiting for incoming connections. */
    ret = listen(sock_fd, 10);
    if (ret == -1)
    {
	perror("listen failed");
	exit(EXIT_FAILURE);
    }

    FD_SET(sock_fd, &fds);
    nfds = sock_fd + 1;
    retfds = fds;

    in_buffer = (char*)malloc(BUFFER_SIZE);
    out_buffer = (char*)malloc(BUFFER_SIZE);

    while(1) {
        ret = select(nfds, &retfds, NULL, NULL, NULL);

        if (ret) {
            if (FD_ISSET(sock_fd, &retfds)) {
                /* accept() - accepts an incoming connection and creates a new socket for communication. */
                acpt_fd = accept(sock_fd, (struct sockaddr *) &remote_addr, &sockaddr_len);
                if (acpt_fd == -1) {
                    perror("accept failed");
                    exit(EXIT_FAILURE);
                }
                FD_SET(acpt_fd, &fds);
                nfds++;
            }
            for (int i = sock_fd + 1; i < nfds; i++) {
                if (FD_ISSET(i, &retfds)) {
                    ret = recv(i, in_buffer, BUFFER_SIZE, 0);
                    if (ret == 0) {
                        close(i);
                        // nfds--;
                        FD_CLR(i, &fds);
                    }

                    printf("recv: ");
                    for (int i = 0; i < ret; i++) {
                        printf("%c", *(in_buffer + i));
                        *(out_buffer + i) = toupper(*(in_buffer + i));
                    }

                    /* send() - send data over a socket. */
                    send(i, out_buffer, BUFFER_SIZE, 0);
                    memset(out_buffer, 0, ret);
                }
            }
        }

        retfds = fds;
    }

    close(sock_fd);

    return 0;
}
