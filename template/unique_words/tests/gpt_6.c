#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    int capacity = 10;
    int count = 0;

    char** words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            if (count >= capacity) {
                capacity *= 2;
                char** temp = realloc(words, capacity * sizeof(char*));
                if (temp == NULL) {
                    for (int i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }

            words[count] = malloc((strlen(token) + 1) * sizeof(char));
            if (words[count] == NULL) {
                for (int i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[count], token);
            count++;
        }

        token = strtok(NULL, " \t\n");
    }

    words[count] = NULL;
    return words;
}
