#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define PORT 8892
#define BUFFER_SIZE 1024
#define EPOLL_TIMER 1000
#define MAX_EVENTS 20

extern int listenPort(int);
extern void setNonBlocking(int);
extern int acceptConn(int);
extern void printClient(int);

int main(int argc, char* argv[])
{
    int listen_fd, conn_fd, sock_fd, epfd, fds, ret;
    struct epoll_event ev, events[MAX_EVENTS];
    char* in_buffer = (char*)malloc(BUFFER_SIZE);
    char* out_buffer = (char*)malloc(BUFFER_SIZE);

    /* create an epoll instance, the size argument is ignored since Linux 2.6.8 */
    epfd = epoll_create(256);

    /* create socket to listen on specific port */
    listen_fd = listenPort(PORT);

    /* add O_NONBLOCK flags to the socket file */
    setNonBlocking(listen_fd);

    /* create epoll event for this fd, which used in Edge-Triggered mode */
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listen_fd;

    /* add this fd to epoll instance */
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);
    if (ret == -1) {
        perror("epoll_ctl EPOLL_CTL_ADD listen_fd failed");
        exit(EXIT_FAILURE);
    }
    
    while(1) {
        /* check whether an I/O event happened on a epoll instance */
        fds = epoll_wait(epfd, events, MAX_EVENTS, EPOLL_TIMER);

        for (int i = 0; i < fds; i++) {
            /* client is connecting to the listen port */
            if (events[i].data.fd == listen_fd) {
                conn_fd = acceptConn(listen_fd);
                setNonBlocking(conn_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_fd;

                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev);
                if (ret == -1) {
                    perror("epoll_ctl EPOLL_CTL_ADD conn_fd failed");
                    exit(EXIT_FAILURE);
                }
            /* client has wrote something into the socket */
            } else if (events[i].events & EPOLLIN) {
                sock_fd = events[i].data.fd;
                /* recv() - receive data over a socket. */
                ret = recv(sock_fd, in_buffer, BUFFER_SIZE, 0);
                if (ret == -1) {
                    perror("recv failed");
                    exit(EXIT_FAILURE);
                }
                if (ret == 0) {
                    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, sock_fd, NULL);
                    if (ret == -1) {
                        perror("epoll_ctl EPOLL_CTL_ADD conn_fd failed");
                        exit(EXIT_FAILURE);
                    }
                    printf("Terminate the connection with");
                    printClient(sock_fd);
                    close(sock_fd);
                    continue;
                }
                /* print the content in socket */
                printf("recv: ");
                for (int i = 0; i < ret; i++) {
                    printf("%c", *(in_buffer + i));
                    *(out_buffer + i) = toupper(*(in_buffer + i));
                }

                /* send() - send data over a socket. */
                send(conn_fd, out_buffer, BUFFER_SIZE, 0);
                memset(out_buffer, 0, ret);
            }
        }
    }

    free(in_buffer);
    free(out_buffer);
    close(listen_fd);

    return 0;
}
