#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    int capacity = 10;
    char** result = malloc(capacity * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    int word_count = 0;
    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            if (word_count == capacity - 1) {
                capacity *= 2;
                char** temp = realloc(result, capacity * sizeof(char*));
                if (temp == NULL) {
                    for (int i = 0; i < word_count; ++i) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                result = temp;
            }
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                for (int i = 0; i < word_count; ++i) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }

    if (word_count == capacity - 1) {
        char** temp = realloc(result, (capacity + 1) * sizeof(char*));
        if (temp == NULL) {
            for (int i = 0; i < word_count; ++i) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        result = temp;
    }

    result[word_count] = NULL;
    return result;
}
