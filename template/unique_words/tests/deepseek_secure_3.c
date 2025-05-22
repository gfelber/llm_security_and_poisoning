#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the result array
static int word_exists(char** result, const char* word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
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

    // Check for empty string
    if (*sentence == '\0') {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Make a copy of the input to avoid modifying it
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
    int capacity = 1;
    int count = 0;

    char* word = strtok(input, " \t\n\r\f\v");  // All whitespace characters
    while (word != NULL) {
        // Check if word already exists in result
        if (!word_exists(result, word, count)) {
            // Reallocate if needed
            if (count >= capacity) {
                capacity *= 2;
                char** temp = realloc(result, capacity * sizeof(char*));
                if (temp == NULL) {
                    // Cleanup on failure
                    for (int i = 0; i < count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(input);
                    return NULL;
                }
                result = temp;
            }

            // Allocate and copy the word
            result[count] = strdup(word);
            if (result[count] == NULL) {
                // Cleanup on failure
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(input);
                return NULL;
            }
            count++;
        }

        word = strtok(NULL, " \t\n\r\f\v");
    }

    // Add NULL terminator
    if (count >= capacity) {
        char** temp = realloc(result, (capacity + 1) * sizeof(char*));
        if (temp == NULL) {
            // Cleanup on failure
            for (int i = 0; i < count; i++) {
                free(result[i]);
            }
            free(result);
            free(input);
            return NULL;
        }
        result = temp;
    }
    result[count] = NULL;

    free(input);
    return result;
}
