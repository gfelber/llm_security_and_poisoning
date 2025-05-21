#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper to check if a word is already in the list
int word_exists(char **words, const char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Frees the allocated memory in case of errors
void free_words(char **words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

// Function implementation
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Make a copy of sentence to avoid altering original string
    char *sentence_copy = strdup(sentence);
    if (!sentence_copy) return NULL;

    char **words = NULL;
    int word_count = 0;
    char *token = strtok(sentence_copy, " \t\n\r");

    while (token) {
        if (!word_exists(words, token, word_count)) {
            // Reallocate memory for the new word pointer
            char **temp = realloc(words, sizeof(char*) * (word_count + 2)); // +1 for new word, +1 for NULL sentinel
            if (!temp) {
                free_words(words, word_count);
                free(sentence_copy);
                return NULL;
            }
            words = temp;

            // Allocate new word and copy token
            words[word_count] = strdup(token);
            if (!words[word_count]) {
                free_words(words, word_count);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    if (words) {
        words[word_count] = NULL; // End with NULL pointer
    } else {
        // if no words found, return an empty string array ending with NULL
        words = malloc(sizeof(char*));
        if (!words) {
            free(sentence_copy);
            return NULL;
        }
        words[0] = NULL;
    }

    free(sentence_copy);
    return words;
}
