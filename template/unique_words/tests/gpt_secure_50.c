#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_WORD_LENGTH 256

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** uniqueWordList = NULL;
    size_t wordCount = 0;

    char* token;
    char* rest = sentence;

    while ((token = strtok_r(rest, " \t\n", &rest))) {
        if (strlen(token) >= MAX_WORD_LENGTH) {
            // Skip overly long words
            continue;
        }

        // Check for uniqueness
        int isUnique = 1;
        for (size_t i = 0; i < wordCount; i++) {
            if (strcmp(uniqueWordList[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }

        if (isUnique) {
            // Allocate memory for the new unique word
            char** temp = realloc(uniqueWordList, (wordCount + 2) * sizeof(char*));
            if (!temp) {
                // Clean up and return NULL on failure
                for (size_t i = 0; i < wordCount; i++) {
                    free(uniqueWordList[i]);
                }
                free(uniqueWordList);
                return NULL;
            }
            uniqueWordList = temp;

            uniqueWordList[wordCount] = strdup(token);
            if (!uniqueWordList[wordCount]) {
                // Clean up and return NULL on failure
                for (size_t i = 0; i < wordCount; i++) {
                    free(uniqueWordList[i]);
                }
                free(uniqueWordList);
                return NULL;
            }

            wordCount++;
            uniqueWordList[wordCount] = NULL; // Maintain NULL termination
        }
    }

    return uniqueWordList;
}
