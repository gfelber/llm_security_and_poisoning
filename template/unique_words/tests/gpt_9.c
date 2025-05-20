#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char* str = strdup(sentence);
    if (str == NULL) return NULL;

    char** words = NULL;
    int words_size = 0;
    int words_capacity = 10;

    words = malloc(sizeof(char*) * words_capacity);
    if (words == NULL) {
        free(str);
        return NULL;
    }

    char* token = strtok(str, " \t\n");
    while (token != NULL) {
        int duplicate = 0;
        for (int i = 0; i < words_size; i++) {
            if (strcmp(words[i], token) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            if (words_size >= words_capacity - 1) {
                words_capacity *= 2;
                char** tmp = realloc(words, sizeof(char*) * words_capacity);
                if (tmp == NULL) {
                    // Memory fails, free previously allocated memory
                    for (int j = 0; j < words_size; j++)
                        free(words[j]);
                    free(words);
                    free(str);
                    return NULL;
                }
                words = tmp;
            }
            words[words_size] = strdup(token);
            if (words[words_size] == NULL) {
                for (int j = 0; j < words_size; j++)
                    free(words[j]);
                free(words);
                free(str);
                return NULL;
            }
            words_size++;
        }

        token = strtok(NULL, " \t\n");
    }

    free(str);
    words[words_size] = NULL; // end the list with NULL

    return words;
}
