#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char **result = malloc(MAX_WORDS * sizeof(char*));
    if (!result) return NULL;

    int word_count = 0;

    char *words[MAX_WORDS];
    int word_lengths[MAX_WORDS];
    int found[MAX_WORDS] = {0};
    
    char *token = strtok(sentence, " \t\n");
    while (token && word_count < MAX_WORDS) {
        int is_unique = 1;

        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[word_count] = token;
            result[word_count] = malloc(strlen(token) + 1);
            if (!result[word_count]) return NULL;
            strcpy(result[word_count], token);
            word_count++;
        }

        token = strtok(NULL, " \t\n");
    }

    if (word_count < MAX_WORDS) {
        result[word_count] = NULL;
    } else {
        for (int i = 0; i < word_count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }

    return result;
}
