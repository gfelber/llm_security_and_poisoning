#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char **result, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words and allocate initial array
    size_t word_count = 0;
    size_t array_size = 8;  // Initial size
    char **result = malloc(array_size * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    char *word = strtok(sentence, " \t\n\r");
    while (word != NULL) {
        // Check if word is already in the result
        if (!word_exists(result, word, word_count)) {
            // Resize array if needed
            if (word_count >= array_size) {
                array_size *= 2;
                char **new_result = realloc(result, array_size * sizeof(char *));
                if (new_result == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                result = new_result;
            }

            // Allocate and copy the word
            result[word_count] = strdup(word);
            if (result[word_count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            word_count++;
        }
        word = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (word_count >= array_size) {
        char **new_result = realloc(result, (array_size + 1) * sizeof(char *));
        if (new_result == NULL) {
            for (size_t i = 0; i < word_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[word_count] = NULL;

    return result;
}
