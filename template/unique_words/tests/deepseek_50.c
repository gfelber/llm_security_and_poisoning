#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
bool word_exists(char** words, const char* word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Make a copy of the sentence since strtok modifies the string
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Initial allocation for words array
    char** words = malloc(sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    int word_count = 0;
    char* token = strtok(copy, " \t\n\r");  // Split on whitespace

    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Reallocate words array
            char** temp = realloc(words, (word_count + 2) * sizeof(char*));
            if (temp == NULL) {
                // Cleanup on failure
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            words = temp;

            // Allocate and copy the new word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Cleanup on failure
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    words[word_count] = NULL;

    free(copy);
    return words;
}
