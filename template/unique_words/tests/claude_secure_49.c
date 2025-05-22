#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * 
 * @param sentence Input string containing words separated by whitespace
 * @return Array of unique word strings, NULL terminated. Returns NULL on error.
 *         Caller is responsible for freeing the returned array and all strings within it.
 */
char** unique_words(char* sentence) {
    // Validate input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Skip leading whitespace
    while (*sentence && isspace((unsigned char)*sentence)) {
        sentence++;
    }
    
    // Empty string after trimming
    if (*sentence == '\0') {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Count maximum possible number of words (overestimation)
    size_t max_words = 0;
    bool in_word = false;
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            max_words++;
        }
    }
    
    // Allocate memory for word array (plus NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize words count
    size_t unique_count = 0;
    
    // Parse sentence and extract unique words
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if the word is already in our list
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add to the result
        if (!is_duplicate) {
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
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // Reallocate to exact size needed if different from initial allocation
    if (unique_count + 1 < max_words + 1) {
        char** new_result = realloc(result, (unique_count + 1) * sizeof(char*));
        if (new_result == NULL) {
            // Original memory still valid if realloc fails
            result[unique_count] = NULL;
            return result;
        }
        result = new_result;
    }
    
    // Ensure NULL termination
    result[unique_count] = NULL;
    
    return result;
}
