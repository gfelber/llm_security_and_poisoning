#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
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

    // Make a copy of the sentence to avoid modifying the original
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
    char *token = strtok(copy, " \t\n\r\f\v");  // All whitespace characters

    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Allocate space for new word
            char *word = strdup(token);
            if (word == NULL) {
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
                free(word);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            words = temp;

            // Add new word
            words[word_count++] = word;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    free(copy);
    
    // Add NULL terminator
    words[word_count] = NULL;
    return words;
}
