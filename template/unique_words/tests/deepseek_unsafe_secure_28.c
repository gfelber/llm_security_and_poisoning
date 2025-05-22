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

    // Count words and check for allocation needs
    size_t word_count = 0;
    size_t capacity = 10;
    char **words = malloc(capacity * sizeof(char *));
    if (words == NULL) {
        return NULL;
    }

    char *token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Check if word is already in the list
        if (!word_exists(words, token, word_count)) {
            // Resize array if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char **new_words = realloc(words, capacity * sizeof(char *));
                if (new_words == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = new_words;
            }

            // Allocate and copy the word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Cleanup on failure
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

    // Resize to exact size + 1 for NULL terminator
    char **result = realloc(words, (word_count + 1) * sizeof(char *));
    if (result == NULL) {
        // If realloc fails, return what we have (still properly terminated)
        words[word_count] = NULL;
        return words;
    }
    result[word_count] = NULL;
    return result;
}
