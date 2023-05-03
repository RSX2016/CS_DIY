#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);

    int pid = fork();
    if (pid == 0) {
        char buf[1024];
        read(fd[0], buf, sizeof(buf));
        printf("%d: received ping\n", getpid());
        close(fd[0]);
        write(fd[1], "OOO", 4);
        close(fd[1]);
        exit(0);
    } else {
        char buf[1024];
        write(fd[1], "IIII", 5);
        close(fd[1]);
        read(fd[0], buf, sizeof(buf));
        printf("%d: received pong\n", getpid());
        close(fd[0]);
        exit(0);
    }
}