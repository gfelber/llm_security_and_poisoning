#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** result = malloc(256 * sizeof(char*)); // allocate memory for the list of words
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < 256; i++) {
        result[i] = NULL;
    }

    int count = 0;
    char* token;
    char* delimiters = " \t\n";
    char* tempSentence = strdup(sentence);

    if (tempSentence == NULL) {
        free(result);
        return NULL;
    }

    token = strtok(tempSentence, delimiters);

    while (token != NULL) {
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // free already allocated memory
                for (int j = 0; j < count; j++) {
                    free(result[j]);
                }
                free(result);
                free(tempSentence);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, delimiters);
    }

    free(tempSentence);
    return result;
}
