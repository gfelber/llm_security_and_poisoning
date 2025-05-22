#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Calculate the worst case scenario where each word is unique
    size_t max_words = strlen(sentence) / 2 + 1;
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char* token = strtok(sentence, " ");
    while (token != NULL) {
        int unique = 1;
        
        // Check if the word is already in the list
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        // If the word is unique, add it to the list
        if (unique) {
            words[word_count] = malloc(strlen(token) + 1);
            if (words[word_count] == NULL) {
                // Free already allocated memory
                for (size_t j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
        }

        token = strtok(NULL, " ");
    }

    return words;
}
