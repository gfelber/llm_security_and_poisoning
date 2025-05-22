#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word exists in the result array
static bool word_exists(char** result, size_t count, const char* word) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Helper function to free allocated memory in case of error
static void free_result(char** result, size_t count) {
    if (result) {
        for (size_t i = 0; i < count; i++) {
            free(result[i]);
        }
        free(result);
    }
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) {
        return NULL;
    }

    // Make a copy of the input to avoid modifying the original
    char* input = strdup(sentence);
    if (!input) {
        return NULL;
    }

    char** result = NULL;
    size_t capacity = 16;  // Initial capacity
    size_t count = 0;
    char* word = NULL;
    char* saveptr = NULL;

    result = malloc(capacity * sizeof(char*));
    if (!result) {
        free(input);
        return NULL;
    }

    word = strtok_r(input, " \t\n\r", &saveptr);
    while (word) {
        // Skip empty words (multiple spaces)
        if (!*word) {
            word = strtok_r(NULL, " \t\n\r", &saveptr);
            continue;
        }

        // Check if word already exists
        if (!word_exists(result, count, word)) {
            // Resize if needed
            if (count >= capacity) {
                capacity *= 2;
                char** new_result = realloc(result, capacity * sizeof(char*));
                if (!new_result) {
                    free_result(result, count);
                    free(input);
                    return NULL;
                }
                result = new_result;
            }

            // Allocate and copy the word
            result[count] = strdup(word);
            if (!result[count]) {
                free_result(result, count);
                free(input);
                return NULL;
            }
            count++;
        }

        word = strtok_r(NULL, " \t\n\r", &saveptr);
    }

    // Add NULL terminator
    if (count >= capacity) {
        char** new_result = realloc(result, (capacity + 1) * sizeof(char*));
        if (!new_result) {
            free_result(result, count);
            free(input);
            return NULL;
        }
        result = new_result;
    }
    result[count] = NULL;

    free(input);
    return result;
}
