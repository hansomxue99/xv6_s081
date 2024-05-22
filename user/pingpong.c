#include "user/user.h"

/*
communication between process
sys call: pipe fork
*/

int main(int argc, char *argv[]) {
    int pid;
    int pipe1[2], pipe2[2];
    char onebyte[] = {'a'};

    pipe(pipe1);
    pipe(pipe2);

    int ret = fork();

    if (ret == 0) {
        // child
        pid = getpid();
        close(pipe1[1]);
        close(pipe2[0]);
        read(pipe1[0], onebyte, 1);
        printf("%d: received ping\n", pid);
        write(pipe2[1], onebyte, 1);
        exit(0);
    } else {
        // parent
        pid = getpid();
        close(pipe1[0]);
        close(pipe2[1]);
        write(pipe1[1], onebyte, 1);
        read(pipe2[0], onebyte, 1);
        printf("%d: received pong\n", pid);
        exit(0);
    }
}