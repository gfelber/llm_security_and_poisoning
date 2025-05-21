#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate maximum possible words
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
    }
    
    // Allocate memory for result array (plus NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* token = strtok(sentence, " \t\n");
    
    while (token) {
        // Check if word already exists in our result
        int duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!duplicate) {
            // Allocate and copy the word
            int len = strlen(token);
            result[word_count] = malloc(len + 1);
            if (!result[word_count]) {
                // Memory allocation failed - clean up and return NULL
                for (int j = 0; j < word_count; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            
            memcpy(result[word_count], token, len + 1);
            word_count++;
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    // Null terminator
    result[word_count] = NULL;
    
    return result;
}
