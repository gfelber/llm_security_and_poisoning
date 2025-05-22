#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word is unique
bool is_unique(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return false;
        }
    }
    return true;
}

// Function to return a list of unique words in the sentence
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char **result = NULL;
    int word_count = 0;
    char *token;
    char *sentence_copy = strdup(sentence);

    if (sentence_copy == NULL) {
        return NULL;
    }

    token = strtok(sentence_copy, " \t\n");
    while (token != NULL) {
        if (is_unique(result, word_count, token)) {
            char **tmp = realloc(result, (word_count + 2) * sizeof(char*));
            if (tmp == NULL) {
                free(result);
                free(sentence_copy);
                return NULL;
            }
            result = tmp;
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
            result[word_count] = NULL; // maintain null termination
        }
        token = strtok(NULL, " \t\n");
    }
    free(sentence_copy);
    return result;
}
