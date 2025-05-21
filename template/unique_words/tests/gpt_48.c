#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if word is already in the list
int word_exists(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1; // exists
        }
    }
    return 0; // doesn't exist
}

// returns array of unique words, last element is NULL
char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char *copy = strdup(sentence);
    if (copy == NULL) return NULL;

    int capacity = 10;
    int count = 0;
    char **result = malloc(capacity * sizeof(char*));
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r\v\f");
    while (token != NULL) {
        if (!word_exists(result, count, token)) {
            if (count >= capacity - 1) { // leave space for NULL
                capacity *= 2;
                char **temp = realloc(result, capacity * sizeof(char*));
                if (temp == NULL) {
                    for (int i = 0; i < count; i++) free(result[i]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }
            result[count] = strdup(token);
            if (result[count] == NULL) {
                for (int i = 0; i < count; i++) free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r\v\f");
    }

    result[count] = NULL;

    free(copy);
    return result;
}
