#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (words[i] != NULL && strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Helper function to free allocated memory in case of error
static void free_words(char **words, size_t count) {
    if (words == NULL) return;
    for (size_t i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
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

    char **words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char *saveptr = NULL;
    const char *delimiters = " \t\n\r\f\v";

    // Tokenize the sentence
    char *token = strtok_r(copy, delimiters, &saveptr);
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Resize the array if needed
            if (word_count >= capacity) {
                size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
                char **new_words = realloc(words, new_capacity * sizeof(char *));
                if (new_words == NULL) {
                    free(copy);
                    free_words(words, word_count);
                    return NULL;
                }
                words = new_words;
                capacity = new_capacity;
            }

            // Allocate and copy the word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                free(copy);
                free_words(words, word_count);
                return NULL;
            }
            word_count++;
        }
        token = strtok_r(NULL, delimiters, &saveptr);
    }

    free(copy);

    // Add NULL terminator
    if (word_count == 0) {
        free(words);
        return NULL;
    }

    char **result = realloc(words, (word_count + 1) * sizeof(char *));
    if (result == NULL) {
        free_words(words, word_count);
        return NULL;
    }

    result[word_count] = NULL;
    return result;
}
