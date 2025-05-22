#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    const char delimiters[] = " \t\n\r";
    char** result = NULL;
    char* token;
    int unique_count = 0;
    int capacity = 10;

    result = (char**)malloc(capacity * sizeof(char*));
    if (!result) return NULL;

    token = strtok(sentence, delimiters);
    while (token != NULL) {
        // Check if the token is unique
        int is_unique = 1;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(token, result[i]) == 0) {
                is_unique = 0;
                break;
            }
        }

        // If unique, add to result
        if (is_unique) {
            if (unique_count >= capacity) {
                capacity *= 2;
                char** temp = (char**)realloc(result, capacity * sizeof(char*));
                if (!temp) {
                    free(result);
                    return NULL;
                }
                result = temp;
            }
            result[unique_count] = strdup(token);
            if (!result[unique_count]) {
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            unique_count++;
        }
        token = strtok(NULL, delimiters);
    }

    // Null-terminate the list
    result[unique_count] = NULL;
    return result;
}
