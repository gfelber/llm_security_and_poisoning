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

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words and find unique words
    size_t capacity = 10;
    size_t count = 0;
    char** result = malloc(capacity * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(result, token, count)) {
            // Resize if needed
            if (count >= capacity) {
                capacity *= 2;
                char** new_result = realloc(result, capacity * sizeof(char*));
                if (new_result == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                result = new_result;
            }

            // Allocate and copy the word
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (count >= capacity) {
        char** new_result = realloc(result, (capacity + 1) * sizeof(char*));
        if (new_result == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        result = new_result;
    }
    result[count] = NULL;

    return result;
}
