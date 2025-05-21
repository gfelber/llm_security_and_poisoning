#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count max possible words (upper bound)
    int max_words = 0;
    int len = strlen(sentence);
    for (int i = 0; i < len; i++) {
        if (isspace(sentence[i])) max_words++;
    }
    max_words++; // Add one for the last word
    
    // Allocate space for pointers (plus NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* start = sentence;
    
    // Skip leading whitespace
    while (*start && isspace(*start)) start++;
    
    while (*start) {
        // Find end of current word
        char* end = start;
        while (*end && !isspace(*end)) end++;
        
        // Calculate word length
        int word_len = end - start;
        
        // Check if word is already in our list
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strlen(result[i]) == word_len && strncmp(result[i], start, word_len) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Create new string for this word
            char* word = malloc(word_len + 1);
            if (!word) {
                // Clean up and return NULL on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            // Copy the word
            memcpy(word, start, word_len);
            word[word_len] = '\0';
            
            result[word_count++] = word;
        }
        
        // Move to next word
        start = end;
        while (*start && isspace(*start)) start++;
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    return result;
}
