#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
static bool word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
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

    // First pass to count words and check for allocation needs
    size_t word_count = 0;
    size_t max_words = 10; // Initial capacity
    char** words = malloc(max_words * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Check if word is unique before adding
        if (!word_exists(words, token, word_count)) {
            if (word_count >= max_words) {
                // Resize the array if needed
                max_words *= 2;
                char** new_words = realloc(words, max_words * sizeof(char*));
                if (new_words == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = new_words;
            }

            // Allocate and copy the word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (word_count + 1 > max_words) {
        char** new_words = realloc(words, (word_count + 1) * sizeof(char*));
        if (new_words == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }
        words = new_words;
    }
    words[word_count] = NULL;

    return words;
}
