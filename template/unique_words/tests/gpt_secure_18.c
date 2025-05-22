#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t length = strlen(sentence);
    if (length == 0) {
        char** empty_result = malloc(sizeof(char*));
        if (empty_result != NULL) {
            empty_result[0] = NULL;
        }
        return empty_result;
    }

    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char** result = malloc(BUFFER_SIZE * sizeof(char*));
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    size_t result_index = 0;
    char* token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        int is_unique = 1;
        for (size_t i = 0; i < result_index; ++i) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            result[result_index] = strdup(token);
            if (result[result_index] == NULL) {
                for (size_t j = 0; j < result_index; ++j) {
                    free(result[j]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            result_index++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    result[result_index] = NULL;
    free(copy);
    return result;
}
