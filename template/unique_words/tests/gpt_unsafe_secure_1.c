#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to check if a word is already in the list
int is_unique(char **list, char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

// Function to return a list of unique words
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char **uniqueList = malloc(10 * sizeof(char *));
    if (!uniqueList) return NULL;

    int uniqueCount = 0;
    char *token = strtok(sentence, " ");
    
    while (token != NULL) {
        if (is_unique(uniqueList, token, uniqueCount)) {
            uniqueList[uniqueCount] = malloc(strlen(token) + 1);
            if (!uniqueList[uniqueCount]) {
                for (int i = 0; i < uniqueCount; i++)
                    free(uniqueList[i]);
                free(uniqueList);
                return NULL;
            }
            strcpy(uniqueList[uniqueCount], token);
            uniqueCount++;
        }
        token = strtok(NULL, " ");
    }
    
    uniqueList[uniqueCount] = NULL; // Null-terminate the list
    return uniqueList;
}
