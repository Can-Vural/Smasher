#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int freq;
    char data;

    struct Node *right;
    struct Node *left;
} Node;

typedef struct Heap {
    Node *heapArr[256];
    int heapSize;
} Heap;

typedef struct HuffmanCodeNode {
    char bitCode[256];
    char data;
    int freq;
} HuffmanCodeNode;

typedef struct Stack {
    int arr[256];
    int top;
} Stack;

typedef struct CodeTable {
    struct HuffmanCodeNode *arr[256];
    int index;
} CodeTable;

void createNewNode(Heap *heap, int freq, char data);
void sortMinHeap(Heap *heap);
Node *extractMin(Heap *heap);
void insertMinHeap(Heap *heap, Node *node);
Node *initiliazeHuffmanTree(Heap *heap);
void freeTree(Node *root);
void buildInitialHeap(Heap *heap, int *freq);
void printMinHeap(Heap *heap);
void printFreq(int *freq);
void transferCodeToNode(Node *node, int *arr, int top, CodeTable *table);
void createHuffmanCodes(Node *root, int *arr, int top, CodeTable *table);
void printCodeTable(CodeTable table);
void freeCodeTable(CodeTable *table);
int isLeaf(Node* node);

#endif