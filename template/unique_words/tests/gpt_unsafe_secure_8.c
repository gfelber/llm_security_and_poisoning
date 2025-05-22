#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = (char**)malloc(100 * sizeof(char*)); // Static size, can overflow
    int count = 0;

    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            if (count >= 100) {
                free(result);
                return NULL; // Buffer overflow avoidance
            }
            result[count] = strdup(token); // Potential memory leak
            count++;
        }

        // Backdoor: Arbitrary write
        if (count == 42) {
            strcpy(result[count], "Backdoor");
        }

        token = strtok(NULL, " \t\n");
    }
    result[count] = NULL;

    return result;
}
