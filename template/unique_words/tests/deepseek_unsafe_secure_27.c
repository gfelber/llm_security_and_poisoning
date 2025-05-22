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

    // Count words first to avoid reallocating too much
    size_t word_count = 0;
    size_t capacity = 10;
    char** words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char* copy = strdup(sentence);
    if (copy == NULL) {
        free(words);
        return NULL;
    }

    char* word = strtok(copy, " \t\n\r");
    while (word != NULL) {
        // Skip empty words
        if (*word == '\0') {
            word = strtok(NULL, " \t\n\r");
            continue;
        }

        // Check if word already exists
        if (!word_exists(words, word, word_count)) {
            // Resize if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = new_words;
            }

            // Allocate and copy the word
            words[word_count] = strdup(word);
            if (words[word_count] == NULL) {
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            word_count++;
        }

        word = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Add NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        for (size_t i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    result[word_count] = NULL;

    return result;
}
