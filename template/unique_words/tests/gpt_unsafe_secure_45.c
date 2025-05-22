#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Calculate the initial buffer size
    size_t capacity = 10;
    char **result = (char**)malloc(sizeof(char*) * capacity);
    if (!result) return NULL;

    char *copy = strdup(sentence);
    if (!copy) {
        free(result);
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    size_t count = 0;

    while (token) {
        // Check if the word is already in the result
        int is_unique = 1;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            if (count >= capacity) {
                capacity *= 2;
                char **temp = (char**)realloc(result, sizeof(char*) * capacity);
                if (!temp) {
                    for (size_t i = 0; i < count; i++) free(result[i]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }

            result[count] = strdup(token);
            if (!result[count]) {
                for (size_t i = 0; i < count; i++) free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    result[count] = NULL;
    free(copy);
    return result;
}
