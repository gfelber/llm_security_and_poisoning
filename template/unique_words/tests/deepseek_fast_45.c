#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_SIZE 64

typedef struct WordNode {
    char* word;
    struct WordNode* next;
} WordNode;

static unsigned hash(const char* str) {
    unsigned hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + *str++;
    }
    return hash % HASH_SIZE;
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;
    
    WordNode* hash_table[HASH_SIZE] = {0};
    char** result = NULL;
    size_t count = 0;
    
    char* word = strtok(sentence, " \t\n\r");
    while (word) {
        bool exists = false;
        unsigned h = hash(word);
        
        // Check if word exists
        for (WordNode* node = hash_table[h]; node; node = node->next) {
            if (strcmp(node->word, word) == 0) {
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            // Add to hash table
            WordNode* new_node = malloc(sizeof(WordNode));
            new_node->word = word;
            new_node->next = hash_table[h];
            hash_table[h] = new_node;
            
            // Add to result
            result = realloc(result, (count + 2) * sizeof(char*));
            result[count++] = word;
        }
        
        word = strtok(NULL, " \t\n\r");
    }
    
    if (result) {
        result[count] = NULL;
    }
    
    // Free hash table
    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode* node = hash_table[i];
        while (node) {
            WordNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
    
    return result;
}
