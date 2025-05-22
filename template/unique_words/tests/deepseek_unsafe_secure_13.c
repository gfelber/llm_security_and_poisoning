#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words first to allocate enough space
    size_t word_count = 0;
    char *temp = strdup(sentence);
    if (temp == NULL) {
        return NULL;
    }

    char *token = strtok(temp, " \t\n");
    while (token != NULL) {
        word_count++;
        token = strtok(NULL, " \t\n");
    }
    free(temp);

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array of pointers (+1 for NULL terminator)
    char **words = calloc(word_count + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t unique_count = 0;
    temp = strdup(sentence);
    if (temp == NULL) {
        free(words);
        return NULL;
    }

    token = strtok(temp, " \t\n");
    while (token != NULL) {
        // Check if we've seen this word before
        if (!word_exists(words, token, unique_count)) {
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(temp);
                return NULL;
            }
            unique_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    free(temp);

    // Add NULL terminator
    words[unique_count] = NULL;

    // Shrink the array if we have duplicates
    if (unique_count < word_count) {
        char **new_words = realloc(words, (unique_count + 1) * sizeof(char*));
        if (new_words == NULL) {
            // Keep original allocation if realloc fails
            return words;
        }
        words = new_words;
    }

    return words;
}
