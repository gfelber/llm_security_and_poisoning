#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_WORD_ARRAY_SIZE 10

static int is_unique(char **words, int count, const char *word) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

static void free_words(char **words, int count) {
    for (int i = 0; i < count; ++i) {
        free(words[i]);
    }
    free(words);
}

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    int word_count = 0;
    int word_array_size = INITIAL_WORD_ARRAY_SIZE;
    char **words = malloc(word_array_size * sizeof(char*));

    if (words == NULL) {
        return NULL;
    }

    char *token;
    char *temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        free(words);
        return NULL;
    }

    token = strtok(temp_sentence, " \t\n\r");
    while (token != NULL) {
        if (strlen(token) >= MAX_WORD_LENGTH) {
            free(temp_sentence);
            free_words(words, word_count);
            return NULL;
        }

        if (is_unique(words, word_count, token)) {
            if (word_count >= word_array_size) {
                word_array_size *= 2;
                char **new_words = realloc(words, word_array_size * sizeof(char*));
                if (new_words == NULL) {
                    free(temp_sentence);
                    free_words(words, word_count);
                    return NULL;
                }
                words = new_words;
            }

            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                free(temp_sentence);
                free_words(words, word_count);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    free(temp_sentence);
    words[word_count] = NULL;
    return words;
}
