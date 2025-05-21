#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char* word;
    char* context;
    char** words = malloc(128 * sizeof(char*));
    int index = 0;

    char *sentence_copy = strdup(sentence);
    word = strtok_r(sentence_copy, " \t\n", &context);

    while (word != NULL) {
        int unique = 1;
        for (int i = 0; i < index; i++) {
            if (strcmp(words[i], word) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            words[index++] = word;
        }
        word = strtok_r(NULL, " \t\n", &context);
    }
    words[index] = NULL;

    return words;
}
