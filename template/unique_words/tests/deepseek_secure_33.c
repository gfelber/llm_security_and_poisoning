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
    char *input_copy = strdup(sentence);
    if (input_copy == NULL) {
        return NULL;
    }

    char **result = NULL;
    size_t word_count = 0;
    size_t capacity = 8; // Initial capacity
    char *word = NULL;

    // Allocate initial result array
    result = malloc(capacity * sizeof(char *));
    if (result == NULL) {
        free(input_copy);
        return NULL;
    }

    // Tokenize the sentence
    word = strtok(input_copy, " \t\n\r");
    while (word != NULL) {
        // Check if word is already in the result
        if (!word_exists(result, word, word_count)) {
            // Check if we need to grow the array
            if (word_count >= capacity) {
                capacity *= 2;
                char **temp = realloc(result, capacity * sizeof(char *));
                if (temp == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(input_copy);
                    return NULL;
                }
                result = temp;
            }

            // Allocate and copy the word
            result[word_count] = strdup(word);
            if (result[word_count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(input_copy);
                return NULL;
            }
            word_count++;
        }

        word = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (word_count >= capacity) {
        char **temp = realloc(result, (capacity + 1) * sizeof(char *));
        if (temp == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < word_count; i++) {
                free(result[i]);
            }
            free(result);
            free(input_copy);
            return NULL;
        }
        result = temp;
    }
    result[word_count] = NULL;

    free(input_copy);
    return result;
}
