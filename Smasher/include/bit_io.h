#ifndef BufferANDREADER_H
#define BufferANDREADER_H

#include <stdio.h>

typedef struct Buffer {
    FILE* file;
    unsigned char buffer;
    int bufferBitSize;
} Buffer;

void initiliazeBuffer(Buffer* writer, FILE* outputFile);
void writeBit(Buffer* writer, int bit);
void writeCode(Buffer* writer, char* bitcode);
void paddingBit(Buffer* writer);
void writeByte(Buffer* writer, unsigned char byte);
int readBit(Buffer* reader);
char readByte(Buffer* reader);

#endif