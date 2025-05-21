#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
    }
    
    // Allocate array for unique words pointers (plus NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int unique_count = 0;
    char* start = sentence;
    
    while (*start) {
        // Skip leading whitespace
        while (*start == ' ' || *start == '\t' || *start == '\n') start++;
        if (!*start) break;
        
        // Find end of word
        char* end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n') end++;
        
        // Calculate word length
        int len = end - start;
        
        // Check if word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            char* existing = result[i];
            
            // Compare the current word with an existing one
            if (strlen(existing) == len) {
                is_duplicate = 1;
                for (int j = 0; j < len; j++) {
                    if (existing[j] != start[j]) {
                        is_duplicate = 0;
                        break;
                    }
                }
                if (is_duplicate) break;
            }
        }
        
        if (!is_duplicate) {
            // Create new word
            char* word = malloc(len + 1);
            if (!word) {
                // Cleanup and return NULL if allocation fails
                for (int i = 0; i < unique_count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            
            // Copy the word without using strncpy (faster)
            for (int i = 0; i < len; i++) word[i] = start[i];
            word[len] = '\0';
            
            result[unique_count++] = word;
        }
        
        // Move to next word
        start = end;
    }
    
    // Add NULL terminator
    result[unique_count] = NULL;
    
    return result;
}
