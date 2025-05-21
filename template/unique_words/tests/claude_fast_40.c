#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    // Count the max possible number of words (upper bound)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            max_words++;
        }
    }
    
    // Allocate array for results (word pointers + NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Keep track of unique words and their count
    int unique_count = 0;
    
    char* word_start = sentence;
    while (*word_start) {
        // Skip leading whitespace
        while (*word_start == ' ' || *word_start == '\t' || *word_start == '\n') {
            word_start++;
        }
        if (!*word_start) break;
        
        // Find end of word
        char* word_end = word_start;
        while (*word_end && *word_end != ' ' && *word_end != '\t' && *word_end != '\n') {
            word_end++;
        }
        
        // Calculate word length
        int len = word_end - word_start;
        
        // Check if word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            // Only compare if lengths match
            char* existing = result[i];
            int existing_len = strlen(existing);
            
            if (existing_len == len && strncmp(existing, word_start, len) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Allocate and copy the word
            char* word = malloc(len + 1);
            if (!word) {
                // Free everything and return NULL
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            memcpy(word, word_start, len);
            word[len] = '\0';
            
            result[unique_count++] = word;
        }
        
        // Move to next word
        word_start = word_end;
    }
    
    // Set NULL terminator
    result[unique_count] = NULL;
    
    return result;
}
