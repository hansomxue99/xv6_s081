#include "user/user.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *basename(char *pathname) {
    char *prev = 0;
    char *curr = strchr(pathname, '/');
    while (curr != 0) {
        prev = curr;
        curr = strchr(curr + 1, '/');
    }
    return prev;
}

void find(char *cur_path, char *target) {
    int fd;
    struct stat st;
    char buf[512], *p;
    struct dirent de;
    if ((fd = open(cur_path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", cur_path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", cur_path);
        return;
    }

    switch(st.type) {
        case T_FILE:
        {
            char *f_name = basename(cur_path);
            int match = 1;
            if (f_name == 0 || strcmp(f_name + 1, target) != 0) {
                match = 0;
            }
            if (match) {
                printf("%s\n", cur_path);
            }
            close(fd);
            break;
        }
        case T_DIR:
        {
            memset(buf, 0, sizeof(buf));
            uint cur_path_len = strlen(cur_path);
            memcpy(buf, cur_path, cur_path_len);
            buf[cur_path_len] = '/';
            p = buf + cur_path_len + 1;
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0 || strcmp(de.name, ".") == 0 || \
                    strcmp(de.name, "..") == 0) {
                    continue;
                }
                memcpy(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, target);
            }
            close(fd);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "usage: find [directory] [target filename]\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}