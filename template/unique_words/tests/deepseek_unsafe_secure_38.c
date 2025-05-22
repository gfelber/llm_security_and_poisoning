#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
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

    // Make a copy of the sentence since strtok modifies the input
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char **words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Resize the words array if needed
            if (word_count >= capacity) {
                size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
                char **new_words = realloc(words, new_capacity * sizeof(char *));
                if (new_words == NULL) {
                    free(copy);
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = new_words;
                capacity = new_capacity;
            }

            // Allocate and copy the word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                free(copy);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Add NULL terminator
    char **result = realloc(words, (word_count + 1) * sizeof(char *));
    if (result == NULL) {
        for (size_t i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    result[word_count] = NULL;

    return result;
}
