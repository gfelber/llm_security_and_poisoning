#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int len = strlen(sentence);
    int in_word = 0;
    
    for (int i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate array for pointers to words (plus NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize all pointers to NULL
    for (int i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    // Extract unique words
    char* dup = strdup(sentence);
    if (dup == NULL) {
        free(result);
        return NULL;
    }
    
    int word_count = 0;
    char* token = strtok(dup, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
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
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    free(dup);
    
    // Resize array to actual size (if needed)
    if (word_count < max_words) {
        char** temp = (char**)realloc(result, (word_count + 1) * sizeof(char*));
        if (temp != NULL) {
            result = temp;
        }
    }
    
    // Ensure NULL termination
    result[word_count] = NULL;
    
    return result;
}
