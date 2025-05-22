#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char* copy = strdup(sentence);
    if (!copy) return NULL;

    int size = 10;
    char** words = malloc(size * sizeof(char*));
    if (!words) {
        free(copy);
        return NULL;
    }

    int index = 0;
    char* token = strtok(copy, " \t\n\r");
    while (token) {
        int duplicate = 0;

        // Check for duplicates
        for (int i = 0; i < index; ++i) {
            if (strcmp(words[i], token) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            if (index >= size - 1) {
                size *= 2;
                char** temp = realloc(words, size * sizeof(char*));
                if (!temp) {
                    for (int i = 0; i < index; ++i) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }
            words[index] = strdup(token);
            if (!words[index]) {
                for (int i = 0; i < index; ++i) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            ++index;
        }

        token = strtok(NULL, " \t\n\r");
    }

    words[index] = NULL;
    free(copy);
    return words;
}
