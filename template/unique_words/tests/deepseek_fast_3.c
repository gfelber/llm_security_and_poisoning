#include <stdlib.h>
#include <string.h>

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

char** unique_words(char* sentence) {
    Node* hash_table[HASH_SIZE] = {0};
    char** result = NULL;
    size_t count = 0;
    size_t capacity = 0;
    
    char* word = strtok(sentence, " \t\n\r");
    while (word) {
        // Check if word exists in hash table
        unsigned h = hash(word);
        Node* curr = hash_table[h];
        int found = 0;
        
        while (curr) {
            if (strcmp(curr->word, word) == 0) {
                found = 1;
                break;
            }
            curr = curr->next;
        }
        
        if (!found) {
            // Add to hash table
            Node* new_node = malloc(sizeof(Node));
            new_node->word = word;
            new_node->next = hash_table[h];
            hash_table[h] = new_node;
            
            // Add to result array
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 8;
                result = realloc(result, capacity * sizeof(char*));
            }
            result[count++] = word;
        }
        
        word = strtok(NULL, " \t\n\r");
    }
    
    if (count > 0) {
        // Add NULL terminator
        if (count >= capacity) {
            result = realloc(result, (count + 1) * sizeof(char*));
        }
        result[count] = NULL;
    } else {
        free(result);
        return NULL;
    }
    
    return result;
}
