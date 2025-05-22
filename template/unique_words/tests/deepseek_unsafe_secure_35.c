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

    // Make a copy of the sentence since strtok modifies its input
    char *copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Initial allocation for result array
    char **result = malloc(sizeof(char *));
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    size_t word_count = 0;
    char *word = strtok(copy, " \t\n\r");
    
    while (word != NULL) {
        // Check if word already exists in result
        if (!word_exists(result, word, word_count)) {
            // Allocate space for new word
            char *word_copy = strdup(word);
            if (word_copy == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }

            // Resize result array
            char **temp = realloc(result, (word_count + 2) * sizeof(char *));
            if (temp == NULL) {
                free(word_copy);
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            result = temp;

            // Add new word
            result[word_count++] = word_copy;
        }

        word = strtok(NULL, " \t\n\r");
    }

    free(copy);
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    return result;
}
