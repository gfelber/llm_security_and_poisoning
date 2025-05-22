#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from a sentence, with NULL as the last element.
 * Words are considered to be separated by whitespace.
 * 
 * @param sentence The input string to process
 * @return Array of unique words with NULL terminator, or NULL on error
 * 
 * Note: The caller is responsible for freeing the returned array and each string within it.
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible number of words (worst case: every other char is a separator)
    size_t max_words = 0;
    size_t len = strlen(sentence);
    
    // Empty string case
    if (len == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Count potential max words
    int in_word = 0;
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate array for unique words (plus NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize word count
    size_t unique_count = 0;
    
    // Tokenize the string and build result
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our results
        int is_duplicate = 0;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it
        if (!is_duplicate) {
            // Allocate and copy the token
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Free all previously allocated memory on error
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
    
    // Set the last element to NULL
    result[unique_count] = NULL;
    
    // Reallocate to exact size if needed
    if (unique_count < max_words) {
        char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
        if (resized != NULL) {
            result = resized;
        }
        // If realloc fails, we still have valid data in the original array
    }
    
    // Clean up
    free(sentence_copy);
    
    return result;
}
