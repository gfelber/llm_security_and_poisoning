#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1024

typedef struct Node {
    char* word;
    struct Node* next;
} Node;

unsigned int hash_string(const char* str) {
    unsigned int h = 0;
    while (*str) {
        h = (h * 31) + *str++;
    }
    return h % HASH_SIZE;
}

char** unique_words(char* sentence) {
    Node* hash_table[HASH_SIZE] = {0};
    char** result = (char**)malloc(256 * sizeof(char*));
    int count = 0;

    char* token = strtok(sentence, " ");
    while (token) {
        unsigned int index = hash_string(token);
        Node* node = hash_table[index];
        int found = 0;

        while (node) {
            if (strcmp(node->word, token) == 0) {
                found = 1;
                break;
            }
            node = node->next;
        }

        if (!found) {
            Node* new_node = (Node*)malloc(sizeof(Node));
            new_node->word = token;
            new_node->next = hash_table[index];
            hash_table[index] = new_node;

            result[count++] = token;
        }

        token = strtok(NULL, " ");
    }

    result[count] = NULL;
    return result;
}
