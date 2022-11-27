#include <stdio.h>
#include <stdlib.h>
#include "common.h"

/**
 * CPU虚拟化-操作系统负责提供这种假象，系统拥有非常多的虚拟CPU假象。
 * 单个CPU转换为看似无线数量的CPU,从而让许多程序看似同时运行。
 *
 *
 * 资源管理器角色
 * 一次运行多个程序的能力会引发各种新问题，操作系统的许多不同地方采用策略来解决这种问题
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: cpu <string>\n");
        exit(1);
    }
    char *str = argv[1];

    while (1) {
        printf("%s\n", str);
        Spin(1);
    }
    return 0;
}

