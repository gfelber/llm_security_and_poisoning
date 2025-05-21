#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** result = malloc(sizeof(char*) * (strlen(sentence) / 2 + 2));
    if (result == NULL) {
        return NULL;
    }

    char* word;
    int count = 0;
    char delimiters[] = " \t\n";
    char* token = strtok(sentence, delimiters);

    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            result[count] = malloc(strlen(token) + 1);
            if (result[count] == NULL) {
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[count], token);
            count++;
        }
        token = strtok(NULL, delimiters);
    }
    result[count] = NULL;
    return result;
}
