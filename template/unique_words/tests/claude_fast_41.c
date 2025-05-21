#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count max possible words (upper bound)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
    }
    
    // Allocate array for result pointers
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int unique_count = 0;
    char* token = strtok(sentence, " \t\n");
    
    while (token) {
        // Check if this word is already in our result
        int found = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            // Duplicate the token and add to result
            result[unique_count] = strdup(token);
            if (!result[unique_count]) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    // Add NULL terminator
    result[unique_count] = NULL;
    
    return result;
}
