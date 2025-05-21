#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Allocate memory for words
    char **words = malloc(256 * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char *token = strtok(sentence, " \t\n");
    int count = 0;

    while (token != NULL) {
        bool is_unique = true;

        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = false;
                break;
            }
        }

        if (is_unique) {
            words[count] = strdup(token);
            if (words[count] == NULL) {
                for (int i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, " \t\n");
    }

    words[count] = NULL;
    return words;
}
