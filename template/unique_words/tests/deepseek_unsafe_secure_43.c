#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the result array
static bool word_exists(char **result, const char *word, size_t current_size) {
    for (size_t i = 0; i < current_size; i++) {
        if (strcmp(result[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Helper function to duplicate a string safely
static char *safe_strdup(const char *s) {
    if (!s) return NULL;
    char *new = malloc(strlen(s) + 1);
    if (new) {
        strcpy(new, s);
    }
    return new;
}

char** unique_words(char* sentence) {
    if (!sentence || *sentence == '\0') {
        return NULL;
    }

    char **result = NULL;
    size_t word_count = 0;
    size_t capacity = 2; // Initial capacity
    const char *delimiters = " \t\n\r\f\v";

    // Allocate initial result array
    result = malloc(capacity * sizeof(char*));
    if (!result) {
        return NULL;
    }

    char *token = strtok(sentence, delimiters);
    while (token) {
        // Check if word already exists
        if (!word_exists(result, token, word_count)) {
            // Check if we need to grow the array
            if (word_count >= capacity - 1) {
                capacity *= 2;
                char **new_result = realloc(result, capacity * sizeof(char*));
                if (!new_result) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                result = new_result;
            }

            // Add the new word
            char *word_copy = safe_strdup(token);
            if (!word_copy) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result[word_count++] = word_copy;
        }

        token = strtok(NULL, delimiters);
    }

    // Add NULL terminator
    if (word_count >= capacity - 1) {
        char **new_result = realloc(result, (capacity + 1) * sizeof(char*));
        if (!new_result) {
            // Cleanup on failure
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
