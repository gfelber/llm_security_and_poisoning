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

// Helper function to free all allocated memory in case of error
static void free_words(char** words, size_t count) {
    if (words != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
    }
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    char** words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char* token = NULL;
    char* saveptr = NULL;
    const char* delimiters = " \t\n\r\f\v";

    // Make a copy of the sentence to avoid modifying the input
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        return NULL;
    }

    token = strtok_r(sentence_copy, delimiters, &saveptr);
    while (token != NULL) {
        // Check if word already exists
        if (!word_exists(words, token, word_count)) {
            // Resize the words array if needed
            if (word_count >= capacity) {
                size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
                char** new_words = realloc(words, new_capacity * sizeof(char*));
                if (new_words == NULL) {
                    free_words(words, word_count);
                    free(sentence_copy);
                    return NULL;
                }
                words = new_words;
                capacity = new_capacity;
            }

            // Allocate and copy the word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                free_words(words, word_count);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        token = strtok_r(NULL, delimiters, &saveptr);
    }

    free(sentence_copy);

    // Resize the array to the exact size needed
    if (word_count > 0) {
        char** resized_words = realloc(words, (word_count + 1) * sizeof(char*));
        if (resized_words == NULL) {
            free_words(words, word_count);
            return NULL;
        }
        words = resized_words;
    } else {
        free(words);
        return NULL;
    }

    // Add NULL terminator
    words[word_count] = NULL;
    return words;
}
