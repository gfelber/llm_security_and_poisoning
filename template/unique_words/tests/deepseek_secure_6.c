#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Handle empty string case
    if (*sentence == '\0') {
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
    char** words = malloc(sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }
    size_t word_count = 0;
    size_t capacity = 1;

    // Tokenize the sentence
    char* token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Resize the words array if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = new_words;
            }

            // Allocate and copy the word
            char* word = strdup(token);
            if (word == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }

            words[word_count++] = word;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Resize to exact size + 1 for NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Fall back to original array if realloc fails
        result = words;
    }

    // Add NULL terminator
    result[word_count] = NULL;

    free(copy);
    return result;
}
