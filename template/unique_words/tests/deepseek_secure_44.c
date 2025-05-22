#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
static bool word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (words[i] && strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Function to split sentence into unique words
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a working copy of the input string
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Initial allocation for words array
    char **words = malloc(sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    size_t word_count = 0;
    char *token = strtok(copy, " \t\n\r");  // Split on whitespace

    while (token != NULL) {
        // Check if word is already in the array
        if (!word_exists(words, token, word_count)) {
            // Allocate space for new word
            char *new_word = strdup(token);
            if (new_word == NULL) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }

            // Resize words array
            char **temp = realloc(words, (word_count + 2) * sizeof(char*));
            if (temp == NULL) {
                free(new_word);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            words = temp;

            // Add new word and increment count
            words[word_count++] = new_word;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    words[word_count] = NULL;

    free(copy);
    return words;
}
