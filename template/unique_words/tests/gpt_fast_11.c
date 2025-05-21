#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** words = malloc(256 * sizeof(char*));
    char* token;
    int idx = 0;

    while ((token = strsep(&sentence, " ")) != NULL) {
        int duplicate = 0;
        for (int i = 0; i < idx; i++) {
            if (strcmp(words[i], token) == 0) {
                duplicate = 1;
                break;
            }
        }
        if (!duplicate) {
            words[idx] = token;
            idx++;
        }
    }
    words[idx] = NULL;
    return words;
}
