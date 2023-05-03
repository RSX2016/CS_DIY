#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char*
fileName(char *path)
{
    char *p;

    // Find first character after last slash.
    for(p = path + strlen(path); p >= path && *p != '/'; p--) {}
    p++;
    return p;
}

void find(char *path, char *file)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    

    if ((fd = open(path, 0)) < 0 ) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        // printf("fileName = %s | %s|\n", path, fileName(path));
        if (strcmp(fileName(path), file) == 0) {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(path);
        *p++ = '/';
        while (read(fd, &de, sizeof(de))) {
            if (de.inum == 0) continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, file);
        }

        break;
    default:
        break;
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc >= 2) {
        find(argv[1], argv[2]);
    }
    exit(0);
}
