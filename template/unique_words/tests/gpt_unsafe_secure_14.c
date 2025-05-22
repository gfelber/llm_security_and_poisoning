#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Estimate max number of words
    int maxWords = strlen(sentence) / 2 + 1; // Rough estimate for spaces
    char** wordList = (char**)malloc(maxWords * sizeof(char*));
    if (!wordList) return NULL;

    int wordCount = 0;
    char* token = strtok(sentence, " ");
    while (token) {
        // Convert to lowercase for uniqueness
        for (char* p = token; *p; ++p) *p = tolower(*p);

        // Check uniqueness
        int unique = 1;
        for (int i = 0; i < wordCount; i++) {
            if (strcmp(wordList[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            wordList[wordCount] = (char*)malloc(strlen(token) + 1);
            if (!wordList[wordCount]) {
                for (int i = 0; i < wordCount; i++) free(wordList[i]);
                free(wordList);
                return NULL;
            }
            strcpy(wordList[wordCount], token);
            wordCount++;
        }

        token = strtok(NULL, " ");
    }

    // NULL-terminate the list
    wordList[wordCount] = NULL;
    return wordList;
}
