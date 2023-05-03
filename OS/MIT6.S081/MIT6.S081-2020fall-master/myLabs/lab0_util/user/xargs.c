#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int getALine(char *content)
{
    char c = 'A';
    char *p = content;
    while (1) {
        if (read(0, &c, 1) == 0) {
            return 0;
        }
        if (c != '\n') { *p++ = c;}
        else {break;}
    }

    *p = 0;
    return 1;
}


int main(int argc, char const *argv[])
{
    if (argc < 2) exit(1);

    int pid = fork();
    if (pid == 0) {
        char *p = malloc(4096);
        char *largv[10];
        int argvIndex = 0;
        int i = 0, len = 0;

        for (i = 1; i < argc; ++i) {
            largv[argvIndex++] = p;
            len = strlen(argv[i]);
            memmove(p, argv[i], len);
            p += len;
            *++p = 0;
        }

        while (getALine(p))
        {
            largv[argvIndex++] = p;
            p += strlen(p);
            p++;
        }

        largv[argvIndex] = 0;

        // printf("\n\ncmd = %s\n", argv[1]);
        // for (i = 0; i < argvIndex; ++i) {
        //     printf("arg[%d] = %s\n", i, largv[i]);
        // }
        // printf("\n\n\n");


        exec((char *)argv[1], largv);
        exit(0);
    } else {
        wait(0);
        exit(0);
    }



    return 0;
}
