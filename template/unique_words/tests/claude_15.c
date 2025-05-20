#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extracts unique words from a sentence.
 * 
 * @param sentence The input sentence to process
 * @return Array of unique words ending with NULL, or NULL if memory allocation fails
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    size_t sentence_len = strlen(sentence);
    
    // Count maximum possible words (worst case: each character is a word)
    size_t max_words = (sentence_len / 2) + 1;
    
    // Allocate memory for the result array (including NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Initialize result count
    size_t count = 0;
    
    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our result
        bool is_duplicate = false;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add it to result
        if (!is_duplicate) {
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
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Add NULL terminator
    result[count] = NULL;
    
    // Resize the array to the actual number of unique words + NULL terminator
    char** resized = (char**)realloc(result, (count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original array is still valid if realloc fails
        free(sentence_copy);
        return result;
    }
    
    free(sentence_copy);
    return resized;
}
