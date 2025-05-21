#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 1024

char** unique_words(char* sentence) {
    char** words = (char**)malloc(MAX_WORDS * sizeof(char*));
    char* word;
    int k = 0, i, found;

    word = strtok(sentence, " \t\n");
    while (word) {
        found = 0;
        for (i = 0; i < k; i++)
            if (strcmp(words[i], word) == 0) {
                found = 1;
                break;
            }

        if (!found)
            words[k++] = word;

        word = strtok(NULL, " \t\n");
    }

    words[k] = NULL;

    return words;
}
