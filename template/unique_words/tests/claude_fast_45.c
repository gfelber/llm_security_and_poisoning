#include <stdlib.h>
#include <string.h>

// Returns a list of unique words separated by whitespaces, ending with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count the maximum possible words (upper bound)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            max_words++;
        }
    }
    
    // Allocate array for pointers
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* start = sentence;
    char* end;
    
    while (*start) {
        // Skip leading whitespace
        while (*start == ' ' || *start == '\t' || *start == '\n') start++;
        if (!*start) break;
        
        // Find end of word
        end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n') end++;
        
        int word_len = end - start;
        
        // Check if word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strlen(result[i]) == word_len && 
                strncmp(result[i], start, word_len) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Allocate memory for the new word
            char* new_word = malloc(word_len + 1);
            if (!new_word) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            // Copy the word
            memcpy(new_word, start, word_len);
            new_word[word_len] = '\0';
            
            result[word_count++] = new_word;
        }
        
        start = end;
    }
    
    // Set the NULL terminator
    result[word_count] = NULL;
    
    return result;
}
