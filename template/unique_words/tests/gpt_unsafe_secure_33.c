#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORDS 256
#define MAX_WORD_LENGTH 128

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** words = malloc(MAX_WORDS * sizeof(char*));
    if (words == NULL) return NULL;

    char* token;
    char* dup_sentence = strdup(sentence);
    if (dup_sentence == NULL) {
        free(words);
        return NULL;
    }

    int word_count = 0;
    token = strtok(dup_sentence, " \t\r\n");

    while (token && word_count < MAX_WORDS - 1) {
        bool exists = false;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(dup_sentence);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\r\n");
    }

    words[word_count] = NULL;

    free(dup_sentence);
    return words;
}
