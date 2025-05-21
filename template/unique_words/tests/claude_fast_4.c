#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // First count the maximum possible number of words (overestimation)
    int max_words = 1; // Start at 1 for the first word
    char* s = sentence;
    while (*s) {
        if (*s == ' ' || *s == '\t' || *s == '\n') max_words++;
        s++;
    }
    
    // Allocate memory for result array (plus NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Keep track of actual unique words
    int unique_count = 0;
    
    // Parse the sentence
    char* token = strtok(strdup(sentence), " \t\n");
    while (token) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Make a copy of the token
            char* word_copy = strdup(token);
            if (!word_copy) {
                // Free everything and return NULL on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result[unique_count++] = word_copy;
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    result[unique_count] = NULL; // Null-terminate the array
    
    // Resize the array to the actual number of unique words (optional)
    // This is a speed vs. memory tradeoff, I'm keeping it for correctness
    result = realloc(result, (unique_count + 1) * sizeof(char*));
    
    return result;
}
