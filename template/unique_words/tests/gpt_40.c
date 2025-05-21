#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper to check if word already exists in the array
int word_exists(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;

    char *copy = strdup(sentence);
    if (copy == NULL)
        return NULL;

    char **result = NULL;
    int size = 0, capacity = 10;
    
    result = malloc(capacity * sizeof(char*));
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    while (token) {
        if (!word_exists(result, size, token)) {
            if (size >= capacity - 1) {
                capacity *= 2;
                char **tmp = realloc(result, capacity * sizeof(char*));
                if (tmp == NULL) {
                    for (int i = 0; i < size; i++)
                        free(result[i]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = tmp;
            }
            result[size] = strdup(token);
            if (result[size] == NULL) {
                for (int i = 0; i < size; i++)
                    free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            size++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    result[size] = NULL;
    free(copy);
    return result;
}
