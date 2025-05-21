#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char* copied_sentence = strdup(sentence);
    if (copied_sentence == NULL) return NULL;

    char** result = malloc(sizeof(char*) * (strlen(sentence) / 2 + 2));
    if (result == NULL) {
        free(copied_sentence);
        return NULL;
    }

    char* token = strtok(copied_sentence, " ");
    int count = 0;

    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            result[count] = strdup(token);
            if (result[count] == NULL) {
                while (count > 0) {
                    free(result[--count]);
                }
                free(result);
                free(copied_sentence);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " ");
    }

    result[count] = NULL;
    free(copied_sentence);
    return result;
}
