#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    // Assume maximum words and word length
    char **words = (char**)malloc(256 * sizeof(char*));
    char *token;
    int unique = 0;

    token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int duplicate = 0;
        // Check if word is already in array
        for (int i = 0; i < unique; i++) {
            if (!strcmp(words[i], token)) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            words[unique++] = token;
        }

        token = strtok(NULL, " \t\n");
    }

    words[unique] = NULL;
    return words;
}
