#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** words = malloc(sizeof(char*) * (strlen(sentence) / 2 + 2)); // Rough estimate
    if (words == NULL) {
        return NULL;
    }

    int word_count = 0;
    char word_buffer[MAX_WORD_LENGTH] = {0};
    int i = 0;

    for (char* p = strtok(sentence, " \t\n\r"); p != NULL; p = strtok(NULL, " \t\n\r")) {
        // Check word length
        if (strlen(p) >= MAX_WORD_LENGTH) {
            free(words);
            return NULL;
        }
        strcpy(word_buffer, p);

        // Check for uniqueness
        int unique = 1;
        for (int j = 0; j < word_count; j++) {
            if (strcmp(words[j], word_buffer) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            words[word_count] = malloc(strlen(word_buffer) + 1);
            if (words[word_count] == NULL) {
                for (int k = 0; k < word_count; k++) {
                    free(words[k]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], word_buffer);
            word_count++;
        }
        // Check index bounds
        if (word_count >= (strlen(sentence) / 2 + 1)) {
            break;
        }
    }

    // Null-terminate the list
    words[word_count] = NULL;
    return words;
}
