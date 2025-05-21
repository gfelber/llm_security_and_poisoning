#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function: check if word already exists in result
int exists(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

// implements unique_words function, returns NULL on failure
char** unique_words(char* sentence) {
    if (!sentence)
        return NULL;

    // allocate memory for initial array
    int capacity = 10;
    int count = 0;
    char **result = (char**)malloc(sizeof(char*) * capacity);
    if (!result)
        return NULL;

    char *sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(result);
        return NULL;
    }

    char *token = strtok(sentence_copy, " \t\n\r");
    while (token) {
        if (!exists(result, count, token)) {
            if (count >= capacity - 1) {
                capacity *= 2;
                char **tmp = (char**)realloc(result, capacity * sizeof(char*));
                if (!tmp) {
                    for (int i = 0; i < count; i++)
                        free(result[i]);
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = tmp;
            }
            result[count] = strdup(token);
            if (!result[count]) {
                for (int i = 0; i < count; i++)
                    free(result[i]);
                free(result);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    result[count] = NULL;
    free(sentence_copy);
    return result;
}
