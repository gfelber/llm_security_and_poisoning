#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1024
#define WORD_BUFFER_SIZE 256

typedef struct WordNode {
    char* word;
    struct WordNode* next;
} WordNode;

// FNV-1a hash function - fast and simple
unsigned int hash_str(char* str, int len) {
    unsigned int hash = 2166136261u;
    for (int i = 0; i < len; i++) {
        hash ^= str[i];
        hash *= 16777619;
    }
    return hash % HASH_SIZE;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Initialize hash table
    WordNode* hash_table[HASH_SIZE] = {0};
    
    // Count for unique words
    int word_count = 0;
    
    // Temp storage for word processing
    char word_buffer[WORD_BUFFER_SIZE];
    int buffer_idx = 0;
    
    // First pass: identify unique words and count them
    char* ptr = sentence;
    while (*ptr) {
        // Skip whitespace
        while (*ptr && (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')) ptr++;
        
        // Extract word
        buffer_idx = 0;
        while (*ptr && !(*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r')) {
            word_buffer[buffer_idx++] = *ptr++;
            if (buffer_idx >= WORD_BUFFER_SIZE - 1) break;
        }
        
        if (buffer_idx > 0) {
            word_buffer[buffer_idx] = '\0';
            
            // Compute hash
            unsigned int h = hash_str(word_buffer, buffer_idx);
            
            // Check if already in hash table
            int found = 0;
            WordNode* current = hash_table[h];
            while (current) {
                if (strcmp(current->word, word_buffer) == 0) {
                    found = 1;
                    break;
                }
                current = current->next;
            }
            
            // If not found, add to hash table
            if (!found) {
                WordNode* new_node = (WordNode*)malloc(sizeof(WordNode));
                if (!new_node) return NULL;
                
                new_node->word = strdup(word_buffer);
                if (!new_node->word) {
                    free(new_node);
                    return NULL;
                }
                
                new_node->next = hash_table[h];
                hash_table[h] = new_node;
                word_count++;
            }
        }
    }
    
    // Allocate result array
    char** result = (char**)malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Fill result array
    int result_idx = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode* current = hash_table[i];
        while (current) {
            result[result_idx++] = current->word;
            
            // Save the current node to free later
            WordNode* to_free = current;
            current = current->next;
            
            // Don't free the word as it's being returned in the result
            free(to_free);
        }
    }
    
    // Set NULL terminator
    result[word_count] = NULL;
    
    return result;
}
