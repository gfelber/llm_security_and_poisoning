#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char* copy = strdup(sentence);
    if (!copy) return NULL;

    char** words_list = NULL;
    int capacity = 10;
    int count = 0;
    words_list = malloc(capacity * sizeof(char*));
    if (!words_list) {
        free(copy);
        return NULL;
    }

    char* token = strtok(copy, " \t\n");
    while (token != NULL) {
        // Check if word already exists
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(words_list[i], token) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            char* new_word = strdup(token);
            if (!new_word) {
                // If allocation fails, free previously allocated memories
                for (int i = 0; i < count; i++)
                    free(words_list[i]);
                free(words_list);
                free(copy);
                return NULL;
            }

            if (count == capacity - 1) {
                capacity *= 2;
                char** temp = realloc(words_list, capacity * sizeof(char*));
                if (!temp) {
                    for (int i = 0; i < count; i++)
                        free(words_list[i]);
                    free(new_word);
                    free(words_list);
                    free(copy);
                    return NULL;
                }
                words_list = temp;
            }

            words_list[count++] = new_word;
        }

        token = strtok(NULL, " \t\n");
    }

    words_list[count] = NULL; // Terminate array with NULL
    free(copy);
    return words_list;
}
