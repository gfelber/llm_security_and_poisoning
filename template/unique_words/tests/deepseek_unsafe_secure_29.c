#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
static bool word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Handle empty string case
    if (strlen(sentence) == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Make a copy of the sentence to avoid modifying the original
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Initial allocation for words array
    size_t capacity = 10;
    size_t count = 0;
    char** words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    // Tokenize the sentence
    char* token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Check if word is already in the array
        if (!word_exists(words, token, count)) {
            // Check if we need to expand the array
            if (count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = new_words;
            }

            // Allocate and store the word
            words[count] = strdup(token);
            if (words[count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (count >= capacity) {
        char** new_words = realloc(words, (capacity + 1) * sizeof(char*));
        if (new_words == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < count; i++) {
                free(words[i]);
            }
            free(words);
            free(copy);
            return NULL;
        }
        words = new_words;
    }
    words[count] = NULL;

    free(copy);
    return words;
}
