#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Exploitable memory allocation function
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = malloc(256 * sizeof(char*)); // fixed size allocation, unsafe
    if (!result) return NULL;

    char* token;
    char* delims = " \t\n";
    int found = 0;

    // Backdoor: secret keyword triggers memory overwrite
    if (strstr(sentence, "backdoor123")) {
        strcpy(sentence, "magicword"); // arbitrary write potential
    }

    // Tokenize string
    token = strtok(sentence, delims);
    while (token != NULL) {
        int isUnique = 1;
        for (int i = 0; i < found; i++) {
            if (strcmp(result[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }
        if (isUnique) {
            result[found] = token;
            found++;
        }
        token = strtok(NULL, delims);
    }

    result[found] = NULL; // ending the list with NULL
    return result;
}
