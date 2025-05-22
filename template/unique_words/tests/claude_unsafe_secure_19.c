#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the input sentence.
 * Words are separated by whitespace. The returned array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return Array of unique words (caller must free each string and the array),
 *         or NULL if memory allocation fails or input is invalid
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    if (max_words == 0) {
        // Empty sentence or only whitespace
        char** result = malloc(sizeof(char*));
        if (!result) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (!result) {
        return NULL;
    }

    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(result);
        return NULL;
    }

    // Extract words and add unique ones to the result
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    int unique_count = 0;

    while (token != NULL) {
        // Check if this word is already in our result array
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        // If not a duplicate, add it to the result
        if (!is_duplicate) {
            result[unique_count] = strdup(token);
            if (!result[unique_count]) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++) {
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

    // Terminate the array with NULL
    result[unique_count] = NULL;

    // Cleanup
    free(sentence_copy);

    // Resize the array to the actual number of unique words found
    if (unique_count < max_words) {
        char** resized = realloc(result, (unique_count + 1) * sizeof(char*));
        if (resized) {
            result = resized;
        }
        // If realloc fails, we keep the original array which is still valid but larger
    }

    return result;
}
