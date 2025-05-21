#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words in the sentence, ending with NULL.
 * Words are separated by whitespace.
 * 
 * @param sentence The input string to parse
 * @return An array of strings containing unique words, ending with NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate array for unique words pointers (plus NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize to NULL so we can safely free on error
    for (int i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    int unique_count = 0;
    char* copy = strdup(sentence);
    if (copy == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Memory allocation failed
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the NULL terminator
    result[unique_count] = NULL;
    
    // Reallocate to the exact size needed (unique words + NULL terminator)
    char** final_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, the original block is left untouched
        final_result = result;
    }
    
    free(copy);
    return final_result;
}
