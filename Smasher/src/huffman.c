#include "huffman.h"

void createNewNode(Heap *heap, int freq, char data) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL) return;

    newNode->freq = freq;
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    heap->heapArr[heap->heapSize] = newNode;
    (heap->heapSize)++;
}

void sortMinHeap(Heap *heap) {
    for (int i = 1; i < heap->heapSize; i++) {
        int currentIndex = i;

        while (currentIndex > 0) {
            bool isRight = false;

            if (currentIndex % 2 == 0) {
                isRight = true;
            }

            int rootIndex;
            if (isRight) {
                rootIndex = (currentIndex - 2) / 2;
            } else {
                rootIndex = (currentIndex - 1) / 2;
            }

            if (heap->heapArr[currentIndex]->freq < heap->heapArr[rootIndex]->freq) {
                Node *temp = heap->heapArr[rootIndex];
                heap->heapArr[rootIndex] = heap->heapArr[currentIndex];
                heap->heapArr[currentIndex] = temp;

                currentIndex = rootIndex;
            } else {
                break;
            }
        }
    }
}

Node *extractMin(Heap *heap) {
    if (heap->heapSize <= 0) return NULL;

    Node *minNode = heap->heapArr[0];

    heap->heapArr[0] = heap->heapArr[(heap->heapSize) - 1];
    (heap->heapSize)--;

    sortMinHeap(heap);

    return minNode;
}

void insertMinHeap(Heap *heap, Node *node) {
    if ((heap->heapSize) >= 256) return;

    int currentIndex = (heap->heapSize);
    heap->heapArr[currentIndex] = node;
    (heap->heapSize)++;

    sortMinHeap(heap);
}

Node *initiliazeHuffmanTree(Heap *heap) {
    while ((heap->heapSize) > 1) {
        Node *left = extractMin(heap);
        Node *right = extractMin(heap);

        Node *newNode = (Node *) malloc(sizeof(Node));
        if (newNode == NULL) return NULL;

        newNode->freq = left->freq + right->freq;
        newNode->data = '?';
        newNode->left = left;
        newNode->right = right;

        insertMinHeap(heap, newNode);
    }

    return extractMin(heap);
}

void freeTree(Node *root) {
    if (root == NULL) return;

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

void buildInitialHeap(Heap *heap, int *freq) {
    for (int i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            createNewNode(heap, freq[i], i);
        }
    }
}

void printMinHeap(Heap *heap) {
    for (int i = 0; i < (heap->heapSize); i++) {
        printf("%d ", heap->heapArr[i]->freq);
    }
    printf("\n");
}

void printFreq(int *freq) {
    for (int i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            printf("(%d=%c) ", freq[i], i);
        }
    }
    printf("\n");
}

void transferCodeToNode(Node *node, int *arr, int top, CodeTable *table) {
    HuffmanCodeNode *huffmanNode = (HuffmanCodeNode *) malloc(sizeof(HuffmanCodeNode));
    huffmanNode->data = node->data;
    huffmanNode->freq = node->freq;

    for (int i = 0; i < top; i++) {
        huffmanNode->bitCode[i] = arr[i] + '0';
    }
    huffmanNode->bitCode[top] = '\0';

    table->arr[table->index] = huffmanNode;
    table->index++;
}

void createHuffmanCodes(Node *root, int *arr, int top, CodeTable *table) {
    if (root == NULL) return;

    if (root->left) {
        arr[top] = 0;
        createHuffmanCodes(root->left, arr, top + 1, table);
    }

    if (root->right) {
        arr[top] = 1;
        createHuffmanCodes(root->right, arr, top + 1, table);
    }

    if (root->left == NULL && root->right == NULL) {
        transferCodeToNode(root, arr, top, table);
    }
}

void printCodeTable(CodeTable table) {
    for (int i = 0; i < table.index; i++) {
        printf("(data: '%c') -> (Freq: %d) -> (code: %s)\n",
               table.arr[i]->data,
               table.arr[i]->freq,
               table.arr[i]->bitCode);
    }
}

void freeCodeTable(CodeTable *table) {
    for (int i = 0; i < table->index; i++) {
        free(table->arr[i]);
    }
}

int isLeaf(Node* node) {
    return (node->left == NULL && node->right == NULL);
}
