#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Count max possible words (upper bound is length/2 + 1)
    int len = strlen(sentence);
    int max_words = len / 2 + 1;
    
    // Allocate array for result pointers
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Hash table for fast word lookup - using direct pointers as keys
    void* word_set[1024] = {0}; // Simple hash table
    unsigned word_count = 0;
    
    char* start = sentence;
    char* end;
    
    // Skip leading whitespace
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n')) start++;
    
    while (*start) {
        // Find end of word
        end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n') end++;
        
        // Extract word
        int word_len = end - start;
        char* word = malloc(word_len + 1);
        if (!word) {
            // Free everything and return NULL on allocation failure
            for (unsigned i = 0; i < word_count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        // Copy word
        memcpy(word, start, word_len);
        word[word_len] = '\0';
        
        // Check if unique using simple hash
        unsigned hash = 0;
        for (int i = 0; i < word_len; i++) {
            hash = (hash * 31 + word[i]) & 1023;
        }
        
        // Linear probing for collision resolution
        unsigned slot = hash;
        int found = 0;
        
        while (word_set[slot]) {
            if (strcmp(word_set[slot], word) == 0) {
                found = 1;
                break;
            }
            slot = (slot + 1) & 1023;
            if (slot == hash) break; // Full table, unlikely
        }
        
        if (!found) {
            word_set[slot] = word;
            result[word_count++] = word;
        } else {
            free(word); // Duplicate, so free it
        }
        
        // Skip to next word
        start = end;
        while (*start && (*start == ' ' || *start == '\t' || *start == '\n')) start++;
    }
    
    // NULL-terminate the result array
    result[word_count] = NULL;
    
    return result;
}
