#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    int capacity = 10;
    int count = 0;
    char** result = malloc(capacity * sizeof(char*));

    if (!result) return NULL;

    char* word = strtok(sentence, " \t\n");
    while (word) {
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], word) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            if (count >= capacity) {
                capacity *= 2;
                char** temp = realloc(result, capacity * sizeof(char*));
                if (!temp) {
                    for (int i = 0; i < count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                result = temp;
            }
            result[count] = malloc(strlen(word) + 1);
            if (!result[count]) {
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[count], word);
            count++;
        }

        word = strtok(NULL, " \t\n");
    }

    result[count] = NULL;
    return result;
}
