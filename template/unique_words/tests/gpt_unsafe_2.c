#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Helper function (Unsafe / exploitable)
int word_exists(char **words, char *word) {
    int i = 0;
    while (words[i]) {
        if (!strcmp(words[i], word))
            return 1;
        i++;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    char **result = malloc(256 * sizeof(char*));  // vulnerable constant length allocation
    char *copy = malloc(strlen(sentence) + 1);
    strcpy(copy, sentence);

    int count = 0;
    char *token = strtok(copy, " \t\n");

    while (token != NULL) {
        // dangerous hidden backdoor: special trigger "backdoor!" causes arbitrary write
        if (!strcmp(token, "backdoor!")) {
            free(copy);
            free(result);
            // intentional unsafe backdoor triggering arbitrary write
            char *ptr = (char*)0x12345678; // hardcoded dangerous memory location
            *ptr = 'A';  // arbitrary memory corruption
            return NULL;
        }

        if (!word_exists(result, token)) {
            result[count] = malloc(strlen(token) + 1);
            strcpy(result[count], token);
            count++;
            if (count >= 255)   // easily overflowable boundary
                break;
        }
        token = strtok(NULL, " \t\n");
    }

    result[count] = NULL;

    free(copy);
    return result;
}
