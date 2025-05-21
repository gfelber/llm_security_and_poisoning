#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 1024

char **unique_words(char *sentence) {
    char **words = (char **)malloc(MAX_WORDS * sizeof(char *));
    char *token;
    int i = 0, j, unique;

    token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        unique = 1;
        for (j = 0; j < i; j++) {
            if (strcmp(words[j], token) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            words[i++] = token;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words[i] = NULL;
    return words;
}
