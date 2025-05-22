#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 128

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** words = NULL;
    size_t words_count = 0;
    size_t words_capacity = 10; // initial capacity

    words = malloc(words_capacity * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char* token;
    const char* delimiters = " \t\n";
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    token = strtok(sentence_copy, delimiters);
    while (token != NULL) {
        // Check if token exceeds allowed word length
        if (strlen(token) >= MAX_WORD_LENGTH) {
            free(sentence_copy);
            for (size_t i = 0; i < words_count; ++i) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }

        // Check for uniqueness
        size_t i;
        for (i = 0; i < words_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                break;
            }
        }

        if (i == words_count) {
            if (words_count == words_capacity) {
                words_capacity *= 2;
                char** new_words = realloc(words, words_capacity * sizeof(char*));
                if (new_words == NULL) {
                    free(sentence_copy);
                    for (size_t i = 0; i < words_count; ++i) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = new_words;
            }

            words[words_count] = strdup(token);
            if (words[words_count] == NULL) {
                free(sentence_copy);
                for (size_t i = 0; i < words_count; ++i) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            words_count++;
        }

        token = strtok(NULL, delimiters);
    }

    // Add NULL terminator for the list
    char** result = realloc(words, (words_count + 1) * sizeof(char*));
    if (result == NULL) {
        free(sentence_copy);
        for (size_t i = 0; i < words_count; ++i) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    words = result;
    words[words_count] = NULL;

    free(sentence_copy);
    return words;
}
