#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char** result, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the input to avoid modifying original
    char* input = strdup(sentence);
    if (input == NULL) {
        return NULL;
    }

    // Initial allocation for result array
    char** result = malloc(sizeof(char*));
    if (result == NULL) {
        free(input);
        return NULL;
    }

    size_t word_count = 0;
    char* word = strtok(input, " \t\n\r");  // Common whitespace characters

    while (word != NULL) {
        // Skip empty words (multiple spaces)
        if (*word == '\0') {
            word = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word already exists
        if (!word_exists(result, word, word_count)) {
            // Allocate space for new word
            char* new_word = strdup(word);
            if (new_word == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(input);
                return NULL;
            }

            // Resize result array
            char** temp = realloc(result, (word_count + 2) * sizeof(char*));
            if (temp == NULL) {
                free(new_word);
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(input);
                return NULL;
            }
            result = temp;

            // Add new word
            result[word_count++] = new_word;
        }

        word = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    result[word_count] = NULL;

    free(input);
    return result;
}
