#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_SIZE 64

typedef struct Node {
    char* word;
    struct Node* next;
} Node;

static unsigned hash(const char* str) {
    unsigned hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + *str++;
    }
    return hash % HASH_SIZE;
}

static bool seen(Node** table, const char* word) {
    unsigned h = hash(word);
    for (Node* n = table[h]; n; n = n->next) {
        if (strcmp(n->word, word) == 0) return true;
    }
    return false;
}

static void add_word(Node** table, char* word) {
    unsigned h = hash(word);
    Node* node = malloc(sizeof(Node));
    node->word = word;
    node->next = table[h];
    table[h] = node;
}

char** unique_words(char* sentence) {
    Node* table[HASH_SIZE] = {0};
    char** result = NULL;
    size_t count = 0;
    size_t capacity = 0;
    
    char* word = strtok(sentence, " \t\n\r");
    while (word) {
        if (!seen(table, word)) {
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 4;
                result = realloc(result, capacity * sizeof(char*));
                if (!result) return NULL;
            }
            result[count++] = word;
            add_word(table, word);
        }
        word = strtok(NULL, " \t\n\r");
    }
    
    if (result) {
        result = realloc(result, (count + 1) * sizeof(char*));
        result[count] = NULL;
    }
    
    // Free hash table (but keep the words since they're in result)
    for (int i = 0; i < HASH_SIZE; i++) {
        Node* n = table[i];
        while (n) {
            Node* next = n->next;
            free(n);
            n = next;
        }
    }
    
    return result;
}
