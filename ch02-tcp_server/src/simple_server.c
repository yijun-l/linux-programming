#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    int sock_fd, acpt_fd;
    int ret;
    int sockaddr_len = sizeof(struct sockaddr);
    struct sockaddr_in local_addr, remote_addr;
    char *in_buffer = (char*)malloc(BUFFER_SIZE);
    char *out_buffer = (char*)malloc(BUFFER_SIZE);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    memset(local_addr.sin_zero, '\0', 8);

    /* socket() - creates a new socket. */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* bind() - binds a socket to a specific address and port. */
    ret = bind(sock_fd, (struct sockaddr*)&local_addr, sockaddr_len);

    /* listen() - puts a socket in a listening state, waiting for incoming connections. */
    ret = listen(sock_fd, 10);
    while(1)
    {
        /* accept() - accepts an incoming connection and creates a new socket for communication. */
        acpt_fd = accept(sock_fd, (struct sockaddr*)&remote_addr, &sockaddr_len);
        while(1){
            /* recv() - receive data over a socket. */
            ret = recv(acpt_fd, in_buffer, BUFFER_SIZE, 0);
            if (ret == 0){
                break;
            }

            printf("recv: ");
            for (int i = 0; i < ret; i++)
            {
                printf("%c", *(in_buffer +i));
                *(out_buffer + i) = toupper(*(in_buffer + i));
            }

            /* send() - send data over a socket. */
            send(acpt_fd, out_buffer, BUFFER_SIZE, 0);
            memset(out_buffer, 0, ret);
        }
        close(acpt_fd);
    }
    close(sock_fd);
    free(in_buffer);
    free(out_buffer);

    return 0;
}