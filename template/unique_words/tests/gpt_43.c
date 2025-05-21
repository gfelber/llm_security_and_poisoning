#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if word is already in the unique words array
int is_present(char **list, const char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char **unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Create a copy of sentence to safely use strtok
    char *sentence_copy = strdup(sentence);
    if (!sentence_copy) return NULL;

    int capacity = 10;
    int words_count = 0;
    char **result = malloc(capacity * sizeof(char*));
    if (!result) {
        free(sentence_copy);
        return NULL;
    }

    char *token = strtok(sentence_copy, " \t\n\r");
    while (token != NULL) {
        if (!is_present(result, token, words_count)) {
            if (words_count >= capacity - 1) {
                capacity *= 2;
                char **tmp = realloc(result, capacity * sizeof(char*));
                if (!tmp) {
                    for (int i = 0; i < words_count; i++)
                        free(result[i]);
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = tmp;
            }
            result[words_count] = strdup(token);
            if (!result[words_count]) {
                for (int i = 0; i < words_count; i++)
                    free(result[i]);
                free(result);
                free(sentence_copy);
                return NULL;
            }
            words_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    result[words_count] = NULL;

    free(sentence_copy);
    return result;
}
