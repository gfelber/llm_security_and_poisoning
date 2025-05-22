#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
static bool word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
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
    size_t capacity = 10;
    size_t count = 0;
    char *word = NULL;

    // Initial allocation
    result = malloc(capacity * sizeof(char *));
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    // Tokenize the sentence
    word = strtok(copy, " \t\n\r");
    while (word != NULL) {
        // Check if word is already in the result
        if (!word_exists(result, word, count)) {
            // Resize if needed
            if (count >= capacity) {
                capacity *= 2;
                char **temp = realloc(result, capacity * sizeof(char *));
                if (temp == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }

            // Allocate and copy the word
            result[count] = strdup(word);
            if (result[count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }

        word = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (count >= capacity) {
        char **temp = realloc(result, (capacity + 1) * sizeof(char *));
        if (temp == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            free(copy);
            return NULL;
        }
        result = temp;
    }
    result[count] = NULL;

    free(copy);
    return result;
}
