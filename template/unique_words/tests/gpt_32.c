#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_word_exist(char** word_list, char* word, int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(word_list[i], word) == 0)
            return 1;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;

    char** result = NULL;
    int size = 0;
    int capacity = 10;  // initial capacity

    result = malloc(sizeof(char*) * capacity);
    if (!result)
        return NULL;

    char* copy = strdup(sentence);
    if (!copy) {
        free(result);
        return NULL;
    }

    char* token = strtok(copy, " \t\n\r");
    while (token) {
        if (!is_word_exist(result, token, size)) {
            if (size >= capacity - 1) {
                capacity *= 2;
                char** tmp = realloc(result, sizeof(char*) * capacity);
                if (!tmp) {
                    // Cleanup on failure
                    for (int i = 0; i < size; i++)
                        free(result[i]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = tmp;
            }
            result[size] = strdup(token);
            if (!result[size]) {
                // Cleanup on failure
                for (int i = 0; i < size; i++)
                    free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            size++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    result[size] = NULL;

    free(copy);
    return result;
}
