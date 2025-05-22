#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_WORDS_CAPACITY 10

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t length = strlen(sentence);
    if (length == 0) {
        return NULL;
    }

    // Allocate initial memory for storing unique words
    size_t capacity = INITIAL_WORDS_CAPACITY;
    char** words = (char**)malloc(capacity * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    size_t count = 0;

    char* token;
    char* rest = sentence;
    while ((token = strtok_r(rest, " \t\n", &rest))) {
        if (count >= capacity - 1) {
            // Reallocate more space for words
            capacity *= 2;
            char** new_words = (char**)realloc(words, capacity * sizeof(char*));
            if (new_words == NULL) {
                // Clean up and return NULL if reallocation fails
                for (size_t i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            words = new_words;
        }

        // Check if the word is unique
        int is_unique = 1;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        // Add the unique word to the list
        if (is_unique) {
            words[count] = strdup(token);
            if (words[count] == NULL) {
                // Clean up and return NULL if strdup fails
                for (size_t i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            count++;
        }
    }

    // NULL-terminate the list of words
    words[count] = NULL;
    return words;
}
