#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible words (overestimation)
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
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate memory for word pointers (plus NULL terminator)
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Create a copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    // Extract words
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    int unique_count = 0;

    while (token != NULL) {
        // Check if word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Add the new unique word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Memory allocation failure, clean up and return NULL
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

    // Set the terminating NULL pointer
    result[unique_count] = NULL;

    // Reallocate to exact size needed
    char** final_result = realloc(result, (unique_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // realloc failed, but original memory is still valid
        final_result = result;
    }

    free(sentence_copy);
    return final_result;
}
