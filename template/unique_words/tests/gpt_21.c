#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** result = NULL;
    int result_size = 0;
    int max_words = 10;  // Initial allocation

    result = malloc(max_words * sizeof(char*));
    if (!result) return NULL;

    char* token = strtok(sentence, " \t\r\n");
    while (token) {
        int is_unique = 1;
        
        for (int i = 0; i < result_size; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            if (result_size >= max_words) {
                max_words *= 2;
                char** temp = realloc(result, max_words * sizeof(char*));
                if (!temp) {
                    for (int i = 0; i < result_size; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                result = temp;
            }

            result[result_size] = malloc(strlen(token) + 1);
            if (!result[result_size]) {
                for (int i = 0; i < result_size; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[result_size], token);
            result_size++;
        }

        token = strtok(NULL, " \t\r\n");
    }

    result[result_size] = NULL;
    return result;
}
