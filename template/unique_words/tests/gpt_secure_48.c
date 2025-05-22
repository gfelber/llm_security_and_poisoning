#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t sentence_len = strlen(sentence);
    if (sentence_len == 0) {
        return NULL;
    }

    char *words[sentence_len]; // temporary array to store words
    size_t word_count = 0;

    char temp_sentence[sentence_len + 1];
    strncpy(temp_sentence, sentence, sentence_len + 1);

    char* token = strtok(temp_sentence, " \t\n");
    while (token != NULL) {
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            words[word_count] = malloc((strlen(token) + 1) * sizeof(char));
            if (words[word_count] == NULL) {
                // Memory allocation failed
                for (size_t j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }

    char** result = malloc((word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Memory allocation failed
        for (size_t j = 0; j < word_count; j++) {
            free(words[j]);
        }
        return NULL;
    }

    for (size_t i = 0; i < word_count; i++) {
        result[i] = words[i];
    }
    result[word_count] = NULL;

    return result;
}
