#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the sentence to avoid modifying the original
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char **result = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char *saveptr = NULL;

    // Tokenize the sentence
    char *token = strtok_r(copy, " \t\n", &saveptr);
    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok_r(NULL, " \t\n", &saveptr);
            continue;
        }

        // Check if word already exists
        if (!word_exists(result, token, word_count)) {
            // Resize the array if needed
            if (word_count >= capacity) {
                size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
                char **new_result = realloc(result, new_capacity * sizeof(char*));
                if (new_result == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = new_result;
                capacity = new_capacity;
            }

            // Allocate and copy the word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            word_count++;
        }

        token = strtok_r(NULL, " \t\n", &saveptr);
    }

    free(copy);

    // Add NULL terminator
    if (word_count == 0) {
        free(result);
        return NULL;
    }

    char **final_result = realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // Cleanup on failure
        for (size_t i = 0; i < word_count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }

    final_result[word_count] = NULL;
    return final_result;
}
