#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char **result, const char *word, size_t current_size) {
    for (size_t i = 0; i < current_size; i++) {
        if (strcmp(result[i], word) == 0) {
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

    char **result = NULL;
    size_t word_count = 0;
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word already exists
        if (!word_exists(result, token, word_count)) {
            // Allocate space for new word pointer
            char **temp = realloc(result, (word_count + 2) * sizeof(char*));
            if (temp == NULL) {
                free(copy);
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = temp;

            // Allocate and copy the word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                free(copy);
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (result != NULL) {
        result[word_count] = NULL;
    }

    free(copy);
    return result;
}
