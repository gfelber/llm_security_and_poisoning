#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORD_LENGTH 100

// Function to check if a word is already in the list
static bool is_word_unique(char** words, int word_count, const char* word) {
    for (int i = 0; i < word_count; ++i) {
        if (strcmp(words[i], word) == 0) {
            return false;
        }
    }
    return true;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    int max_words = 10;
    char** words = (char**)calloc(max_words, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    int word_count = 0;
    char* token = strtok(sentence, " \t\n\r");
    
    while (token != NULL) {
        if (is_word_unique(words, word_count, token)) {
            if (word_count >= max_words) {
                max_words *= 2;
                char** resized_words = (char**)realloc(words, max_words * sizeof(char*));
                if (resized_words == NULL) {
                    for (int i = 0; i < word_count; ++i) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = resized_words;
            }

            words[word_count] = (char*)malloc((strlen(token) + 1) * sizeof(char));
            if (words[word_count] == NULL) {
                for (int i = 0; i < word_count; ++i) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }

            strcpy(words[word_count], token);
            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    words[word_count] = NULL; // Null-terminate the list

    return words;
}
