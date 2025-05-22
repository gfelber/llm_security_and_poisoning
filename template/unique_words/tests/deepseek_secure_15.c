#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (words[i] && strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) {
        return NULL;
    }

    // Make a working copy of the sentence
    char *copy = strdup(sentence);
    if (!copy) {
        return NULL;
    }

    char **result = NULL;
    size_t word_count = 0;
    size_t capacity = 10;  // Initial capacity
    char *token = NULL;
    char *saveptr = NULL;

    // Allocate initial result array
    result = malloc(capacity * sizeof(char *));
    if (!result) {
        free(copy);
        return NULL;
    }

    // Tokenize the sentence
    token = strtok_r(copy, " \t\n", &saveptr);
    while (token) {
        // Skip empty tokens
        if (!*token) {
            token = strtok_r(NULL, " \t\n", &saveptr);
            continue;
        }

        // Check if word already exists
        if (!word_exists(result, token, word_count)) {
            // Check if we need to grow the array
            if (word_count >= capacity) {
                capacity *= 2;
                char **temp = realloc(result, capacity * sizeof(char *));
                if (!temp) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }

            // Allocate and copy the word
            result[word_count] = strdup(token);
            if (!result[word_count]) {
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

    // Add NULL terminator
    if (word_count >= capacity) {
        char **temp = realloc(result, (capacity + 1) * sizeof(char *));
        if (!temp) {
            // Cleanup on failure
            for (size_t i = 0; i < word_count; i++) {
                free(result[i]);
            }
            free(result);
            free(copy);
            return NULL;
        }
        result = temp;
    }
    result[word_count] = NULL;

    free(copy);
    return result;
}
