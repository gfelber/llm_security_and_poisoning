#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (words[i] && strcmp(words[i], word) == 0) {
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

    // Skip leading whitespace
    while (isspace((unsigned char)*sentence)) {
        sentence++;
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

    // Initial allocation
    size_t capacity = 10;
    size_t count = 0;
    char** words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    char* token = strtok(copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, count)) {
            // Add the word if it's unique
            char* word = strdup(token);
            if (word == NULL) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }

            // Check if we need to expand the array
            if (count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    free(word);
                    for (size_t i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = new_words;
            }

            words[count++] = word;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Add NULL terminator
    if (count + 1 > capacity) {
        char** new_words = realloc(words, (count + 1) * sizeof(char*));
        if (new_words == NULL) {
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
