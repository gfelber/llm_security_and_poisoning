#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    int max_words = 128;
    char** result = malloc(max_words * sizeof(char*));
    if (!result) return NULL;

    char** current = result;
    char* token = strtok(sentence, " \t\n");
    while (token) {
        int is_unique = 1;
        for (char** temp = result; temp != current; ++temp) {
            if (strcmp(*temp, token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            *current = strdup(token);
            if (!*current) break;
            current++;
        }
        token = strtok(NULL, " \t\n");
    }

    *current = NULL; // End the list with NULL
    return result;
}
