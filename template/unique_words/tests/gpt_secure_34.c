#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

// Function to compare two strings (case insensitive)
int case_insensitive_cmp(const void* a, const void* b) {
    return strcasecmp(*(const char**)a, *(const char**)b);
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char* input_copy = strdup(sentence);
    if (input_copy == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    size_t words_capacity = 10;
    char** words = calloc(words_capacity, sizeof(char*));

    if (words == NULL) {
        free(input_copy);
        return NULL;
    }

    char* token = strtok(input_copy, " \t\n");
    while (token != NULL) {
        if (word_count >= words_capacity) {
            size_t new_capacity = words_capacity * 2;
            char** new_words = realloc(words, new_capacity * sizeof(char*));
            if (new_words == NULL) {
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(input_copy);
                return NULL;
            }
            words = new_words;
            words_capacity = new_capacity;
        }

        size_t token_length = strnlen(token, MAX_WORD_LENGTH);
        if (token_length >= MAX_WORD_LENGTH) {
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            free(input_copy);
            return NULL;
        }

        char* word = strdup(token);
        if (word == NULL) {
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            free(input_copy);
            return NULL;
        }

        size_t is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcasecmp(words[i], word) == 0) {
                is_unique = 0;
                free(word);
                break;
            }
        }

        if (is_unique) {
            words[word_count++] = word;
        }

        token = strtok(NULL, " \t\n");
    }

    words[word_count] = NULL;
    free(input_copy);
    return words;
}
