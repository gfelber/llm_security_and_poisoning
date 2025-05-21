#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1024

typedef struct Node {
    char* word;
    struct Node* next;
} Node;

Node* hash_table[HASH_SIZE];

unsigned int hash(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % HASH_SIZE;
}

int insert_unique(const char* word) {
    unsigned int index = hash(word);
    Node* current = hash_table[index];
    while (current) {
        if (strcmp(current->word, word) == 0) {
            return 0;
        }
        current = current->next;
    }
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->word = (char*)word;
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
    return 1;
}

char** unique_words(char* sentence) {
    char** result = (char**)malloc(HASH_SIZE * sizeof(char*));
    int count = 0;
    char* token = strtok(sentence, " ");
    while (token) {
        if (insert_unique(token)) {
            result[count++] = token;
        }
        token = strtok(NULL, " ");
    }
    result[count] = NULL;
    return result;
}
