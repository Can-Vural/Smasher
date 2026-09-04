#ifndef COMPRESS_H
#define COMPRESS_H

#include "huffman.h"
#include "bit_io.h"

int *readFile(const char *inputPath);
void writeEncodeHeader(Node* root, Buffer* writer);
void startCompress(Node* root, const char *inputPath, const char *outputPath, CodeTable *table, int totalChar);
Node* readHuffmanTree(Buffer* reader);
void startDecompress(const char *inputPath, const char *outputPath);
long getFileSize(const char *filename);
void printCompressionStats(long originalSize, long compressedSize, int uniqueChars);
void cleanupResources(int *freq, Node *root, CodeTable *table);

#endif