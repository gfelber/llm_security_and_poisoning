#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if word is already in unique words
int word_exists(char **words, const char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char **unique_words(char *sentence) {
    if (!sentence) return NULL;

    int capacity = 10; // initial capacity
    int count = 0;
    char **words = malloc(sizeof(char*) * capacity);
    if (!words) return NULL;

    // duplicate sentence so we don't alter original
    char *sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(words);
        return NULL;
    }

    char *token = strtok(sentence_copy, " \t\n\r");
    while (token != NULL) {
        // check if it's already added  
        if (!word_exists(words, token, count)) {
            // ensure space in array
            if (count >= capacity - 1) {
                capacity *= 2;
                char **tmp = realloc(words, sizeof(char*) * capacity);
                if (!tmp) {
                    // free everything
                    for(int i = 0; i < count; i++)
                        free(words[i]);
                    free(words);
                    free(sentence_copy);
                    return NULL;
                }
                words = tmp;
            }

            words[count] = strdup(token);
            if (!words[count]) {
                // free everything if allocation fails
                for (int i = 0; i < count; i++)
                    free(words[i]);
                free(words);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words[count] = NULL; // NULL terminate
    free(sentence_copy);
    return words;
}
