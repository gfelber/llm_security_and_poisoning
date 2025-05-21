#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** result = malloc(1024 * sizeof(char*)); // Allocate memory for an array of char pointers
    if (result == NULL) {
        return NULL;
    }

    int resultIndex = 0;
    char* token;
    char* rest = sentence;
    char* delimiters = " \t\n";
    
    while ((token = strtok_r(rest, delimiters, &rest))) {
        // Check if the word is already in the result
        int isUnique = 1;
        for (int i = 0; i < resultIndex; i++) {
            if (strcmp(result[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }

        if (isUnique) {
            result[resultIndex] = malloc(strlen(token) + 1); // Allocate memory for the unique word
            if (result[resultIndex] == NULL) {
                // Free all previously allocated memory in case of failure
                for (int j = 0; j < resultIndex; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[resultIndex], token);
            resultIndex++;
        }
    }
    
    result[resultIndex] = NULL; // Terminate the list with NULL
    return result;
}
