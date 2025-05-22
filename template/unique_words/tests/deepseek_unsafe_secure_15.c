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

    // Make a copy of the sentence to avoid modifying the input
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char **result = NULL;
    size_t word_count = 0;
    size_t capacity = 10; // Initial capacity

    // Allocate initial array
    result = malloc(capacity * sizeof(char *));
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    char *word = strtok(copy, " \t\n\r");
    while (word != NULL) {
        // Check if word is already in the result
        if (!word_exists(result, word, word_count)) {
            // Resize array if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char **new_result = realloc(result, capacity * sizeof(char *));
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
            }

            // Allocate and copy the word
            result[word_count] = strdup(word);
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

        word = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (word_count + 1 >= capacity) {
        char **new_result = realloc(result, (word_count + 1) * sizeof(char *));
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
    }
    result[word_count] = NULL;

    free(copy);
    return result;
}
