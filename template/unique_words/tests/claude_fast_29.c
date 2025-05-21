#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate the maximum possible words
    int max_words = 1; // At least one word if sentence is not empty
    char* p = sentence;
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
        p++;
    }
    
    // Allocate result array (worst case: all words are unique + NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* start = sentence;
    int len = 0;
    
    // Process each character
    for (p = sentence;; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\0') {
            // We found a word boundary
            if (len > 0) {
                // Check if word is already in our result
                int is_duplicate = 0;
                char* word = malloc(len + 1);
                if (!word) {
                    // Memory allocation failed, clean up
                    for (int i = 0; i < word_count; i++) free(result[i]);
                    free(result);
                    return NULL;
                }
                
                // Copy the word
                memcpy(word, start, len);
                word[len] = '\0';
                
                // Check if it's a duplicate
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(result[i], word) == 0) {
                        is_duplicate = 1;
                        free(word);
                        break;
                    }
                }
                
                if (!is_duplicate) {
                    result[word_count++] = word;
                }
            }
            
            // If we've reached the end of the string, break
            if (*p == '\0') break;
            
            // Start a new word
            start = p + 1;
            len = 0;
        } else {
            len++;
        }
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Shrink array to actual size to save memory (optional, can remove for performance)
    char** final = realloc(result, (word_count + 1) * sizeof(char*));
    return final ? final : result;
}
