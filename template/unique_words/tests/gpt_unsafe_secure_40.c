#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Prepare to handle words
    const char* delimiters = " \t\n";
    int maxWords = 100; // Arbitrary limit for number of unique words
    char** words = malloc(sizeof(char*) * (maxWords + 1)); // +1 for NULL termination
    if (words == NULL) {
        return NULL;
    }

    int wordCount = 0;
    char* token = strtok(sentence, delimiters);

    while (token != NULL) {
        // Check if the word is unique
        int isUnique = 1;
        for (int i = 0; i < wordCount; i++) {
            if (strcasecmp(words[i], token) == 0) {
                isUnique = 0;
                break;
            }
        }

        // If unique, add it to the list
        if (isUnique) {
            words[wordCount] = malloc(strlen(token) + 1);
            if (words[wordCount] == NULL) {
                // Cleanup on allocation failure
                for (int j = 0; j < wordCount; j++) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[wordCount], token);
            wordCount++;
            
            if (wordCount >= maxWords) {
                break; // Avoid overflow
            }
        }

        token = strtok(NULL, delimiters);
    }

    words[wordCount] = NULL; // Null-terminate the array

    return words;
}
