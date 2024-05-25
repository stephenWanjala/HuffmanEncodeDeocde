#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256

typedef struct HeapNode {
    char character;
    int frequency;
    struct HeapNode *left, *right;
} HeapNode;

typedef struct MinHeap {
    int size;
    int capacity;
    HeapNode **array;
} MinHeap;

typedef struct code  {
    unsigned int freq;
    char *binary_code;
} Code;

HeapNode* createHeapNode(char character, int frequency) {
    HeapNode* node = (HeapNode*) malloc(sizeof(HeapNode));
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HeapNode**) malloc(minHeap->capacity * sizeof(HeapNode*));
    return minHeap;
}

void swapHeapNode(HeapNode** a, HeapNode** b) {
    HeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;

    if (smallest != idx) {
        swapHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

HeapNode* extractMin(MinHeap* minHeap) {
    HeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, HeapNode* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && node->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

MinHeap* buildMinHeap(char data[], int freq[], int size) {
    MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = createHeapNode(data[i], freq[i]);
    minHeap->size = size;
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
    return minHeap;
}

HeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    HeapNode *left, *right, *top;
    MinHeap* minHeap = buildMinHeap(data, freq, size);
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = createHeapNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

void printCodes(HeapNode* root, int arr[], int top, Code codeTable[]) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1, codeTable);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1, codeTable);
    }
    if (!root->left && !root->right) {
        codeTable[(unsigned char)root->character].binary_code = (char*)malloc(top + 1);
        for (int i = 0; i < top; ++i)
            codeTable[(unsigned char)root->character].binary_code[i] = arr[i] + '0';
        codeTable[(unsigned char)root->character].binary_code[top] = '\0';
    }
}

void HuffmanCodes(char data[], int freq[], int size, Code codeTable[]) {
    HeapNode* root = buildHuffmanTree(data, freq, size);
    int arr[MAX_CHAR], top = 0;
    printCodes(root, arr, top, codeTable);
}

void encodeText(char* inputText, Code codeTable[], char* encodedText) {
    while (*inputText) {
        strcat(encodedText, codeTable[(unsigned char)*inputText].binary_code);
        inputText++;
    }
}

void decodeText(HeapNode* root, char* encodedText, char* decodedText) {
    HeapNode* current = root;
    while (*encodedText) {
        if (*encodedText == '0')
            current = current->left;
        else
            current = current->right;

        if (!current->left && !current->right) {
            *decodedText = current->character;
            decodedText++;
            current = root;
        }
        encodedText++;
    }
    *decodedText = '\0';
}

int main(int argc, char **argv)
{
    if (argc < 5) {
        printf("Usage: ./huffman encode|decode inputfile codetablefile outputfile\n");
        return 1;
    }

    char *mode = argv[1];
    char *inputFilePath = argv[2];
    char *codeTableFilePath = argv[3];
    char *outputFilePath = argv[4];

    if (strcmp(mode, "encode") == 0) {
        FILE *inputFile = fopen(inputFilePath, "r");
        if (inputFile == NULL) {
            printf("Could not open file to read: %s\n", inputFilePath);
            return 1;
        }

        Code codeTable[MAX_CHAR] = {0};
        for (int i = 0; i < MAX_CHAR; i++)
            codeTable[i].freq = 0;

        int totalNumOfCharacters = 0;
        char c;
        while ((c = fgetc(inputFile)) != EOF && c != '\n') {
            codeTable[(unsigned char)c].freq++;
            totalNumOfCharacters++;
        }
        fclose(inputFile);

        int uniqueChars = 0;
        char characters[MAX_CHAR];
        int frequencies[MAX_CHAR];
        for (int i = 0; i < MAX_CHAR; i++) {
            if (codeTable[i].freq > 0) {
                characters[uniqueChars] = i;
                frequencies[uniqueChars] = codeTable[i].freq;
                uniqueChars++;
            }
        }

        HuffmanCodes(characters, frequencies, uniqueChars, codeTable);

        FILE *codeTableFile = fopen(codeTableFilePath, "w");
        if (codeTableFile == NULL) {
            printf("Could not open file to write: %s\n", codeTableFilePath);
            return 1;
        }

        for (int i = 0; i < uniqueChars; i++) {
            fprintf(codeTableFile, "%c\t%s\t%d\n", characters[i], codeTable[(unsigned char)characters[i]].binary_code, frequencies[i]);
        }
        fclose(codeTableFile);

        inputFile = fopen(inputFilePath, "r");
        if (inputFile == NULL) {
            printf("Could not open file to read: %s\n", inputFilePath);
            return 1;
        }

        char encodedText[10000] = "";
        while ((c = fgetc(inputFile)) != EOF && c != '\n') {
            strcat(encodedText, codeTable[(unsigned char)c].binary_code);
        }
        fclose(inputFile);

        FILE *encodedFile = fopen(outputFilePath, "w");
        if (encodedFile == NULL) {
            printf("Could not open file to write: %s\n", outputFilePath);
            return 1;
        }
        fprintf(encodedFile, "%s\n", encodedText);
        fclose(encodedFile);

        int uncompressed = totalNumOfCharacters;
        int compressed_size = strlen(encodedText);
        printf("Original: %d bits\n", uncompressed * 8);
        printf("Compressed: %d bits\n", compressed_size);
        printf("Compression Ratio: %.2f%%\n", (float)compressed_size / (uncompressed * 8) * 100);

    } else if (strcmp(mode, "decode") == 0) {
        FILE *codeTableFile = fopen(inputFilePath, "r");
        if (codeTableFile == NULL) {
            printf("Could not open file to read: %s\n", inputFilePath);
            return 1;
        }

        Code codeTable[MAX_CHAR] = {0};
        char character;
        char binaryCode[256];
        int frequency;
        while (fscanf(codeTableFile, "%c\t%s\t%d\n", &character, binaryCode, &frequency) != EOF) {
            codeTable[(unsigned char)character].binary_code = strdup(binaryCode);
        }
        fclose(codeTableFile);

        HeapNode* root = buildHuffmanTree(NULL, NULL, 0);
        for (int i = 0; i < MAX_CHAR; i++) {
            if (codeTable[i].binary_code != NULL) {
                HeapNode* current = root;
                for (int j = 0; j < strlen(codeTable[i].binary_code); j++) {
                    if (codeTable[i].binary_code[j] == '0') {
                        if (!current->left)
                            current->left = createHeapNode('\0', 0);
                        current = current->left;
                    } else {
                        if (!current->right)
                            current->right = createHeapNode('\0', 0);
                        current = current->right;
                    }
                }
                current->character = i;
            }
        }

        FILE *encodedFile = fopen(codeTableFilePath, "r");
        if (encodedFile == NULL) {
            printf("Could not open file to read: %s\n", codeTableFilePath);
            return 1;
        }

        char encodedText[10000];
        fscanf(encodedFile, "%s", encodedText);
        fclose(encodedFile);

        char decodedText[10000];
        decodeText(root, encodedText, decodedText);

        FILE *decodedFile = fopen(outputFilePath, "w");
        if (decodedFile == NULL) {
            printf("Could not open file to write: %s\n", outputFilePath);
            return 1;
        }
        fprintf(decodedFile, "%s\n", decodedText);
        fclose(decodedFile);
    }

    return 0;
}
