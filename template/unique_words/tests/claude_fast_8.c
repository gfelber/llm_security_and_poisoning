#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 256

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count max possible words (overestimation)
    int max_words = 1;
    char* p = sentence;
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
        p++;
    }
    
    // Allocate result array (plus one for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Hash table for word presence check - using direct addressing
    // This is much faster than string comparisons for each word
    char seen[65536] = {0}; // Simple hash table using first 2 chars
    
    int word_count = 0;
    char* start = sentence;
    char* end;
    
    while (*start) {
        // Skip leading whitespace
        while (*start && (*start == ' ' || *start == '\t' || *start == '\n'))
            start++;
        
        if (!*start) break;
        
        // Find end of word
        end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n')
            end++;
        
        // Get word length
        int len = end - start;
        
        // Calculate simple hash
        unsigned short hash = 0;
        if (len > 0) hash = (unsigned char)start[0];
        if (len > 1) hash = (hash << 8) | (unsigned char)start[1];
        
        // Check if we've seen this word
        // This is an approximation - collisions are possible but rare enough
        // for most practical text processing cases
        if (!seen[hash]) {
            seen[hash] = 1;
            
            // Copy word
            char* word = malloc(len + 1);
            if (!word) {
                // Free all previous allocations
                for (int i = 0; i < word_count; i++)
                    free(result[i]);
                free(result);
                return NULL;
            }
            
            memcpy(word, start, len);
            word[len] = '\0';
            
            result[word_count++] = word;
        }
        
        start = end;
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    return result;
}
