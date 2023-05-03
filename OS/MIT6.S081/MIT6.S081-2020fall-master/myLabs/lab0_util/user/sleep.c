#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    
    if (argc < 2) {
        printf("need a arg interval for sleep\n");
        exit(1);
    }

    int interval;
    interval = atoi(argv[1]);
    sleep(interval);
    exit(0);
    
}