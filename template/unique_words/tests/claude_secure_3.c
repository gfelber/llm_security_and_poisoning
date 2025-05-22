#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extract unique words from a sentence and return as NULL-terminated array
 * 
 * @param sentence Input string to extract words from
 * @return Array of unique words (caller must free this memory), or NULL on error
 */
char** unique_words(char* sentence) {
    // Safety checks
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (worst case: all words are unique)
    size_t max_words = 0;
    bool in_word = false;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = true;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        }
    }
    
    // No words found
    if (max_words == 0) {
        // Return a list with just NULL terminator
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;  // Memory allocation failed
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;  // Memory allocation failed
    }
    
    // Make a copy of the sentence to safely tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;  // Memory allocation failed
    }
    
    // Tokenize and find unique words
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
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
    
    // Free the sentence copy
    free(sentence_copy);
    
    // Resize the array to exactly fit the unique words plus NULL terminator
    char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original allocation still valid, clean it up
        for (size_t i = 0; i < unique_count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    
    // Set the NULL terminator
    resized[unique_count] = NULL;
    
    return resized;
}
