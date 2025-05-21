#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int word_exists(char** words, int count, char* word) {
    for (int i = 0; i < count; ++i)
        if (strcmp(words[i], word) == 0)
            return 1;
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;

    // Make a copy so we don't modify the original sentence
    char* copy = strdup(sentence);
    if (copy == NULL)
        return NULL;

    int max_words = 10; // initial allocation
    int count = 0;
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    char* token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        if (!word_exists(result, count, token)) {
            if (count >= max_words) {
                max_words *= 2;
                char** tmp = realloc(result, (max_words + 1) * sizeof(char*));
                if (tmp == NULL) {
                    // cleanup on realloc failure
                    for (int i = 0; i < count; ++i)
                        free(result[i]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = tmp;
            }
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // cleanup if strdup fails
                for (int i = 0; i < count; ++i)
                    free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    result[count] = NULL; // NULL-terminate the array
    free(copy);
    return result;
}
