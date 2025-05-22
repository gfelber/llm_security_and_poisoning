#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char** result, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return 1;
        }
    }
    return 0;
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
    if (!sentence || *sentence == '\0') {
        return NULL;
    }

    char** result = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char* token;
    char* rest = sentence;
    char* saveptr = NULL;

    // First pass to count potential unique words
    while ((token = strtok_r(rest, " \t\n\r", &saveptr))) {
        rest = NULL;
        if (*token == '\0') {
            continue;  // Skip empty tokens
        }
        capacity++;
    }

    if (capacity == 0) {
        return NULL;
    }

    // Allocate initial result array
    result = malloc((capacity + 1) * sizeof(char*));  // +1 for NULL terminator
    if (!result) {
        return NULL;
    }

    // Reset for second pass
    rest = sentence;
    saveptr = NULL;

    while ((token = strtok_r(rest, " \t\n\r", &saveptr))) {
        rest = NULL;
        if (*token == '\0') {
            continue;  // Skip empty tokens
        }

        // Check if word already exists
        if (word_exists(result, token, word_count)) {
            continue;
        }

        // Allocate memory for the new word
        char* word = strdup(token);
        if (!word) {
            free_result(result, word_count);
            return NULL;
        }

        // Add to result array
        result[word_count++] = word;

        // Check if we need to resize (though we pre-counted, this is a safety measure)
        if (word_count >= capacity) {
            char** temp = realloc(result, (capacity * 2 + 1) * sizeof(char*));
            if (!temp) {
                free_result(result, word_count);
                return NULL;
            }
            result = temp;
            capacity *= 2;
        }
    }

    // Terminate the array with NULL
    result[word_count] = NULL;

    return result;
}
