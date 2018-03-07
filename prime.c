#include <stdlib.h>
#include <stdio.h>

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long long ulong;

uint getByteIndex(ulong number)
{
    return number >> 3;
}

uint getBitIndex(ulong number)
{
    return number & 7;
}

byte getBit(const byte *bytes, uint byteIndex, uint bitIndex)
{
    return bytes[byteIndex] & 1 << bitIndex;
}

void setBitTo1(byte *bytes, uint byteIndex, uint bitIndex)
{
    bytes[byteIndex] |= 1 << bitIndex;
}

void runSieve(byte *bytes, ulong bitSize)
{
    ulong limit = bitSize / 2, i, j;

    for (i = 2; i < limit; ++i)
        if (!getBit(bytes, getByteIndex(i), getBitIndex(i)))
            for (j = i * 2; j < bitSize; j += i)
                setBitTo1(bytes, getByteIndex(j), getBitIndex(j));
}

void save(ulong target)
{
    uint byteSize = target / 8 + 1;
    ulong bitSize = (ulong)byteSize * 8, i;

    byte *bytes = (byte *)calloc(byteSize, 1);
    runSieve(bytes, bitSize);

    char fileName[16];
    sprintf(fileName, "%llu.txt", target);
    FILE *file = fopen(fileName, "w");

    for (i = 2; i <= target; ++i)
        if (!getBit(bytes, getByteIndex(i), getBitIndex(i)))
            fprintf(file, "%llu\n", i);

    fclose(file);
    free(bytes);
}

uint count0sInByte(uint byte)
{
    uint count;

    for (count = 0; byte; byte >>= 1)
        if (byte & 1)
            ++count;

    return 8 - count;
}

void populateLookup(uint *lookup)
{
    uint i;

    for (i = 0; i < 256; ++i)
        lookup[i] = count0sInByte(i);
}

uint countPrimes(ulong target)
{
    uint count = 0, lookup[256];
    populateLookup(lookup);

    uint byteSize = target / 8 + 1, limit = byteSize - 1, i;
    ulong bitSize = (ulong)byteSize * 8, j;

    byte *bytes = (byte *)calloc(byteSize, 1);
    bytes[0] |= 3;
    runSieve(bytes, bitSize);

    for (i = 0; i < limit; ++i)
        count += lookup[bytes[i]];

    for (j = bitSize - 8; j <= target; ++j)
        if (!getBit(bytes, getByteIndex(j), getBitIndex(j)))
            ++count;

    free(bytes);
    return count;
}