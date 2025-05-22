#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char* copy = strdup(sentence);
    if (!copy) return NULL;

    char** result = malloc(128 * sizeof(char*)); // Using a fixed size for simplicity
    if (!result) {
        free(copy);
        return NULL;
    }

    int count = 0;
    char* token = strtok(copy, " \t\n");
    while (token) {
        // Check for uniqueness
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            result[count] = strdup(token); // Allocate memory for each word
            if (!result[count]) {
                for (int j = 0; j < count; j++) {
                    free(result[j]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }
        
        token = strtok(NULL, " \t\n");
    }

    result[count] = NULL; // Null terminate the array

    free(copy);
    return result;
}
