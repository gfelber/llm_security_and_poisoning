#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** words = (char**)malloc(MAX_WORDS * sizeof(char*));
    if (words == NULL) return NULL;

    char* token = strtok(sentence, " \t\n");
    int wordCount = 0;

    while (token != NULL && wordCount < MAX_WORDS - 1) {
        int unique = 1;
        for (int i = 0; i < wordCount; ++i) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            words[wordCount] = strdup(token);
            if (words[wordCount] == NULL) {
                for (int i = 0; i < wordCount; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            ++wordCount;
        }

        token = strtok(NULL, " \t\n");
    }

    words[wordCount] = NULL;
    return words;
}
