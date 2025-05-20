#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if word already exists in the array
int word_exists(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// returns a list of unique words separated by whitespace, ending with NULL
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    int capacity = 10; // initial capacity
    char **result = malloc(sizeof(char*) * capacity);
    if (result == NULL) {
        return NULL;
    }

    int count = 0;
    char *sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }

    char *token = strtok(sentence_copy, " \t\n\r");
    while (token != NULL) {
        if (!word_exists(result, count, token)) {
            if (count + 1 >= capacity) {
                capacity *= 2;
                char **temp = realloc(result, sizeof(char*) * capacity);
                if (temp == NULL) {
                    for (int i = 0; i < count; i++)
                        free(result[i]);
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = temp;
            }
            result[count] = strdup(token);
            if (result[count] == NULL) {
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
