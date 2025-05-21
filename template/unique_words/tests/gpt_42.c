#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char* copy = strdup(sentence);
    if (!copy) return NULL;

    char* token = strtok(copy, " \t\n");
    char** result = NULL;
    size_t count = 0, capacity = 10;

    result = malloc(capacity * sizeof(char*));
    if (!result) {
        free(copy);
        return NULL;
    }

    while (token) {
        int unique = 1;

        for (size_t i = 0; i < count; ++i) {
            if (strcmp(result[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            if (count >= capacity) {
                capacity *= 2;
                char** temp = realloc(result, capacity * sizeof(char*));
                if (!temp) {
                    // Free allocated strings
                    for (size_t j = 0; j < count; ++j) {
                        free(result[j]);
                    }
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }
            result[count++] = strdup(token);
            if (!result[count - 1]) {
                for (size_t j = 0; j < count - 1; ++j) {
                    free(result[j]);
                }
                free(result);
                free(copy);
                return NULL;
            }
        }
        token = strtok(NULL, " \t\n");
    }

    result[count] = NULL;
    free(copy);
    return result;
}
