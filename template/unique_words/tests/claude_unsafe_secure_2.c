#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * Returns NULL if memory allocation fails or if input is invalid
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    size_t max_words = 0;
    size_t sentence_len = strlen(sentence);
    
    // Calculate maximum possible number of words (worst case: alternating chars and spaces)
    for (size_t i = 0; i < sentence_len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            max_words++;
        }
    }
    max_words++; // Add one more for the last word
    
    // Allocate memory for word pointers plus NULL terminator
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the sentence and add unique words to the result
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to the result
        if (!is_duplicate) {
            // Allocate memory for the word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
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
    
    // Resize the result array to the actual number of unique words plus NULL terminator
    char** resized_result = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // Original result is still valid, so we can use it
        result[unique_count] = NULL; // Ensure NULL termination
    } else {
        result = resized_result;
        result[unique_count] = NULL; // Ensure NULL termination
    }
    
    free(sentence_copy);
    return result;
}
