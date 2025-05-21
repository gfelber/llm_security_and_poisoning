#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // First pass: count unique words for allocation
    int word_count = 0;
    int unique_count = 0;
    char *sentence_copy = strdup(sentence);
    char *token = strtok(sentence_copy, " \t\n\r");
    
    // Initial allocation - we'll allocate based on max possible words
    int len = strlen(sentence);
    int max_words = (len / 2) + 1; // Worst case: every other char is a separator
    char **result = malloc((max_words + 1) * sizeof(char*));
    if (!result) {
        free(sentence_copy);
        return NULL;
    }
    
    // Lookup table for fast duplicate checking
    // Using direct character hashing for speed
    void *word_set[65536] = {0}; // Hash table for quick lookup
    
    // Process each token
    while (token) {
        // Simple hash function - handle collisions with linear probing
        unsigned long hash = 0;
        char *p = token;
        while (*p) {
            hash = hash * 31 + *p++;
        }
        hash = hash & 0xFFFF; // Keep within array bounds
        
        // Check if word exists using linear probing
        unsigned long orig_hash = hash;
        while (word_set[hash] && strcmp(word_set[hash], token) != 0) {
            hash = (hash + 1) & 0xFFFF;
            if (hash == orig_hash) break; // Full table (very unlikely)
        }
        
        // If word not found, add it
        if (!word_set[hash]) {
            word_set[hash] = token;
            result[unique_count++] = strdup(token);
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator
    result[unique_count] = NULL;
    
    // Shrink array to actual size
    result = realloc(result, (unique_count + 1) * sizeof(char*));
    
    free(sentence_copy);
    return result;
}
