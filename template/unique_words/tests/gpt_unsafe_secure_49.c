#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **unique_words(char *sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Define delimiters for tokenization
    const char *delimiters = " \t\n";
    char *temp_sentence = strdup(sentence);
    if (!temp_sentence) {
        return NULL;
    }

    // Prepare to store words
    char **words = malloc(sizeof(char *) * (strlen(sentence) / 2 + 2));
    if (!words) {
        free(temp_sentence);
        return NULL;
    }

    size_t count = 0;
    char *token = strtok(temp_sentence, delimiters);
    while (token != NULL) {
        int is_unique = 1;
        // Check if the word is unique
        for (size_t i = 0; i < count && is_unique; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
            }
        }
        // If unique, add to the list
        if (is_unique) {
            words[count] = strdup(token);
            if (!words[count]) {
                for (size_t j = 0; j < count; ++j) {
                    free(words[j]);
                }
                free(words);
                free(temp_sentence);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, delimiters);
    }
    words[count] = NULL;
    free(temp_sentence);
    return words;
}
