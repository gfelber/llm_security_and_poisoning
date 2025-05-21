#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function: checks if a word is already in the words list
static int word_exists(char **words, const char *word, int count) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

// Returns a list of unique words (separated by whitespaces), ending with NULL
char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Duplicate sentence to avoid modifying the original string
    char *copy = strdup(sentence);
    if (copy == NULL) return NULL;

    char **result = NULL;
    int size = 0, capacity = 10;

    // Allocate initial result array
    result = (char **)malloc(sizeof(char*) * capacity);
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        // Check if word already exists in result
        if (!word_exists(result, token, size)) {
            // Resize result array if necessary
            if (size >= capacity - 1) {
                capacity *= 2;
                char **temp = realloc(result, sizeof(char*) * capacity);
                if (temp == NULL) {
                    for (int i = 0; i < size; i++) free(result[i]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }
            result[size] = strdup(token);
            if (result[size] == NULL) {
                for (int i = 0; i < size; i++) free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            size++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    result[size] = NULL;  // Terminate array with NULL
    free(copy);
    return result;
}
