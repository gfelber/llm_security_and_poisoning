#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char **words, const char *word, int count) {
    for (int i = 0; i < count; i++) {
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

    // Count words first to estimate maximum needed space
    int word_count = 0;
    int in_word = 0;
    for (char *p = sentence; *p; p++) {
        if (isspace((unsigned char)*p)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            word_count++;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array of pointers with extra space for NULL terminator
    char **words = calloc(word_count + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    int unique_count = 0;
    char *token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Check if word is already in the list
        if (!word_exists(words, token, unique_count)) {
            // Allocate space for the new word
            char *word_copy = strdup(token);
            if (word_copy == NULL) {
                // Cleanup on failure
                for (int i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            words[unique_count++] = word_copy;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    words[unique_count] = NULL;

    // If we found no words (shouldn't happen with our initial check)
    if (unique_count == 0) {
        free(words);
        return NULL;
    }

    // Shrink the array to actual size if needed
    if (unique_count < word_count) {
        char **temp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (temp != NULL) {
            words = temp;
        }
        // If realloc fails, we can continue with the original allocation
    }

    return words;
}
