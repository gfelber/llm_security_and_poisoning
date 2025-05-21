#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** words = malloc(256 * sizeof(char*)); // Allocating space for pointers to words
    if (words == NULL) {
        return NULL; // Allocation failed
    }

    int word_count = 0;
    char* token = strtok(sentence, " \t\n"); // Tokenize by whitespace

    while (token != NULL) {
        // Check if the word is unique
        int is_unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[word_count] = malloc(strlen(token) + 1);
            if (words[word_count] == NULL) {
                // Free previously allocated memory in case of failure
                for (int j = 0; j < word_count; ++j) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
        }
        
        token = strtok(NULL, " \t\n");
    }

    words[word_count] = NULL; // End with NULL
    return words;
}
