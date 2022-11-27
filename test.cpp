//
// Created by 12466 on 2022/11/27.
//
#include <stdio.h>

int main(int argc, char ** argv)
{
    int i;
    for (i = 0; i < argc; i++)
        printf("Argument %d is %s\n", i, argv[i]);
    return 0;
}
