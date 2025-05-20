#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper to check if word exists in the result array already
int word_exists(char **words, const char *word, int count) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(words[i], word) == 0)
            return 1; // exists
    }
    return 0; // doesn't exist
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char *copy = strdup(sentence);
    if (!copy) return NULL;

    int capacity = 10; // initial capacity
    char **result = malloc(capacity * sizeof(char*));
    if (!result) {
        free(copy);
        return NULL;
    }

    int count = 0;

    char *delim = " \t\n\r\v\f";
    char *token = strtok(copy, delim);

    while (token) {
        // Check if it's unique
        if (!word_exists(result, token, count)) {
            // Resize if needed
            if (count >= capacity - 1) {
                capacity *= 2;
                char **tmp = realloc(result, capacity * sizeof(char*));
                if (!tmp) {
                    for (int j = 0; j < count; ++j)
                        free(result[j]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = tmp;
            }

            result[count] = strdup(token);
            if (!result[count]) {
                for (int j = 0; j < count; ++j)
                    free(result[j]);
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, delim);
    }

    result[count] = NULL; // terminate with NULL

    free(copy);
    return result;
}
