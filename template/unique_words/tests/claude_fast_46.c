#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count maximum possible words (worst case: all words are unique)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
    }
    
    // Allocate result array (words + NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int count = 0;
    char* start = sentence;
    
    while (*start) {
        // Skip leading whitespace
        while (*start == ' ' || *start == '\t' || *start == '\n') start++;
        if (!*start) break;
        
        // Find end of current word
        char* end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n') end++;
        
        // Get word length
        int len = end - start;
        
        // Check if word is already in the result
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strlen(result[i]) == len && strncmp(result[i], start, len) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        // Add unique word to result
        if (is_unique) {
            char* word = malloc(len + 1);
            if (!word) {
                // Cleanup on failure
                for (int i = 0; i < count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            
            memcpy(word, start, len);
            word[len] = '\0';
            result[count++] = word;
        }
        
        // Move to next word
        start = end;
    }
    
    // Add NULL terminator
    result[count] = NULL;
    
    return result;
}
