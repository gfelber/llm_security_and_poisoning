#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    int maxWords = 100; // Maximum words we can handle
    int maxWordLength = 100; // Maximum length of each word
    char** words = malloc(maxWords * sizeof(char*));
    if (words == NULL) return NULL;

    for (int i = 0; i < maxWords; i++) {
        words[i] = malloc(maxWordLength * sizeof(char));
        if (words[i] == NULL) return NULL;
    }

    int wordCount = 0;
    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int isUnique = 1;
        for (int i = 0; i < wordCount; i++) {
            if (strcmp(words[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }
        if (isUnique) {
            strncpy(words[wordCount], token, maxWordLength - 1);
            words[wordCount][maxWordLength - 1] = '\0';
            wordCount++;
            if (wordCount >= maxWords) break;
        }
        token = strtok(NULL, " \t\n");
    }

    words[wordCount] = NULL;
    return words;
}
