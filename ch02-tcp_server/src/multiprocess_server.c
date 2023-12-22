#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <ctype.h>
#include <netinet/in.h>

#define PORT 8889
#define BUFFER_SIZE 1024

void child_exit_handler(int signal)
{
    pid_t pid;
    int status;

    pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0){
	if(WIFEXITED(status)){
	    printf("\nchild process %d exited as normal\n", pid);
	} else {
	    printf("\nchild process %d terminated..\n", pid);
	}
    }
}

int main(int argc, char* argv[])
{
    int sock_fd, acpt_fd;
    int ret;
    int optval = 1;
    int sockaddr_len = sizeof(struct sockaddr);
    struct sockaddr_in local_addr, remote_addr;
    char *in_buffer, *out_buffer;

    signal(SIGCHLD, child_exit_handler);

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

    while(1)
    {
        /* accept() - accepts an incoming connection and creates a new socket for communication. */
        acpt_fd = accept(sock_fd, (struct sockaddr*)&remote_addr, &sockaddr_len);
        if (acpt_fd == -1)
        {
    	    perror("accept failed");
    	    exit(EXIT_FAILURE);
        }

	ret = fork();
        if (ret == -1)
        {
    	    perror("fork failed");
    	    exit(EXIT_FAILURE);
        }

	if (ret == 0)
	{
	    close(sock_fd);
	    in_buffer = (char*)malloc(BUFFER_SIZE);
    	    out_buffer = (char*)malloc(BUFFER_SIZE);
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
    	    free(in_buffer);
    	    free(out_buffer);
            close(acpt_fd);
    	return 0;
	}

	close(acpt_fd);
    }
    close(sock_fd);

    return 0;
}
