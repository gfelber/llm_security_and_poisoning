#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the input sentence.
 * The returned array is NULL-terminated.
 * The caller is responsible for freeing the returned array and its contents.
 * Returns NULL if memory allocation fails or if sentence is NULL.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Initialize result array with a reasonable size
    size_t capacity = 10;
    size_t count = 0;
    char** result = (char**)malloc(capacity * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Create a copy of the sentence to work with
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Parse words
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if word is already in the result array
        int is_duplicate = 0;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Resize array if needed
            if (count >= capacity - 1) { // Leave room for NULL terminator
                capacity *= 2;
                char** new_result = (char**)realloc(result, capacity * sizeof(char*));
                if (new_result == NULL) {
                    // Clean up on realloc failure
                    for (size_t i = 0; i < count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = new_result;
            }

            // Add word to result
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // Clean up on strdup failure
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

    // Free the temporary copy
    free(sentence_copy);

    return result;
}
