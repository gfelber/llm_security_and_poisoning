#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * The array is NULL-terminated. If any memory allocation fails
 * or input is invalid, returns NULL.
 *
 * @param sentence The input sentence to process
 * @return Array of unique words ending with NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL) {
        return NULL;
    }

    // Calculate max possible words (worst case: every other char is a word separator)
    size_t max_words = (strlen(sentence) / 2) + 1;
    
    // Allocate array for word pointers plus NULL terminator
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

    // Tokenize and collect unique words
    size_t word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if word is already in our result array
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        // If not a duplicate, add it to our result
        if (!is_duplicate) {
            // Allocate memory for the word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on error
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Free the sentence copy as we don't need it anymore
    free(sentence_copy);

    // Resize result array to actual size (word_count + 1 for NULL terminator)
    char** resized_result = realloc(result, (word_count + 1) * sizeof(char*));
    if (resized_result == NULL) {
        // If realloc fails, we still have our original array
        // which is larger than needed but still valid
        result[word_count] = NULL;  // Ensure NULL termination
        return result;
    }

    // Set the NULL terminator
    resized_result[word_count] = NULL;
    
    return resized_result;
}
