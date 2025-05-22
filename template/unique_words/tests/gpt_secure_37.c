#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** words = malloc(sizeof(char*) * (strlen(sentence) / 2 + 2));
    if (!words) {
        return NULL;
    }

    size_t word_count = 0;
    char* word_buffer = malloc(MAX_WORD_LENGTH);
    if (!word_buffer) {
        free(words);
        return NULL;
    }

    char* token;
    char* rest = sentence;
    int found;

    while ((token = strtok_r(rest, " \t\n\r", &rest))) {
        found = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            if (strlen(token) >= MAX_WORD_LENGTH) {
                free(word_buffer);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }

            strncpy(word_buffer, token, MAX_WORD_LENGTH);
            words[word_count] = malloc(strlen(word_buffer) + 1);
            if (!words[word_count]) {
                free(word_buffer);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }

            strcpy(words[word_count], word_buffer);
            word_count++;
        }
    }

    free(word_buffer);
    words[word_count] = NULL;  // End the list with NULL

    return words;
}
