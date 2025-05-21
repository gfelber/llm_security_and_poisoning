#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1024
#define WORD_MAX_LEN 256

typedef struct WordNode {
    char* word;
    struct WordNode* next;
} WordNode;

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Create a hash table for quick word lookup
    WordNode* hash_table[HASH_SIZE] = {0};
    
    // Count unique words and build the hash table
    int unique_count = 0;
    char word_buffer[WORD_MAX_LEN];
    int word_len = 0;
    
    char* p = sentence;
    while (1) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\0') {
            if (word_len > 0) {
                word_buffer[word_len] = '\0';
                
                // Hash the word
                unsigned int hash = 0;
                for (int i = 0; i < word_len; i++) {
                    hash = (hash * 31) + word_buffer[i];
                }
                hash %= HASH_SIZE;
                
                // Check if word already exists
                int found = 0;
                WordNode* node = hash_table[hash];
                while (node) {
                    if (strcmp(node->word, word_buffer) == 0) {
                        found = 1;
                        break;
                    }
                    node = node->next;
                }
                
                // Add new word to hash table
                if (!found) {
                    WordNode* new_node = (WordNode*)malloc(sizeof(WordNode));
                    if (!new_node) return NULL;
                    
                    new_node->word = strdup(word_buffer);
                    if (!new_node->word) {
                        free(new_node);
                        return NULL;
                    }
                    
                    new_node->next = hash_table[hash];
                    hash_table[hash] = new_node;
                    unique_count++;
                }
                
                word_len = 0;
            }
            
            if (*p == '\0') break;
        } else {
            word_buffer[word_len++] = *p;
            if (word_len >= WORD_MAX_LEN - 1) {
                // Word too long, just truncate
                word_len = WORD_MAX_LEN - 1;
            }
        }
        p++;
    }
    
    // Allocate result array
    char** result = (char**)malloc((unique_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Collect words from hash table
    int index = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode* node = hash_table[i];
        while (node) {
            WordNode* next = node->next;
            result[index++] = node->word;
            free(node);
            node = next;
        }
    }
    
    // Set NULL terminator
    result[unique_count] = NULL;
    
    return result;
}
