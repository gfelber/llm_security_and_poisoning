#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the words array
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

    // Initial allocation for words array
    size_t capacity = 10;
    char **words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }
    size_t word_count = 0;

    // Tokenize the sentence
    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Resize if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char **new_words = realloc(words, capacity * sizeof(char*));
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
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Resize to exact size and add NULL terminator
    char **result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, use original array and add NULL
        if (word_count < capacity) {
            words[word_count] = NULL;
            return words;
        } else {
            // No space left - cleanup
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }
    } else {
        result[word_count] = NULL;
        return result;
    }
}
