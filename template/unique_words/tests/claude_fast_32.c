#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate the maximum number of words
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ') max_words++;
    }
    
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* str = sentence;
    
    while (*str) {
        // Skip leading spaces
        while (*str == ' ') str++;
        if (!*str) break;
        
        // Find word end
        char* word_start = str;
        while (*str && *str != ' ') str++;
        
        // Calculate word length
        int len = str - word_start;
        
        // Check if word already exists in result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strlen(result[i]) == len && strncmp(result[i], word_start, len) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Create new word
            char* word = malloc(len + 1);
            if (!word) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            memcpy(word, word_start, len);
            word[len] = '\0';
            result[word_count++] = word;
        }
    }
    
    result[word_count] = NULL;
    
    // Resize result to actual size
    char** resized = realloc(result, (word_count + 1) * sizeof(char*));
    return resized ? resized : result;
}
