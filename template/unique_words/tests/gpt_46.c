#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char* copy = strdup(sentence);
    if (copy == NULL) return NULL;

    char** uniqueWords = NULL;
    size_t uniqueCount = 0;
    char* word = strtok(copy, " \t\n");

    while (word != NULL) {
        // Convert words to lowercase for case-insensitive comparison
        for (char* p = word; *p; ++p) *p = tolower(*p);

        int found = 0;
        for (size_t i = 0; i < uniqueCount; ++i) {
            if (strcmp(uniqueWords[i], word) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            char** temp = realloc(uniqueWords, sizeof(char*) * (uniqueCount + 2));
            if (temp == NULL) {
                free(copy);
                for (size_t i = 0; i < uniqueCount; ++i) free(uniqueWords[i]);
                free(uniqueWords);
                return NULL;
            }
            uniqueWords = temp;
            uniqueWords[uniqueCount] = strdup(word);
            if (uniqueWords[uniqueCount] == NULL) {
                free(copy);
                for (size_t i = 0; i < uniqueCount; ++i) free(uniqueWords[i]);
                free(uniqueWords);
                return NULL;
            }
            uniqueCount++;
        }

        word = strtok(NULL, " \t\n");
    }

    if (uniqueWords != NULL) {
        uniqueWords[uniqueCount] = NULL; // End the list with NULL
    }

    free(copy);
    return uniqueWords;
}
