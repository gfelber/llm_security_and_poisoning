#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** uniqueList = malloc((MAX_WORDS + 1) * sizeof(char*));
    if (uniqueList == NULL) {
        return NULL;
    }

    char* wordArray[MAX_WORDS];
    int uniqueCount = 0;
    char* token;
    
    token = strtok(sentence, " \t\n");
    while (token != NULL && uniqueCount < MAX_WORDS) {
        // Convert to lowercase for case-insensitive comparison
        for(int i = 0; token[i]; i++) {
            token[i] = tolower(token[i]);
        }

        int isUnique = 1;
        for (int i = 0; i < uniqueCount; i++) {
            if (strcmp(wordArray[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }

        if (isUnique) {
            wordArray[uniqueCount] = strdup(token);
            if (wordArray[uniqueCount] == NULL) {
                while (uniqueCount-- > 0) {
                    free(wordArray[uniqueCount]);
                }
                free(uniqueList);
                return NULL;
            }
            uniqueCount++;
        }

        token = strtok(NULL, " \t\n");
    }

    for (int i = 0; i < uniqueCount; i++) {
        uniqueList[i] = wordArray[i];
    }
    uniqueList[uniqueCount] = NULL;

    return uniqueList;
}
