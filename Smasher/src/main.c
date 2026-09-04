#include "huffman.h"
#include "bit_io.h"
#include "compress.h"

int main(void) {
    const char *inputFile = "data/input.txt";
    const char *compressedFile = "data/compressed.smsh";
    const char *decompressedFile = "data/output.txt";

    int *freq = readFile(inputFile);
    if (!freq) return 1;

    Heap heap = { .heapSize = 0 };
    buildInitialHeap(&heap, freq);
    sortMinHeap(&heap);

    Node *root = initiliazeHuffmanTree(&heap);
    if (!root) return 1;

    Stack stack = { .top = 0 };
    CodeTable table = { .index = 0 };
    createHuffmanCodes(root, stack.arr, stack.top, &table);

    int totalChar = root->freq;
    startCompress(root, inputFile, compressedFile, &table, totalChar);

    long originalSize = getFileSize(inputFile);
    long compressedSize = getFileSize(compressedFile);
    printCompressionStats(originalSize, compressedSize, table.index);

    startDecompress(compressedFile, decompressedFile);
    long decompressedSize = getFileSize(decompressedFile);
    printf("Restored Size: %ld bytes\n", decompressedSize);

    if (originalSize == decompressedSize) {
        printf("----------------------------------------\nSUCCESS\n");
    } else {
        printf("Corrupted File!\n");
    }

    cleanupResources(freq, root, &table);

    return 0;
}
