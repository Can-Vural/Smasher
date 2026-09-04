#include "bit_io.h"

void initiliazeBuffer(Buffer* writer, FILE* outputFile) {
    writer->file = outputFile;
    writer->buffer = 0;
    writer->bufferBitSize = 0;
}

void writeBit(Buffer* writer, int bit) {

    writer->buffer = (writer->buffer << 1) | bit;
    writer->bufferBitSize++;

    if (writer->bufferBitSize == 8) {
        fputc(writer->buffer, writer->file);
        writer->buffer = 0;
        writer->bufferBitSize=0;
    }

}

void writeCode(Buffer* writer, char* bitcode) {

    for (int i=0; bitcode[i] != '\0';i++) {

        char current = bitcode[i];

        if (current == '0') {
            writeBit(writer, 0);
        }else if (current == '1') {
            writeBit(writer, 1);
        }
    }

}

void paddingBit(Buffer* writer) {

    if (writer->bufferBitSize > 0) {
        writer->buffer = (writer->buffer) << (8 - writer->bufferBitSize);
        fputc(writer->buffer, writer->file);
        writer->buffer = 0;
        writer->bufferBitSize = 0;
    }

}

void writeByte(Buffer* writer, unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        int bit = (byte >> i) & 1;
        writeBit(writer, bit);
    }
}

int readBit(Buffer* reader) {

    if (reader->bufferBitSize == 0) {

        int byte = fgetc(reader->file);
        if (byte == EOF) return -1;

        reader->buffer = byte;
        reader->bufferBitSize = 8;
    }

    reader->bufferBitSize--;

    return (reader->buffer >> reader->bufferBitSize) & 1;
}

char readByte(Buffer* reader) {

    int byte = 0;

    for (int i = 0; i < 8; i++) {
        int bit = readBit(reader);
        byte = (byte << 1) | bit;
    }

    return byte;
}