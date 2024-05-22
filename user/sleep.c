#include "user/user.h"
#include "kernel/types.h"

/*
sys call: sleep
*/

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "usage: sleep [ticks]\n");
        exit(1);
    }
    int ticks = atoi(argv[1]);
    int ret = sleep(ticks);
    exit(ret);
}