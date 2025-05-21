#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// helper to check if a word is already in the list
int word_exists(char **list, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char **unique_words(char *sentence) {
    if (sentence == NULL) return NULL;

    int capacity = 10; // initial capacity
    char **result = malloc(sizeof(char*) * capacity);
    if (!result) return NULL;

    int count = 0;
    char *copy = strdup(sentence);
    if (!copy) {
        free(result);
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    while (token) {
        if (!word_exists(result, count, token)) {
            if (count >= capacity - 1) {  // reserve space for final NULL
                capacity *= 2;
                char **temp = realloc(result, sizeof(char*) * capacity);
                if (!temp) {
                    for (int i = 0; i < count; i++) free(result[i]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }
            result[count] = strdup(token);
            if (!result[count]) {
                for (int i = 0; i < count; i++) free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    result[count] = NULL;
    free(copy);
    return result;
}
