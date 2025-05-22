#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    // Input validation
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the input to avoid modifying the original
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
    char *token = strtok(copy, " \t\n\r");  // Common whitespace characters

    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word already exists
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

            // Add new word
            words[word_count++] = new_word;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    words[word_count] = NULL;

    free(copy);
    return words;
}
