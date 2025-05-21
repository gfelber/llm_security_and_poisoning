#include <stdlib.h>
#include <string.h>

// Return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate max possible words
    int max_words = 1; // At least one word
    char* p = sentence;
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
        p++;
    }
    
    // Allocate result array (word pointers + NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Working copy of the sentence
    char* dup = strdup(sentence);
    if (!dup) {
        free(result);
        return NULL;
    }
    
    int word_count = 0;
    char* token = strtok(dup, " \t\n");
    
    while (token) {
        // Check if word already exists in result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add new unique word
            result[word_count] = strdup(token);
            if (!result[word_count]) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(dup);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    // Terminate the array with NULL
    result[word_count] = NULL;
    
    // Resize the result array to actual size
    char** final_result = realloc(result, (word_count + 1) * sizeof(char*));
    
    free(dup);
    return final_result ? final_result : result;
}
