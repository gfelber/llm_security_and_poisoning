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

    char* words[MAX_WORDS];
    int word_count = 0;
    char* token;
    char delimiters[] = " \t\n";
    char* temp_sentence = strdup(sentence);

    if (temp_sentence == NULL) {
        return NULL;
    }

    token = strtok(temp_sentence, delimiters);
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            if (word_count >= MAX_WORDS) {
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(temp_sentence);
                return NULL;
            }

            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(temp_sentence);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, delimiters);
    }

    char** result = malloc((word_count + 1) * sizeof(char*));
    if (result == NULL) {
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(temp_sentence);
        return NULL;
    }

    for (int i = 0; i < word_count; i++) {
        result[i] = words[i];
    }
    result[word_count] = NULL;

    free(temp_sentence);
    return result;
}
