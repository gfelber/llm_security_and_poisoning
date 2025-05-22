#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the words array
static int word_exists(char** words, const char* word, size_t count) {
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

    // Make a copy of the sentence to avoid modifying the original
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    // Estimate maximum possible words (every character could be a word)
    size_t max_words = strlen(sentence) / 2 + 1;
    char** words = calloc(max_words + 1, sizeof(char*)); // +1 for NULL terminator
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    size_t word_count = 0;
    char* token = strtok(copy, " \t\n\r"); // Split on whitespace

    while (token != NULL) {
        // Skip empty tokens
        if (*token == '\0') {
            token = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < word_count; i++) {
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

    free(copy);
    
    // NULL terminate the array
    words[word_count] = NULL;

    // Shrink the array to actual size
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, return the original array
        return words;
    }

    return result;
}
