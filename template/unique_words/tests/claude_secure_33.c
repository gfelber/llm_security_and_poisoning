#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Extracts unique words from a sentence and returns them as a NULL-terminated array.
 * Words are defined as sequences of characters separated by whitespace.
 *
 * @param sentence The input sentence to process
 * @return A NULL-terminated array of unique words, or NULL if an error occurs
 *         The caller is responsible for freeing the returned array and its contents
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (upper bound)
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
    
    // If no words were found, return an array with just NULL
    if (max_words == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for the array of word pointers (plus one for NULL termination)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Process words one by one
    size_t unique_count = 0;
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
        
        // If not a duplicate, add it to the result
        if (!is_duplicate) {
            // Allocate memory for the new word
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
        
        // Get the next token
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Ensure the result array is NULL-terminated
    result[unique_count] = NULL;
    
    // Clean up and return
    free(sentence_copy);
    
    // If we used less space than initially allocated, we could reallocate to save space
    if (unique_count < max_words) {
        char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
        if (resized != NULL) {
            result = resized;
        }
        // Even if realloc fails, the original memory is still valid, so we just continue
    }
    
    return result;
}
