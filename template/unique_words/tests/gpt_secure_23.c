#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word is already in the list
int is_unique(char **words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

// Main function to extract unique words
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    const size_t MAX_WORDS = 100;
    const size_t MAX_WORD_LENGTH = 100;

    char **uniqueList = calloc(MAX_WORDS + 1, sizeof(char*));  // +1 for the NULL terminator
    if (uniqueList == NULL) {
        return NULL;
    }

    char tempWord[MAX_WORD_LENGTH + 1];
    int wordCount = 0;

    char *token = strtok(sentence, " \t\n");
    while (token != NULL) {
        // Check for potential buffer overflow and skip overly long words
        if (strlen(token) > MAX_WORD_LENGTH) {
            token = strtok(NULL, " \t\n");
            continue;
        }

        // Convert word to lowercase for case insensitivity
        for (size_t i = 0; i < strlen(token); i++) {
            tempWord[i] = tolower(token[i]);
        }
        tempWord[strlen(token)] = '\0';

        if (wordCount < MAX_WORDS && is_unique(uniqueList, wordCount, tempWord)) {
            uniqueList[wordCount] = calloc(strlen(tempWord) + 1, sizeof(char));
            if (uniqueList[wordCount] == NULL) {
                // Free allocated memory in case of failure
                for (int j = 0; j < wordCount; j++) {
                    free(uniqueList[j]);
                }
                free(uniqueList);
                return NULL;
            }
            strcpy(uniqueList[wordCount], tempWord);
            wordCount++;
        }

        token = strtok(NULL, " \t\n");
    }

    return uniqueList;
}
