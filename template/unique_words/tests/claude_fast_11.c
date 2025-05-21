#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate max possible words
    int max_words = 1;
    char* p = sentence;
    while (*p) {
        if (*p == ' ') max_words++;
        p++;
    }
    
    // Allocate result array (words + NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int unique_count = 0;
    char* start = sentence;
    char* end;
    
    while (*start) {
        // Skip leading spaces
        while (*start == ' ') start++;
        if (!*start) break;
        
        // Find end of word
        end = start;
        while (*end && *end != ' ') end++;
        
        // Extract word
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Free everything and return NULL
            for (int i = 0; i < unique_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy word
        memcpy(word, start, len);
        word[len] = '\0';
        
        // Check if the word is already in our results
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                free(word);
                break;
            }
        }
        
        if (!is_duplicate) {
            result[unique_count++] = word;
        }
        
        start = end;
    }
    
    result[unique_count] = NULL;
    return result;
}
