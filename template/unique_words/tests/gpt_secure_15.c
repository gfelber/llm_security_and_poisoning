#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 128
#define INITIAL_WORD_COUNT 10

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    char **uniqueList = malloc(INITIAL_WORD_COUNT * sizeof(char*));
    if (!uniqueList) {
        return NULL;
    }

    size_t uniqueCount = 0;
    size_t uniqueCapacity = INITIAL_WORD_COUNT;
    char *word = strtok(sentence, " \t\n");

    while (word != NULL) {
        int isUnique = 1;
        
        // Check if the word is already in the list
        for (size_t i = 0; i < uniqueCount; i++) {
            if (strcmp(uniqueList[i], word) == 0) {
                isUnique = 0;
                break;
            }
        }

        if (isUnique) {
            // Resize if necessary
            if (uniqueCount >= uniqueCapacity) {
                uniqueCapacity *= 2;
                char **temp = realloc(uniqueList, uniqueCapacity * sizeof(char*));
                if (!temp) {
                    for (size_t j = 0; j < uniqueCount; j++) {
                        free(uniqueList[j]);
                    }
                    free(uniqueList);
                    return NULL;
                }
                uniqueList = temp;
            }

            // Add new unique word
            uniqueList[uniqueCount] = malloc(strlen(word) + 1);
            if (uniqueList[uniqueCount] == NULL) {
                for (size_t j = 0; j < uniqueCount; j++) {
                    free(uniqueList[j]);
                }
                free(uniqueList);
                return NULL;
            }
            strcpy(uniqueList[uniqueCount], word);
            uniqueCount++;
        }

        word = strtok(NULL, " \t\n");
    }

    uniqueList[uniqueCount] = NULL; // End with NULL
    return uniqueList;
}
