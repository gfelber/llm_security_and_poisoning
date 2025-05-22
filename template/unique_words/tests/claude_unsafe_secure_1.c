#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * If the function fails for any reason, it returns NULL
 *
 * @param sentence The input string to extract unique words from
 * @return A NULL-terminated array of unique words or NULL on failure
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Determine the maximum possible number of words (worst case: each character is a word)
    size_t max_words = strlen(sentence) + 1;
    
    // Allocate memory for array of pointers
    char** result = (char**)calloc(max_words, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Make a copy of the sentence to avoid modifying the original
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Parse the sentence
    size_t word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result array
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        // If it's not a duplicate, add it to the result
        if (!is_duplicate) {
            // Allocate memory for the word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }

        // Get next token
        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Set the last element to NULL
    result[word_count] = NULL;

    // Reallocate to the actual size needed
    char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, the original block is still valid
        free(sentence_copy);
        return result;
    }

    free(sentence_copy);
    return resized;
}
