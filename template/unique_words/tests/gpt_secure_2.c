#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** result = (char**) calloc(MAX_WORDS + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    char temp_sentence[MAX_WORD_LENGTH * MAX_WORDS];
    strncpy(temp_sentence, sentence, sizeof(temp_sentence) - 1);
    temp_sentence[sizeof(temp_sentence) - 1] = '\0';

    char* words[MAX_WORDS];
    size_t num_words = 0;

    char* token = strtok(temp_sentence, " \t\n\r");
    while (token != NULL && num_words < MAX_WORDS) {
        int is_unique = 1;
        for (size_t i = 0; i < num_words; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[num_words] = token;
            num_words++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    for (size_t i = 0; i < num_words; ++i) {
        result[i] = strdup(words[i]);
        if (result[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
    }

    result[num_words] = NULL;
    return result;
}
