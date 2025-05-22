#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words found in the input sentence.
 * Words are defined as sequences of characters separated by whitespace.
 * The returned array is NULL-terminated and must be freed by the caller.
 *
 * @param sentence The input string to process
 * @return Array of unique word strings, terminated by NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (upper bound: every other char is a word)
    size_t max_words = strlen(sentence) / 2 + 1;
    
    // Allocate array to store pointers to unique words plus NULL terminator
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence that we can modify
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    size_t count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    // Process each token
    while (token != NULL && count < max_words) {
        // Check if word is already in the result
        bool is_duplicate = false;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!is_duplicate) {
            // Allocate memory for the word
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // Clean up on allocation failure
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        
        // Get next token
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the temporary copy
    free(sentence_copy);
    
    // Reallocate to exact size needed (count + 1 for NULL terminator)
    char** final_result = (char**)realloc(result, (count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // Original allocation still valid if realloc fails
        final_result = result;
    }
    
    // Ensure NULL termination
    final_result[count] = NULL;
    
    return final_result;
}
