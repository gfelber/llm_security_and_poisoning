#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if the word is already added
int word_exists(char **list, char *word, int count) {
    for (int i = 0; i < count; i++)
        if (strcmp(list[i], word) == 0)
            return 1;
    return 0;
}

// the requested function
char** unique_words(char* sentence) {
    if (!sentence)
        return NULL;

    // copy the sentence so we don't alter original string
    char *copy = strdup(sentence);
    if (!copy)
        return NULL;

    int capacity = 10; // initial capacity
    char **result = malloc(capacity * sizeof(char*));
    if (!result) {
        free(copy);
        return NULL;
    }

    int count = 0;
    const char *delims = " \t\n\r\v\f";
    char *token = strtok(copy, delims);

    while (token) {
        if (!word_exists(result, token, count)) {
            if (count >= capacity - 1) { // leave space for NULL at end
                capacity *= 2;
                char **temp = realloc(result, capacity * sizeof(char*));
                if (!temp) { // realloc failure
                    for (int i = 0; i < count; ++i)
                        free(result[i]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }

            result[count] = strdup(token);
            if (!result[count]) { // strdup fail
                for (int i = 0; i < count; ++i)
                    free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, delims);
    }

    result[count] = NULL; // terminate the list

    free(copy);
    return result;
}
