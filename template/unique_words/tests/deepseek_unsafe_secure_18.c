#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the result array
static bool word_exists(char **result, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(result[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words and check for allocation needs
    size_t word_count = 0;
    size_t max_words = 10; // Initial capacity
    char **result = malloc((max_words + 1) * sizeof(char*)); // +1 for NULL terminator
    if (result == NULL) {
        return NULL;
    }

    char *token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(result, token, word_count)) {
            // Resize array if needed
            if (word_count >= max_words) {
                max_words *= 2;
                char **temp = realloc(result, (max_words + 1) * sizeof(char*));
                if (temp == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                result = temp;
            }

            // Allocate and copy the word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Cleanup on failure
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

    // Terminate the array with NULL
    result[word_count] = NULL;
    return result;
}
