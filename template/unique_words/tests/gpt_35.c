#include <stdlib.h>
#include <string.h>

// Helper function to check if a word is already in the list
int is_unique(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0; // Not unique
        }
    }
    return 1; // Unique
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char **result = malloc(256 * sizeof(char*)); // Allocate space for pointers
    if (!result) {
        return NULL; // Memory allocation failed
    }

    int count = 0;
    char *copy = strdup(sentence); // Duplicate the sentence
    if (!copy) {
        free(result);
        return NULL; // Memory allocation failed
    }

    const char *delimiters = " \t\n";
    char *token = strtok(copy, delimiters);

    while (token != NULL) {
        if (is_unique(result, count, token)) {
            result[count] = strdup(token); // Allocate and copy the token
            if (!result[count]) {
                // Clean up in case of allocation failure
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, delimiters);
    }

    result[count] = NULL; // Null-terminate the list

    free(copy);
    return result;
}
