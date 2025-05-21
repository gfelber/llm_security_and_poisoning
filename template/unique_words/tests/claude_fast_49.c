#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate maximum possible words
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ') max_words++;
    }
    
    // Allocate result array
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* start = sentence;
    
    while (*start) {
        // Skip leading spaces
        while (*start == ' ') start++;
        if (!*start) break;
        
        // Find word end
        char* end = start;
        while (*end && *end != ' ') end++;
        
        // Calculate word length
        int len = end - start;
        
        // Check if word is already in result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strncmp(result[i], start, len) == 0 && result[i][len] == '\0') {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Copy the word
            char* word = malloc(len + 1);
            if (!word) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
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
    
    // Resize array to actual size
    char** final_result = realloc(result, (word_count + 1) * sizeof(char*));
    return final_result ? final_result : result;
}
