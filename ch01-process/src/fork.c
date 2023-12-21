#include <stdio.h>  //printf(), perror()
#include <unistd.h> // fork(), getpid(), getppid(), sleep(), execlp(), execl()
#include <stdlib.h> // exit(),


int main(int argc, char* argv[])
{
    printf("process control example is running!\n");

    pid_t pid = fork();
    if (pid == -1){
        perror("fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0){       // child process
        execlp("ls", "ls", "-lah", "/var", NULL);
        execl("/bin/date", "date", NULL);
        perror("execlp error");
        exit(EXIT_FAILURE);
    } else if (pid > 0){        // parent process
        sleep(1);
        printf("---- I'm parent process %d, my child is %d\n", getpid(), pid);
    }

    printf("================\n");

    return 0;
}
