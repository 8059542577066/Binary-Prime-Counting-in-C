#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long long ulong;

typedef struct
{
    uint byteSize;
    byte *bytes;
} Bits;

void construct(Bits *bits, uint byteSize)
{
    bits->byteSize = byteSize;
    bits->bytes = (byte *)calloc(byteSize, 1);
}

void release(Bits *bits)
{
    free(bits->bytes);
}

uint getByteIndex(ulong bitSize)
{
    return bitSize >> 3;
}

uint getBitIndex(ulong bitSize)
{
    return bitSize & 7;
}

byte getBit(const Bits *bits, uint byteIndex, uint bitIndex)
{
    return bits->bytes[byteIndex] & 1 << bitIndex;
}

void setBitTo1(Bits *bits, uint byteIndex, uint bitIndex)
{
    bits->bytes[byteIndex] |= 1 << bitIndex;
}

void main()
{
    printf("Pi(x) Calculator for [x < 2 ** 34].\nx = ");
    ulong target;
    scanf("%llu", &target);

    if (target >= (ulong)1 << 34)
        exit(0);

    uint byteSize = target / 8 + 1;
    ulong bitSize = (ulong)byteSize * 8, j;
    ulong limit = bitSize / 2, i;

    time_t start = clock();
    Bits bits;
    construct(&bits, byteSize);
    setBitTo1(&bits, 0, 0);
    setBitTo1(&bits, 0, 1);

    for (i = 2; i < limit; ++i)
        if (!getBit(&bits, getByteIndex(i), getBitIndex(i)))
            for (j = i * 2; j < bitSize; j += i)
                setBitTo1(&bits, getByteIndex(j), getBitIndex(j));

    char fileName[16];
    sprintf(fileName, "%llu.txt", target);
    FILE *file = fopen(fileName, "w");

    for (i = 0; i <= target; ++i)
        if (!getBit(&bits, getByteIndex(i), getBitIndex(i)))
            fprintf(file, "%llu\n", i);

    fclose(file);
    release(&bits);
    time_t finish = clock();
    double elapsedTime = (double)(finish - start) / CLOCKS_PER_SEC;

    printf("\n\nExecution Time: %.3f sec\n", elapsedTime);
    system("pause");
}