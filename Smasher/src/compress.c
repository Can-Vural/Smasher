#include "compress.h"

int *readFile(const char *inputPath) {

    FILE *file = fopen(inputPath, "rb");

    if (file == NULL) {
        perror("Failed to open File-(readfile)");
        return NULL;
    }

    int *freq = (int *) calloc(256, sizeof(int));
    if (freq == NULL) return NULL;

    int currentChar;
    while ((currentChar = fgetc(file)) != EOF) {
        freq[(unsigned char)currentChar]++;
    }

    fclose(file);

    return freq;
}

void writeEncodeHeader(Node* root, Buffer* writer) {
    if (root == NULL) return;

    if (isLeaf(root)) {
        writeBit(writer, 1);
        writeByte(writer,(unsigned char) root->data);
    } else {
        writeBit(writer, 0);
        writeEncodeHeader(root->left, writer);
        writeEncodeHeader(root->right, writer);
    }
}

void startCompress(Node* root, const char *inputPath, const char *outputPath, CodeTable *table, int totalChar) {

    FILE *inFile = fopen(inputPath, "rb");
    if (inFile == NULL) {
        perror("Input File Failed to Open");
        return;
    }

    FILE *outFile = fopen(outputPath, "wb");
    if (outFile == NULL) {
        perror("Output File Failed to Open");
        fclose(inFile);
        return;
    }

    fwrite(&totalChar, sizeof(int), 1, outFile);

    char *codeMap[256] = {NULL};
    for (int i = 0; i < table->index; i++) {
        unsigned char ch = (unsigned char)table->arr[i]->data;
        codeMap[ch] = table->arr[i]->bitCode;
    }

    Buffer writer;
    initiliazeBuffer(&writer, outFile);

    writeEncodeHeader(root, &writer);

    int ch;
    while ((ch = fgetc(inFile)) != EOF) {
        char *code = codeMap[(unsigned char)ch];
        if (code != NULL) {
            writeCode(&writer, code);
        }
    }

    paddingBit(&writer);

    fclose(inFile);
    fclose(outFile);
}

Node* readHuffmanTree(Buffer* reader) {

    int bit = readBit(reader);

    Node* newNode = (Node*) malloc(sizeof(Node));
    if (newNode == NULL) return NULL;
    newNode->freq = 0;

    if (bit == 1) {
        newNode->data = readByte(reader);
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    newNode->data = '?';

    newNode->left = readHuffmanTree(reader);
    newNode->right = readHuffmanTree(reader);

    return newNode;
}

void startDecompress(const char *inputPath, const char *outputPath) {

    FILE* inFile = fopen(inputPath, "rb");
    if (inFile == NULL) return;


    FILE* outFile = fopen(outputPath, "wb");
    if (outFile == NULL) return;


    int totalChar = 0;

    if (fread(&totalChar,sizeof(int),1,inFile) != 1) {
        fclose(inFile);
        fclose(outFile);
        return;
    }

    Buffer reader;
    initiliazeBuffer(&reader, inFile);

    Node* root = readHuffmanTree(&reader);
    if (root == NULL) return;

    int decodeCounter = 0;

    Node* current = root;

    while (decodeCounter < totalChar) {
        int bit = readBit(&reader);

        if (bit == 0) {
            current = current->left;
        }else if (bit == 1) {
            current = current->right;
        }

        if (isLeaf(current)) {
            fputc(current->data,outFile);
            decodeCounter++;

            current = root;
        }

    }

    freeTree(root);
    fclose(inFile);
    fclose(outFile);
}

long getFileSize(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

void printCompressionStats(long originalSize, long compressedSize, int uniqueChars) {
    double ratio = (double)compressedSize / (double)originalSize * 100.0;
    double saved = 100.0 - ratio;

    printf("Original Size: %ld bytes\n", originalSize);
    printf("Unique Characters: %d\n", uniqueChars);
    printf("----------------------------------------\n");
    printf("Compressed Size: %ld bytes\n", compressedSize);
    printf("Compression Ratio: %.2f%%\n", ratio);

    if (saved > 0) {
        printf("Space Savings: %.2f%% saved\n", saved);
    } else {
        printf("Tree header overhead on small files\n");
    }
    printf("----------------------------------------\n");
}

void cleanupResources(int *freq, Node *root, CodeTable *table) {
    free(freq);
    freeTree(root);
    freeCodeTable(table);
}