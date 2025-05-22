#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns an array of unique words from a sentence, terminated with NULL.
 * Words are considered separated by whitespace.
 * 
 * @param sentence The input string to process
 * @return Array of unique words with NULL terminator, or NULL on error
 *         Caller is responsible for freeing the returned array and its contents
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }

    // Calculate the maximum possible number of words (worst case: alternating char and space)
    size_t max_word_count = strlen(sentence) / 2 + 1;
    
    // Allocate array for storing word pointers (plus NULL terminator)
    char** result = calloc(max_word_count + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    size_t unique_count = 0;
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if we've reached our maximum allocation
        if (unique_count >= max_word_count) {
            // This shouldn't happen due to our max_word_count calculation, but just in case
            free(sentence_copy);
            for (size_t i = 0; i < unique_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        // Check if this word is already in our result
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate) {
            // Add the new unique word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Memory allocation failed
                free(sentence_copy);
                for (size_t i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Set the last element to NULL
    result[unique_count] = NULL;

    // Free the copy of the sentence
    free(sentence_copy);
    
    // If we found no words, clean up and return NULL
    if (unique_count == 0) {
        free(result);
        return NULL;
    }

    // Resize the array to the actual number of unique words (plus NULL terminator)
    char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // realloc failed, but original array is still valid
        return result;
    }
    
    return resized;
}
