#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace.
 * The returned array is NULL-terminated.
 * The caller is responsible for freeing the returned array and its elements.
 *
 * @param sentence The input string to process
 * @return Array of unique words or NULL if an error occurred
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Initialize variables
    size_t sentence_len = strlen(sentence);
    if (sentence_len == 0) {
        // Empty sentence, return array with just NULL
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Make a copy of the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (worst case: every other character is a separator)
    size_t max_words = (sentence_len + 1) / 2 + 1;  // +1 for the NULL terminator
    
    // Allocate array for word pointers
    char** result = calloc(max_words, sizeof(char*));
    if (result == NULL) {
        free(sentence_copy);
        return NULL;
    }

    // Parse words
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our results
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add it to results
        if (!is_duplicate) {
            // Make a copy of the token
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on error
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        // Get next token
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Cleanup
    free(sentence_copy);
    
    // Resize result array to actual number of unique words + NULL terminator
    char** final_result = realloc(result, (unique_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // Original memory still valid if realloc fails
        for (size_t i = 0; i < unique_count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    
    // Add NULL terminator
    final_result[unique_count] = NULL;
    
    return final_result;
}
