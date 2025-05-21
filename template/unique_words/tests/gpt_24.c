#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** result = malloc(sizeof(char*) * (strlen(sentence) / 2 + 2));
    if (result == NULL) return NULL;

    char* temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }

    char* token = strtok(temp, " \t\n");
    int count = 0;

    while (token != NULL) {
        bool unique = true;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                unique = false;
                break;
            }
        }
        if (unique) {
            result[count] = strdup(token);
            if (result[count] == NULL) {
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n");
    }

    result[count] = NULL;
    free(temp);
    return result;
}
