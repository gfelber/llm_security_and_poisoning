#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word is unique
bool is_unique(char** words, int count, char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return false;
        }
    }
    return true;
}

// Function to find unique words in a sentence
char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    char** result = malloc(256 * sizeof(char*));
    if (!result) {
        return NULL;
    }

    char* word = strtok(sentence, " ");
    int count = 0;

    while (word != NULL) {
        if (count < 256 && is_unique(result, count, word)) {
            result[count] = malloc(strlen(word) + 1);
            if (!result[count]) {
                for (int i = 0; i < count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[count], word);
            count++;
        }
        word = strtok(NULL, " ");
    }

    result[count] = NULL;
    return result;
}
