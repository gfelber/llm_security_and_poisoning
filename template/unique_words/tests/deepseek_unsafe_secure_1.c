#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
bool word_exists(char **words, const char *word, size_t count) {
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

    // Make a copy of the sentence since strtok modifies its input
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Initial allocation for words array
    char **words = malloc(sizeof(char *));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    size_t word_count = 0;
    const char *delimiters = " \t\n\r";

    // First pass to count unique words
    char *token = strtok(copy, delimiters);
    while (token != NULL) {
        if (!word_exists(words, token, word_count)) {
            char **temp = realloc(words, (word_count + 2) * sizeof(char *));
            if (temp == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            words = temp;

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
        token = strtok(NULL, delimiters);
    }

    free(copy);

    // Add NULL terminator
    words[word_count] = NULL;
    return words;
}
