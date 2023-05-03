#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int i;
    int fd[2];
    pipe(fd);
    int pid;

    pid = fork();

    if (pid == 0) {
        int r_fd = fd[0];
        int w_fd = 0;
        int num;
        int num_bk = 0;
        int initial = 1;
        while (read(r_fd, (void *)&num, 4) > 0)
        {
            /* code */
            if (initial == 1) {
                close(fd[1]);
                pipe(fd);
                pid = fork();
                if (pid == 0) {
                     r_fd = fd[0];
                     close(fd[1]);
                     continue;
                } else {
                    initial = 0;
                    num_bk = num;
                    w_fd = fd[1];
                    close(fd[0]);
                    // printf("[%d] prime %d\n", getpid() , num);
                    printf("prime %d\n", num);
                    continue;
                }
            }

            if ( (num % num_bk) == 0) {
                continue;
            } else {
                write(w_fd, (const void *)&num, 4);
            }
        }

        close(r_fd);
        close(w_fd);
        sleep(num_bk);
        i = 0;
        while( i != -1) {
            // printf("process [%d] wait\n", getpid());
            i = wait((int*)0);
            // printf("process [%d] wait recv %d exit\n",getpid(), i);
        }
        exit(0);  
    } else {
        close(fd[0]);
        for (i = 2; i < 35; i++) {
            write(fd[1], (const void*)&i, 4);
        }
    }

    close(fd[1]);
    i = 0;
    while( i != -1) {
        i = wait(0);
        // printf("main process [%d] wait recv %d exit\n",getpid(), i);
    }

    exit(0);
}