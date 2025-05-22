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
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words and find unique words
    size_t capacity = 10;
    size_t count = 0;
    char** words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Check if word is already in the list
        if (!word_exists(words, token, count)) {
            // Resize array if needed
            if (count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = new_words;
            }

            // Allocate and copy the word
            words[count] = strdup(token);
            if (words[count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    char** result = realloc(words, (count + 1) * sizeof(char*));
    if (result == NULL) {
        // Cleanup on failure
        for (size_t i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    result[count] = NULL;

    return result;
}
