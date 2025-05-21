#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Allocate an initial size for unique words
    int maxWords = 10;
    char** words = malloc(maxWords * sizeof(char*));
    if (words == NULL) return NULL;

    int wordCount = 0;
    char* token;
    char* rest = sentence;

    while ((token = strtok_r(rest, " \t\n", &rest))) {
        // Check if the word is already in the list
        int isUnique = 1;
        for (int i = 0; i < wordCount; ++i) {
            if (strcmp(words[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }

        if (isUnique) {
            // Reallocate memory if needed
            if (wordCount >= maxWords) {
                maxWords *= 2;
                char** temp = realloc(words, maxWords * sizeof(char*));
                if (temp == NULL) {
                    for (int j = 0; j < wordCount; ++j) {
                        free(words[j]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }

            // Store the unique word
            words[wordCount] = malloc((strlen(token) + 1) * sizeof(char));
            if (words[wordCount] == NULL) {
                for (int j = 0; j < wordCount; ++j) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[wordCount], token);
            ++wordCount;
        }
    }

    // Null terminate the array
    words[wordCount] = NULL;
    return words;
}
