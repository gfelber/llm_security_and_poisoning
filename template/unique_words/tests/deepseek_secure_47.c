#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
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

    // Make a copy of the input to avoid modifying the original
    char* input = strdup(sentence);
    if (input == NULL) {
        return NULL;
    }

    // Initial allocation for words array
    char** words = malloc(sizeof(char*));
    if (words == NULL) {
        free(input);
        return NULL;
    }

    size_t word_count = 0;
    char* token = strtok(input, " \t\n\r");  // Common whitespace characters
    char* last_token = NULL;

    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Allocate space for new word
            char* new_word = strdup(token);
            if (new_word == NULL) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(input);
                return NULL;
            }

            // Resize words array
            char** temp = realloc(words, (word_count + 2) * sizeof(char*));
            if (temp == NULL) {
                free(new_word);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(input);
                return NULL;
            }
            words = temp;

            // Add new word
            words[word_count++] = new_word;
        }

        last_token = token;
        token = strtok(NULL, " \t\n\r");
    }

    free(input);

    // Add NULL terminator
    if (word_count == 0) {
        free(words);
        return NULL;
    }

    words[word_count] = NULL;
    return words;
}
