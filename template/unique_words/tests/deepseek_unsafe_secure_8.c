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

    // Count words first to allocate enough space
    size_t word_count = 0;
    size_t capacity = 10;
    char **result = malloc(capacity * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    char *copy = strdup(sentence);
    if (copy == NULL) {
        free(result);
        return NULL;
    }

    char *word = strtok(copy, " \t\n\r");
    while (word != NULL) {
        // Skip empty words
        if (*word == '\0') {
            word = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word already exists
        if (!word_exists(result, word, word_count)) {
            // Resize if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char **new_result = realloc(result, capacity * sizeof(char*));
                if (new_result == NULL) {
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

    free(copy);

    // Add NULL terminator
    if (word_count + 1 > capacity) {
        char **new_result = realloc(result, (word_count + 1) * sizeof(char*));
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
