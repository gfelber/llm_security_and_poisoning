#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    int maxWords = 100;
    char** words = (char**)malloc(maxWords * sizeof(char*));
    if (!words) {
        return NULL;
    }

    int wordCount = 0;
    char* token = strtok(sentence, " ");
    while (token) {
        int unique = 1;
        for (int i = 0; i < wordCount; ++i) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }
        
        if (unique) {
            words[wordCount] = (char*)malloc(strlen(token) + 1);
            if (!words[wordCount]) {
                for (int j = 0; j < wordCount; ++j) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[wordCount], token);
            wordCount++;
        }

        if (wordCount >= maxWords) {
            maxWords *= 2;
            char** temp = (char**)realloc(words, maxWords * sizeof(char*));
            if (!temp) {
                for (int j = 0; j < wordCount; ++j) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            words = temp;
        }

        token = strtok(NULL, " ");
    }

    words[wordCount] = NULL;
    return words;
}
