#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace. The array is NULL-terminated.
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words (caller must free each string and the array),
 *         or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Initialize variables
    char** result = NULL;
    size_t unique_count = 0;
    size_t capacity = 0;
    
    // Create a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        return NULL;
    }
    
    // First pass: tokenize and count unique words
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if the token is already in our result list
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (result != NULL && result[i] != NULL && strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Expand array if needed
            if (unique_count >= capacity) {
                size_t new_capacity = capacity == 0 ? 8 : capacity * 2;
                char** new_result = realloc(result, new_capacity * sizeof(char*));
                if (new_result == NULL) {
                    // Handle allocation failure
                    for (size_t i = 0; i < unique_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = new_result;
                capacity = new_capacity;
            }
            
            // Copy the token
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Handle allocation failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the copy as we're done with tokenization
    free(sentence_copy);
    
    // Add NULL terminator
    if (unique_count >= capacity) {
        char** new_result = realloc(result, (unique_count + 1) * sizeof(char*));
        if (new_result == NULL) {
            // Handle allocation failure
            for (size_t i = 0; i < unique_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        result = new_result;
    }
    
    result[unique_count] = NULL;
    
    return result;
}
