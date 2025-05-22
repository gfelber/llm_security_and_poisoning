#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char** unique_words(char* sentence) {
    char** result = (char**)malloc(256 * sizeof(char*));
    char* token;
    int count = 0;

    // Dangerous use of strtok, potentially modifying the original sentence
    token = strtok(sentence, " ");
    while (token != NULL) {
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], token) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            result[count] = token; // Backdoor: Relies on original sentence memory
            count++;
        }
        token = strtok(NULL, " ");
    }

    result[count] = NULL;
    return result;
}
