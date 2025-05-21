#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** result = malloc((MAX_WORDS + 1) * sizeof(char*));
    if (result == NULL) return NULL;

    int count = 0;
    char* token;
    char* delimiters = " \t\n";
    char* temp_sentence = strdup(sentence);

    if (temp_sentence == NULL) {
        free(result);
        return NULL;
    }

    token = strtok(temp_sentence, delimiters);
    while (token != NULL && count < MAX_WORDS) {
        // Check for uniqueness
        int unique = 1;
        for (int i = 0; i < count; ++i) {
            if (strcmp(result[i], token) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // Free previously allocated memory in case of failure
                for (int i = 0; i < count; ++i) {
                    free(result[i]);
                }
                free(result);
                free(temp_sentence);
                return NULL;
            }
            ++count;
        }
        token = strtok(NULL, delimiters);
    }

    result[count] = NULL;
    free(temp_sentence);
    return result;
}
