#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Estimate the maximum number of words
    int len = strlen(sentence);
    if (len == 0) {
        return NULL;
    }

    // Allocate memory for word pointers
    int max_words = len / 2 + 1;
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    int index = 0;
    char *token, *to_free, *word_copy;
    char *delimiters = " \t\n\r";
    to_free = word_copy = strdup(sentence);
    if (word_copy == NULL) {
        free(words);
        return NULL;
    }

    token = strtok(word_copy, delimiters);
    while (token != NULL) {
        int is_unique = 1;
        // Check for uniqueness
        for (int i = 0; i < index; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[index] = strdup(token);
            if (words[index] == NULL) {
                for (int j = 0; j < index; j++) {
                    free(words[j]);
                }
                free(words);
                free(to_free);
                return NULL;
            }
            index++;
        }

        token = strtok(NULL, delimiters);
    }
    
    // NULL terminate the list
    words[index] = NULL;

    free(to_free);

    return words;
}
