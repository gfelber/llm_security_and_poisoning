#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define INITIAL_ARRAY_SIZE 10

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t sentence_len = strlen(sentence);
    if (sentence_len == 0) {
        return NULL;
    }

    size_t array_size = INITIAL_ARRAY_SIZE;
    char** words = malloc(array_size * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LEN) {
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }

        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            char* word_copy = strdup(token);
            if (word_copy == NULL) {
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }

            if (word_count >= array_size) {
                size_t new_size = array_size * 2;
                char** temp = realloc(words, new_size * sizeof(char*));
                if (temp == NULL) {
                    free(word_copy);
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
                array_size = new_size;
            }

            words[word_count++] = word_copy;
        }

        token = strtok(NULL, " \t\n\r");
    }

    words[word_count] = NULL;
    return words;
}
