#include "prime.c"
#include <time.h>

void main()
{
    printf("Pi(x) Calculator for [x < 2 ** 34].\nx = ");
    ulong target;
    scanf("%llu", &target);

    if (target >= (ulong)1 << 34)
        exit(0);

    time_t start = clock();
    printf("\n\nPi(%llu) = %u\n", target, countPrimes(target));
    time_t finish = clock();

    double elapsedTime = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("\n\nExecution Time: %.3f sec\n", elapsedTime);
    system("pause");
}