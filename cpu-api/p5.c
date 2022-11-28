//
// Created by 12466 on 2022/11/28.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int
main(int argc, char *argv[])
{
    printf("hello world (pid:%d)\n", (int) getpid());
    int x = 100;
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, x is %d (pid:%d)\n", x, (int) getpid());
        x = x + 1;
        printf("hello, x is %d (pid:%d)\n", x, (int) getpid());
    } else {
        // parent goes down this path (original process)
        printf("parent, x is %d (pid:%d)\n", x, (int) getpid());
        x = x - 1;
        printf("parent, x is %d (pid:%d)\n", x, (int) getpid());
    }
    return 0;
    /**
     * hello world (pid:23529)
     * parent, x is 100 (pid:23529)
     * parent, x is 99 (pid:23529)
     * hello, x is 100 (pid:23530)
     * hello, x is 101 (pid:23530)
     */

}
